#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include <algorithm>

using namespace std::literals::string_literals;

// rule := color "bags contains" bag_rules
// bag_rules := bag_rule |
//              bag_rules "," bag_rule
// bag_rule := NUMBER color ("bag" | "bags")
// color := IDENTIFIER IDENTIGIER


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

using RuleTarget = std::vector<std::tuple<size_t, std::string>>;
using RuleSet = std::map<std::string, RuleTarget>;

void add_rule(RuleSet& rules, const std::string& srule)
{
    auto tokens = tokenize(srule, {" "s, ","s, "."s});

    assert(7 < tokens.size());
    assert(tokens[1] == " ");
    assert(tokens[3] == " ");
    auto source_color = tokens[0] + " "s + tokens[2];
    assert(tokens[4] == "bags");
    assert(tokens[5] == " ");
    assert(tokens[6] == "contain");
    assert(tokens[7] == " ");

    RuleTarget target;

    auto i = 8u;
    while (i < tokens.size())
    {
        // 1 clear lime bag.
        assert(i + 5 < tokens.size());
        if (tokens[i] == "no")
        {
            assert(tokens[i+1] == " ");
            assert(tokens[i+2] == "other");
            assert(tokens[i+3] == " ");
            assert(tokens[i+4] == "bags");
            assert(tokens[i+5] == ".");
            i += 6;
        }
        else
        {
            assert(i + 7 < tokens.size());
            assert(std::all_of(tokens[i].begin(), tokens[i].end(), std::isdigit));
            auto count = std::stoi(tokens[i]);
            assert(tokens[i+1] == " ");
            assert(tokens[i+3] == " ");
            assert(tokens[i+5] == " ");
            auto target_color = tokens[i+2] + " "s + tokens[i+4];
            assert(tokens[i+6] == "bags" || tokens[i+6] == "bag");
            if (tokens[i + 7] == ",")
            {
                assert(tokens[i+8] == " ");
                i += 9;
            }
            else
            {
                assert(tokens[i+7] == ".");
                i += 8;
            }
            target.push_back(std::make_tuple(count, target_color));
        }
    }

    assert(rules.find(source_color) == rules.end());
    rules[source_color] = target;
}

std::map<std::string, std::set<std::string>> reverse_rules(const RuleSet& rules)
{
    std::map<std::string, std::set<std::string>> result;

    for (const auto& [source, targets] : rules)
    {
        for (const auto& [count, target] : targets)
        {
            auto i = result.find(target);
            if (i != result.end())
            {
                i->second.insert(source);
            }
            else
            {
                result[target] = {source};
            }
        }
    }

    return result;
}

auto get_color_conainters(const RuleSet& rules, const std::string& target)
{
    auto rrules = reverse_rules(rules);

    std::set<std::string> result = rrules[target];
    std::queue<std::string> todo;

    for (const auto& color : result)
    {
        todo.push(color);
    }

    while (!todo.empty())
    {
        auto tmp = rrules[todo.front()];
        for (const auto& t : tmp)
        {
            auto [iter, didInsert] = result.insert(t);
            if (didInsert)
            {
                todo.push(t);
            }
        }
        todo.pop();
    }
    return result;
}

void get_contained_bags_impl(std::vector<std::string>& result, const RuleSet& rules, const std::string& source)
{
    auto i = rules.find(source);
    assert(i != rules.end());

    const auto& targets = i->second;
    for (auto [count, target] : targets)
    {
        for (auto c = 0u; c < count; c++)
        {
            result.push_back(target);
            get_contained_bags_impl(result, rules, target);
        }
    }
}

auto get_contained_bags(const RuleSet& rules, const std::string& source)
{
    std::vector<std::string> result;
    get_contained_bags_impl(result, rules, source);
    return result;
}

int main()
{
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input.txt" << std::endl;
    }

    RuleSet ruleset;

    while (input.good())
    {
        std::string line;
        std::getline(input, line);

        auto tokens = tokenize(line, {" ", ",", "."});

        add_rule(ruleset, line);
    }

    auto colors = get_color_conainters(ruleset, "shiny gold");

    std::cout << "The shiny gold bag can be contained in " << colors.size() << " bag colors." << std::endl;

    auto cb = get_contained_bags(ruleset, "shiny gold");
    std::cout << "The shiny gold bag must contain " << cb.size() << " bags." << std::endl;
}
