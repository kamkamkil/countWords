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
private:
public:
    std::atomic<bool> eow;
    std::mutex mutex;
    std::mutex mutex2;
    leaf *children[64];
    leaf() : eow(false), children{nullptr} { a = 1; };
    ~leaf();
    int a;
};
class Counter
{
private:
    int threadAmount;
    std::string filePath;
    leaf *root;
    std::atomic<int> words;
    void singleThread(std::string filePath, unsigned long begin, unsigned long end, int coreId);
    void addWord(std::string word);
    int countWordsThreads();

public:
    int countWords();
    void clear_tree();
    Counter(std::string filePath, int threadAmount) : threadAmount(threadAmount), filePath(filePath), words(0) { root = new leaf; };
    ~Counter()
    {
        delete root;
    };
    void set_threads(int t) { threadAmount = t; };

private:
};
void Counter::clear_tree()
{
    delete root;
    root = new leaf;
    words = 0;
}
leaf::~leaf()
{
    for (size_t i = 0; i < 64; i++)
    {
        delete children[i];
    }
}
void Counter::addWord(std::string word)
{
    leaf *ptr = root;
    if (word.length() == 0)
        return;
    for (auto &&letter : word)
    {
        {
            if (ptr->children[letter - 'A'] == nullptr)
            {
                const std::lock_guard<std::mutex> lock(ptr->mutex);
                if (ptr->children[letter - 'A'] == nullptr)
                    ptr->children[letter - 'A'] = new leaf;
            }
        }
        ptr = ptr->children[letter - 'A'];
    }
    if (!ptr->eow.load())
    {
        {
            const std::lock_guard<std::mutex> lock(ptr->mutex);
            if (!ptr->eow.load())
            {
                ptr->eow.store(true);
            }
        }
        words++;
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
    std::vector<std::thread> p{}; // todo rename and make new type name or smth
    for (size_t i = 0; i < threadAmount - 1; i++)
    {
        unsigned long startPos = currentPos;
        currentPos += lenForThread;
        file.seekg(currentPos, std::ios::beg);
        while (file.get() != ' ')
        {
            currentPos--;
            file.seekg(-2, std::ios::cur);
        }
        p.push_back(std::thread(&Counter::singleThread, this, std::ref(filePath), startPos, currentPos, i));
        std::cout << "start " << startPos << "end " << currentPos << std::endl;
        currentPos++;
    }
        std::cout << "start " << currentPos << "end " << len << std::endl;

    p.push_back(std::thread(&Counter::singleThread, this, std::ref(filePath), currentPos, len, threadAmount));
    file.close();
    for (auto &&i : p)
    {
        i.join();
    }

    return words;
}

void Counter::singleThread(std::string filePath, unsigned long begin, unsigned long end, int coreId)
{
    // std::cout << "start thread : " << coreId << std::endl;
    std::fstream file;
    file.open(filePath, std::ios_base::in);
    std::string word;
    char k;
    unsigned long pos = begin;
    file.seekg(begin, std::ios::beg);
    while (pos < end && std::getline(file, word, ' '))
    {
        pos += word.length();
        addWord(word);
    }
    file.close();
}