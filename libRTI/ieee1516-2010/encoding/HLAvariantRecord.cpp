#include <RTI/encoding/HLAvariantRecord.h>
#include <RTI/VariableLengthData.h>
#include "HLAvariantRecordImplementation.h"

#include <cstring>

namespace rti1516e
{

HLAvariantRecord::HLAvariantRecord(const DataElement &discriminantPrototype)
{
    _impl = new HLAvariantRecordImplementation(discriminantPrototype);
}

HLAvariantRecord::HLAvariantRecord(const HLAvariantRecord &rhs)
{
    _impl = new HLAvariantRecordImplementation(*rhs._impl);
}

HLAvariantRecord::~HLAvariantRecord()
{
    if(_impl)
        delete _impl;
    _impl = NULL;
}

std::auto_ptr<DataElement> HLAvariantRecord::clone() const
{
    return std::auto_ptr<rti1516e::DataElement>(new HLAvariantRecord(*this));
}

VariableLengthData HLAvariantRecord::encode() const
throw (EncoderException)
{
    VariableLengthData vlData;
    encode(vlData);
    return vlData;
}

void HLAvariantRecord::encode(VariableLengthData &inData) const
throw (EncoderException)
{
    std::vector<Octet> buffer;
    encodeInto(buffer);
    if(buffer.size() > 0)
        inData.setData(&buffer[0], buffer.size());
}

void HLAvariantRecord::encodeInto(std::vector<Octet> &buffer) const
throw (EncoderException)
{
    _impl->encodeInto(buffer);
}

void HLAvariantRecord::decode(const VariableLengthData &inData)
throw (EncoderException)
{
    std::vector<Octet> buffer(inData.size());
    if(buffer.size() > 0)
        std::memcpy(&buffer[0], inData.data(), inData.size());
    decodeFrom(buffer, 0);
}

size_t HLAvariantRecord::decodeFrom(const std::vector<Octet> &buffer, size_t index)
throw (EncoderException)
{
    return _impl->decodeFrom(buffer, index);
}

size_t HLAvariantRecord::getEncodedLength() const
throw (EncoderException)
{
    return _impl->getEncodedLength();
}

unsigned int HLAvariantRecord::getOctetBoundary() const
{
    return _impl->getOctetBoundary();
}

bool HLAvariantRecord::isSameTypeAs(const DataElement &inData) const
{
    const HLAvariantRecord &va = static_cast<HLAvariantRecord>(inData);
    return _impl->isSameTypeAs(*va._impl);
}

bool HLAvariantRecord::isSameTypeAs(const DataElement &discriminant, const DataElement &inData) const
throw (EncoderException)
{
    return _impl->isSameTypeAs(discriminant, inData);
}

bool HLAvariantRecord::hasMatchingDiscriminantTypeAs(const DataElement &dataElement) const
{
    return _impl->hasMatchingDiscriminantTypeAs(dataElement);
}

void HLAvariantRecord::addVariant(const DataElement &discriminant, const DataElement &valuePrototype)
throw (EncoderException)
{
    _impl->addVariant(discriminant, valuePrototype);
}

void HLAvariantRecord::addVariantPointer(const DataElement &discriminant, DataElement *valuePtr)
throw (EncoderException)
{
    _impl->addVariantPointer(discriminant, valuePtr);
}

void HLAvariantRecord::setDiscriminant(const DataElement &discriminant)
throw (EncoderException)
{
    _impl->setDiscriminant(discriminant);
}

void HLAvariantRecord::setVariant(const DataElement &discriminant, const DataElement &value)
throw (EncoderException)
{
    _impl->setVariant(discriminant, value);
}

void HLAvariantRecord::setVariantPointer(const DataElement &discriminant, DataElement *valuePtr)
throw (EncoderException)
{
    _impl->setVariantPointer(discriminant, valuePtr);
}

const DataElement &HLAvariantRecord::getDiscriminant() const
{
    return _impl->getDiscriminant();
}

const DataElement &HLAvariantRecord::getVariant() const
throw (EncoderException)
{
    return _impl->getVariant();
}

}
