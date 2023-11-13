#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include "../generator.cpp"
#include "../counter.cpp"

void thread_scaling(unsigned long long size)
{
    auto file = semiRandomGenerator(50, size, Catch::Generators::Detail::getSeed());
    if (file)
    {
        Counter c(file.value().first, 1);
        BENCHMARK("1 core")
        {
            c.countWords();
        };
        c.clear_tree();
        c.set_threads(4);
        BENCHMARK("4 core")
        {
            c.countWords();
        };
        c.clear_tree();
        c.set_threads(8);
        BENCHMARK("8 core")
        {
            c.countWords();
        };
        c.clear_tree();
        c.set_threads(16);
        BENCHMARK("16 core")
        {
            c.countWords();
        };
        c.clear_tree();
        c.set_threads(32);
        BENCHMARK("32 core")
        {
            c.countWords();
        };
        c.clear_tree();
        c.set_threads(4);
    }
}

void different_word_amount(unsigned long long size)
{
    {
        auto file = semiRandomGenerator(100, size, Catch::Generators::Detail::getSeed());
        if (file)
        {
            Counter c(file.value().first, 8);
            BENCHMARK("every word the same")
            {
                c.countWords();
            };
        }
    }
    {
        auto file = semiRandomGenerator(75, size, Catch::Generators::Detail::getSeed());
        if (file)
        {
            Counter c(file.value().first, 8);
            BENCHMARK("75% same words")
            {
                c.countWords();
            };
        }
    }

    {
        auto file = semiRandomGenerator(50, size, Catch::Generators::Detail::getSeed());
        if (file)
        {
            Counter c(file.value().first, 8);
            BENCHMARK("50% same words")
            {
                c.countWords();
            };
        }
    }

    {
        auto file = semiRandomGenerator(25, size, Catch::Generators::Detail::getSeed());
        if (file)
        {
            Counter c(file.value().first, 8);
            BENCHMARK("25% same words")
            {
                c.countWords();
            };
        }
    }

    {
        auto file = semiRandomGenerator(0, size, Catch::Generators::Detail::getSeed());
        if (file)
        {
            Counter c(file.value().first, 8);
            BENCHMARK("every word different")
            {
                c.countWords();
            };
        }
    }
}

TEST_CASE("thread scaling 1Mb", "[light]")
{
    thread_scaling(1024 * 1024);
}

TEST_CASE("thread scaling 254Mb", "[.][medium]")
{
    thread_scaling(1024 * 1024 * 254);
}

TEST_CASE("thread scaling 1Gb", "[.][heavy]")
{
    thread_scaling(1024 * 1024 * 1024);
}

TEST_CASE("file type 1Mb", "[light]")
{
    different_word_amount(1024 * 1024);
}

TEST_CASE("file type 254Mb", "[.][medium]")
{
    different_word_amount(1024 * 1024 * 254);
}

TEST_CASE("file type 1Gb", "[.][heavy]")
{
    different_word_amount(1024 * 1024 * 1024);
}
