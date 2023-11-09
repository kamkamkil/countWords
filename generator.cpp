#include <iostream>
#include <fstream>
#include <string>
//fixme after Z there are a couple of symbols 
class Word
{
public:
    Word(/* args */) : word("A"){};
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

// todo add error handling
//  make it better
int simpleGenerator(
    unsigned long amountOfRecurringWorlds, unsigned long amountOfEveryWord)
{
    if (amountOfEveryWord < amountOfRecurringWorlds)
    {
        return -1;
    }
    std::ofstream file;
    file.open("smpGen" + std::to_string(amountOfRecurringWorlds) + std::to_string(amountOfEveryWord));
    unsigned long amountOfRepetition = amountOfEveryWord / (amountOfRecurringWorlds - 1);
    unsigned long currentWord = 0;
    Word word;
    for (size_t i = 0; i < amountOfRecurringWorlds; i++)
    {
        word.advance();
        for (size_t i = 0; i < amountOfRepetition; i++)
        {
            file << word.word << ' ';
        }
    }
    for (size_t i = 0; i < amountOfEveryWord - (amountOfRecurringWorlds -1)*amountOfRepetition; i++)
    {
        file << word.word << ' ';
    }

    file.close();
    return 1;
}
