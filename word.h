class Word{
	enum class Type {
		Literal,
		Var,
		Name,
		Equal,
		Operator_var,
		Operator_name,
		Paren_open,
		Paren_close,
		Brace_open,
		Brace_close,
		Bar
	} type;
public:
	char *addr;
	size_t len;
	Word(char *addr, size_t len) : addr(addr), len(len) {}
	virtual void print() = 0;
	virtual int start();
	virtual bool equal();
	virtual bool var(std::stack<double> &, std::stack<Word *> &);
	virtual int priority();
};

class Literal : public Word{
public:
	int point;
	Literal(char *addr, int len, int point) : Word(addr, len), point(point) {}
	void print();
	bool var(std::stack<double> &, std::stack<Word *> &);
};

class Var : public Word{
public:
	Var(char *addr, int len) : Word(addr, len) {}
	void print();
	int start();
};

class Name : public Word{
public:
	Name(char *addr, int len) : Word(addr, len) {}
	void print();
	int start();
};

class Equal : public Word{
public:
	Equal(char *addr) : Word(addr, 1) {}
	void print();
	bool equal();
};

class Operator_var : public Word{
public:
	Operator_var(char *addr) : Word(addr, 1) {}
	void print();
};

class Operator_name : public Word{
public:
	Operator_name(char *addr) : Word(addr, 1) {}
	void print();
};

class Paren_open: public Word{
public:
	Paren_open(char *addr) : Word(addr, 1) {}
	void print();
};

class Paren_close: public Word{
public:
	Paren_close(char *addr) : Word(addr, 1) {}
	void print();
};

class Brace_open: public Word{
public:
	Brace_open(char *addr) : Word(addr, 1) {}
	void print();
};

class Brace_close: public Word{
public:
	Brace_close(char *addr) : Word(addr, 1) {}
	void print();
};

class Bar : public Word{
public:
	Bar(char *addr) : Word(addr, 1) {}
	void print();
};
