#include <iostream>

#include <ieee1516-2010/RTI/encoding/BasicDataElements.h>

#include "HLAarrayImplementation.h"
#include "PrintInfo.h"

namespace rti1516e {

HLAarrayImplementation::HLAarrayImplementation(const rti1516e::DataElement &a_prototype)
    : _pDataElementPrototype(a_prototype.clone())
{

}

HLAarrayImplementation::HLAarrayImplementation(const HLAarrayImplementation &a_rhs)
    : _pDataElementPrototype(a_rhs._pDataElementPrototype->clone())
{
    for(auto it = a_rhs._vectorpDataElement.begin(); it != a_rhs._vectorpDataElement.end(); it++)
    {
        if(*it != NULL)
            _vectorpDataElement.push_back((*it)->clone().release());
    }
}

HLAarrayImplementation::~HLAarrayImplementation()
{
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        delete (*it);
        *it = NULL;
    }
    _vectorpDataElement.clear();
}

size_t HLAarrayImplementation::size() const
{
    return _vectorpDataElement.size();
}

void HLAarrayImplementation::addElement(const DataElement &a_dataElement)
  throw(EncoderException)
{
    if(!a_dataElement.isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"Data type is not the same than prototype");
    _vectorpDataElement.push_back(a_dataElement.clone().release());
}

void HLAarrayImplementation::addElementPointer(DataElement *a_dataElement)
  throw (EncoderException)
{
    if(a_dataElement == NULL)
        throw EncoderException(L"Data Element pointer is NULL");
    if(!a_dataElement->isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"Data type is not the same than prototype");
    _vectorpDataElement.push_back(a_dataElement);
}

const DataElement &HLAarrayImplementation::get(size_t a_index) const
  throw(EncoderException)
{
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(_vectorpDataElement[a_index] == NULL)
        throw EncoderException(L"DataElement is NULL");
    return *_vectorpDataElement[a_index];
}

void HLAarrayImplementation::set(size_t a_index, const DataElement &a_dataElement)
  throw (EncoderException)
{
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(!a_dataElement.isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"DataElement is not the same type as prototype");
    _vectorpDataElement[a_index] = a_dataElement.clone().release();
}

void HLAarrayImplementation::setElementPointer(size_t a_index, DataElement *a_dataElement)
{
    if(a_dataElement == NULL)
        throw EncoderException(L"DataElement is NULL");
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(!a_dataElement->isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"DataElement is not the same type as prototype");
    _vectorpDataElement[a_index] = a_dataElement;
}

unsigned int HLAarrayImplementation::getOctetBoundary() const
{
    uint maxBoundarySize = 0;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        maxBoundarySize = std::max((*it)->getOctetBoundary(), maxBoundarySize);
    }
    return maxBoundarySize;
}

size_t HLAarrayImplementation::getEncodedLength() const
{
    size_t totalEncodedLength = 0;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        totalEncodedLength += (*it)->getEncodedLength();
    }
    return totalEncodedLength;
}

bool HLAarrayImplementation::isSameTypeAs(const HLAarrayImplementation &a_rhs)
{
    return _pDataElementPrototype->isSameTypeAs(*a_rhs._pDataElementPrototype);
}

}
