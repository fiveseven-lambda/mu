#include <fstream>

class Source{
	std::ifstream f;
	struct{
		Char ch;
		bool isbuf;
	} buf;
public:
	Source(const char []);
private:
	Char getc();
public:
	Char get(bool);
};
