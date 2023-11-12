#pragma once

#include <fstream>
#include <String>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <thread>
#include <filesystem>
#include <future>
#include <mutex>
using wordSet = std::unordered_set<std::string>;

class counterV3
{
private:
    int threadAmount;
    std::string filePath;
    int countWordsThreads();
    void singleThread(std::string filePath, unsigned long begin, unsigned long end, int coreId);
    std::unordered_set<std::string> set;
    std::mutex mutex;

public:
    counterV3(std::string filePath, int threadAmount) : threadAmount(threadAmount), filePath(filePath){};
    ~counterV3() = default;
    void set_threads(int t) { threadAmount = t; };
    int count();
};

void counterV3::singleThread(std::string filePath, unsigned long begin, unsigned long end, int coreId)
{
    // std::cout << "setsize" << set.size();
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::string word;
    char k;
    unsigned long pos = begin;
    file.seekg(begin, std::ios::beg);
    while (pos < end && std::getline(file, word, ' '))
    {
        pos += word.length();
        const std::lock_guard<std::mutex> lock(mutex);
        set.insert(word);
    }
    file.close();
}

// todo check how many words can fit in 32gb
int counterV3::countWordsThreads()
{
    std::fstream file;
    file.open(filePath);
    unsigned long len = std::filesystem::file_size(filePath);
    unsigned long lenForThread = len / threadAmount;
    unsigned long currentPos = 0;
    std::vector<std::thread> p{}; // todo rename and make new type name or smth
    for (size_t i = 0; i < threadAmount - 1; i++)
    {
        unsigned long startPos = currentPos;
        currentPos += lenForThread;
        file.seekg(currentPos, std::ios::beg);
        while (file.get() != ' ')
        {
            currentPos--;
            file.seekg(-2, std::ios::cur);
        }
        p.push_back(std::thread(&counterV3::singleThread, this, std::ref(filePath), startPos, currentPos, i));
        currentPos++;
    }
    p.push_back(std::thread(&counterV3::singleThread, this, std::ref(filePath), currentPos, len, threadAmount));
    file.close();
    for (auto &&i : p)
    {
        i.join();
    }

    return set.size();
}

int counterV3::count()
{
    return countWordsThreads();
}
