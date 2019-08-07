#ifndef ENUMERATED_DATA_TYPE_H
#define ENUMERATED_DATA_TYPE_H

#include "DataType.h"

namespace certi {

class EnumerateException : public std::exception
{
public:
    EnumerateException(const std::string &mess) : _mess(mess) {}

    const char *what() const throw() override {
        return _mess.c_str();
    }

private:
    std::string _mess;

};

class Enumerator
{
public:
    Enumerator() : _name(""), _value("") {}
    Enumerator(const std::string &a_name, const std::string &a_value) : _name(a_name), _value(a_value) {}
    Enumerator(const Enumerator &a_rhs) : _name(a_rhs.name()), _value(a_rhs.value()) {}

    virtual ~Enumerator() {}

    std::string name() const
    {
        return _name;
    }

    void setName(const std::string &name)
    {
        _name = name;
    }

    std::string value() const
    {
        return _value;
    }

    void setValue(const std::string &value)
    {
        _value = value;
    }

private:
    std::string _name;
    std::string _value;
};

class EnumeratedDataType : public DataType
{
public:
    EnumeratedDataType() : DataType(EncodableDataType::CATEGORY::EnumeratedDataType, EncodableDataType::TYPE::HLAunknown, EncodableDataType::toString(EncodableDataType::TYPE::HLAunknown)),
                                    _representation(), _semantics(""), _enumerators(), _representationTemp("")
    {
        _representation = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _representation->setCategory(EncodableDataType::CATEGORY::NA);
    }

    EnumeratedDataType(const EncodableDataType::TYPE &a_name, std::shared_ptr<DataType> representation, const std::string &semantics, const std::map<std::string, std::shared_ptr<Enumerator>> &enumerators) :
                                DataType(EncodableDataType::CATEGORY::EnumeratedDataType, a_name, EncodableDataType::toString(a_name)),
                                _representation(representation), _semantics(semantics), _enumerators(enumerators), _representationTemp("")
    {

    }

    EnumeratedDataType(const EnumeratedDataType &a_rhs) : DataType(EncodableDataType::CATEGORY::EnumeratedDataType, a_rhs.name(), a_rhs.nameStr()),
                                _representation(a_rhs.representation()), _semantics(a_rhs.semantics()), _enumerators(a_rhs.enumerators()), _representationTemp(representationTemp())
    {

    }

    virtual ~EnumeratedDataType() {}

    std::shared_ptr<DataType> representation() const
    {
        return _representation;
    }

    void setRepresentation(const std::shared_ptr<DataType> &representation)
    {
        _representation = representation;
    }

    std::string representationTemp()
    {
        return _representationTemp;
    }

    void setRepresentationTemp(const std::string &representationTemp)
    {
        _representationTemp = representationTemp;
    }

    std::string semantics() const
    {
        return _semantics;
    }

    void setSemantics(const std::string &semantics)
    {
        _semantics = semantics;
    }

    std::map<std::string, std::shared_ptr<Enumerator>> enumerators() const
    {
        return _enumerators;
    }

    void setEnumerators(const std::map<std::string, std::shared_ptr<Enumerator>> &enumerators)
    {
        _enumerators = enumerators;
    }

    void addEnumerator(const std::shared_ptr<Enumerator> &enumerator)
    {
        _enumerators.emplace(enumerator->name(), enumerator);
    }

    std::shared_ptr<Enumerator> getEnumerator(const std::string &name)
    {
        if(_enumerators.count(name) == 0)
            throw EnumerateException(std::string("Enumerator ") + name + std::string(" does not exist."));
        return _enumerators.at(name);
    }


private:

    std::shared_ptr<DataType> _representation;
    std::string _semantics;
    std::map<std::string, std::shared_ptr<Enumerator>> _enumerators;
    std::string _representationTemp;
};

}

#endif
