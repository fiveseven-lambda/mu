#include <fstream>
#include <iostream>
#include <cstdarg>
#include <vector>
#include <stack>

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

enum Type { Space, Digit, Alphabet, Operator };

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
	for(const Token &i : token){
		if(i.type == Type::Digit){
			int tmp = 0;
			for(const Char &j : i.str){
				tmp = tmp * 10 + (j.c - '0');
			}
			stack.push(tmp);
		}else if(i.type == Type::Operator){
			if(i.getstr() == "+"){
				int a, b;
				a = stack.top();
				stack.pop();
				b = stack.top();
				stack.pop();
				stack.push(a + b);
			}else if(i.getstr() == "*"){
				int a, b;
				a = stack.top();
				stack.pop();
				b = stack.top();
				stack.pop();
				stack.push(a * b);
			}else{
				std::cerr << "error: unknown operator \"" << i.getstr() << '\"' << std::endl;
			}
		}
	}
	for(; !stack.empty(); stack.pop()){
		std::cout << stack.top() << std::endl;
	}
}
