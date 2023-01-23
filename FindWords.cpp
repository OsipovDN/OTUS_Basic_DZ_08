// Read files and prints top k word by frequency

#include <algorithm>
#include <vector>
#include <thread>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <memory>
#include <mutex>

const size_t TOPK = 10;

using Counter = std::map<std::string, std::size_t>;
std::string tolower(const std::string& str);
void sum_words(Counter&& temp, Counter& counter,std::mutex& m);
void count_words(std::istream& stream, Counter& counter);
void count_words(std::istream& stream, Counter& counter,std::mutex& m);
void print_topk(std::ostream& stream, const Counter&, const size_t k);

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> thr_vec;
    Counter freq_dict;
    std::mutex freq_dict_mut;
    thr_vec.reserve(argc - 1);
    if (argc <= 2) {
        if (argc == 2) {
            std::ifstream input(argv[1]);
            if (!input.is_open()) {
                std::cerr << "Failed to open file " << argv[1] << '\n';
                return EXIT_FAILURE;
            }
            count_words(input, freq_dict);
        }
        else {
            std::cerr << "Usage: topk_words [FILES...]\n";
            return EXIT_FAILURE;
        }
    }
    else {    
        for (auto i = 1; i < argc; ++i) {
            std::ifstream input(argv[i]);
            auto work = [&freq_dict, &freq_dict_mut, in=std::move(input)]()mutable {
                count_words(in, freq_dict, freq_dict_mut); 
            };
            thr_vec.emplace_back(std::move(work));
        }   
    }
    for (auto& it : thr_vec) {
        it.join();
    }
    auto col=0;
    for (const auto it : freq_dict) {
        col += it.second;
    }
    std::cout << col << std::endl;

    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}

std::string tolower(const std::string& str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
        std::back_inserter(lower_str),
        [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

void sum_words(Counter&& temp, Counter& counter, std::mutex& m) {
    std::lock_guard Lock(m);
    for (auto ittemp : temp) {
        counter[ittemp.first] += ittemp.second;
    }
}

void count_words(std::istream& stream, Counter& counter) {
    std::for_each(std::istream_iterator<std::string>(stream),
        std::istream_iterator<std::string>(),
        [&counter](const std::string& s) {
            ++counter[tolower(s)];
        });
}

void count_words(std::istream& stream, Counter& counter, std::mutex& m) {
    Counter temp;
    std::for_each(std::istream_iterator<std::string>(stream),
        std::istream_iterator<std::string>(),
        [&temp](const std::string& s) {
            ++temp[tolower(s)];
        });
    sum_words(std::move(temp),counter, m);
}

void print_topk(std::ostream& stream, const Counter& counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
        std::begin(words), std::begin(words) + k, std::end(words),
        [](auto lhs, auto& rhs) { return lhs->second > rhs->second; });

    std::for_each(
        std::begin(words), std::begin(words) + k,
        [&stream](const Counter::const_iterator& pair) {
            stream << std::setw(4) << pair->second << " " << pair->first
                << '\n';
        });
}

