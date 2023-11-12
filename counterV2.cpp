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
constexpr int BUFFER_SIZE = 1024; // todo thing about this type

int loadBuffer(char* buffer,std::fstream* file)
{
    return file->read(&buffer[0], BUFFER_SIZE).gcount();
}

std::unordered_set<std::string> singleThreadV2(std::string filePath, int begin, int end)
{
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::unordered_set<std::string> set;
    std::string word;
    char k;
    int pos = begin;
    char buffer[2][BUFFER_SIZE];
    bool currentBuffer = 0;
    file.seekg(begin, std::ios::beg);
    int word_size = 0;
    int word_begin = 0;
    int word_count = 0;
    int extracted = file.read(&buffer[currentBuffer][0], BUFFER_SIZE).gcount();
    std::string rest;
    while (pos < end)
    {
        auto future = std::async(&loadBuffer,buffer[!currentBuffer],&file);
        // std::cout << "buffor :\\" << std::string(buffer[currentBuffer], BUFFER_SIZE) << "/" << std::endl;
        if (extracted != 0)
        {
            int starting = 0;
            if (buffer[currentBuffer][0] == ' ')
            {
                // std::cout << "word count: " << word_count << " pos : " << pos << " _word_: \\" << rest << "/ word size :" <<rest.size()<< std::endl;
                //  std::cout << rest << ' ';
                word_count++;
                set.insert(rest);
                rest.clear();
                starting = 1;
                word_begin = 1;
            }
            for (size_t i = starting; i < (pos + extracted < end ? extracted : end - pos); i++)
            {
                if (buffer[currentBuffer][i] != ' ')
                {
                    word_size++;
                }
                else
                {
                    word = std::string(rest + std::string(buffer[currentBuffer] + word_begin, word_size));
                    auto w = set.insert(rest + std::string(buffer[currentBuffer] + word_begin, word_size));
                    // pos += word_size;
                    // std::cout << "word count: " << word_count << " pos : " << pos << " -word-: \\" << word << "/ word size :" << word_size <<std::endl;
                    // std::cout << word << ' ';
                    word_count++;
                    word_begin = i + 1;
                    word_size = 0;
                    rest.clear();
                }
            }
            if (word_size != 0)
            {
                rest += std::string(buffer[currentBuffer] + word_begin, word_size);
                word_size = 0;
                // std::cout << "rest =>" << rest << std::endl;
            }
        }
        pos += extracted;
        word_begin = 0;
        extracted = future.get();
        currentBuffer = !currentBuffer;
    }
    file.close();
    return set;
}

// todo check how many words can fit in 32gb
int countWordsThreadsV2(std::string filePath, int threads)
{
    std::fstream file;
    file.open(filePath);
    unsigned long len = std::filesystem::file_size(filePath);
    unsigned long lenForThread = len / threads;
    unsigned long currentPos = 0;
    std::vector<std::future<std::unordered_set<std::string>>> p{}; // todo rename and make new type name or smth
    for (size_t i = 0; i < threads - 1; i++)
    {
        unsigned long startPos = currentPos;
        currentPos += lenForThread;
        file.seekg(currentPos, std::ios::beg);
        while (file.get() != ' ')
        {
            currentPos--;
            file.seekg(-2, std::ios::cur);
        }
        p.push_back(std::async(&singleThreadV2, std::ref(filePath), startPos, currentPos));
        currentPos++;
    }
    p.push_back(std::async(&singleThreadV2, std::ref(filePath), currentPos, len));

    file.close();
    wordSet set;
    for (auto &&k : p)
    {
        set.merge(k.get());
    }

    return set.size();
}
