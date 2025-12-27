#include <cctype>
#include <stdarg.h>
#include <chrono>


#include <slogger/StringUtils.hpp>

namespace StringUtils
{
std::string to_TAI_timestamp(const std::chrono::milliseconds& timepoint)
{
    using namespace std::chrono;
    const auto secs = duration_cast<seconds>(timepoint).count();
    const auto nsecs =
        duration_cast<nanoseconds>(timepoint - seconds(secs)).count();
    return std::to_string(secs) + ":" + std::to_string(nsecs);
}

// we get name lists like: [foo, local]
bool last_item_equals(const std::vector<std::string>& name_list, const std::string& last_item_name)
{
    if (name_list.empty())
    {
        return false;
    }
    const auto& last = name_list.back();
    if (last.size() >= last_item_name.size())
    {
        if (last == last_item_name)
        {
            return true;
        }
    }
    return false;
}

std::string to_upper(const std::string& s)
{
    std::string ret;
    for (char c : s)
    {
        ret += toupper(c);
    }
    return ret;
}


std::string remove(const std::string_view& in, char delete_char)
{
    std::string s;
    for (char ch : in)
    {
        if (ch != delete_char)
        {
            s += ch;
        }
    }
    return s;
}


std::string replace(const std::string_view& in, char from, char to)
{
    std::string s;
    for (char ch : in)
    {
        if (ch == from)
        {
            ch = to;
        }
        s += ch;
    }
    return s;
}


bool ends_with(const std::string_view& s, const std::string_view& end)
{
    if (end.length() > s.length())
    {
        return false;
    }
    if (s.substr(s.length() - end.length()) == end)
    {
        return true;
    }
    return false;
}


std::string trim(const std::string_view& s)
{
    if (s.length() == 0)
        return std::string(s);

    size_t i = s.length() - 1;
    while (i > 0)
    {
        char c = s[i];
        if (!isspace(c))
        {
            break;
        }
        i--;
    }
    return std::string(s.substr(0, i + 1));
}

std::optional<uint32_t> hex_string_to_int(const std::string& s)
{
    errno = 0;
    char* end = nullptr;
    uint32_t ret = strtol(s.c_str(), &end, 16);
    if (errno != 0)
    {
        return std::nullopt;
    }
    return ret;
}


std::optional<int32_t> parse_int(const std::string& s)
{
    errno = 0;
    char* end = nullptr;
    uint32_t ret = strtol(s.c_str(), &end, 10);
    if (errno != 0)
    {
        return std::nullopt;
    }
    if (end == s.c_str())
    {
        return std::nullopt;
    }
    return ret;
}


std::string to_string(const ToStringMixin& m)
{
    return m.to_string();
}

std::string to_string(const std::vector<std::string>::const_iterator& first,
    const std::vector<std::string>::const_iterator& last,
    const std::string& sep)
{
    std::string ret;
    if (sep == ", ") {
        ret += "[";
    }
    std::string comma = "";
    for (auto iter = first; iter != last; ++iter)
    {
        ret += comma;
        ret += *iter;
        comma = sep;
    }
    if (sep == ", ") {
        ret += "]";
    }
    return ret;
}

std::string to_string(const std::vector<std::string>& list,
    const std::string& sep)
{
    return to_string(list.cbegin(), list.cend(), sep);
}



std::string to_mdns_string(const std::vector<std::string>& list)
{
    std::string ret;
    for (const auto& e : list)
    {
        char ch = e.length();
        ret.append(std::string(&ch, 1));
        ret += e;
    }

    char terminator = 0;
    ret.append(&terminator, 1);
    return ret;
}


std::vector<std::string> split(const std::string_view& s, const char sep)
{
    std::string sub;
    std::vector<std::string> ret;
    for (std::size_t i = 0; i < s.length(); i++)
    {
        const char ch = s[i];
        if (ch == sep)
        {
            if (sub.length() > 0)
            {
                ret.push_back(sub);
            }
            sub = "";
        }
        else
        {
            sub += ch;
        }
    }
    if (sub.length() > 0)
    {
        ret.push_back(sub);
    }
    return ret;
}

} // namespace StringUtils
