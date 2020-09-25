#ifndef ENUMERATED_DATA_TYPE_H
#define ENUMERATED_DATA_TYPE_H

#include "DataType.h"

namespace certi {

///
/// \brief The EnumerateException class
///
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

///
/// \brief The Enumerator class
///
class Enumerator
{
public:
    ///
    /// \brief Enumerator
    ///
    Enumerator() : _name(""), _value("") {}

    ///
    /// \brief Enumerator
    /// \param a_name Name of the Enumerator
    /// \param a_value
    ///
    Enumerator(const std::string &a_name, const std::string &a_value) : _name(a_name), _value(a_value) {}

    ///
    /// \brief Enumerator
    /// \param a_rhs
    ///
    Enumerator(const Enumerator &a_rhs) : _name(a_rhs.name()), _value(a_rhs.value()) {}

    ///
    /// \brief ~Enumerator
    ///
    virtual ~Enumerator() {}

    ///
    /// \brief name
    /// \return name
    ///
    std::string name() const
    {
        return _name;
    }

    ///
    /// \brief setName
    /// \param name
    ///
    void setName(const std::string &name)
    {
        _name = name;
    }

    ///
    /// \brief value
    /// \return value
    ///
    std::string value() const
    {
        return _value;
    }

    ///
    /// \brief setValue
    /// \param value
    ///
    void setValue(const std::string &value)
    {
        _value = value;
    }

private:
    std::string _name;
    std::string _value;
};


///
/// \brief The EnumeratedDataType class
///
class EnumeratedDataType : public DataType
{
public:

    ///
    /// \brief EnumeratedDataType Default constructor
    ///
    EnumeratedDataType() : DataType(EncodableDataType::CATEGORY::EnumeratedDataType, EncodableDataType::TYPE::HLAunknown, "HLAunknown"),
                                    _representation(), _semantics(""), _enumerators(), _representationTemp("")
    {
        _representation = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _representation->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ///
    /// \brief EnumeratedDataType Copy constructor
    /// \param a_rhs Right hand side copy constructor
    ///
    EnumeratedDataType(const EnumeratedDataType &a_rhs) : DataType(a_rhs),
                                _representation(a_rhs.representation()), _semantics(a_rhs.semantics()), _enumerators(a_rhs.enumerators()), _representationTemp(representationTemp())
    {

    }

    ///
    /// \brief ~EnumeratedDataType
    ///
    virtual ~EnumeratedDataType() {}

    ///
    /// \brief representation
    /// \return a shared ptr to the dataType of the enumerator
    ///
    std::shared_ptr<DataType> representation() const
    {
        return _representation;
    }

    ///
    /// \brief setRepresentation
    /// \param representation
    ///
    void setRepresentation(const std::shared_ptr<DataType> &representation)
    {
        _representation = representation;
    }

    ///
    /// \brief representationTemp
    /// \return name of the datatype. It's a string representation.
    ///
    std::string representationTemp()
    {
        return _representationTemp;
    }

    ///
    /// \brief setRepresentationTemp
    /// \param representationTemp
    ///
    void setRepresentationTemp(const std::string &representationTemp)
    {
        _representationTemp = representationTemp;
    }

    ///
    /// \brief semantics
    /// \return semantics of the Enumerated datatype
    ///
    std::string semantics() const
    {
        return _semantics;
    }

    ///
    /// \brief setSemantics
    /// \param semantics
    ///
    void setSemantics(const std::string &semantics)
    {
        _semantics = semantics;
    }

    ///
    /// \brief enumerators
    /// \return A map of Enumerator
    ///
    std::map<std::string, std::shared_ptr<Enumerator>> enumerators() const
    {
        return _enumerators;
    }

    ///
    /// \brief setEnumerators
    /// \param enumerators
    ///
    void setEnumerators(const std::map<std::string, std::shared_ptr<Enumerator>> &enumerators)
    {
        _enumerators = enumerators;
    }

    ///
    /// \brief addEnumerator
    /// \param enumerator
    ///
    void addEnumerator(const std::shared_ptr<Enumerator> &enumerator)
    {
        _enumerators.emplace(enumerator->name(), enumerator);
    }

    ///
    /// \brief getEnumerator
    /// \param name The name of the enumerator
    /// \return a specif enumerator
    ///
    std::shared_ptr<Enumerator> getEnumerator(const std::string &name)
    {
        if(_enumerators.count(name) == 0)
            throw EnumerateException(std::string("Enumerator ") + name + std::string(" does not exist."));
        return _enumerators.at(name);
    }

    ///
    /// \brief calTotalSize Calcul total size with padding. Work only if there is no Dynamic Array inside
    /// \return The total size
    ///
    virtual size_t calTotalSize() {
        _totalSize = _representation->totalSize();
        return _totalSize;
    }

private:

    std::shared_ptr<DataType> _representation;
    std::string _semantics;
    std::map<std::string, std::shared_ptr<Enumerator>> _enumerators;
    std::string _representationTemp;
};

}

#endif
