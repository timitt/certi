#include <RTI/encoding/DataElement.h>

namespace rti1516e
{

DataElement::~DataElement()
{
}

bool
DataElement::isSameTypeAs(DataElement const& inData) const
{
  return false;
}

Integer64
DataElement::hash() const
{
  try {
    std::vector<Octet> buffer;
    encodeInto(buffer);
    Integer64 hash = 8191;
    for (std::vector<Octet>::const_iterator i = buffer.begin(); i != buffer.end(); ++i)
      hash = hash*Integer64(127) + Integer64(*i);
    return hash;
  } catch (const EncoderException&) {
    return -1;
  }
}

}
