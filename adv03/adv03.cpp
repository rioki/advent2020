
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

char get_char(const std::vector<std::string>& values, size_t x, size_t y)
{
    auto& line = values.at(x);
    auto i = y % line.size();
    return line[i];
}

size_t get_trees(const std::vector<std::string>& values, size_t dx, size_t dy)
{
    auto trees = 0;
    auto i = 0u;
    auto j = 0u;
    while(i < values.size())
    {
        auto c = get_char(values, i, j);
        if (c == '#')
        {
            trees++;
        }
        i += dx;
        j += dy;
    }
    return trees;
}

int main()
{
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input.txt" << std::endl;
    }

    std::vector<std::string> values;
    while (input.good())
    {
        std::string line;
        std::getline(input, line);
        values.push_back(line);
    }

    auto r11 = get_trees(values, 1, 1);
    auto r13 = get_trees(values, 1, 3);
    auto r15 = get_trees(values, 1, 5);
    auto r17 = get_trees(values, 1, 7);
    auto r21 = get_trees(values, 2, 1);

    std::cout << "Right 1, down 1. " << r11 << std::endl;
    std::cout << "Right 3, down 1. " << r13 << std::endl;
    std::cout << "Right 5, down 1. " << r15 << std::endl;
    std::cout << "Right 7, down 1. " << r17 << std::endl;
    std::cout << "Right 1, down 2. " << r21 << std::endl;
    std::cout << "Result (mult) " << r11 * r13 * r15 * r17 * r21 << std::endl;

    return 0;
}
