#ifndef SIMPLE_DATA_TYPE_H
#define SIMPLE_DATA_TYPE_H
#include "DataType.h"
#include "BasicDataType.h"

namespace certi {

///
/// \brief The SimpleDataType class
///
class SimpleDataType : public DataType
{
public:
    ///
    /// \brief SimpleDataType
    ///
    SimpleDataType() : DataType(EncodableDataType::CATEGORY::SimpleDataType, EncodableDataType::TYPE::HLAunknown, "HLAunknown")
                        , _representation(), _units("NA"), _resolution("NA"), _accuracy("NA") {}

    ///
    /// \brief SimpleDataType Copy constructor
    /// \param a_rhs
    ///
    SimpleDataType(const SimpleDataType &a_rhs) : DataType(a_rhs),
        _representation(a_rhs._representation), _units(a_rhs._units), _resolution(a_rhs._resolution),
        _accuracy(a_rhs._accuracy) {}

    ///
    /// \brief representation
    /// \return representation
    ///
    std::shared_ptr<BasicDataType> representation() const
    {
        return _representation;
    }

    ///
    /// \brief setRepresentation
    /// \param representation
    ///
    void setRepresentation(std::shared_ptr<BasicDataType> representation)
    {
        _representation.swap(representation);
    }

    ///
    /// \brief units
    /// \return
    ///
    std::string units() const
    {
        return _units;
    }

    ///
    /// \brief setUnits
    /// \param units
    ///
    void setUnits(const std::string &units)
    {
        _units = units;
    }

    ///
    /// \brief resolution
    /// \return resolution
    ///
    std::string resolution() const
    {
        return _resolution;
    }

    ///
    /// \brief setResolution
    /// \param resolution
    ///
    void setResolution(const std::string &resolution)
    {
        _resolution = resolution;
    }

    ///
    /// \brief accuracy
    /// \return accuracy
    ///
    std::string accuracy() const
    {
        return _accuracy;
    }

    ///
    /// \brief setAccuracy
    /// \param accuracy
    ///
    void setAccuracy(const std::string &accuracy)
    {
        _accuracy = accuracy;
    }

    ///
    /// \brief semantics
    /// \return semantics
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
    /// \brief calTotalSize Calcul total size with padding.
    /// \return The total size
    ///
    virtual size_t calTotalSize() {
        _totalSize = _representation->size();
        return _totalSize;
    }

private:
    std::shared_ptr<BasicDataType> _representation;
    std::string _units;
    std::string _resolution;
    std::string _accuracy;
    std::string _semantics;

};



}

#endif
