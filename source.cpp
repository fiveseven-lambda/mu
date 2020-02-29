#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <vector>
#include <stack>
#include <iostream>

#include "source.h"

Source::Source(const char filename[]){
	fd = open(filename, O_RDONLY);
	struct stat stat;
	fstat(fd, &stat);
	size = stat.st_size;
	map = reinterpret_cast<char *>(mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0));
}

void Source::free(){
	munmap(map, size);
	close(fd);
}

bool Source::break_word(){
	std::vector<Word *> tmp;
	bool ret = true;
	bool in_brace = false;
	for(size_t i = 0; i < size; ++i){
		if(map[i] == '#'){
			switch(map[++i]){
				case '#':
					for(; map[i] != '\n'; ++i);
					break;
				case '(':
					for(unsigned paren = 1; paren;){
						++i;
						if(i == size){
							std::cerr << "error: unterminated comment" << std::endl;
							return false;
						}else switch(map[i]){
							case '(':
								++paren;
								break;
							case ')':
								--paren;
						}
					}
					break;
				default:
					std::cerr << "error: wrong style of comment" << std::endl;
					ret = false;
			}
		}else if(map[i] == '='){
			if(!in_brace){
				switch(tmp.size()){
					case 0:
						std::cerr << "error" << std::endl;
						ret = false;
						break;
					case 1:
						break;
					default:
						Word *last = tmp.back();
						tmp.pop_back();
						words.push_back(tmp);
						tmp.clear();
						tmp.push_back(last);
				}
			}
			tmp.push_back(new Equal(map + i));
		}else if(map[i] == '+' || map[i] == '-' || map[i] == '*' || map[i] == '/'){
			tmp.push_back(new Operator_var(map + i));
		}else if(map[i] == ',' || map[i] == ';'){
			tmp.push_back(new Operator_name(map + i));
		}else if(map[i] == '('){
			tmp.push_back(new Paren_open(map + i));
		}else if(map[i] == ')'){
			tmp.push_back(new Paren_close(map + i));
		}else if(map[i] == '{'){
			in_brace = true;
			tmp.push_back(new Brace_open(map + i));
		}else if(map[i] == '}'){
			in_brace = false;
			tmp.push_back(new Brace_close(map + i));
		}else if(map[i] == '|'){
			tmp.push_back(new Bar(map + i));
		}else if(isalpha(map[i])){
			size_t j;
			for(j = 1; i + j < size && (map[i + j] == '_' || isalnum(map[i + j])); ++j);
			tmp.push_back(new Var(map + i, j));
			i += j - 1;
		}else if(map[i] == '%'){
			size_t j;
			for(j = 1; i + j < size && (map[i + j] == '_' || isalnum(map[i + j])); ++j);
			tmp.push_back(new Name(map + i, j));
			i += j - 1;
		}else if(map[i] == '.' || isdigit(map[i])){
			unsigned point = -1;
			size_t j;
			for(j = 0; i + j < size; ++j){
				if(map[i + j] == '.'){
					if(point == -1) point = j;
					else break;
				}else if(!isdigit(map[i + j])){
					break;
				}
			}
			tmp.push_back(new Literal(map + i, j, point));
			i += j - 1;
		}else if(!isspace(map[i])){
			std::cerr << "error: unknown character \'" << map[i] << "\'" << std::endl;
			ret = false;
		}
	}
	words.push_back(tmp);
	return ret;
}
