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
constexpr size_t CHILDREN_TAB_SIZE = 26;
class leaf
{
public:
    bool eow = false;
    std::mutex eowMutex;
    std::mutex addLetterMutex;
    leaf *children[CHILDREN_TAB_SIZE];
    leaf() : eow(false), children{nullptr} {};
    ~leaf();
};

leaf::~leaf()
{
    for (size_t i = 0; i < CHILDREN_TAB_SIZE; i++)
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
    std::atomic<unsigned long long> wordsCount;
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
        if (current->children[letter - 'a'] == nullptr)
        {
            const std::lock_guard<std::mutex> lock(current->addLetterMutex);
            if (current->children[letter - 'a'] == nullptr)
                current->children[letter - 'a'] = new leaf;
        }
        current = current->children[letter - 'a'];
    }
    if (not current->eow)
    {
        const std::lock_guard<std::mutex> lock(current->eowMutex);
        if (not current->eow)
        {
            current->eow = true;
            wordsCount++;
        }
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
            if (not file.seekg(-2, std::ios::cur))
            {
                addThread = false;
                break;
            }
        }
        if (addThread)
        {
            threadPool.push_back(std::thread(&Counter::singleThread, this, startPos, currentPos, i));
            currentPos++;
        }
        else
            currentPos = 0;
    }

    threadPool.push_back(std::thread(&Counter::singleThread, this, currentPos, len, threadAmount));
    file.close();
    for (auto &&i : threadPool)
    {
        i.join();
    }

    return wordsCount;
}

void Counter::singleThread(unsigned long begin, unsigned long end, int tId)
{
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::string word;
    unsigned long pos = begin;
    file.seekg(begin, std::ios::beg);
    while (file.tellg() < end and std::getline(file, word, ' '))
    {
        addWord(word);
    }
    file.close();
}