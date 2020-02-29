#include <vector>
#include <stack>
#include <iostream>

#include "source.h"

int main(){
	Source source("test");
	if(source.break_word()){
		for(auto &i : source.words){
			std::cout << "sentence: " << i.size() << " words" << std::endl;
			for(auto j : i){
				j->print();
			}
		}
	}
	source.free();
}

