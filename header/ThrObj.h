#pragma once
#include <map>
#include <string>
#include <memory>

class ThrObj {
private:
	/*using Counter = std::map<std::string, std::size_t>;
	Counter word;
	std::ifstream& in_;*/
	std::unique_ptr<Impl> pImpl;
public:
	explicit ThrObj(std::ifstream& in);
	~ThrObj();
	void count_words(std::ifstream& stream, Counter& counter);
	
};