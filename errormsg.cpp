#include <iostream>
#include "char.hpp"
#include "errormsg.hpp"

std::ostream &fatal_error(){
	return std::cerr << "fatal error: ";
}
std::ostream &fatal_error(Char ch){
	return std::cerr << "fatal error at (" << ch.line << ", " << ch.pos << "): ";
}
std::ostream &error(const Token &tok){
	if(tok.type == Type::EOFToken) return std::cerr << "error at the end of file: ";
	else return std::cerr << "error at [" << tok.vec.front().line << ", " << tok.vec.front().pos
		<< " - " << tok.vec.back().line << ", " << tok.vec.back().pos << "]: ";
}
