#include "StringFormatter.h"

#include <utility>
#include <iostream>

StringFormatter::StringFormatter(const std::string& string) :
    m_original(string)
{

}
StringFormatter::StringFormatter(std::string&& string) :
    m_original(std::move(string))
{

}

const std::string& StringFormatter::originalString() const
{
    return m_original;
}

std::string StringFormatter::formattedString(const std::vector<StringFormatter::Rule>& rules) const
{
    return replaceOccurences(m_original, rules);
}
std::string StringFormatter::formatString(const std::string& string, const std::vector<Rule>& rules)
{
    return replaceOccurences(string, rules);
}

std::string StringFormatter::replaceOccurences(const std::string& originalStr, const std::vector<StringFormatter::Rule>& rules)
{
    std::string str{originalStr};
    for(const auto& rule : rules)
    {
        const std::string& old = rule.original;
        const std::string& replacement = rule.replacement;
        size_t n = 0;
        while ((n = str.find(old, n)) != std::string::npos)
        {
            str.replace(n, old.size(), replacement);
            n += replacement.size();
        }
    }
    return str;
}
