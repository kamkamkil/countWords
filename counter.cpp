#include <fstream>
#include <String>
#include <iostream>
#include <set>

int countWordsPOC(std::string filePath)
{
    std::fstream file;
    file.open(filePath);
    std::set<std::string> set;
    std::string word ;
    char k;
    while ((k = file.get()) != EOF)
    {
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
    int a = 0;
    for (auto &&i : set)
    {
        std::cout << a << " : " << i << std::endl;
        a++;  
    }
    
    return set.size();
}