// Robert Lawton
// CS321 HW2 - Part 1
// 3/1/2020

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <sstream>
#include <fstream>

using namespace std::chrono;

// Selected 50 words from the text file
// All words are lowercase and contain no special characters
std::vector<std::pair<std::string, int>> word_list = {
    {"ideas",     0}, 
    {"am",        0}, 
    {"who",       0},
    {"her",       0}, 
    {"when",      0}, 
    {"are",       0},
    {"you",       0}, 
    {"going",     0}, 
    {"to",        0},
    {"do",        0}, 
    {"attracted", 0}, 
    {"about",     0},
    {"how",       0}, 
    {"then",      0}, 
    {"way",       0},
    {"things",    0}, 
    {"are",       0}, 
    {"if",        0},
    {"like",      0}, 
    {"hide",      0}, 
    {"quickly",   0},
    {"blue",      0}, 
    {"farther",   0}, 
    {"animal",    0},
    {"she",       0}, 
    {"the",       0}, 
    {"family",    0}, 
    {"best",      0}, 
    {"time",      0}, 
    {"there",     0},
    {"their",     0}, 
    {"your",      0}, 
    {"when",      0},
    {"where",     0}, 
    {"him",       0}, 
    {"fight",     0},
    {"nasty",     0}, 
    {"princess",  0}, 
    {"vigilance", 0},
    {"looking",   0}, 
    {"voodoo",    0}, 
    {"wind",      0},
    {"jewel",     0}, 
    {"outside",   0}, 
    {"house",     0},
    {"home",      0}, 
    {"town",      0}, 
    {"name",      0},
    {"grown",     0}, 
    {"strange",   0}
};
 
std::mutex increment_mutex;

// grab words from partition and see if they match one from the list, 
// incremenet word_list[i].second if they do
void count_words(std::string& text) {
    std::string word;
    std::istringstream is(text);
    while(std::getline(is, word)) {
        for(int i = 0; i < 50; i++) {
            if(word_list[i].first == word) {
                std::lock_guard<std::mutex> guard(increment_mutex);
                word_list[i].second++;
            }
        }
    }
}

int main() {
    
    std::string filename = "cs321/HW2/mil_words.txt";
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cout << "Unable to open " << filename << std::endl;
        exit(1);
    } 
    
    
    // Partition file into multiple pieces
    int count = 0;
    std::string line;
    std::string text1,text2,text3,text4,text5,text6,text7,text8;
    while(std::getline(file, line)) {
        if     (count < 1000000/8) text1 += line + "\n";
        else if(count < 2000000/8) text2 += line + "\n";
        else if(count < 3000000/8) text3 += line + "\n";
        else if(count < 4000000/8) text4 += line + "\n";
        else if(count < 5000000/8) text5 += line + "\n";
        else if(count < 6000000/8) text6 += line + "\n";
        else if(count < 7000000/8) text7 += line + "\n";
        else                       text8 += line + "\n";
        count++;
    }

    auto start = high_resolution_clock::now();

    std::thread t1(count_words, std::ref(text1));
    std::thread t2(count_words, std::ref(text2));
    std::thread t3(count_words, std::ref(text3));
    std::thread t4(count_words, std::ref(text4));
    std::thread t5(count_words, std::ref(text5));
    std::thread t6(count_words, std::ref(text6));
    std::thread t7(count_words, std::ref(text7));
    std::thread t8(count_words, std::ref(text8));

    t1.join(); t2.join(); t3.join(); t4.join();
    t5.join(); t6.join(); t7.join(); t8.join();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    for(int i = 0; i < 50; i++) {
        std::cout << word_list[i].first << ": " << word_list[i].second << std::endl;
    }

    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}