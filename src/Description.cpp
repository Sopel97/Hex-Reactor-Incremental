#include "Description.h"

template <class T>
Description<T>::DescriptionPart::DescriptionPart() :
    m_predicate([](const T& arg)->bool{return false;}),
    m_descriptionPart()
{
}
template <class T>
Description<T>::DescriptionPart::DescriptionPart(const std::string& description) :
    m_predicate([](const T& arg)->bool{return true;}),
    m_descriptionPart(description)
{

}
template <class T>
Description<T>::DescriptionPart::DescriptionPart(std::function<bool(const T&)>&& predicate, const std::string& description) :
    m_predicate(std::move(predicate)),
    m_descriptionPart(description)
{

}

template <class T>
Description<T>::Description() :
    m_descriptionParts()
{

}
template <class T>
Description<T>::Description(const std::vector<Description<T>::DescriptionPart>& descriptionParts) :
    m_descriptionParts(descriptionParts)
{

}

template <class T>
bool Description<T>::DescriptionPart::predicate(const T& predicateArgument) const
{
    return m_predicate(predicateArgument);
}
template <class T>
const std::string& Description<T>::DescriptionPart::descriptionPart() const
{
    return m_descriptionPart;
}


template <class T>
void Description<T>::addDescriptionPart(const std::string& description)
{
    m_descriptionParts.emplace_back(description);
}
template <class T>
void Description<T>::addDescriptionPart(std::function<bool(T)>&& predicate, const std::string& description)
{
    m_descriptionParts.emplace_back(predicate, description);
}

template <class T>
std::string Description<T>::buildDescription(const T& predicateArgument) const
{
    std::string fullDescription;
    for(const auto& descriptionPart : m_descriptionParts)
    {
        if(descriptionPart.predicate(predicateArgument))
        {
            fullDescription += descriptionPart.descriptionPart();
        }
    }
    return fullDescription;
}
