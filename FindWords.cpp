// Read files and prints top k word by frequency

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

const size_t TOPK = 10;

using Counter = std::map<std::string, std::size_t>;
using WordTab = std::vector<std::map<std::string, std::size_t>>;

//std::string tolower(const std::string &str);
void count(std::istream stream, Counter& obj);
void count_words(std::istream stream, Counter& obj, std::mutex& m);

void print_topk(std::ostream& stream, const Counter&, const size_t k);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();
    Counter freq_dict;  //Пара <слово, колич. повторений> (std::map)
    std::mutex m_freq_dict;
    //Каждый файл обрабатывается отдельно последовательно
    if (argc == 2) {
        //Если аргумент один (один файл), 
        //то выполняем в одном потоке
        std::ifstream input{ argv[2] };   //Обрабатываем первый файл
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[2] << '\n';
            return EXIT_FAILURE;
        }
        count(input, freq_dict);
    }
    else {
        //Если введено несколько файлов
        std::vector<std::thread> vec;
        vec.reserve(argc - 1);

        for (int i = 1; i < argc; ++i) {
            std::ifstream in(argv[i]);
            if (!in.is_open()) {
                //Проверям открытие
                std::cerr << "Failed to open file " << argv[i] << '\n';
                return EXIT_FAILURE;
            }
            /*auto work = [&freq_dict](std::ifstream& in, std::mutex& mut) {
                obj.count_words(freq_dict, std::ref(mut));
            }; */
            vec[i] = std::thread(count_words, std::move(in), std::ref(m_freq_dict));
        }
        for (int i = 1; i < argc; ++i) {
            vec[i].join();
        }
    }


    //Вывод таблицы с посчитанными словами на экран
    print_topk(std::cout, freq_dict, TOPK);
    //Расчет времени выполнения и вывод на экран
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
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
            stream << std::setw(4) << pair->second << " " << pair->first << '\n';
        });
}

void count(std::istream& stream,Counter& obj) {

    std::for_each(std::istream_iterator<std::string>(stream),
        std::istream_iterator<std::string>(),
        [&obj](const std::string& s, std::mutex& m) {
            ++obj[s]; });   //s=tolower(s)
};

void count_words(std::istream stream, Counter& obj, std::mutex& m) {

    std::for_each(std::istream_iterator<std::string>(stream),
        std::istream_iterator<std::string>(),
        [&obj](const std::string& s, std::mutex& m) {
            m.lock();
            ++obj[s];
            m.unlock(); });   //s=tolower(s)
};