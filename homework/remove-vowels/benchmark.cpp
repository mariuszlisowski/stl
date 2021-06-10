#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

const int LOOP_COUNT{ 10000 };

/* 1 */
void removeVowels_regex(std::vector<std::string> strings) {
    std::regex vowels("a|e|i|o|u|y|A|E|I|O|U|Y");
    std::for_each(strings.begin(), strings.end(),
                  [&vowels](std::string& str) {
                      str = std::regex_replace(str, vowels, "");
                  });
}

/* 2 */
void removeVowels_loops(std::vector<std::string> vec) {
    std::string str = {"aeiouyAEIOUY"};
    for (auto& ele : vec) {
        for (auto const& s : str) {
            ele.erase(std::remove(ele.begin(), ele.end(), s), ele.end());
        }
    }
}

/* 3 */
void removeVowels_foreach(std::vector<std::string> strings) {
    std::string vowels{ "aeiouyAEIOUY" };
    std::for_each(strings.begin(), strings.end(),
                  [&vowels](std::string& str) {
                      str.erase(std::remove_if(str.begin(),
                                               str.end(),
                                               [&vowels](const char c) {
                                                   return std::string(vowels).find(c) != 
                                                          std::string::npos;
                                               }),
                                str.end());
                  });
}

/* 4 */
void removeVowels_for(std::vector<std::string> strings) {
    std::string vowels{ "aeiouyAEIOUY" };
    for (auto& str : strings) {
        str.erase(std::remove_if(str.begin(),
                                 str.end(),
                                 [&vowels](const char c) {
                                     return std::string(vowels).find(c) !=
                                            std::string::npos;
                                 }),
                   str.end());
    };
}

void benchmark(std::function<void(std::vector<std::string>)> funct,
               std::vector<std::string>& words)
{
    for (int i{}; i < LOOP_COUNT; ++i) {
        funct(words);
    }
}

void print_duration(duration<double, std::milli> result, int benchmark_no) {
    std::cout << benchmark_no << ". " << std::left << std::setw(10) 
              << result.count() << " ms" << std::endl;
}

int main() {
    std::vector<std::string> words = {
        "Somebody once told me the world is gonna roll me",
        "I ain't the sharpest tool in the shed",
        "She was looking kind of dumb with her finger and her thumb",
        "In the shape of an L on her forehead"
    };

    /* 1 */
    auto t1 = high_resolution_clock::now();
    benchmark(removeVowels_regex, words);
    auto t2 = high_resolution_clock::now();
    print_duration(t2 - t1, 1);

    /* 2 */
    t1 = high_resolution_clock::now();
    benchmark(removeVowels_loops, words);
    t2 = high_resolution_clock::now();
    print_duration(t2 - t1, 2);

    /* 3 */
    t1 = high_resolution_clock::now();
    benchmark(removeVowels_foreach, words);
    t2 = high_resolution_clock::now();
    print_duration(t2 - t1, 3);

    /* 4 */
    t1 = high_resolution_clock::now();
    benchmark(removeVowels_for, words);
    t2 = high_resolution_clock::now();
    print_duration(t2 - t1, 4);

    return 0;
}
