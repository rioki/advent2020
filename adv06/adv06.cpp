#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <map>

int main()
{
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input.txt" << std::endl;
    }

    std::vector<std::tuple<int, std::map<char, int>>> customs_forms;
    customs_forms.push_back(std::make_tuple(0, std::map<char, int>()));

    while (input.good())
    {
        std::string line;
        std::getline(input, line);

        for (auto& c : line)
        {
            auto i = std::get<1>(customs_forms.back()).find(c);
            if (i != std::get<1>(customs_forms.back()).end())
            {
                i->second++;
            }
            else
            {
                std::get<1>(customs_forms.back()).insert(std::make_pair(c, 1));
            }
        }

        if (line.empty())
        {
            customs_forms.push_back(std::make_tuple(0, std::map<char, int>()));
        }
        else
        {
            std::get<0>(customs_forms.back())++;
        }
    }

    std::cout << "Got " << customs_forms.size() << " custom forms." << std::endl;

    auto sum1 = 0u;
    for (const auto& [count, form] : customs_forms)
    {
        sum1 += form.size();
    }
    std::cout << "Sum 1 " << sum1 << std::endl;

    auto sum2 = 0u;
    for (const auto& [travelers, form]  : customs_forms)
    {
        for (const auto& [question, count] : form)
        {
            if (travelers == count)
            {
                sum2++;
            }
        }
    }
    std::cout << "Sum 2 " << sum2 << std::endl;
}
