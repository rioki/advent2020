#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>

std::tuple<int, int> get_seat_number(const std::string& id)
{
    assert(id.size() == 10);

    int row_min = 0;
    int row_max = 127;

    for (auto i = 0u; i < 7; i++)
    {
        int d = row_max - row_min;
        if (id[i] == 'F')
        {
            row_max = row_max - (d / 2) - 1;
        }
        else
        {
            assert(id[i] == 'B');
            row_min = row_min + (d / 2) + 1;
        }
    }

    int col_min = 0;
    int col_max = 7;
    for (auto i = 7u; i < 10; i++)
    {
        int d = col_max - col_min;
        if (id[i] == 'L')
        {
            col_max = col_max - (d / 2) - 1;
        }
        else
        {
            assert(id[i] == 'R');
            col_min = col_min + (d / 2) + 1;
        }
    }

    return std::make_tuple(row_min, col_min);
}

int get_seat_id(int row, int col)
{
    return row * 8 + col;
}

int main()
{
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input.txt" << std::endl;
    }

    std::vector<std::string> lines;
    while (input.good())
    {
        std::string line;
        std::getline(input, line);
        lines.push_back(line);
    }

    int max_id = 0;
    std::vector<bool> seats(1016, false);
    for (const auto& line : lines)
    {
        auto [row, col] = get_seat_number(line);
        auto id = get_seat_id(row, col);
        assert(seats[id] == false);
        seats[id] = true;
        max_id = std::max(id, max_id);
    }

    std::cout << "Max id is: " << max_id << std::endl;

    for (auto i = 0u; i < seats.size(); i++)
    {
        if (seats[i] == true)
        {
            std::cout << " X ";
        }
        else
        {
            std::cout << " _ ";
        }
        if (i % 8 == 7)
        {
            std::cout << std::endl;
        }
    }

    std::cout << "Open seats are: ";
    for (auto i = 0u; i < seats.size() - 3; i++)
    {
        if (seats[i] == true && seats[i+1] == false && seats[i+2] == true)
        {
            std::cout << i + 1 << " ";
        }
    }


}
