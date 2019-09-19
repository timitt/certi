#include <cstring>

#include <RTI/encoding/HLAvariableArray.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/VariableLengthData.h>

#include "HLAvariableArrayImplementation.h"


namespace rti1516e
{

HLAvariableArray::HLAvariableArray(const rti1516e::DataElement &prototype) :
    _impl(new HLAvariableArrayImplementation(prototype))
{

}

HLAvariableArray::HLAvariableArray(const HLAvariableArray &rhs) :
    _impl(new HLAvariableArrayImplementation(*rhs._impl))
{

}

HLAvariableArray::~HLAvariableArray()
{
    delete _impl;
    _impl = NULL;
}

std::auto_ptr<DataElement> HLAvariableArray::clone() const
{
    return std::auto_ptr<rti1516e::DataElement>(new HLAvariableArray(*this));
}

VariableLengthData HLAvariableArray::encode() const
  throw (EncoderException)
{
    VariableLengthData vlData;
    encode(vlData);
    return vlData;
}

void HLAvariableArray::encode(VariableLengthData &inData) const
  throw (EncoderException)
{
    std::vector<Octet> buffer;
    encodeInto(buffer);
    if(buffer.size() > 0)
        inData.setData(&buffer[0], buffer.size());
}

void HLAvariableArray::encodeInto(std::vector<Octet> &buffer) const
  throw (EncoderException)
{
    _impl->encodeInto(buffer);
}

void HLAvariableArray::decode(const VariableLengthData &inData)
  throw (EncoderException)
{
    std::vector<Octet> buffer(inData.size());
    if(buffer.size() > 0)
        std::memcpy(&buffer[0], inData.data(), inData.size());
    decodeFrom(buffer, 0);
}

size_t HLAvariableArray::decodeFrom(const std::vector<Octet> &buffer, size_t index)
  throw (EncoderException)
{
    return _impl->decodeFrom(buffer, index);
}

size_t HLAvariableArray::getEncodedLength() const
  throw (EncoderException)
{
    return _impl->getEncodedLength();
}

unsigned int HLAvariableArray::getOctetBoundary() const
{
    return _impl->getOctetBoundary();
}

size_t HLAvariableArray::size() const
{
    return _impl->size();
}

bool HLAvariableArray::isSameTypeAs(const DataElement &inData) const
{
    const HLAvariableArray &va = static_cast<HLAvariableArray>(inData);
    return _impl->isSameTypeAs(*va._impl);
}

bool HLAvariableArray::hasPrototypeSameTypeAs(const DataElement &dataElement) const
{
    const HLAvariableArray &va = static_cast<HLAvariableArray>(dataElement);
    return _impl->isSameTypeAs(*va._impl);
}

void HLAvariableArray::addElement(const DataElement &dataElement)
  throw (EncoderException)
{
    _impl->addElement(dataElement);
}

void HLAvariableArray::addElementPointer(DataElement *dataElement)
  throw (EncoderException)
{
    _impl->addElementPointer(dataElement);
}

void HLAvariableArray::set(size_t index, const DataElement &dataElement)
  throw (EncoderException)
{
    _impl->set(index, dataElement);
}

void HLAvariableArray::setElementPointer(size_t index, DataElement *dataElement)
  throw (EncoderException)
{
    _impl->setElementPointer(index, dataElement);
}

const DataElement &HLAvariableArray::get(size_t index) const
  throw (EncoderException)
{
    return _impl->get(index);
}

const DataElement &HLAvariableArray::operator [](size_t index) const
  throw (EncoderException)
{
    return _impl->get(index);
}

}
