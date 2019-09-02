#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "EncodableDataType.h"

namespace certi {

class DataTypeException : public std::exception
{
public:
    DataTypeException(const std::string &mess) : _mess(mess) {}

    const char *what() const throw() override {
        return _mess.c_str();
    }

private:
    std::string _mess;

};

class DataType
{
public:
    DataType():_category(EncodableDataType::CATEGORY::NA), _name(EncodableDataType::TYPE::HLAunknown), _nameUnknown(""), _totalSize(0){}

    DataType(const EncodableDataType::CATEGORY &a_category, const EncodableDataType::TYPE &a_name, const std::string& a_nameUnknown) :
        _category(a_category), _name(a_name), _nameUnknown(a_nameUnknown), _totalSize(0) {}

    DataType(const DataType &a_rhs) : _category(a_rhs._category), _name(a_rhs._name), _nameUnknown(a_rhs._nameUnknown), _totalSize(a_rhs._totalSize) {}

    virtual ~DataType() {}

    EncodableDataType::CATEGORY category() const
    {
        return _category;
    }

    void setCategory(const EncodableDataType::CATEGORY &a_category)
    {
        _category = a_category;
    }

    EncodableDataType::TYPE name() const
    {
        return _name;
    }

    //\brief: Get the String shape of the name
    //return If the name is HLAunknown the string _nameUnkonwn is returned,
    //              in other case we get the String traduction of the enumerate
    std::string nameStr() const
    {
        if(_name == EncodableDataType::TYPE::HLAunknown)
            return _nameUnknown;
        else
            return EncodableDataType::toString(_name);
    }

    //\brief: Set the name of the type
    //param name is an EncodableDataType::TYPE enumerate which indicate the type
    //param nameStr if name is HLAunknown, it is possible to store the name in the String shape.
    void setName(const std::string &&name)
    {
        _name = EncodableDataType::toType(name);
        if(_name == EncodableDataType::TYPE::HLAunknown)
            _nameUnknown = name;
    }

    size_t totalSize() const
    {
        return _totalSize;
    }

    virtual size_t calTotalSize() = 0;

protected:
    EncodableDataType::CATEGORY _category;
    EncodableDataType::TYPE _name;
    std::string _nameUnknown;
    size_t _totalSize;
};



}
#endif
