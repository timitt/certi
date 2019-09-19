#include <RTI/encoding/HLAopaqueData.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/VariableLengthData.h>
#include "HLAopaqueDataImplementation.h"

namespace rti1516e
{

HLAopaqueData::HLAopaqueData()
{

}

HLAopaqueData::HLAopaqueData(const Octet *inData, size_t dataSize)
{
    _impl = new HLAopaqueDataImplementation(inData, dataSize);
}

HLAopaqueData::HLAopaqueData(Octet **inData, size_t bufferSize, size_t dataSize)
throw (EncoderException)
{
    _impl = new HLAopaqueDataImplementation(inData, bufferSize, dataSize);
}

HLAopaqueData::HLAopaqueData(const HLAopaqueData &rhs)
{
    _impl = new HLAopaqueDataImplementation(*rhs._impl);
}

HLAopaqueData::~HLAopaqueData()
{
    delete _impl;
    _impl = NULL;
}

std::auto_ptr<DataElement> HLAopaqueData::clone() const
{
    return std::auto_ptr<DataElement>(new HLAopaqueData(*this));
}

VariableLengthData HLAopaqueData::encode() const
throw (EncoderException)
{
    VariableLengthData vlData;
    encode(vlData);
    return vlData;
}

void HLAopaqueData::encode(VariableLengthData &inData) const
throw (EncoderException)
{
    std::vector<Octet> buffer;
    encodeInto(buffer);
    if(buffer.size() > 0)
        inData.setData(&buffer[0], buffer.size());
}

void HLAopaqueData::encodeInto(std::vector<Octet> &buffer) const
throw (EncoderException)
{
    _impl->encodeInto(buffer);
}

void HLAopaqueData::decode(const VariableLengthData &inData)
throw (EncoderException)
{
    std::vector<Octet> buffer(inData.size());
    if(buffer.size() > 0)
        std::memcpy(&buffer[0], inData.data(), inData.size());
    decodeFrom(buffer, 0);
}

size_t HLAopaqueData::decodeFrom(const std::vector<Octet> &buffer, size_t index)
throw (EncoderException)
{
    return _impl->decodeFrom(buffer, index);
}

size_t HLAopaqueData::getEncodedLength() const
throw (EncoderException)
{
    return _impl->dataSize();
}

unsigned int HLAopaqueData::getOctetBoundary() const
{
    return 1;
}

size_t HLAopaqueData::bufferLength() const
{
    return _impl->bufferSize();
}

size_t HLAopaqueData::dataLength() const
{
    return _impl->dataSize();
}

void HLAopaqueData::setDataPointer(Octet **inData, size_t bufferSize, size_t dataSize)
throw (EncoderException)
{
    _impl->setDataPointer(inData, bufferSize, dataSize);
}

void HLAopaqueData::set(const Octet *inData, size_t dataSize)
{
    _impl->set(inData, dataSize);
}

const Octet *HLAopaqueData::get() const
{
    return _impl->get();
}

HLAopaqueData::operator const Octet *() const
{
    return _impl->get();
}



}
