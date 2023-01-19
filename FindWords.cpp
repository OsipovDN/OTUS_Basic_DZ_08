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
void count_words(std::istream& stream, Counter& counter);
void print_topk(std::ostream& stream, const Counter&, const size_t k);

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> thr_vec;
    Counter freq_dict;
    std::mutex dict_m;
    if (argc <= 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }
    else {    
        //thr_vec.reserve(argc);
        for (int i = 1; i < argc; ++i) {
            std::ifstream input{ argv[i] };
            if (!input.is_open()) {
                std::cerr << "Failed to open file " << argv[i] << '\n';
                return EXIT_FAILURE;
            }
            auto work = [&freq_dict](std::istream& in) {count_words(std::move(in), freq_dict); };
            thr_vec.push_back(std::thread (std::move(work),std::move(input)));
            std::cout << argv[i] << std::endl;
        }
        for (auto& ptr: thr_vec) {
            ptr.join();
        }
    }
        
    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";

}       

void count_words(std::istream& stream, Counter& counter) {
    std::for_each(std::istream_iterator<std::string>(stream),
        std::istream_iterator<std::string>(),
        [& counter](const std::string& s) {
            ++counter[s];
        });
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

