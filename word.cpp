#include <cstdio>
#include <stack>
#include <cmath>
#include "word.h"

int Word::start(){
	return -1;
}
bool Word::equal(){
	return false;
}
bool Word::var(std::stack<double> &, std::stack<Word *> &){
	return false;
}
int Word::priority(){
	return 0;
}

void Literal::print(){
	printf("Literal: %.*s (point : %d)\n", len, addr, point);
}
bool Literal::var(std::stack<double> &operand_stack, std::stack<Word *> &){
	double val = 0;
	for(int i = 0; i < len; ++i){
		if(i != point) val = val * 10 + (addr[i] - '0');
	}
	if(point != -1) val *= std::pow(.1, len - point - 1);
	operand_stack.push(val);
	return true;
}
void Var::print(){
	printf("Var: %.*s\n", len, addr);
}
int Var::start(){
	return 0;
}
void Name::print(){
	printf("Name: %.*s\n", len, addr);
}
int Name::start(){
	return 1;
}
void Equal::print(){
	printf("Equal: %.*s\n", len, addr);
}
bool Equal::equal(){
	return true;
}
void Operator_var::print(){
	printf("Operator(var): %.*s\n", len, addr);
}
void Operator_name::print(){
	printf("Operator(name): %.*s\n", len, addr);
}
void Paren_open::print(){
	printf("Paren(open): %.*s\n", len, addr);
}
void Paren_close::print(){
	printf("Paren(close): %.*s\n", len, addr);
}
void Brace_open::print(){
	printf("Brace(open): %.*s\n", len, addr);
}
void Brace_close::print(){
	printf("Brace(close): %.*s\n", len, addr);
}
void Bar::print(){
	printf("Bar: %.*s\n", len, addr);
}
