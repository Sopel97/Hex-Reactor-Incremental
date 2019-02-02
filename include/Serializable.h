#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

class BinaryStream;

class Serializable
{
public:
    virtual void serialize(BinaryStream& stream) const = 0;
    virtual void deserialize(BinaryStream& stream) = 0;
};

#endif // SERIALIZABLE_H
