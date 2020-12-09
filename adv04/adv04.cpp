
#include <cassert>
#include <string>
#include <optional>
#include <tuple>
#include <vector>
#include <iostream>
#include <fstream>

enum class Token
{
    NONE,
    NEW_LINE,
    WHITE_SPACE,
    COLON,
    CDATA,
    IDENTIFIER,
    END_OF_FILE
};

enum class Keyword
{
    BYR,
    IYR,
    EYR,
    HGT,
    HCL,
    ECL,
    PID,
    CID
};

struct Passport
{
    std::optional<std::string> byr; // Birth Year
    std::optional<std::string> iyr; // Issue Year
    std::optional<std::string> eyr; // Expiration Year
    std::optional<std::string> hgt; // Height
    std::optional<std::string> hcl; // Hair Color
    std::optional<std::string> ecl; // Eye Color
    std::optional<std::string> pid; // Passport ID
    std::optional<std::string> cid; // Country ID)
};

struct ParseError : public std::runtime_error
{
    ParseError(const std::string& msg)
    : std::runtime_error(msg) {}
};

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

class PassportParser
{
public:

    explicit PassportParser(std::istream& i)
    : input(i)
    {
        get_next_token();
    }

    std::vector<Passport> parse_passports()
    {
        std::vector<Passport> result;

        while (next_token != Token::END_OF_FILE)
        {
            auto passport = parse_passport();
            result.push_back(passport);
            parse_newline();
        }

        return result;
    }

    Passport parse_passport()
    {
        Passport result;
        while (next_token != Token::NEW_LINE && next_token != Token::END_OF_FILE)
        {
            auto [keyword, value] = parse_key_value_pair();
            parse_whitespace();

            switch (keyword)
            {
                case Keyword::BYR:
                    result.byr = value;
                    break;
                case Keyword::IYR:
                    result.iyr = value;
                    break;
                case Keyword::EYR:
                    result.eyr = value;
                    break;
                case Keyword::HGT:
                    result.hgt = value;
                    break;
                case Keyword::HCL:
                    result.hcl = value;
                    break;
                case Keyword::ECL:
                    result.ecl = value;
                    break;
                case Keyword::PID:
                    result.pid = value;
                    break;
                case Keyword::CID:
                    result.cid = value;
                    break;
                default:
                    assert(false);
                    break;
            }
        }
        return result;
    }

    std::tuple<Keyword, std::string> parse_key_value_pair()
    {
        auto keyword = parse_keyword();
        parse_colon();
        auto value = parse_value();
        return std::make_tuple(keyword, value);
    }

    Keyword parse_keyword()
    {
        auto [token, value] = get_next_token();
        if (token == Token::IDENTIFIER)
        {
            switch (hash(value))
            {
                case hash("byr"):
                    return Keyword::BYR;
                case hash("iyr"):
                    return Keyword::IYR;
                case hash("eyr"):
                    return Keyword::EYR;
                case hash("hgt"):
                    return Keyword::HGT;
                case hash("hcl"):
                    return Keyword::HCL;
                case hash("ecl"):
                    return Keyword::ECL;
                case hash("pid"):
                    return Keyword::PID;
                case hash("cid"):
                    return Keyword::CID;
                default:
                    throw ParseError("Expected keyword got " + value);
            }
        }
        else
        {
            throw ParseError("Expected identifier got " + value);
        }
    }

    void parse_colon()
    {
        auto [token, value] = get_next_token();
        if (token !=  Token::COLON)
        {
            throw ParseError("Expected ':' got " + value);
        }
    }

    std::string parse_value()
    {
        auto [token, value] = get_next_token();
        if (token !=  Token::CDATA)
        {
            throw ParseError("Expected value got " + value);
        }
        return value;
    }

    void parse_whitespace()
    {
        auto [token, value] = get_next_token();
        if (token != Token::WHITE_SPACE && token != Token::NEW_LINE && token != Token::END_OF_FILE)
        {
            throw ParseError("Expected white space got " + value);
        }
    }

    void parse_newline()
    {
        auto [token, value] = get_next_token();
        if (token != Token::NEW_LINE && token != Token::END_OF_FILE)
        {
            throw ParseError("Expected new line got " + value);
        }
    }

    std::tuple<Token, std::string> get_next_token()
    {
        this_token = next_token;
        this_value = next_value;
        std::tie(next_token, next_value) = lex_token();
        return std::make_tuple(this_token, this_value);
    }

    std::tuple<Token, std::string> lex_token()
    {
        if (next_token == Token::COLON)
        {
            return lex_cdata();
        }
        else
        {
            switch (input.peek())
            {
                case EOF:
                    return std::make_tuple(Token::END_OF_FILE, std::string());
                case ' ': case '\t':
                    return lex_whitespace();
                case '\r': case '\n':
                    return lex_newline();
                case ':':
                    return std::make_tuple(Token::COLON, std::string(static_cast<char>(input.get()), 1));
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                case 'y': case 'z':
                    return lex_identifier();
                default:
                {
                    auto c = input.get();
                    throw ParseError("Expected token got " + std::string(c, 1));
                }
            }
        }
    }

    std::tuple<Token, std::string> lex_newline()
    {
        auto c = input.get();
        if (c != '\r' && c != '\n')
        {
            throw ParseError(std::string("Expected newline got ") + std::string(c, 1));
        }
line++;
if (c == '\r' && input.peek() == '\n')
{
    input.get();
    return std::make_tuple(Token::NEW_LINE, std::string("\r\n"));
}
return std::make_tuple(Token::NEW_LINE, std::string(c, 1));
    }

    std::tuple<Token, std::string> lex_whitespace()
    {
        std::string value;
        while (true)
        {
            switch (input.peek())
            {
            case ' ': case '\t':
                value.push_back(static_cast<char>(input.get()));
                break;
            default:
                return std::make_tuple(Token::WHITE_SPACE, value);
            }
        }
    }

    std::tuple<Token, std::string> lex_identifier()
    {
        std::string value;
        while (true)
        {
            switch (input.peek())
            {
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
            case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
            case 's': case 't': case 'u': case 'v': case 'w': case 'x':
            case 'y': case 'z':
                value.push_back(static_cast<char>(input.get()));
                break;
            default:
                return std::make_tuple(Token::IDENTIFIER, value);
            }
        }
    }

    std::tuple<Token, std::string> lex_cdata()
    {
        std::string value;
        while (true)
        {
            switch (input.peek())
            {
            case ' ': case '\t': case '\n': case ':': case EOF:
                return std::make_tuple(Token::CDATA, value);
            default:
                value.push_back(static_cast<char>(input.get()));
                break;
            }
        }
    }

private:
    std::istream& input;
    unsigned int line = 1;

    Token next_token;
    Token this_token;
    std::string next_value;
    std::string this_value;

};

bool validate_byr(const std::string& value)
{
    size_t s = 0;
    auto v = std::stoi(value, &s);
    return s == 4 && v >= 1920 && v <= 2002;
}

bool validate_iyr(const std::string& value)
{
    size_t s = 0;
    auto v = std::stoi(value, &s);
    return s == 4 && v >= 2010 && v <= 2020;
}

bool validate_eyr(const std::string& value)
{
    size_t s = 0;
    auto v = std::stoi(value, &s);
    return s == 4 && v >= 2020 && v <= 2030;
}

bool validate_hgt(const std::string& value)
{
    size_t s = 0;
    auto v = std::stoi(value, &s);
    auto unit = value.substr(s);
    if (unit == "cm")
    {
        return v >= 150 && v <= 193;
    }
    else if (unit == "in")
    {
        return v >= 59 && v <= 76;
    }
    else
    {
        return false;
    }
}

bool validate_hcl(const std::string& value)
{
    if (value[0] == '#')
    {
        size_t s = 0;
        auto v = std::stoi(value.substr(1), &s, 16);
        assert(v != -1);
        return s == 6;
    }
    else
    {
        return false;
    }
}

bool validate_ecl(const std::string& value)
{
    switch (hash(value))
    {
        case hash("amb"):
        case hash("blu"):
        case hash("brn"):
        case hash("gry"):
        case hash("grn"):
        case hash("hzl"):
        case hash("oth"):
            return true;
        default:
            return false;
    }
}

bool validate_pid(const std::string& value)
{
    if (value.size() == 9)
    {
        for (auto& c : value)
        {
            if (!std::isdigit(c))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool validate(const Passport& passport)
{
    return passport.byr.has_value() && validate_byr(passport.byr.value()) &&
           passport.iyr.has_value() && validate_iyr(passport.iyr.value()) &&
           passport.eyr.has_value() && validate_eyr(passport.eyr.value()) &&
           passport.hgt.has_value() && validate_hgt(passport.hgt.value()) &&
           passport.hcl.has_value() && validate_hcl(passport.hcl.value()) &&
           passport.ecl.has_value() && validate_ecl(passport.ecl.value()) &&
           passport.pid.has_value() && validate_pid(passport.pid.value());
}

int main()
{
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input.txt" << std::endl;
    }

    PassportParser parser(input);
    auto passports = parser.parse_passports();

    auto valid = 0u;
    for (auto& passport : passports)
    {
        if (validate(passport))
        {
            valid++;
        }
    }

    std::cout << "Read " << passports.size() << " passports." << std::endl;
    std::cout << "  " << valid << " valid." << std::endl;

    return 0;
}
