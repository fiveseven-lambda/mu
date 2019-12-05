#include <fstream>
#include <iostream>
#include <cstdarg>
#include <vector>
#include <stack>
#include <memory>

struct Char{
	int c;
	int line, pos;
};

[[noreturn]] void fatal_error(Char c, const char message[], ...){
	va_list args;
	va_start(args, message);
	fprintf(stderr, "fatal error at (%d, %d): ", c.line, c.pos);
	vfprintf(stderr, message, args);
	va_end(args);
	exit(-1);
}

class Source{
	std::ifstream f;
	struct{
		Char c;
		bool isbuf;
	} buf;
public:
	Source(const char filename[]): f(filename), buf{.isbuf = false} {}
private:
	Char getc(){
		static int line = 1, pos = 1;
		Char ret;
		ret.c = f.get();
		ret.line = line;
		ret.pos = pos;
		if(ret.c == '\n'){
			++line;
			pos = 1;
		}else{
			++pos;
		}
		return ret;
	}
public:
	Char get(bool ispeek = 0){
		if(!buf.isbuf){
			while((buf.c = getc()).c == '#'){
				char end;
				Char tmp = getc();
				switch(tmp.c){
					case '(':
						end = ')';
						break;
					case '#':
						end = '\n';
						break;
					default:
						fatal_error(tmp, "wrong style of comment\n");
				}
				while((tmp = getc()).c != end){
					if(tmp.c == EOF){
						fatal_error(tmp, "unterminated comment\n");
						exit(-1);
					}
				}
			}
		}
		buf.isbuf = ispeek;
		return buf.c;
	}
};

enum class Type { Space, Digit, Alphabet, Operator };

Type check_type(char c){
	if(isspace(c)){
		return Type::Space;
	}else if(isdigit(c)){
		return Type::Digit;
	}else if(isalpha(c)){
		return Type::Alphabet;
	}else{
		return Type::Operator;
	}
}

class Token{
public:
	Type type;
	std::vector<Char> str;
	Token(Type type, Char c) : type(type), str(1, c) {}
	std::string getstr() const {
		std::string ret;
		for(const Char &i : str) ret.push_back(i.c);
		return ret;
	}
};

class Operator{
public:
	virtual void apply(std::stack<int> &) = 0;
	virtual int priority() = 0;
};
class Plus: public Operator{
public:
	void apply(std::stack<int> &stack){
		int tmp = stack.top();
		stack.pop();
		stack.top() += tmp;
	}
	int priority(){
		return 0;
	}
};
class Minus: public Operator{
public:
	void apply(std::stack<int> &stack){
		int tmp = stack.top();
		stack.pop();
		stack.top() -= tmp;
	}
	int priority(){
		return 0;
	}
};
class Product: public Operator{
public:
	void apply(std::stack<int> &stack){
		int tmp = stack.top();
		stack.pop();
		stack.top() *= tmp;
	}
	int priority(){
		return 1;
	}
};
std::unique_ptr<Operator> getop(const std::string &str){
	Operator *ret;
	if(str == "+"){
		ret = new Plus();
	}else if(str == "-"){
		ret = new Minus();
	}else if(str == "*"){
		ret = new Product();
	}else{
		fprintf(stderr, "unknown operator %s\n", str);
	}
	return std::unique_ptr<Operator>(ret);
}


int main(){
	Source source("test");
	std::vector<Token> token;
	Type type = Type::Space;
	for(Char tmp; (tmp = source.get()).c != EOF;){
		Type t = check_type(tmp.c);
		if(t != Type::Space){
			if(t == type){
				token.back().str.push_back(tmp);
			}else{
				token.push_back(Token(t, tmp));
			}
		}
		type = t;
	}

	std::stack<int> stack;
	std::stack<std::unique_ptr<Operator>> op;
	for(const Token &i : token){
		if(i.type == Type::Digit){
			int tmp = 0;
			for(const Char &j : i.str){
				tmp = tmp * 10 + (j.c - '0');
			}
			stack.push(tmp);
		}else if(i.type == Type::Operator){
			std::unique_ptr<Operator> tmp = getop(i.getstr());
			while(!op.empty() && op.top()->priority() >= tmp->priority()){
				op.top()->apply(stack);
				op.pop();
			}
			op.push(std::move(tmp));
		}
	}
	while(!op.empty()){
		op.top()->apply(stack);
		op.pop();
	}
	for(; !stack.empty(); stack.pop()){
		std::cout << stack.top() << std::endl;
	}
}
