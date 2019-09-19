#include <cstring>
#include <RTI/encoding/HLAfixedRecord.h>
#include <RTI/VariableLengthData.h>
#include "HLAfixedRecordImplementation.h"

namespace rti1516e {

HLAfixedRecord::HLAfixedRecord() :
    _impl(new HLAfixedRecordImplementation())
{

}

HLAfixedRecord::HLAfixedRecord(const HLAfixedRecord &rhs) :
    _impl(new HLAfixedRecordImplementation(*rhs._impl))
{

}

HLAfixedRecord::~HLAfixedRecord()
{
    if(_impl) {
        delete _impl;
        _impl = NULL;
    }

}

std::auto_ptr<DataElement> HLAfixedRecord::clone() const
{
    return std::auto_ptr<rti1516e::DataElement>(new HLAfixedRecord(*this));
}

VariableLengthData HLAfixedRecord::encode() const
throw (EncoderException)
{
    VariableLengthData vlData;
    encode(vlData);
    return vlData;
}

void HLAfixedRecord::encode(VariableLengthData &inData) const
throw (EncoderException)
{
    std::vector<Octet> buffer;
    encodeInto(buffer);
    if(buffer.size() > 0)
        inData.setData(&buffer[0], buffer.size());
}

void HLAfixedRecord::encodeInto(std::vector<Octet> &buffer) const
throw (EncoderException)
{
    _impl->encodeInto(buffer);
}

void HLAfixedRecord::decode(const VariableLengthData &inData)
throw (EncoderException)
{
    std::vector<Octet> buffer(inData.size());
    if(buffer.size() > 0)
        std::memcpy(&buffer[0], inData.data(), inData.size());
    decodeFrom(buffer, 0);
}

size_t HLAfixedRecord::decodeFrom(const std::vector<Octet> &buffer, size_t index)
throw (EncoderException)
{
    return _impl->decodeFrom(buffer, index);
}

size_t HLAfixedRecord::getEncodedLength() const
throw (EncoderException)
{
    return _impl->getEncodedLength();
}

unsigned int HLAfixedRecord::getOctetBoundary() const
{
    return _impl->getOctetBoundary();
}

bool HLAfixedRecord::isSameTypeAs(const DataElement &inData) const
{
    const HLAfixedRecord *fixedRecord = static_cast<const HLAfixedRecord*>(&inData);
    if(!fixedRecord)
        return false;
    return _impl->isSameTypeAs(*fixedRecord->_impl);
}

bool HLAfixedRecord::hasElementSameTypeAs(size_t index, const DataElement &inData) const
{
    return _impl->hasElementSameTypeAs(index, inData);
}

size_t HLAfixedRecord::size() const
{
    return _impl->size();
}

void HLAfixedRecord::appendElement(const DataElement &dataElement)
{
    _impl->appendElement(dataElement);
}

void HLAfixedRecord::appendElementPointer(DataElement *dataElement)
{
    _impl->appendElementPointer(dataElement);
}

void HLAfixedRecord::set(size_t index, const DataElement &dataElement)
throw (EncoderException)
{
    _impl->set(index, dataElement);
}

void HLAfixedRecord::setElementPointer(size_t index, DataElement *dataElement)
throw (EncoderException)
{
    _impl->setElementPointer(index, dataElement);
}

const DataElement &HLAfixedRecord::get(size_t index) const
throw (EncoderException)
{
    return _impl->get(index);
}

const DataElement &HLAfixedRecord::operator [](size_t index) const
throw (EncoderException)
{
    return _impl->get(index);
}

}
