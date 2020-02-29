#include "word.h"

class Source{
	int fd;
	size_t size;
	char *map;
public:
	std::vector<std::vector<Word *>> words;
	Source(const char[]);
	Source(const Source &) = delete;
	Source &operator=(const Source &) = delete;
	void free();
	bool break_word();
};
