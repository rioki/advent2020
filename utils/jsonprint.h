
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <iomanip>

namespace jsonprint
{
    void print_escaped(std::ostream& os, const std::string& str)
    {
        os << "\"";
        for (size_t i = 0; i < str.size(); i++)
        {
            // non printables
            if (str[i] < 32)
            {
                switch (str[i])
                {
                case '\b':
                    os << "\\b";
                    break;
                case '\f':
                    os << "\\f";
                    break;
                case '\n':
                    os << "\\n";
                    break;
                case '\r':
                    os << "\\r";
                    break;
                case '\t':
                    os << "\\t";
                    break;
                default:
                    os << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (unsigned int)str[i];
                    break;
                }
            }
            else if (str[i] == '"')
            {
                os << "\\\"";
            }
            else if (str[i] == '\\')
            {
                os << "\\\\";
            }
            else
            {
                os << str[i];
            }
        }
        os << "\"";
    }

    struct indent
    {
        unsigned int spaces;
        unsigned int level;

        indent(unsigned int s, unsigned int l)
            : spaces(s), level(l) {}
    };

    std::ostream& operator << (std::ostream& os, indent in)
    {
        unsigned int cnt = in.spaces * in.level;

        for (unsigned int i = 0; i < cnt; i++)
        {
            os << " ";
        }
        return os;
    }

    template <typename T>
    void write(std::ostream& os, unsigned int spaces, unsigned int level, const T& value)
    {
        os << "\"" << value << "\"";
    }

    inline
    void write(std::ostream& os, unsigned int spaces, unsigned int level, const std::string& value)
    {
        print_escaped(os, value);
    }

    template <typename V>
    void write_tuple_value(std::ostream& os, unsigned int spaces, unsigned int level, V value, size_t i, size_t max_size)
    {
        write(os, spaces, level, value);
        if (i != max_size - 1)
        {
            if (spaces == 0)
            {
                os << ",";
            }
            else
            {
                os << ",\n" << indent(spaces, level);
            }
        }
    }

    template <typename Tuple, size_t... I>
    void write_tuple(std::ostream& os, unsigned int spaces, unsigned int level, const Tuple& arr, std::index_sequence<I...> int_seq)
    {
        level++;

        if (spaces == 0)
        {
            os << "[";
        }
        else
        {
            os << "[\n" << indent(spaces, level);
        }

        (write_tuple_value(os, spaces, level, std::get<I>(arr), I, int_seq.size()) , ...);
        level--;

        if (spaces == 0)
        {
            os << "]";
        }
        else
        {
            os << "\n" << indent(spaces, level) << "]";
        }
    }

    template <typename... T>
    void write(std::ostream& os, unsigned int spaces, unsigned int level, const std::tuple<T...>& arr)
    {
        write_tuple(os, spaces, level, arr, std::make_index_sequence<sizeof...(T)>());
    }

    template <typename T>
    void write(std::ostream& os, unsigned int spaces, unsigned int level, const std::vector<T>& arr)
    {
        level++;

        if (spaces == 0)
        {
            os << "[";
        }
        else
        {
            os << "[\n" << indent(spaces, level);
        }

        auto i = arr.begin();
        while (i != arr.end())
        {
            write(os, spaces, level, *i);
            i++;

            if (i != arr.end())
            {
                if (spaces == 0)
                {
                    os << ",";
                }
                else
                {
                    os << ",\n" << indent(spaces, level);
                }
            }
        }

        level--;

        if (spaces == 0)
        {
            os << "]";
        }
        else
        {
            os << "\n" << indent(spaces, level) << "]";
        }
    }

    template <typename K, typename V>
    void write(std::ostream& os, unsigned int spaces, unsigned int level, const std::map<K, V>& obj)
    {
        level++;
        if (spaces == 0)
        {
            os << "{";
        }
        else
        {
            os << "{\n" << indent(spaces, level);
        }

        auto i = obj.begin();
        while (i != obj.end())
        {
            print_escaped(os, i->first);
            if (spaces == 0)
            {
                os << ":";
            }
            else
            {
                os << ": ";
            }

            write(os, spaces, level, i->second);
            i++;

            if (i != obj.end())
            {
                if (spaces == 0)
                {
                    os << ",";
                }
                else
                {
                    os << ",\n" << indent(spaces, level);
                }
            }
        }

        level--;

        if (spaces == 0)
        {
            os << "}";
        }
        else
        {
            os << "\n" << indent(spaces, level) << "}";
        }
    }

    template <typename... T>
    std::ostream& operator << (std::ostream& os, const std::tuple<T...>& arr)
    {
        auto spaces = os.width();
        os.width(0);
        write(os, spaces, 0, arr);
        return os;
    }

    template <typename T>
    std::ostream& operator << (std::ostream& os, const std::vector<T>& arr)
    {
        auto spaces = os.width();
        os.width(0);
        write(os, spaces, 0, arr);
        return os;
    }

    template <typename K, typename V>
    std::ostream& operator << (std::ostream& os, const std::map<K, V>& obj)
    {
        auto spaces = os.width();
        os.width(0);
        write(os, spaces, 0, obj);
        return os;
    }
}
