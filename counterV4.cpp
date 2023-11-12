#include <fstream>
#include <String>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <thread>
#include <filesystem>
#include <future>
#include <mutex>
#include <atomic>
#include <syncstream>
class leaf
{
public:
    std::atomic<bool> eow;
    std::mutex eowMutex;
    std::mutex addLetterMutex;
    leaf *children[60];
    leaf() : eow(false), children{nullptr} {};
    ~leaf();
};

leaf::~leaf()
{
    for (size_t i = 0; i < 60; i++)
    {
        delete children[i];
    }
}



class Counter
{
private:
    int threadAmount;
    std::string filePath;
    leaf *root;
    std::atomic<int> wordsCount;
    void singleThread(unsigned long begin, unsigned long end, int tId);
    void addWord(std::string word);
    int countWordsThreads();

public:
    int countWords();
    void clear_tree();
    Counter(std::string filePath, int threadAmount) : threadAmount(threadAmount), filePath(filePath), wordsCount(0) { root = new leaf; };
    ~Counter()
    {
        delete root;
    };
    void set_threads(int t) { threadAmount = t; };

};
void Counter::clear_tree()
{
    delete root;
    root = new leaf;
    wordsCount = 0;
}
void Counter::addWord(std::string word)
{
    leaf *current = root;
    if (word.length() == 0)
        return;
    for (auto &&letter : word)
    {
        {
            if (current->children[letter - 'A'] == nullptr)
            {
                const std::lock_guard<std::mutex> lock(current->addLetterMutex);
                if (current->children[letter - 'A'] == nullptr)
                    current->children[letter - 'A'] = new leaf;
            }
        }
        current = current->children[letter - 'A'];
    }
    if (!current->eow.load())
    {
        {
            const std::lock_guard<std::mutex> lock(current->eowMutex);
            if (!current->eow.load())
            {
                current->eow.store(true);
            }
        }
        wordsCount++;
    }
}

int Counter::countWords()
{
    return countWordsThreads();
}

int Counter::countWordsThreads()
{
    std::fstream file;
    file.open(filePath);
    unsigned long len = std::filesystem::file_size(filePath);
    unsigned long lenForThread = len / threadAmount;
    unsigned long currentPos = 0;
    std::vector<std::thread> threadPool{};
    for (size_t i = 0; i < threadAmount - 1; i++)
    {
        bool addThread = true;
        unsigned long startPos = currentPos;
        currentPos += lenForThread;
        file.seekg(currentPos, std::ios::beg);
        while (file.get() != ' ')
        {
            currentPos--;
            if (!file.seekg(-2, std::ios::cur))
            {
                addThread = false;
                break;
            }
        }
        if (addThread)
        {
            threadPool.push_back(std::thread(&Counter::singleThread, this, startPos, currentPos,i));
            currentPos++;
        }
        else
            currentPos = 0;
    }

    threadPool.push_back(std::thread(&Counter::singleThread, this, currentPos, len,threadAmount));
    file.close();
    for (auto &&i : threadPool)
    {
        i.join();
    }

    return wordsCount;
}

void Counter::singleThread(unsigned long begin, unsigned long end , int tId)
{
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::string word;
    unsigned long pos = begin;
    file.seekg(begin, std::ios::beg);
    while (file.tellg() < end && std::getline(file, word, ' '))
    {
        addWord(word);
    }
    file.close();
}