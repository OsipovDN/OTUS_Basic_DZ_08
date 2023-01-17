#pragma once
#include <memory>


class ThrObj {
private:
	std::unique_ptr<Impl> pImpl;
public:
	explicit ThrObj(std::ifstream& in);
	explicit ThrObj(std::ifstream&&);
	explicit operator =(std::ifstream&&);
	~ThrObj();
	void count_words();
};