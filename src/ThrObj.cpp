#include "ThrObj.h"
#include <ifstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <string>


using Counter = std::map<std::string, std::size_t>;
struct Impl{
	Counter word;
	std::istream& in_;
	explicit Impl(std::istream& in) :in_(std::move(in)) {};
};

explicit ThrObj::ThrObj(std::istream& in):pImpl(new Impl(in)) {
	count_words();
};

explicit ThrObj::ThrObj(std::istream&&) = default;
explicit ThrObj ThrObj::operator =(std::istream&&) = default;
ThrObj::~ThrObj() = default;

explicit void ThrObj::count_words() {
	std::for_each(std::istream_iterator<std::string>(Impl::in_),
		std::istream_iterator<std::string>(),
		[&word](const std::string& s) { ++word[s]; });   //s=tolower(s)
};