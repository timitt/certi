#ifndef VARIANT_RECORD_DATA_TYPE_H
#define VARIANT_RECORD_DATA_TYPE_H

#include <vector>

#include "EnumeratedDataType.h"

namespace certi {

class Alternative
{
public:

    ///
    /// \brief Alternative
    ///
    Alternative() : _enumerator(), _name(""), _type(), _semantics(""), _enumeratorTemp(""), _typeTemp("")
    {
        _type = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _type->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ///
    /// \brief Alternative Copy constructor
    /// \param a_rhs
    ///
    Alternative(const Alternative &a_rhs) : _enumerator(a_rhs.enumerator()), _name(a_rhs.name()), _type(a_rhs.type()), _semantics(a_rhs.semantics()),
                                            _enumeratorTemp(a_rhs.enumeratorTemp()), _typeTemp(a_rhs.typeTemp())
    {

    }

    ///
    /// \brief ~Alternative
    ///
    virtual ~Alternative()
    {

    }

    ///
    /// \brief enumerator
    /// \return The enumerator of the alternative
    ///
    std::shared_ptr<Enumerator> enumerator() const
    {
        return _enumerator;
    }

    ///
    /// \brief setEnumerator
    /// \param enumerator
    ///
    void setEnumerator(const std::shared_ptr<Enumerator> &enumerator)
    {
        _enumerator = enumerator;
    }

    ///
    /// \brief name
    /// \return the name of the Alternative
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
    /// \brief type
    /// \return A shared pointer to the datatype of the Alternative
    ///
    std::shared_ptr<DataType> type() const
    {
        return _type;
    }

    ///
    /// \brief setType
    /// \param type
    ///
    void setType(const std::shared_ptr<DataType> &type)
    {
        _type = type;
    }

    ///
    /// \brief semantics
    /// \return
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
    /// \brief enumeratorTemp.
    /// \return Name of the enumertar.
    ///
    std::string enumeratorTemp() const
    {
        return _enumeratorTemp;
    }

    ///
    /// \brief setEnumeratorTemp
    /// \param enumeratorTemp
    ///
    void setEnumeratorTemp(const std::string &enumeratorTemp)
    {
        _enumeratorTemp = enumeratorTemp;
    }

    ///
    /// \brief typeTemp
    /// \return The name of the datatype of the alternative
    ///
    std::string typeTemp() const
    {
        return _typeTemp;
    }

    ///
    /// \brief setTypeTemp
    /// \param typeTemp
    ///
    void setTypeTemp(const std::string &typeTemp)
    {
        _typeTemp = typeTemp;
    }


private:
    std::shared_ptr<Enumerator> _enumerator;
    std::string _name;
    std::shared_ptr<DataType> _type;
    std::string _semantics;
    std::string _enumeratorTemp;
    std::string _typeTemp;
};

///
/// \brief The VariantRecordDataType class
///
class VariantRecordDataType : public DataType
{
public:
    ///
    /// \brief VariantRecordDataType
    ///
    VariantRecordDataType() : DataType(EncodableDataType::CATEGORY::VariantRecordDataType, EncodableDataType::TYPE::HLAunknown, "HLAunknown"),
                                _discriminantName(""), _discriminantType(), _alternatives(), _discriminantTypeTemp("")
    {
        _discriminantType = std::make_shared<EnumeratedDataType>(); //By default datatype will be unknown
        _discriminantType->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ///
    /// \brief VariantRecordDataType Copy constructor
    /// \param a_rhs
    ///
    VariantRecordDataType(const VariantRecordDataType &a_rhs) : DataType(a_rhs),
                                                                _discriminantName(a_rhs.discriminantName()), _discriminantType(a_rhs.discriminantType()),
                                                                _alternatives(a_rhs.calternatives()), _discriminantTypeTemp(a_rhs._discriminantTypeTemp)
    {

    }

    ///
    /// \brief discriminantName
    /// \return The name of the discrimant
    ///
    std::string discriminantName() const
    {
        return _discriminantName;
    }

    ///
    /// \brief setDiscriminantName
    /// \param discriminantName
    ///
    void setDiscriminantName(const std::string &discriminantName)
    {
        _discriminantName = discriminantName;
    }

    ///
    /// \brief discriminantType
    /// \return discriminentType
    ///
    std::shared_ptr<EnumeratedDataType> discriminantType() const
    {
        return _discriminantType;
    }

    ///
    /// \brief setDiscriminantType
    /// \param discriminantType
    ///
    void setDiscriminantType(const std::shared_ptr<EnumeratedDataType> &discriminantType)
    {
        _discriminantType = discriminantType;
    }


    std::vector<Alternative> calternatives() const
    {
        return _alternatives;
    }

    std::vector<Alternative>& alternatives()
    {
        return _alternatives;
    }

    ///
    /// \brief setAlternatives
    /// \param alternatives
    ///
    void setAlternatives(const std::vector<Alternative> &alternatives)
    {
        _alternatives = alternatives;
    }

    ///
    /// \brief addAlternative
    /// \param alternative
    ///
    void addAlternative(const Alternative &alternative)
    {
        _alternatives.push_back(alternative);
    }

    ///
    /// \brief discriminantTypeTemp
    /// \return The name of the datatype of the discriminant
    ///
    std::string discriminantTypeTemp() const
    {
        return _discriminantTypeTemp;
    }

    ///
    /// \brief setDiscriminantTypeTemp
    /// \param discriminantTypeTemp
    ///
    void setDiscriminantTypeTemp(const std::string &discriminantTypeTemp)
    {
        _discriminantTypeTemp = discriminantTypeTemp;
    }

    ///
    /// \brief calTotalSize Calcul total size with padding. Work only if there is no Dynamic Array inside
    /// \return The cal total Size
    ///
    virtual size_t calTotalSize() {
        _totalSize = 0;
        return _totalSize;
    }

private:
    std::string _discriminantName;
    std::shared_ptr<EnumeratedDataType> _discriminantType;
    std::vector<Alternative> _alternatives;
    std::string _discriminantTypeTemp;

};

}

#endif
