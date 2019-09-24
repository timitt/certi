#ifndef ARRAY_DATA_TYPE_H
#define ARRAY_DATA_TYPE_H

#include "DataType.h"
#include <ieee1516-2010/RTI/encoding/Padding.h>

namespace certi {

class ArrayDataType : public DataType
{
public:

    ///
    /// \brief ArrayDataType Default constructor
    ///
    ArrayDataType() : DataType(EncodableDataType::CATEGORY::ArrayDataType, EncodableDataType::TYPE::HLAunknown, "HLAunknown"),
        _typeElements(), _cardinality("0"), _encoding(""), _semantics(""), _typeElementsTemp("") {

        _typeElements = std::make_shared<BasicDataType>(); //By default datatype will be unknown
        _typeElements->setCategory(EncodableDataType::CATEGORY::NA);
    }

    ///
    /// \brief ArrayDataType Copy constructor
    /// \param a_rhs Right hand side which have to be copy
    ///
    ArrayDataType(const ArrayDataType &a_rhs) :
        DataType(a_rhs), _typeElements(a_rhs.typeElements()), _cardinality(a_rhs.cardinality()), _encoding(a_rhs.encoding()),
        _semantics(a_rhs.semantics()), _typeElementsTemp(a_rhs.typeElementsTemp()) {}


    ///
    /// \brief typeElements
    /// \return Return the dataType of Elements of the array
    ///
    std::shared_ptr<DataType> typeElements() const
    {
        return _typeElements;
    }

    ///
    /// \brief setTypeElements
    /// \param typeElements
    ///
    void setTypeElements(const std::shared_ptr<DataType> &typeElements)
    {
        _typeElements = typeElements;
    }

    ///
    /// \brief cardinality
    /// \return Cardinality ( number for fixed array, "Dynamic" for variable array )
    ///
    std::string cardinality() const
    {
        return _cardinality;
    }

    ///
    /// \brief setCardinality
    /// \param cardinality
    ///
    void setCardinality(const std::string &cardinality)
    {
        _cardinality = cardinality;
    }

    ///
    /// \brief encoding
    /// \return Encoding name
    ///
    std::string encoding() const
    {
        return _encoding;
    }

    ///
    /// \brief setEncoding
    /// \param encoding
    ///
    void setEncoding(const std::string &encoding)
    {
        _encoding = encoding;
    }

    ///
    /// \brief semantics
    /// \return Semantics content
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
    /// \brief typeElementsTemp
    /// \return typeElementsTemp. This typeElementsTemp it's a String representation of the Datatype element.
    ///
    std::string typeElementsTemp() const
    {
        return _typeElementsTemp;
    }

    ///
    /// \brief setTypeElementsTemp
    /// \param typeElementsTemp
    ///
    void setTypeElementsTemp(const std::string &typeElementsTemp)
    {
        _typeElementsTemp = typeElementsTemp;
    }

    ///
    /// \brief calTotalSize
    /// \return 0 if the array has a dynamic Cardinality, otherwise the size of the fixed array
    ///
    virtual size_t calTotalSize() {
        if(_cardinality == "Dynamic")
            _totalSize = 0;
        else {
            uint P = padding::HlaArray::calculPaddingAfterEachElements(_typeElements->calTotalSize(), _typeElements->calTotalSize());
            _totalSize = P * (_typeElements->calTotalSize() - 1);
        }
        return _totalSize;
    }


private:
    std::shared_ptr<DataType> _typeElements;
    std::string _cardinality;
    std::string _encoding;
    std::string _semantics;
    std::string _typeElementsTemp;
};


}


#endif
