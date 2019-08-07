#ifndef SIMPLE_DATA_TYPE_H
#define SIMPLE_DATA_TYPE_H
#include "DataType.h"
#include "BasicDataType.h"

namespace certi {

class SimpleDataType : public DataType
{
public:
    SimpleDataType() : DataType(EncodableDataType::CATEGORY::SimpleDataType, EncodableDataType::TYPE::HLAunknown, EncodableDataType::toString(EncodableDataType::TYPE::HLAunknown))
                        , _representation(), _units("NA"), _resolution("NA"), _accuracy("NA") {}

    SimpleDataType(const EncodableDataType::CATEGORY &a_category, const EncodableDataType::TYPE &a_name,
                   std::shared_ptr<BasicDataType> a_representation, const std::string &a_units,
                   const std::string &a_resolution, const std::string &a_accuracy)
        : DataType(a_category, a_name, EncodableDataType::toString(a_name)), _representation(a_representation), _units(a_units), _resolution(a_resolution),
                    _accuracy(a_accuracy) {}

    SimpleDataType(const SimpleDataType &a_rhs) : DataType(a_rhs.category(), a_rhs.name(), a_rhs.nameStr()),
        _representation(a_rhs._representation), _units(a_rhs._units), _resolution(a_rhs._resolution),
        _accuracy(a_rhs._accuracy) {}

    std::shared_ptr<BasicDataType> representation() const
    {
        return _representation;
    }

    void setRepresentation(std::shared_ptr<BasicDataType> representation)
    {
        _representation.swap(representation);
    }


    std::string units() const
    {
        return _units;
    }

    void setUnits(const std::string &units)
    {
        _units = units;
    }

    std::string resolution() const
    {
        return _resolution;
    }

    void setResolution(const std::string &resolution)
    {
        _resolution = resolution;
    }

    std::string accuracy() const
    {
        return _accuracy;
    }

    void setAccuracy(const std::string &accuracy)
    {
        _accuracy = accuracy;
    }


    std::string semantics() const
    {
        return _semantics;
    }

    void setSemantics(const std::string &semantics)
    {
        _semantics = semantics;
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
