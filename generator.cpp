#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <optional>
//fixme after Z there are a couple of symbols 
class Word
{
public:
    Word(/* args */) : word("A"){};
    Word(std::string s) : word(s){};
    ~Word(){};
    void advance()
    {
        if (word.back() != 'z')
        {
            advance_letter(word.back());
        }
        else
        {
            bool flag = true;
            for (size_t i = word.length() - 1; i > 0; i--)
            {
                if (word[i] != 'z')
                {
                    advance_letter(word[i]);
                    flag = false; 
                    break;
                }
            }
            if (flag)
            {
                word = std::string(word.length() + 1,'A');
            }
            
        }
    }
    Word &operator++()
    {
        advance();
        return *this;
    }

    std::string word;
private:
    void advance_letter(char& c)
    {
        if (c == 'Z')
        {
            c = 'a';
        }else
        {
            c++;
        }
    }
};

std::optional<std::string> simpleGenerator(
    unsigned long amountOfRecurringWorlds, unsigned long amountOfEveryWord,std::string startingWord = "A" , int spaces = 1)
{
    if (amountOfEveryWord < amountOfRecurringWorlds)
    {
        return {};
    }
    std::ofstream file;
    file.open("smpGen" + std::to_string(amountOfRecurringWorlds) + std::to_string(amountOfEveryWord));
    if(!file.is_open())
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
            file << word.word << std::string(spaces,' ');
        }
    }
    for (size_t i = 0; i < amountOfEveryWord - (amountOfRecurringWorlds -1)*amountOfRepetition; i++)
    {
        file << word.word << ' ';
    }

    file.close();
    return "smpGen" + std::to_string(amountOfRecurringWorlds) + std::to_string(amountOfEveryWord);
}
