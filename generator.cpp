#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <utility>
#include <random>
class Word
{
public:
    Word(/* args */) : word("a"){};
    Word(std::string s) : word(s){};
    ~Word(){};
    void advance()
    {
        if (word.back() != 'z')
        {
            word.back()++;
        }
        else
        {
            bool sameLetter = true;
            for (size_t i = word.length() - 1; i > 0; i--)
            {
                if (word[i] != 'z')
                {
                    word[i]++;
                    sameLetter = false;
                    break;
                }
            }
            if (sameLetter)
            {
                word = std::string(word.length() + 1, 'a');
            }
        }
    }
    Word &operator++()
    {
        advance();
        return *this;
    }

    std::string word;
};

std::optional<std::string> simpleGenerator(
    unsigned long amountOfRecurringWorlds, unsigned long amountOfEveryWord, std::string startingWord = "a", int spaces = 1)
{
    if (amountOfEveryWord < amountOfRecurringWorlds)
    {
        return {};
    }
    std::ofstream file;
    file.open("smpGen" + std::to_string(amountOfRecurringWorlds) + std::to_string(amountOfEveryWord));
    if (!file.is_open())
    {
        return {};
    }
    unsigned long amountOfRepetition = amountOfEveryWord / (amountOfRecurringWorlds - 1);
    unsigned long currentWord = 0;
    Word word(startingWord);
    for (size_t i = 0; i < amountOfRecurringWorlds; i++)
    {
        word.advance();
        for (size_t i = 0; i < amountOfRepetition; i++)
        {
            file << word.word << std::string(spaces, ' ');
        }
    }
    for (size_t i = 0; i < amountOfEveryWord - (amountOfRecurringWorlds - 1) * amountOfRepetition; i++)
    {
        file << word.word << ' ';
    }

    file.close();
    return "smpGen" + std::to_string(amountOfRecurringWorlds) + std::to_string(amountOfEveryWord);
}

std::optional<std::pair<std::string, unsigned long>> semiRandomGenerator(int probability, unsigned long size, uint32_t seed)
{
    std::ofstream file;
    std::string fileName = "semiRand" + std::to_string(probability) + std::to_string(size);
    file.open(fileName);
    if (!file.is_open())
    {
        return {};
    }
    unsigned long currentSize = 0;
    Word word;
    std::random_device rd;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(1, 100);
    std::vector<Word> usedWords;
    usedWords.push_back(word);
    file << word.word << ' ';
    while (currentSize < size)
    {
        auto k = dist(gen); 
        if (k < probability)
        {
            if (usedWords.size() > 1)
            {
                std::uniform_int_distribution<> dist_words(1, usedWords.size());
                auto randomWord = usedWords[dist_words(gen) - 1].word;
                file << randomWord << ' ';
                currentSize += randomWord.length();
            }else
            file << word.word;
        }
        else
        {
            word.advance();
            file << word.word << ' ';
            currentSize += word.word.length();
            usedWords.push_back(word.word);
        }
        currentSize++;
    }
    file.close();
    return std::make_pair(fileName, usedWords.size());
}
