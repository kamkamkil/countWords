#include <iostream>
#include <fstream>
#include <string>

//todo add error handling 
// make it better 
int simpleGenerator(
    unsigned long amountOfRecurringWorlds, unsigned long amountOfEveryWord)
{
    if (amountOfEveryWord < amountOfRecurringWorlds)
    {
        return -1;
    }
    std::ofstream file;
    file.open("smpGen" + std::to_string(amountOfRecurringWorlds) +  std::to_string(amountOfEveryWord));
    unsigned long amountOfRepetition = amountOfEveryWord / (amountOfRecurringWorlds - 1);
    unsigned long currentWord = 0;
    unsigned long allWord = amountOfEveryWord - amountOfRecurringWorlds - 1;
    char word = 'a';
    for (size_t i = 0; i < allWord; i++)
    {
        word++;
        for (size_t i = 0; i < amountOfRepetition; i++)
        {
            file << word << ' ';
        }
    }
    for (size_t i = 0; i < amountOfEveryWord - (amountOfRecurringWorlds) * amountOfRepetition; i++)
    {
        file << word << ' ';
    }
    
    file.close();
    return 1;
}
