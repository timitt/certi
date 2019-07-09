#include <RTI/encoding/DataElement.h>

namespace rti1516e
{

DataElement::~DataElement()
{
}

bool DataElement::isSameTypeAs(DataElement const& inData) const
{
    return typeid(*this) == typeid(inData);
}

Integer64 DataElement::hash() const
{
    return -1;
}

}
