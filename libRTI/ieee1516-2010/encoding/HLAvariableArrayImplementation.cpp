#include <iostream>

#include <ieee1516-2010/RTI/encoding/BasicDataElements.h>

#include "HLAvariableArrayImplementation.h"
#include "PrintInfo.h"

namespace rti1516e {

//\brief Constructor of class VariableArrayImplementation
//@param a_prototype A prototype of the DataElement
HLAvariableArrayImplementation::HLAvariableArrayImplementation(const rti1516e::DataElement &a_prototype)
 : HLAarrayImplementation(a_prototype)
{

}

//\brief Copy Constructor of class VariableArrayImplementation
//@param a_prototype A prototype of the DataElement
HLAvariableArrayImplementation::HLAvariableArrayImplementation(const rti1516e::HLAvariableArrayImplementation &a_rhs)
 : HLAarrayImplementation(a_rhs)
{
    for(auto it = a_rhs._vectorpDataElement.begin(); it != a_rhs._vectorpDataElement.end(); it++)
    {
        if(*it != NULL)
            _vectorpDataElement.push_back((*it)->clone().release());
    }
}

//\brief Add dynamically an element
//@param a_dataElement the element to set
void HLAvariableArrayImplementation::addElement(const DataElement &a_dataElement)
  throw(EncoderException)
{
    if(!a_dataElement.isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"Data type is not the same than prototype");
    _vectorpDataElement.push_back(a_dataElement.clone().release());
}

//\brief Add a pointer to element
//@param a_dataElement the pointer to the element to set. The user is responsible of the memory which is pointed.
void HLAvariableArrayImplementation::addElementPointer(DataElement *a_dataElement)
  throw (EncoderException)
{
    if(a_dataElement == NULL)
        throw EncoderException(L"Data Element pointer is NULL");
    if(!a_dataElement->isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"Data type is not the same than prototype");
    _vectorpDataElement.push_back(a_dataElement);
}

//\brief Encode elements into a Byte buffer
//@param a_buffer The buffer wich will contain the result of encoding process
void HLAvariableArrayImplementation::encodeInto(std::vector<Octet> &a_buffer) const
  throw(EncoderException)
{
    if(this->size() > INT32_MAX)
    {
        throw EncoderException(L"Array size is too big for int32");
    }
    HLAinteger32BE nbElements(Integer32(this->size()));
    nbElements.encodeInto(a_buffer);

    uint P = this->calculPaddingAfterNbElements();

    for(uint i = 0; i < P; i++)
    {
        const Octet octet(0);
        a_buffer.push_back(octet);
    }

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
    if(a_buffer.size() > 0)
        PrintInfo<>(Encode::encode, &a_buffer[0], a_buffer.size());
}

//\brief decode elements from a Byte buffer
//@param a_buffer The buffer wich will contain the buffer to decode
//@param a_index The start index to decode
//@return The number of Byte in buffer decoded
size_t HLAvariableArrayImplementation::decodeFrom(const std::vector<Octet> &a_buffer, size_t a_index)
    throw (EncoderException)
{
    HLAinteger32BE nbElements;
    a_index = nbElements.decodeFrom(a_buffer, a_index);
    uint P = this->calculPaddingAfterNbElements();
    a_index += P;
    _vectorpDataElement.reserve(nbElements.get());
    for(int i=0; i<nbElements.get(); i++) {
        _vectorpDataElement.push_back(_pDataElementPrototype->clone().release());
    }
    size_t paddingEachElem = 0;
    if(_vectorpDataElement.size() > 0)
        paddingEachElem = this->calculPaddingAfterEachElements(*_vectorpDataElement[0]);
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++) {
        a_index = (*it)->decodeFrom(a_buffer, a_index);
        if(it+1 != _vectorpDataElement.end())
            a_index += paddingEachElem;
    }
    if(a_buffer.size() > 0)
        PrintInfo<>(Encode::decode, &a_buffer[0], a_buffer.size());
    return a_index;
}

}
