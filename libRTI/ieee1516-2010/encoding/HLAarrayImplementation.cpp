#include <iostream>

#include "HLAarrayImplementation.h"
#include "PrintInfo.h"

namespace rti1516e {
//\brief Constructor of Base class ArrayImplementation
//@param a_prototype A prototype of the DataElement
HLAarrayImplementation::HLAarrayImplementation(const rti1516e::DataElement &a_prototype)
    : _pDataElementPrototype(a_prototype.clone())
{

}

//\brief Copy Constructor of Base class ArrayImplementation
//@param a_prototype A prototype of the DataElement
HLAarrayImplementation::HLAarrayImplementation(const HLAarrayImplementation &a_rhs)
    : _pDataElementPrototype(a_rhs._pDataElementPrototype->clone())
{

}

//\brief Destructor of Base class ArrayImplementation
HLAarrayImplementation::~HLAarrayImplementation()
{
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        if(*it != NULL) {
            delete (*it);
            *it = NULL;
        }
    }
    _vectorpDataElement.clear();
}

//\brief Size of array
//@return The size of array
size_t HLAarrayImplementation::size() const
{
    return _vectorpDataElement.size();
}

//\brief Get an element of the array
//@param a_index Index of the element to get
//@return A DataElement reference
const DataElement &HLAarrayImplementation::get(size_t a_index) const
  throw(EncoderException)
{
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(_vectorpDataElement[a_index] == NULL)
        throw EncoderException(L"DataElement is NULL");
    return *_vectorpDataElement[a_index];
}

//\brief Set an element by this index
//@param a_index Index of the element to get
//@param a_dataElement the element to set
void HLAarrayImplementation::set(size_t a_index, const DataElement &a_dataElement)
  throw (EncoderException)
{
    if(_vectorpDataElement.size() <= a_index)
        throw EncoderException(L"Index out of range");
    if(!a_dataElement.isSameTypeAs(*_pDataElementPrototype))
        throw EncoderException(L"DataElement is not the same type as prototype");
    if(_vectorpDataElement[a_index])
        delete _vectorpDataElement[a_index];
    _vectorpDataElement[a_index] = a_dataElement.clone().release();
}

//\brief Set an pointer to element by this index
//@param a_index Index of the element to get
//@param a_dataElement the pointer to the element to set. The user is responsible of the memory which is pointed.
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

//\brief Get max octet Boundary of the array
//@return the max octet boundary between each element of the array
unsigned int HLAarrayImplementation::getOctetBoundary() const
{
    uint maxBoundarySize = 0;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        maxBoundarySize = std::max((*it)->getOctetBoundary(), maxBoundarySize);
    }
    return maxBoundarySize;
}

//\brief Get encoded length of the array
//@return the encoded length
size_t HLAarrayImplementation::getEncodedLength() const
{
    size_t totalEncodedLength = 0;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        totalEncodedLength += (*it)->getEncodedLength();
    }
    return totalEncodedLength;
}

//\brief Test if is the same type as another array
//@return True if it the same type
bool HLAarrayImplementation::isSameTypeAs(const HLAarrayImplementation &a_rhs)
{
    return _pDataElementPrototype->isSameTypeAs(*a_rhs._pDataElementPrototype);
}

//\brief Calcul padding after NbElements
//@return the number of padding Bytes
uint HLAarrayImplementation::calculPaddingAfterNbElements() const
{
    //Calcul padding after nbElements indication
    uint P = 0;
    uint V = std::max(_pDataElementPrototype->getOctetBoundary(), 4u);
    uint R = (4+P)%V;
//    R = (4+P)&(V-1); //Use this code to replace the modulo calculation by mask intead classic modulo
    P = (R == 0) ? 0:(V-R);

    return P;
}

//\brief Calcul padding after Each elements
//@return the number of padding Bytes after each elements
uint HLAarrayImplementation::calculPaddingAfterEachElements(const DataElement &a_dataElement) const
{
    //Calcul padding after each element which depends on the encoded lenght of the element
    size_t sizeElement = a_dataElement.getEncodedLength();
    uint V = std::max(a_dataElement.getOctetBoundary(), 4u);
    uint P = 0;
    uint R = (sizeElement+P)%V;
//    R = (sizeElement+P)&(V-1); //Use this code to replace the modulo calculation by mask intead classic modulo
    P = (R == 0) ? 0:(V-R);

    return P;
}

}
