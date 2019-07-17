#include "HLAfixedRecordImplementation.h"
#include "PrintInfo.h"

namespace rti1516e {

HLAfixedRecordImplementation::HLAfixedRecordImplementation()
{

}

HLAfixedRecordImplementation::HLAfixedRecordImplementation(const HLAfixedRecordImplementation &a_rhs)
{
    for(DataElement* pElem : a_rhs._vectorpDataElement) {
       if(pElem)
           _vectorpDataElement.push_back(pElem->clone().release());
       else
           _vectorpDataElement.push_back(NULL);
    }
}

HLAfixedRecordImplementation::~HLAfixedRecordImplementation()
{
    for(DataElement* pElem : _vectorpDataElement) {
        if(pElem) {
            delete pElem;
            pElem = NULL;
        }
    }
    _vectorpDataElement.clear();
}

bool HLAfixedRecordImplementation::isSameTypeAs(const HLAfixedRecordImplementation &a_rhs) const
{
    if(_vectorpDataElement.size() != a_rhs._vectorpDataElement.size())
        return false;
    for(size_t i=0; i < a_rhs._vectorpDataElement.size(); i++) {
        if(!_vectorpDataElement[i]->isSameTypeAs(*a_rhs._vectorpDataElement[i]))
            return false;
    }
    return true;
}

bool HLAfixedRecordImplementation::hasElementSameTypeAs(size_t a_index, const DataElement &a_inData) const
{
    if(a_index >= _vectorpDataElement.size())
        return false;
    return a_inData.isSameTypeAs(*_vectorpDataElement[a_index]);
}

size_t HLAfixedRecordImplementation::size() const
{
    return _vectorpDataElement.size();
}

void HLAfixedRecordImplementation::appendElement(const DataElement &a_dataElement)
{
    _vectorpDataElement.push_back(a_dataElement.clone().release());
}

void HLAfixedRecordImplementation::appendElementPointer(DataElement *a_pdataElement)
    throw (EncoderException)
{
    if(!a_pdataElement)
        throw EncoderException(L"Null pointer exception");
    _vectorpDataElement.push_back(a_pdataElement);
}

void HLAfixedRecordImplementation::set(size_t a_index, const DataElement &a_dataElement)
    throw (EncoderException)
{
    if(a_index >= _vectorpDataElement.size())
        throw EncoderException(L"Index out of bound");
    if(!_vectorpDataElement[a_index]->isSameTypeAs(a_dataElement))
        throw EncoderException(L"Data element has not the same type");
    if(_vectorpDataElement[a_index])
        delete _vectorpDataElement[a_index];
    _vectorpDataElement[a_index] = a_dataElement.clone().release();
}

void HLAfixedRecordImplementation::setElementPointer(size_t a_index, DataElement *a_pDataElement)
    throw (EncoderException)
{
    if(!a_pDataElement)
        throw EncoderException(L"DataElement pointer is null");
    if(a_index >= _vectorpDataElement.size())
        throw EncoderException(L"Index out of bound");
    if(!_vectorpDataElement[a_index]->isSameTypeAs(*a_pDataElement))
        throw EncoderException(L"Data element has not the same type");
    if(_vectorpDataElement[a_index])
        delete _vectorpDataElement[a_index];
    _vectorpDataElement[a_index] = a_pDataElement;
}

const DataElement &HLAfixedRecordImplementation::get(size_t a_index) const
    throw (EncoderException)
{
    if(a_index >= _vectorpDataElement.size())
        throw EncoderException(L"Index out of range");
    return *_vectorpDataElement[a_index];
}

size_t HLAfixedRecordImplementation::getEncodedLength() const
    throw (EncoderException)
{
    size_t lenght = 0;
    for(DataElement* pElem : _vectorpDataElement) {
        if(pElem) {
            lenght += pElem->getEncodedLength();
        }
    }
    return lenght;
}

unsigned int HLAfixedRecordImplementation::getOctetBoundary() const
{
    uint maxBoundarySize = 0;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        maxBoundarySize = std::max((*it)->getOctetBoundary(), maxBoundarySize);
    }
    return maxBoundarySize;
}

void HLAfixedRecordImplementation::encodeInto(std::vector<Octet> &a_buffer) const
    throw (EncoderException)
{
    if(_vectorpDataElement.size() == 0)
        throw EncoderException(L"HlaFixedRecord is empty");

    uint offset = 0;
    for(uint i=0; i<_vectorpDataElement.size(); i++)
    {
        if(!_vectorpDataElement[i])
            throw EncoderException(L"DataElement is a NULL pointer");
        _vectorpDataElement[i]->encodeInto(a_buffer);
        uint PElement = this->calculPaddingAfterEachElements(offset, i);
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

size_t HLAfixedRecordImplementation::decodeFrom(const std::vector<Octet> &a_buffer, size_t a_index)
    throw (EncoderException)
{
    if(_vectorpDataElement.size() == 0)
        throw EncoderException(L"HlaFixedRecord is empty");
    if(_vectorpDataElement[0] == NULL)
        throw EncoderException(L"HlaFixedRecord first element is NULL");

    uint offset = 0;
    for(size_t i = 0; i < _vectorpDataElement.size(); i++) {
        a_index = _vectorpDataElement[i]->decodeFrom(a_buffer, a_index);
        size_t paddingEachElem = this->calculPaddingAfterEachElements(offset, i);
        if(i < _vectorpDataElement.size()-1)
            a_index += paddingEachElem;
    }
    PrintInfo<>(Encode::decode, &a_buffer[0], a_buffer.size());
    return a_index;
}

uint HLAfixedRecordImplementation::calculPaddingAfterEachElements(uint &a_offset, const uint a_index) const
{
    uint P = 0;
    if(a_index < _vectorpDataElement.size() - 1) {
        //Calcul padding after each element which depends on the encoded lenght of the element
        size_t sizeElement = _vectorpDataElement[a_index]->getEncodedLength();
        uint V = _vectorpDataElement[a_index + 1]->getOctetBoundary();
        uint R = (a_offset+sizeElement+P)%V;
        std::cout << "R: " << R << std::endl;
        //R = (sizeElement+P)&(V-1); //Use this code to replace the modulo calculation by mask intead classic modulo
        P = (R == 0) ? 0:(V-R);
        std::cout << "P: " << P << std::endl;
        std::cout << "offset: " << a_offset << std::endl;
        a_offset += (sizeElement + P);
        std::cout << "offset: " << a_offset << std::endl;
    }
    else {
        P = 0;
    }

    return P;
}


}
