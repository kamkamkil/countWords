#pragma once

#include <fstream>
#include <String>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <thread>
#include <filesystem>
#include <future>

using wordSet = std::unordered_set<std::string>;

int countWordsPOC(std::string filePath)
{
    std::fstream file;
    file.open(filePath);
    std::unordered_set<std::string> set;
    std::string word;
    char k;
    while (std::getline(file,word,' '))
    {

        set.insert(word);
        std::cout << word << std::endl;
    }
    file.close();
    return set.size();
}

std::unordered_set<std::string> singleThread(std::string filePath, int begin, int end)
{
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::unordered_set<std::string> set;
    std::string word;
    char k;
    int pos = begin;
    file.seekg(begin, std::ios::beg);
    while (pos < end && std::getline(file,word,' '))
    {
        pos+=word.length();
        set.insert(word);

    }
    file.close();
    return set;
}

// todo check how many words can fit in 32gb
int countWordsThreads(std::string filePath, int threads)
{
    std::fstream file;
    file.open(filePath);
    file.seekg(0, std::ios::end);
    unsigned long len  = std::filesystem::file_size(filePath);
    file.seekg(0);
    file.clear();
    unsigned long  lenForThread = len / threads;
    unsigned long  currentPos = 0;
    std::vector<std::tuple<int, int>> tPos;
    for (size_t i = 0; i < threads - 1; i++)
    {
        unsigned long  startPos = currentPos;
        currentPos += lenForThread;
        file.seekg(currentPos, std::ios::beg);
        while (file.get() != ' ')
        {
            currentPos--;
            file.seekg(-2, std::ios::cur);
        }
        tPos.push_back({startPos, currentPos});
        currentPos++;
    }
    tPos.push_back({currentPos, len});
    file.close();
    std::vector<std::future<std::unordered_set<std::string>>> p{};
    for (auto &&t : tPos)
    {
        const int a = std::get<0>(t);
        const int b = std::get<1>(t);
        p.push_back( std::async(&singleThread,  std::ref(filePath), a, b));
    }
    wordSet set;
    for (auto &&k : p)
    {
        set.merge(k.get());
    }

    return set.size();
}
