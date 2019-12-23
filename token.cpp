#include <iostream>
#include <vector>
#include "char.hpp"
#include "source.hpp"
#include "token.hpp"

Token::Token(Source &source){
	int c;
	while(isspace(c = source.get(1).c)) source.get(0);
	if(c == EOF){
		type = Type::EOFToken;
	}else if(isalnum(c)){
		type = Type::Literal;
		while(isalnum(source.get(1).c)){
			if(isalpha(source.get(1).c)) type = Type::Name;
			add(source.get(0));
		}
	}else if(c == '='){
		type = Type::Equal;
		add(source.get(0));
	}else if(c == ','){
		type = Type::Comma;
		add(source.get(0));
	}
}
void Token::add(const Char &ch){
	vec.push_back(ch);
}
bool Token::operator<(const Token &str) const {
	if(vec.size() != str.vec.size()){
		return vec.size() < str.vec.size();
	}
	for(int i = 0; i < vec.size(); ++i){
		if(vec[i].c != str.vec[i].c){
			return vec[i].c < str.vec[i].c;
		}
	}
	return false;
}

std::ostream &operator<<(std::ostream &os, const Token &tok){
	for(const Char &i : tok.vec) os << (char)i.c;
	return os;
}
