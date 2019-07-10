#include <RTI/encoding/HLAfixedArray.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/VariableLengthData.h>

namespace rti1516e
{

HLAfixedArray::HLAfixedArray(const DataElement &protoType, size_t length)
{

}

HLAfixedArray::HLAfixedArray(const HLAfixedArray &rhs)
{

}

HLAfixedArray::~HLAfixedArray()
{

}

std::auto_ptr<DataElement> HLAfixedArray::clone() const
{

}

VariableLengthData HLAfixedArray::encode() const
throw (EncoderException)
{

}

void HLAfixedArray::encode(VariableLengthData &inData) const
throw (EncoderException)
{

}

void HLAfixedArray::encodeInto(std::vector<Octet> &buffer) const
throw (EncoderException)
{

}

void HLAfixedArray::decode(const VariableLengthData &inData)
throw (EncoderException)
{

}

size_t HLAfixedArray::decodeFrom(const std::vector<Octet> &buffer, size_t index)
throw (EncoderException)
{

}

size_t HLAfixedArray::getEncodedLength() const
throw (EncoderException)
{

}

unsigned int HLAfixedArray::getOctetBoundary() const
{

}

bool HLAfixedArray::isSameTypeAs(const DataElement &inData) const
{

}

bool HLAfixedArray::hasPrototypeSameTypeAs(const DataElement &dataElement) const
{

}

size_t HLAfixedArray::size() const
{

}

void HLAfixedArray::set(size_t index, const DataElement &dataElement)
throw (EncoderException)
{

}

void HLAfixedArray::setElementPointer(size_t index, DataElement *dataElement)
throw (EncoderException)
{

}

const DataElement &HLAfixedArray::get(size_t index) const
throw (EncoderException)
{

}

const DataElement &HLAfixedArray::operator [](size_t index) const
throw (EncoderException)
{

}



}
