#include "HLAfixedArrayImplementation.h"
#include <RTI/encoding/BasicDataElements.h>
#include "PrintInfo.h"

namespace rti1516e {

//\brief Constructor of class ArrayFixedImplementation
//@param a_prototype A prototype of the DataElement
//@param a_length The size of the fixed array
HLAfixedArrayImplementation::HLAfixedArrayImplementation(const DataElement &a_prototype, size_t a_length)
 : HLAarrayImplementation(a_prototype), _length(a_length)
{
    for(size_t i=0; i<_length; i++) {
        _vectorpDataElement.push_back(a_prototype.clone().release());
    }
}

//\brief Copy Constructor of class ArrayFixedImplementation
//@param a_rhs ArrayFixedImplementation onject to copy
HLAfixedArrayImplementation::HLAfixedArrayImplementation(const HLAfixedArrayImplementation &a_rhs)
 : HLAarrayImplementation(a_rhs), _length(a_rhs._length)
{
    _vectorpDataElement.resize(_length, 0);
    for(size_t i=0; i<a_rhs.size(); i++) {
        if(a_rhs._vectorpDataElement[i])
            _vectorpDataElement[i] = a_rhs._vectorpDataElement[i]->clone().release();
    }
}

//\brief Destructor of Base class ArrayImplementation
HLAfixedArrayImplementation::~HLAfixedArrayImplementation()
{
    for(size_t i=0; i < _vectorpDataElement.size(); i++) {
        if(_vectorpDataElement[i])
            delete _vectorpDataElement[i];
        _vectorpDataElement[i] = NULL;
    }
    _vectorpDataElement.clear();
}

//\brief Encode elements into a Byte buffer
//@param a_buffer The buffer wich will contain the result of encoding process
void HLAfixedArrayImplementation::encodeInto(std::vector<Octet> &a_buffer) const
  throw(EncoderException)
{
    if(_vectorpDataElement.size() == 0)
        throw EncoderException(L"HlaFixedArray is empty");
    else if(_vectorpDataElement[0] == NULL)
        throw EncoderException(L"HlaFixedArray first element is NULL");

    uint PElement = 0;
    if(_vectorpDataElement.size() > 0)
        PElement = this->calculPaddingAfterEachElements(*_vectorpDataElement[0]);

    for(uint i=0; i<_vectorpDataElement.size(); i++)
    {
        _vectorpDataElement[i]->encodeInto(a_buffer);
        if(i != _vectorpDataElement.size()-1)
        {
            for(uint j = 0; j < PElement; j++)
            {
                const Octet octet(0);
                a_buffer.push_back(octet);
            }
        }
    }
    PrintInfo<>(Encode::encode, &a_buffer[0], a_buffer.size());
}

//\brief decode elements from a Byte buffer
//@param a_buffer The buffer wich will contain the buffer to decode
//@param a_index The start index to decode
//@return The number of Byte in buffer decoded
size_t HLAfixedArrayImplementation::decodeFrom(const std::vector<Octet> &a_buffer, size_t a_index)
    throw (EncoderException)
{
    if(_vectorpDataElement.size() == 0)
        throw EncoderException(L"HlaFixedArray is empty");
    if(_vectorpDataElement[0] == NULL)
        throw EncoderException(L"HlaFixedArray first element is NULL");

    size_t paddingEachElem = 0;
    if(_vectorpDataElement.size() > 0)
        paddingEachElem = this->calculPaddingAfterEachElements(*_vectorpDataElement[0]);
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++) {
        a_index = (*it)->decodeFrom(a_buffer, a_index);
        if(it+1 != _vectorpDataElement.end())
            a_index += paddingEachElem;
    }
    PrintInfo<>(Encode::decode, &a_buffer[0], a_buffer.size());
    return a_index;
}

}
