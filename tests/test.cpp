#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/catch_session.hpp>
#include <filesystem>
#include <set>
#include <string>
#include "../counter.cpp"
#include "../generator.cpp"

std::set<std::string> toDelate;

int main(int argc, char *argv[])
{
    bool notDelateFiles = false;
    Catch::Session session;

    using namespace Catch::Clara;
    auto cli = session.cli() | Opt(notDelateFiles)
                                   ["-N"]["--no-delate"]("should files be deleted after testing");

    session.cli(cli);

    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0)
        return returnCode;

    auto failed =  session.run();
    if (!notDelateFiles)
    {
        for (auto &&file : toDelate)
        {
            std::filesystem::remove(file);
        }
    }

    return failed;
}

TEST_CASE("small file single core", "[single-core]")
{
    int distinctiveWords = 100;
    int allWords = 201;
    auto filePath = simpleGenerator(distinctiveWords, allWords);
    if (filePath)
    {
        Counter c(filePath.value(), 1);
        REQUIRE(c.countWords() == distinctiveWords);
        toDelate.insert(filePath.value());
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
        REQUIRE(c.countWords() == distinctiveWords);
        c.clear_tree();
        c.set_threads(4);
        REQUIRE(c.countWords() == distinctiveWords);
        c.clear_tree();
        c.set_threads(8);
        REQUIRE(c.countWords() == distinctiveWords);
        c.set_threads(16);
        c.clear_tree();
        REQUIRE(c.countWords() == distinctiveWords);
        toDelate.insert(filePath.value());
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
    if (file.is_open())
    {
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
        toDelate.insert("spaces.txt");
    }
}

TEST_CASE("one big word", "[multi-core][single-core]")
{
    std::ofstream file;
    file.open("oneWord.txt");
    if (file.is_open())
    {
        for (size_t i = 0; i < 2048; i++)
        {
            file << 'a';
        }
        file.close();
        Counter c("oneWord.txt", 1);
        REQUIRE(c.countWords() == 1);
        c.set_threads(4);
        c.clear_tree();
        REQUIRE(c.countWords() == 1);
        toDelate.insert("oneWord.txt");
    }
}

TEST_CASE("stability fast", "[multi-core]")
{
    int distinctiveWords = 1000;
    int allWords = 2001;
    auto filePath = simpleGenerator(distinctiveWords, allWords);
    if (filePath)
    {
        Counter c(filePath.value(), 2);
        for (size_t i = 0; i < 50; i++)
        {
            c.clear_tree();
            c.set_threads(2);
            REQUIRE(c.countWords() == distinctiveWords);
            c.clear_tree();
            c.set_threads(4);
            REQUIRE(c.countWords() == distinctiveWords);
            c.clear_tree();
            c.set_threads(8);
            REQUIRE(c.countWords() == distinctiveWords);
            c.set_threads(16);
            c.clear_tree();
            REQUIRE(c.countWords() == distinctiveWords);
        }
        toDelate.insert(filePath.value());
    }
    else
    {
        FAIL();
    }
}

TEST_CASE("multiple spaces", "[multi-core]")
{
    int distinctiveWords = 100;
    int allWords = 201;
    auto filePath = simpleGenerator(distinctiveWords, allWords, "A", 4);
    if (filePath)
    {
        Counter c(filePath.value(), 2);

        REQUIRE(c.countWords() == distinctiveWords);
        toDelate.insert(filePath.value());
    }
    else
    {
        FAIL();
    }
}

TEST_CASE("empty file", "[multi-core][single-core]")
{
    std::ofstream file;
    file.open("empty.txt");
    file.close();
    Counter c("empty.txt", 1);
    REQUIRE(c.countWords() == 0);
    c.set_threads(4);
    REQUIRE(c.countWords() == 0);
    c.set_threads(8);
    REQUIRE(c.countWords() == 0);
    toDelate.insert("empty.txt");
}
TEST_CASE("fast benchmark core scaling", "[multi-core][single-core][!benchmark]")
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
        toDelate.insert(filePath.value());
    }
    else
    {
        FAIL();
    }
}

TEST_CASE("1mb file", "[multi-core]")
{
    auto testFile = semiRandomGenerator(25, 1024 * 1024, Catch::Generators::Detail::getSeed());
    if (testFile)
    {
        Counter c(testFile.value().first, 16);
        REQUIRE(c.countWords() == testFile.value().second);
        toDelate.insert(testFile.value().first);
    }
}

TEST_CASE("100mb file", "[multi-core]")
{
    auto testFile = semiRandomGenerator(25, 1024 * 1024 * 100, Catch::Generators::Detail::getSeed());
    if (testFile)
    {
        Counter c(testFile.value().first, 16);
        REQUIRE(c.countWords() == testFile.value().second);
        toDelate.insert(testFile.value().first);
    }
}

TEST_CASE("1Gb_file", "[.][heavy][multi-core]")
{
    auto testFile = semiRandomGenerator(25, 1024 * 1024 * 1024, Catch::Generators::Detail::getSeed());
    if (testFile)
    {
        Counter c(testFile.value().first, 16);
        REQUIRE(c.countWords() == testFile.value().second);
        toDelate.insert(testFile.value().first);
    }
}
