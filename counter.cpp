#pragma once

#include <fstream>
#include <String>
#include <iostream>
#include <set>
#include <vector>
#include <thread>
#include <filesystem>

int countWordsPOC(std::string filePath)
{
    std::fstream file;
    file.open(filePath);
    std::set<std::string> set;
    std::string word;
    char k;
    while ((k = file.get()) != EOF)
    {
        if (k != ' ')
        {
            word += k;
        }
        else
        {
            set.insert(word);
            word.clear();
        }
    }
    file.close();
    return set.size();
}

int singleThread(std::string filePath, int begin, int end)
{
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::set<std::string> set;
    std::string word;
    char k;
    int pos = begin;
    file.seekg(begin, std::ios::beg);
    while (pos < end && (k = file.get()) != EOF)
    {
        pos++;
        if (k != ' ')
        {
            word += k;
        }
        else
        {
            // std::cout << word << " ";
            set.insert(word);
            word.clear();
        }
    }
    file.close();
    return set.size();
}

// todo check how many words can fit in 32gb
int countWordsThreads(std::string filePath, int threads)
{
    std::fstream file;
    file.open(filePath);
    file.seekg(0, std::ios::end);
    // std::streamsize len = file.tellg();
    unsigned long len  = std::filesystem::file_size(filePath);
    std::cout << "Plik ma dlugosc " << len << " bajtow\n";
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
    std::vector<std::thread> p;
    for (auto &&t : tPos)
    {
        const int a = std::get<0>(t);
        const int b = std::get<1>(t);
         p.push_back(std::thread(singleThread,  std::ref(filePath), std::ref(a), std::ref(b)));
    }
    for (auto &&k : p)
    {
        k.join();
    }

    return 0;
}
