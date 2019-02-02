#ifndef BINARYSTREAM_H
#define BINARYSTREAM_H

#include <string>
#include <cstring>
#include <iostream>

class BinaryStream //TODO: IMPORTANT very often misworks and gives wrong values on read
{
public:
    BinaryStream();
    BinaryStream(const std::string& buffer);
    BinaryStream(std::string&& buffer);
    template <class T>
    BinaryStream& operator << (const T& inValue) //always makes a memory dump of the object (won't work as expected for non-pod types)
    {
        size_t bytesToWrite = sizeof(inValue);
        m_streamData.append(reinterpret_cast<const char*>(&inValue), bytesToWrite);
        return *this;
    }
    BinaryStream& operator << (const std::string& inValue);
    template <class T>
    BinaryStream& operator >> (T& outValue)
    {
        size_t bytesToRead = sizeof(outValue);
        std::memcpy(&outValue, &(m_streamData[m_readOffset]), bytesToRead);
        m_readOffset += bytesToRead;
        return *this;
    }
    BinaryStream& operator >> (std::string& outValue);
    const std::string& str() const;
protected:
    size_t m_readOffset;
    std::string m_streamData;
};

#endif // BINARYSTREAM_H
