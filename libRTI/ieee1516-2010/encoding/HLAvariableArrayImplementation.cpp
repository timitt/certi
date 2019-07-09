#include <iostream>

#include <ieee1516-2010/RTI/encoding/BasicDataElements.h>

#include "HLAvariableArrayImplementation.h"
#include "PrintInfo.h"

namespace rti1516e {

HLAvariableArrayImplementation::HLAvariableArrayImplementation(const rti1516e::DataElement &a_prototype)
    : _pDataElementPrototype(a_prototype.clone())
{

}

HLAvariableArrayImplementation::HLAvariableArrayImplementation(const HLAvariableArrayImplementation &a_rhs)
    : _pDataElementPrototype(a_rhs._pDataElementPrototype->clone())
{
    for(auto it = a_rhs._vectorpDataElement.begin(); it != a_rhs._vectorpDataElement.end(); it++)
    {
        if(*it != NULL)
            _vectorpDataElement.push_back((*it)->clone().release());
    }
}

HLAvariableArrayImplementation::~HLAvariableArrayImplementation()
{
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        delete (*it);
        *it = NULL;
    }
    _vectorpDataElement.clear();
}

void HLAvariableArrayImplementation::encodeInto(std::vector<Octet> &a_buffer) const
  throw(EncoderException)
{
    if(this->size() > INT32_MAX)
    {
        throw EncoderException(L"Array size is too big for int32");
    }
    HLAinteger32BE nbElements(Integer32(this->size()));
    nbElements.encodeInto(a_buffer);

    uint P = 0;
    uint V = std::max(this->get(0).getOctetBoundary(), 4u);
    uint R = (4+P)%V;
    P += R == 0 ? 0:(V-R);
    R = (4+P)%V;

    for(uint i = 0; i < P; i++)
    {
        const Octet octet(0);
        a_buffer.push_back(octet);
    }

    for(uint i=0; i<_vectorpDataElement.size(); i++)
    {
        _vectorpDataElement[i]->encodeInto(a_buffer);
        if(i != _vectorpDataElement.size()-1)
        {
            size_t sizeElement = _vectorpDataElement[i]->getEncodedLength();
            uint VElement = std::max(_vectorpDataElement[i]->getOctetBoundary(), 4u);
            uint PElement = 0;
            uint RElement = (sizeElement+PElement)%VElement;
            std::cout << "Reste : " << RElement << std::endl;
            PElement += RElement == 0 ? 0:(VElement-RElement);
            std::cout << "P : " << PElement << std::endl;
            RElement = (sizeElement+PElement)%VElement;
            std::cout << "Reste : " << RElement << std::endl;
            for(uint j = 0; j < PElement; j++)
            {
                const Octet octet(0);
                a_buffer.push_back(octet);
            }
        }
    }

    PrintInfo<>(Encode::encode, &a_buffer[0], a_buffer.size());

}

size_t HLAvariableArrayImplementation::size() const
{
    return _vectorpDataElement.size();
}

void HLAvariableArrayImplementation::addElement(const DataElement &a_dataElement)
  throw(EncoderException)
{
    if(!a_dataElement.isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"Data type is not the same than prototype");
    _vectorpDataElement.push_back(a_dataElement.clone().release());
}

void HLAvariableArrayImplementation::addElementPointer(DataElement *a_dataElement)
  throw (EncoderException)
{
    if(a_dataElement == NULL)
        throw EncoderException(L"Data Element pointer is NULL");
    if(!a_dataElement->isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"Data type is not the same than prototype");
    _vectorpDataElement.push_back(a_dataElement);
}

const DataElement &HLAvariableArrayImplementation::get(size_t a_index) const
  throw(EncoderException)
{
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(_vectorpDataElement[a_index] == NULL)
        throw EncoderException(L"DataElement is NULL");
    return *_vectorpDataElement[a_index];
}

void HLAvariableArrayImplementation::set(size_t a_index, const DataElement &a_dataElement)
  throw (EncoderException)
{
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(!a_dataElement.isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"DataElement is not the same type as prototype");
    _vectorpDataElement[a_index] = a_dataElement.clone().release();
}

void HLAvariableArrayImplementation::setElementPointer(size_t a_index, DataElement *a_dataElement)
{
    if(a_dataElement == NULL)
        throw EncoderException(L"DataElement is NULL");
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(!a_dataElement->isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"DataElement is not the same type as prototype");
    _vectorpDataElement[a_index] = a_dataElement;
}

unsigned int HLAvariableArrayImplementation::getOctetBoundary() const
{
    uint maxBoundarySize = 0;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        maxBoundarySize = std::max((*it)->getOctetBoundary(), maxBoundarySize);
    }
    return maxBoundarySize;
}

size_t HLAvariableArrayImplementation::getEncodedLength() const
{
    size_t totalEncodedLength = 0;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        totalEncodedLength += (*it)->getEncodedLength();
    }
    return totalEncodedLength;
}

bool HLAvariableArrayImplementation::isSameTypeAs(const HLAvariableArrayImplementation &a_rhs)
{
    return _pDataElementPrototype->isSameTypeAs(*a_rhs._pDataElementPrototype);
}

}
