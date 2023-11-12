#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "../counterV4.cpp"
#include "../generator.cpp"
#include <filesystem>
#include "../counter.cpp"

TEST_CASE("small file single core", "[single-core]")
{
    int distinctiveWords = 100;
    int allWords = 201;
    auto filePath = simpleGenerator(distinctiveWords, allWords);
    if (filePath)
    {
        Counter c(filePath.value(), 1);
        REQUIRE(c.countWords() == 100);
    }
    else
    {
        FAIL();
    }
}
TEST_CASE("small file multi core", "[multi-core]")
{
    int distinctiveWords = 100;
    int allWords = 201;
    auto filePath = simpleGenerator(distinctiveWords, allWords);
    if (filePath)
    {
        Counter c(filePath.value(), 2);
        REQUIRE(c.countWords() == 100);
        c.clear_tree();
        c.set_threads(4);
        REQUIRE(c.countWords() == 100);
        c.clear_tree();
        c.set_threads(8);
        REQUIRE(c.countWords() == 100);
        c.set_threads(16);
        c.clear_tree();
        REQUIRE(c.countWords() == 100);
    }
    else
    {
        FAIL();
    }
}

TEST_CASE("only spaces", "[multi-core][single-core]")
{
    std::ofstream file;
    file.open("spaces.txt");
    for (size_t i = 0; i < 2048; i++)
    {
        file << ' ';
    }
    file.close();
    Counter c("spaces.txt", 1);
    REQUIRE(c.countWords() == 0);
    c.set_threads(4);
    c.clear_tree();
    REQUIRE(c.countWords() == 0);
}

TEST_CASE("one big word", "[multi-core][single-core]")
{
    std::ofstream file;
    file.open("oneWord.txt");
    for (size_t i = 0; i < 2048; i++)
    {
        file << 'a';
    }
    file.close();
    Counter c("oneWord.txt", 1);
    REQUIRE(c.countWords() == 1);
    c.set_threads(4);
    REQUIRE(c.countWords() == 1);
}
TEST_CASE("fast benchmark ", "[multi-core][single-core][!benchmark]")
{
    int distinctiveWords = 50;
    int allWords = 2001;
    auto filePath = simpleGenerator(distinctiveWords, allWords);
    if (filePath)
    {
        Counter c(filePath.value(), 1);
        BENCHMARK("1 core ")
        {
            c.countWords();
        };
        c.set_threads(4);
        c.clear_tree();
        BENCHMARK("4 core ")
        {
            c.countWords();
        };
        c.set_threads(8);
        c.clear_tree();

        BENCHMARK("8 core ")
        {
            c.countWords();
        };
        c.set_threads(16);
        c.clear_tree();

        BENCHMARK("16 core ")
        {
            c.countWords();
        };
    }
    else
    {
        FAIL();
    }
}
