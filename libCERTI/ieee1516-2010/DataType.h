#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "EncodableDataType.h"

namespace certi {

///
/// \brief The DataTypeException class
///
/// \class DataTypeException DataType.h
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

///
/// \brief The DataType class
///
/// \class DataType DataType.h
class DataType
{
public:

    ///
    /// \brief This constructor initializes all attributes with defaults values
    ///
    DataType():_category(EncodableDataType::CATEGORY::NA), _name(EncodableDataType::TYPE::HLAunknown), _nameUnknown(""), _totalSize(0), _encodableDataType(){}

    ///
    /// \brief DataType Constructor with attributes initialized by parameter
    /// \param a_category Category of the DataType. This is an Enum EncodableDataType::CATEGORY
    /// \param a_name Name of the dataTYype which is a an Enum EncodableDataType::TYPE
    /// \param a_nameUnknown Real name if the name is not exist in the Enum EncodableDataType::TYPE.
    ///         In this case EncodableDataType::TYPE::HLAunknown have to be passed in the second paramter
    ///
    DataType(const EncodableDataType::CATEGORY &a_category, const EncodableDataType::TYPE &a_name, const std::string& a_nameUnknown) :
        _category(a_category), _name(a_name), _nameUnknown(a_nameUnknown), _totalSize(0), _encodableDataType() {}

    ///
    /// \brief DataType Copy constructor
    /// \param a_rhs The right hand side Datatype to copy
    ///
    DataType(const DataType &a_rhs) : _category(a_rhs._category), _name(a_rhs._name), _nameUnknown(a_rhs._nameUnknown), _totalSize(a_rhs._totalSize), _encodableDataType(a_rhs._encodableDataType) {}

    ///
    /// \brief ~DataType
    ///
    virtual ~DataType() {}

    ///
    /// \brief category
    /// \return the EncodableDataType::CATEGORY enumerator of the Datatype
    ///
    EncodableDataType::CATEGORY category() const
    {
        return _category;
    }

    ///
    /// \brief setCategory set Category of the datatype
    /// \param a_category the EncodableDataType::CATEGORY enumerator
    ///
    void setCategory(const EncodableDataType::CATEGORY &a_category)
    {
        _category = a_category;
    }

    ///
    /// \brief name
    /// \return get name of the datatype. The name it's a EncodableDataType::TYPE enumerator
    ///
    EncodableDataType::TYPE name() const
    {
        return _name;
    }

    ///
    /// \brief: Get the String shape of the name
    /// \return If the name is HLAunknown the string _nameUnkonwn is returned,
    ///              in other case we get the String traduction of the enumerate
    std::string nameStr()
    {
        if(_name == EncodableDataType::TYPE::HLAunknown)
            return _nameUnknown;
        else
            return _encodableDataType.toStringType(std::move(_name));
    }

    ///
    /// \brief: Set the name of the type
    /// \param name is an EncodableDataType::TYPE enumerate which indicate the type
    /// \param nameStr if name is HLAunknown, it is possible to store the name in the String shape.
    ///
    void setName(const std::string &&name)
    {
        _name = _encodableDataType.toType(name);
        if(_name == EncodableDataType::TYPE::HLAunknown)
            _nameUnknown = name;
    }

    ///
    /// \brief totalSize
    /// \return the total size of the value type ( for dynamic type the size is always 0 )
    ///
    size_t totalSize() const
    {
        return _totalSize;
    }

    ///
    /// \brief calTotalSize Virtual pure function which have to be redefined in all Children datatype classes
    /// \return the total size ( 0 for dynamic value )
    ///
    virtual size_t calTotalSize() = 0;

    EncodableDataType encodableDataType()
    {
        return _encodableDataType;
    }

protected:
    EncodableDataType::CATEGORY _category;
    EncodableDataType::TYPE _name;
    std::string _nameUnknown;
    size_t _totalSize;
    EncodableDataType _encodableDataType;
};





}
#endif
