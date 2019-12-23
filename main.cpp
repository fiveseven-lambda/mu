#include <fstream>
#include <iostream>
#include <cstdarg>
#include <cctype>
#include <vector>
#include <stack>
#include <memory>
#include <map>

#include "char.hpp"
#include "source.hpp"
#include "errormsg.hpp"
#include "token.hpp"

class score{
public:
	virtual void print() = 0;
};
class single: public score{
public:
	double freq, len;
	single(double freq, double len):freq(freq),len(len){}
	void print(){
		std::cout << freq;
	}
};
class multi: public score{
public:
	std::vector<std::pair<int, std::unique_ptr<score>>> vec;
	void print(){
		std::cout << "[";
		for(int i = 0; i < vec.size(); ++i){
			std::cout << i << ":(" << vec[i].first << ")";
			vec[i].second->print();
			std::cout << " ]"[i == vec.size() - 1];
		}
	}
};

int main(){
	Source source("test");
	while(true){
		Token tok(source);
		if(tok.type == Type::Literal){
		       std::cout << "Literal \"" << tok << '\"' << std::endl;
		       error(tok) << std::endl;
		}else if(tok.type == Type::Name){
		       std::cout << "Name \"" << tok << '\"' << std::endl;
		}else if(tok.type == Type::Equal){
		       std::cout << "Equal \"" << tok << '\"' << std::endl;
		}else if(tok.type == Type::Comma){
		       std::cout << "Comma \"" << tok << '\"' << std::endl;
		}else if(tok.type == Type::EOFToken) break;
	}
}
