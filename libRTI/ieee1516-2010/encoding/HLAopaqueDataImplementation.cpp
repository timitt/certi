#include "HLAopaqueDataImplementation.h"
#include <RTI/encoding/HLAvariableArray.h>
#include <RTI/encoding/BasicDataElements.h>
#include <iostream>

namespace rti1516e {

//\brief Constructor of class HLAopaqueDataImplementation
HLAopaqueDataImplementation::HLAopaqueDataImplementation() :
    _tabOctet(NULL), _dataSize(0), _bufferSize(0), _handleMemory(true)
{

}

//\brief Copy Constructor of class HLAopaqueDataImplementation
//@param a_rhs HLAopaqueDataImplementation object to copy
HLAopaqueDataImplementation::HLAopaqueDataImplementation(const rti1516e::HLAopaqueDataImplementation &a_rhs) :
    _tabOctet(NULL), _dataSize(a_rhs._dataSize), _bufferSize(a_rhs._bufferSize), _handleMemory(true)
{
    _tabOctet = new Octet[_dataSize];

    for(size_t i=0; i<_dataSize; i++) {
        if(a_rhs._tabOctet[i]) {
            _tabOctet[i] = a_rhs._tabOctet[i];
        }
    }

}

//\brief Copy Constructor of class HLAopaqueDataImplementation
//@param a_inData pointer to tab Octet
//@param a_dataSize size of tab Octet
HLAopaqueDataImplementation::HLAopaqueDataImplementation(const Octet *a_inData, size_t a_dataSize) :
    _tabOctet(NULL), _dataSize(a_dataSize), _bufferSize(a_dataSize), _handleMemory(true)
{
    _tabOctet = new Octet[_dataSize];

    for(size_t i=0; i<_dataSize; i++) {
        if(a_inData) {
            _tabOctet[i] = a_inData[i];
        }
    }
}

//\brief Constructor of class HLAopaqueDataImplementation. The memory have to be handled by the caller.
//@param a_inData pointer to pointer to tab Octet
//@param a_bufferSize size of memory
//@param a_dataSize size of tab Octet
HLAopaqueDataImplementation::HLAopaqueDataImplementation(Octet **a_inData, size_t a_bufferSize, size_t a_dataSize)
    throw (EncoderException) :
    _dataSize(a_dataSize), _bufferSize(a_bufferSize), _handleMemory(false)
{
    if(!a_inData)
        throw EncoderException(L"Data is a NULL pointer");
    if(!a_bufferSize)
        throw EncoderException(L"Buffer size cannot be 0");
    if(!a_dataSize)
        throw EncoderException(L"Data size cannot be 0");

    _tabOctet = *a_inData;
}

//\brief Destructor of Base class HLAopaqueDataImplementation
HLAopaqueDataImplementation::~HLAopaqueDataImplementation()
{
    if(_handleMemory) {
        delete []_tabOctet;
        _tabOctet = NULL;
    }
}

//\brief Return data size
size_t HLAopaqueDataImplementation::dataSize() const
{
    return _dataSize;
}

//\brief Return buffer size
size_t HLAopaqueDataImplementation::bufferSize() const
{
    return _bufferSize;
}

//\brief set the pointer tab of HLAopaqueDataImplementation. The memory have to be handled by the caller.
//@param a_inData pointer to pointer to tab Octet
//@param a_bufferSize size of memory
//@param a_dataSize size of tab Octet
void HLAopaqueDataImplementation::setDataPointer(Octet **a_inData, size_t a_bufferSize, size_t a_dataSize)
    throw (EncoderException)
{
    if(!a_inData)
        throw EncoderException(L"Data is a NULL pointer");
    if(!a_bufferSize)
        throw EncoderException(L"Buffer size cannot be 0");
    if(!a_dataSize)
        throw EncoderException(L"Data size cannot be 0");

    if(_handleMemory) {
        delete []_tabOctet;
        _tabOctet = NULL;
    }
    _bufferSize = a_bufferSize;
    _dataSize = a_dataSize;
    _handleMemory = false;
    _tabOctet = *a_inData;
}

//\brief set the pointer tab of HLAopaqueDataImplementation.
//@param a_inData pointer to pointer to tab Octet
//@param a_dataSize size of tab Octet
void HLAopaqueDataImplementation::set(const Octet *a_inData, size_t a_dataSize)
{
    if(_handleMemory) {
        delete []_tabOctet;
        _tabOctet = NULL;
    }

    _dataSize = a_dataSize;
    _bufferSize = a_dataSize;
    _handleMemory = true;
    _tabOctet = new Octet[_dataSize];

    for(size_t i=0; i<_dataSize; i++) {
        if(a_inData) {
            _tabOctet[i] = a_inData[i];
        }
    }
}

//\brief pointer to Octet tab
const Octet *HLAopaqueDataImplementation::get() const
{
    return _tabOctet;
}

//\brief Encode elements into a Byte buffer
//@param a_buffer The buffer wich will contain the result of encoding process
void HLAopaqueDataImplementation::encodeInto(std::vector<Octet> &a_buffer) const
    throw (EncoderException)
{
    HLAbyte hLAoctet;
    HLAvariableArray hLAvariableArray(hLAoctet);

    for(size_t i=0; i<_dataSize; i++) {
        HLAbyte hlaOctetItem(_tabOctet[i]);
        hLAvariableArray.addElement(hlaOctetItem);
    }

    hLAvariableArray.encodeInto(a_buffer);
}

//\brief decode elements from a Byte buffer
//@param a_buffer The buffer wich will contain the buffer to decode
//@param a_index The start index to decode
//@return The number of Byte in buffer decoded
size_t HLAopaqueDataImplementation::decodeFrom(const std::vector<Octet> &a_buffer, size_t a_index)
    throw (EncoderException)
{
    HLAbyte hLAoctet;
    HLAvariableArray hLAvariableArray(hLAoctet);

    hLAvariableArray.decodeFrom(a_buffer, a_index);
    if(hLAvariableArray.size() != _dataSize)
        throw EncoderException(L"Buffer size doesn't match with HLAopaqueData object size");

    size_t globalIndex=0;
    for(size_t i=0; i<_dataSize; i++) {
        _tabOctet[i] = static_cast<const HLAoctet&>(hLAvariableArray.get(i));
    }

    return a_index+globalIndex;
}

}
