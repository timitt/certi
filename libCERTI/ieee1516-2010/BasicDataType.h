#ifndef BASIC_DATA_TYPE_H
#define BASIC_DATA_TYPE_H
#include "DataType.h"

namespace certi {

class BasicDataType : public DataType
{
public:
    BasicDataType() : DataType(EncodableDataType::CATEGORY::BasicDataType, EncodableDataType::TYPE::HLAunknown, EncodableDataType::toString(EncodableDataType::TYPE::HLAunknown))
                        , _size(0), _endian(), _interpretation(""), _encoding(""){}

    BasicDataType(const EncodableDataType::TYPE &a_name, const EncodableDataType::ENDIAN &a_endian,
                  const std::string &a_interpretation, const std::string &a_encoding)
        : DataType(EncodableDataType::CATEGORY::BasicDataType, a_name, EncodableDataType::toString(a_name)), _endian(a_endian),
          _interpretation(a_interpretation), _encoding(a_encoding) {}

    BasicDataType(const BasicDataType &a_rhs)
        : DataType(a_rhs.category(), a_rhs.name(), a_rhs.nameStr()), _endian(a_rhs.endian()),
          _interpretation(a_rhs.interpretation()), _encoding(a_rhs.encoding()) {}

    virtual ~BasicDataType(){}

    size_t size() const
    {
        return _size;
    }

    void setSize(const size_t &size)
    {
        _size = size;
    }

    EncodableDataType::ENDIAN endian() const
    {
        return _endian;
    }

    void setEndian(const EncodableDataType::ENDIAN &endian)
    {
        _endian = endian;
    }

    std::string interpretation() const
    {
        return _interpretation;
    }

    void setInterpretation(const std::string &interpretation)
    {
        _interpretation = interpretation;
    }

    std::string encoding() const
    {
        return _encoding;
    }

    void setEncoding(const std::string &encoding)
    {
        _encoding = encoding;
    }

    virtual size_t calTotalSize() {
        _totalSize = _size;
        return _totalSize;
    }

private:
    size_t _size;
    EncodableDataType::ENDIAN _endian;
    std::string _interpretation;
    std::string _encoding;
};













}

#endif
