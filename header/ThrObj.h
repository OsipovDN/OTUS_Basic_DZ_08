#pragma once
#include <memory>


class ThrObj {
private:
	std::unique_ptr<Impl> pImpl;
public:
	explicit ThrObj(std::istream& in);
	explicit ThrObj(std::istream&&);
	explicit ThrObj& operator =(std::istream&&);
	~ThrObj();
	void count_words();
};