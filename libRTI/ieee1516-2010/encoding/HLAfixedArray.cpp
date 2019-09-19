#include <cstring>
#include <RTI/encoding/HLAfixedArray.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/VariableLengthData.h>
#include "HLAfixedArrayImplementation.h"


namespace rti1516e
{

HLAfixedArray::HLAfixedArray(const DataElement &protoType, size_t length)
    : _impl(new HLAfixedArrayImplementation(protoType, length))
{

}

HLAfixedArray::HLAfixedArray(const HLAfixedArray &rhs) :
    _impl(new HLAfixedArrayImplementation(*rhs._impl))
{

}

HLAfixedArray::~HLAfixedArray()
{
    delete _impl;
    _impl = NULL;
}

std::auto_ptr<DataElement> HLAfixedArray::clone() const
{
    return std::auto_ptr<rti1516e::DataElement>(new HLAfixedArray(*this));
}

VariableLengthData HLAfixedArray::encode() const
throw (EncoderException)
{
    VariableLengthData vlData;
    encode(vlData);
    return vlData;
}

void HLAfixedArray::encode(VariableLengthData &inData) const
throw (EncoderException)
{
    std::vector<Octet> buffer;
    encodeInto(buffer);
    if(buffer.size() > 0)
        inData.setData(&buffer[0], buffer.size());
}

void HLAfixedArray::encodeInto(std::vector<Octet> &buffer) const
throw (EncoderException)
{
    _impl->encodeInto(buffer);
}

void HLAfixedArray::decode(const VariableLengthData &inData)
throw (EncoderException)
{
    std::vector<Octet> buffer(inData.size());
    if(buffer.size() > 0)
        std::memcpy(&buffer[0], inData.data(), inData.size());
    decodeFrom(buffer, 0);
}

size_t HLAfixedArray::decodeFrom(const std::vector<Octet> &buffer, size_t index)
throw (EncoderException)
{
    return _impl->decodeFrom(buffer, index);
}

size_t HLAfixedArray::getEncodedLength() const
throw (EncoderException)
{
    return _impl->getEncodedLength();
}

unsigned int HLAfixedArray::getOctetBoundary() const
{
    return _impl->getOctetBoundary();
}

bool HLAfixedArray::isSameTypeAs(const DataElement &inData) const
{
    const HLAfixedArray *fa = static_cast<const HLAfixedArray*>(&inData);
    if(fa == NULL)
        return false;
    return _impl->isSameTypeAs(*fa->_impl);
}

bool HLAfixedArray::hasPrototypeSameTypeAs(const DataElement &dataElement) const
{
    const HLAfixedArray *fa = static_cast<const HLAfixedArray*>(&dataElement);
    if(fa == NULL)
        return false;
    return _impl->isSameTypeAs(*fa->_impl);
}

size_t HLAfixedArray::size() const
{
    return _impl->size();
}

void HLAfixedArray::set(size_t index, const DataElement &dataElement)
throw (EncoderException)
{
    _impl->set(index, dataElement);
}

void HLAfixedArray::setElementPointer(size_t index, DataElement *dataElement)
throw (EncoderException)
{
    _impl->setElementPointer(index, dataElement);
}

const DataElement &HLAfixedArray::get(size_t index) const
throw (EncoderException)
{
    return _impl->get(index);
}

const DataElement &HLAfixedArray::operator [](size_t index) const
throw (EncoderException)
{
    return _impl->get(index);
}



}
