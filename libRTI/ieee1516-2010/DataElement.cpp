#include <RTI/encoding/DataElement.h>

namespace rti1516e
{

DataElement::~DataElement()
{
}

bool DataElement::isSameTypeAs(DataElement const& inData) const
{
  return false;
}

Integer64 DataElement::hash() const
{
  return -1;
}

}
