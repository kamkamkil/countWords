#include <iostream>
#include "generator.cpp"
// #include "counterV2.cpp"
#include "counterV4.cpp"
// #include "counter.cpp"
// #include "counter.cpp"
#include <thread>

void usage()
{
        std::cout << "wordCounter:" << std::endl;
        std::cout << "usage:" << std::endl;
        std::cout << "  wordCounter [FILE PATH] option" << std::endl;
        std::cout << "where option are:" << std::endl;
        std::cout << "-?, -h, --help                  display usage information" << std::endl;
        std::cout << "-t, --threads                   set how many threads will be used " << std::endl;
        std::cout << "-q, --quiet                     disable printing to standard output " << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "usage:" << std::endl;
        std::cout << "  wordCounter [FILE PATH] option" << std::endl;
        return -1;
    }
    if (argv[1][0] == '-')
    {
        usage();
        return 0;
    }
    std::string filePath(argv[1]);
    bool quiet = false;
    int threadsAmount = std::thread::hardware_concurrency();
    for (size_t i = 2; i <= argc - 1; i++)
    {
        std::string options(argv[i]);
        if (options == "-t" || options == "--threads")
            if (i + 1 >= argc)
                threadsAmount = atoi(argv[i + 1]);
        if (options == "-?" || options == "-h" || options == "--help")
            usage();
        if (options == "-q" || options == "--quiet")
            quiet = true;
    }
    if (threadsAmount == 1)
    {
    std::cout << "WARNING: hardware_concurrency recommend using 1 thread use -t to set how many thread program should use" << std::endl;
    }
    
    Counter c(filePath,threadsAmount);

    auto amount = c.countWords();
    if (!quiet)
    {
        std::cout << amount << std::endl;
    }

    return amount;    
}

