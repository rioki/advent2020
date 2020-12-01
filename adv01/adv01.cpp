
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int main()
{
    std::vector<int> values;

    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input.txt" << std::endl;
    }

    while (input.good())
    {
        std::string line;
        std::getline(input, line);
        values.push_back(std::stoi(line));
    }

    std::cout << "First" << std::endl;
    for (size_t i = 0; i < values.size(); i++)
    {
        for (size_t j = 0; j < values.size(); j++)
        {
            if (values[i] + values[j] == 2020)
            {
                std::cout << "Found: " << values[i] << " + " << values[j] << " == 2020" << std::endl;
                std::cout << "Result: " << values[i] << " * " << values[j] << " == " << values[i] * values[j] << std::endl;
            }
        }
    }

    std::cout << "Second" << std::endl;
    for (size_t i = 0; i < values.size(); i++)
    {
        for (size_t j = 0; j < values.size(); j++)
        {
            for (size_t k = 0; k < values.size(); k++)
            {
                if (values[i] + values[j] + values[k] == 2020)
                {
                    std::cout << "Found: " << values[i] << " + " << values[j] << " + " << values[k] << " == 2020" << std::endl;
                    std::cout << "Result: " << values[i] << " * " << values[j] << " * " << values[k] << " == " << values[i] * values[j] * values[k] << std::endl;
                }
            }
        }
    }

    return 0;
}
