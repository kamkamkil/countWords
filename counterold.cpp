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

std::unordered_set<std::string> singleThread(std::string filePath, unsigned long begin, unsigned long end, int coreId)
{
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::unordered_set<std::string> set;
    std::string word;
    char k;
    unsigned long pos = begin;
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
    unsigned long len  = std::filesystem::file_size(filePath);
    unsigned long  lenForThread = len / threads;
    unsigned long  currentPos = 0;
    std::vector<std::future<std::unordered_set<std::string>>> p{}; // todo rename and make new type name or smth 
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
        p.push_back( std::async(&singleThread,  std::ref(filePath), startPos, currentPos,i));
        currentPos++;
    }
    p.push_back( std::async(&singleThread,  std::ref(filePath), currentPos, len,threads));

    file.close();
    wordSet set;
    for (auto &&k : p)
    {
        set.merge(k.get());
    }

    return set.size();
}
