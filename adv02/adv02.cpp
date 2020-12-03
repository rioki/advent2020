
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>

unsigned int count_letters(const std::string& str, char letter)
{
    unsigned int r = 0u;
    for (auto& c : str)
    {
        if (c == letter)
        {
            r++;
        }
    }
    return r;
}

bool safe_check(const std::string& str, unsigned int pos, char letter)
{
    if (pos < str.length())
    {
        return str[pos] == letter;
    }
    else
    {
        return false;
    }
}

int main()
{
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input.txt" << std::endl;
    }

    unsigned int valid1 = 0;
    unsigned int valid2 = 0;
    while (input.good())
    {
        char dummy1, dummy2;
        unsigned int min;
        unsigned int max;
        char letter;
        std::string password;

        input >> min >> dummy1 >> max >> letter >> dummy2 >> password;
        assert(dummy1 == '-');
        assert(dummy2 == ':');

        auto c = count_letters(password, letter);
        if (c >= min && c <= max)
        {
            valid1++;
        }

        // 1 based indexes
        auto a = safe_check(password, min - 1, letter);
        auto b = safe_check(password, max - 1, letter);
        if ((a && !b) || (!a && b))
        {
            valid2++;
        }
    }

    std::cout << "First: Valid passwords: " << valid1 << std::endl;
    std::cout << "Second: Valid passwords: " << valid2 << std::endl;

    return 0;
}
