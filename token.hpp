enum class Type { EOFToken, Literal, Name, Equal, Comma};

class Token{
public:
	std::vector<Char> vec;
	Type type;
	Token() = delete;
	Token(Source &source);
	void add(const Char &ch);
	bool operator<(const Token &) const;
	friend std::ostream &operator<<(std::ostream &os, const Token &tok);
	friend std::ostream &error(const Token &);
};
