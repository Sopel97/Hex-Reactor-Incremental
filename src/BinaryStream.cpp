#include "BinaryStream.h"

#include <utility>
#include <iostream>

BinaryStream::BinaryStream() :
    m_readOffset(0u)
{
}
BinaryStream::BinaryStream(const std::string& buffer) :
    m_readOffset(0u),
    m_streamData(buffer)
{

}
BinaryStream::BinaryStream(std::string&& buffer) :
    m_readOffset(0u),
    m_streamData(std::move(buffer))
{
}
BinaryStream& BinaryStream::operator << (const std::string& inValue)
{
    operator << (static_cast<size_t>(inValue.size()));
    m_streamData.append(inValue);
    return *this;
}
BinaryStream& BinaryStream::operator >> (std::string& outValue)
{
    size_t size;
    operator >> (size);
    outValue.assign(m_streamData.data() + m_readOffset, size);
    m_readOffset += size;
    return *this;
}
const std::string& BinaryStream::str() const
{
    return m_streamData;
}
