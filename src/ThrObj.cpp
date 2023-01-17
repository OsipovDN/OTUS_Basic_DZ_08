#include "ThrObj.h"
#include <map>
#include <string>
using Counter = std::map<std::string, std::size_t>;
struct Impl{
	Counter word;
	std::ifstream& in_;
	Impl(std::ifstream& in) :in_(in) {};
};

explicit ThrObj(std::ifstream& in):pImpl(new ThrObj::Impl(in)) {
	Impl.in_ = in;
	count_words(in, word);
};
	~ThrObj() = default;

void count_words(std::ifstream& stream, Counter& counter) {
	std::for_each(std::ifstream_iterator<std::string>(stream),
		std::ifstream_iterator<std::string>(),
		[&counter](const std::string& s) { ++counter[s]; });   //s=tolower(s)
};