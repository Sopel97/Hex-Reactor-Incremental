#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <vector>
#include <string>
#include <functional>

template <class T>
class Description
{
public:
    class DescriptionPart
    {
    public:
        DescriptionPart();
        DescriptionPart(const std::string& description);
        DescriptionPart(std::function<bool(const T&)>&& predicate, const std::string& description);
        bool predicate(const T& predicateArgument) const;
        const std::string& descriptionPart() const;
    protected:
        std::function<bool(const T&)> m_predicate;
        std::string m_descriptionPart;
    };
    Description();
    Description(const std::vector<DescriptionPart>& descriptionParts);

    void addDescriptionPart(const std::string& description);
    void addDescriptionPart(std::function<bool(T)>&& predicate, const std::string& description);

    std::string buildDescription(const T& predicateArgument) const;
protected:
    std::vector<DescriptionPart> m_descriptionParts;
};

#include "../src/Description.cpp"

#endif // DESCRIPTION_H
