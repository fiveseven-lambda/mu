#include <fstream>
#include <iostream>
#include "char.hpp"
#include "source.hpp"
#include "errormsg.hpp"


Source::Source(const char filename[]): f(filename), buf{.isbuf = false} {
	if(!f){
		fatal_error() << "file \'" << filename << "\' not found" << std::endl;
		exit(-1);
	}
}
Char Source::getc(){
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

Char Source::get(bool ispeek = 0){
	if(!buf.isbuf){
		while((buf.ch = getc()).c == '#'){
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
					fatal_error(tmp) << "wrong style of comment" << std::endl;
					exit(-1);
			}
			while((tmp = getc()).c != end){
				if(tmp.c == EOF){
					fatal_error(tmp) << "unterminated comment" << std::endl;
					exit(-1);
				}
			}
		}
	}
	buf.isbuf = ispeek;
	return buf.ch;
}
