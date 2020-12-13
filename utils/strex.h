
#pragma once

#include <string>
#include <vector>
#include <tuple>

namespace strex
{
    constexpr
    unsigned int hash(const char* str, int h = 0)
    {
        return !str[h] ? 5381 : (hash(str, h+1)*33) ^ str[h];
    }

    inline
    unsigned int hash(const std::string& str)
    {
        return hash(str.c_str());
    }

    inline
    std::tuple<size_t, size_t> mutli_find(const std::string& str, size_t start, const std::vector<std::string>& patterns)
    {
        auto i = start;
        std::vector<size_t> js(patterns.size(), 0);
        while (i < str.size())
        {
            for (auto pi = 0u; pi < patterns.size(); pi++)
            {
                if (str[i] == patterns[pi][js[pi]])
                {
                    // TODO aborting at the first pattern not longest match...
                    js[pi]++;
                    if (js[pi] == patterns[pi].size())
                    {
                        return std::make_tuple(i, pi);
                    }
                }
                else
                {
                    js[pi] = 0;
                }
            }
            i++;
        }
        return std::make_tuple(std::string::npos, 0);
    }

    inline
    std::vector<std::string> tokenize(const std::string& str, const std::vector<std::string>& delimiters)
    {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = 0;
        size_t pat = 0;

        while ((start != std::string::npos) && (start < str.size()))
        {
            std::tie(end, pat) = mutli_find(str, start, delimiters);

            if (end == std::string::npos)
            {
                auto token = str.substr(start);
                if (!token.empty())
                {
                    tokens.push_back(token);
                }
                start = std::string::npos;
            }
            else
            {
                auto token = str.substr(start, end - start);
                if (!token.empty())
                {
                    tokens.push_back(token);
                }
                tokens.push_back(delimiters[pat]);
                start = end + delimiters[pat].size();
            }
        }

        return tokens;
    }
}

