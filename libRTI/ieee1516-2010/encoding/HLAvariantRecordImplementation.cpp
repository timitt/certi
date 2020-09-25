#include "HLAvariantRecordImplementation.h"
#include <RTI/encoding/Padding.h>
#include "PrintInfo.h"
#include <stdexcept>

namespace rti1516e {

//\brief Default constructor of class HLAvariantRecordImplementation
//@param a_prototype The prototype of the discriminant. This is necessary because discriminant must have the same type.
HLAvariantRecordImplementation::HLAvariantRecordImplementation(const DataElement &a_prototype) :
    _pDataElementDiscriminent(a_prototype.clone().release())
{

}

//\brief Copy constructor of class HLAvariantRecordImplementation
//@param a_rhs The HLAvariantRecordImplementation object which have to be copied
HLAvariantRecordImplementation::HLAvariantRecordImplementation(const HLAvariantRecordImplementation &a_rhs) :
    _pDataElementDiscriminent(a_rhs._pDataElementDiscriminent->clone().release())
{
    for(auto it = a_rhs._vectorpDataElement.begin(); it != a_rhs._vectorpDataElement.end(); it++)
    {
        if(it->second != NULL) {
            _vectorpDataElement[it->first] = it->second->clone().release();
        }
    }
}

//\brief Default constructor of class HLAvariantRecordImplementation
HLAvariantRecordImplementation::~HLAvariantRecordImplementation()
{
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++)
    {
        if(it->first) {
            delete (it->first);
        }
        if(it->second) {
            delete (it->second);
            it->second = NULL;
        }
    }
    _vectorpDataElement.clear();

}

//\brief Get the encoded length in the HLAvariantRecordImplementation object
//@return The size encoded
size_t HLAvariantRecordImplementation::getEncodedLength() const
throw (EncoderException)
{
    try
    {
        if(!hasMatchingDiscriminantTypeAs(*_pDataElementDiscriminent))
            return 0;

        size_t sizeEncoded = 0;
        for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++) {
            sizeEncoded += it->first->getEncodedLength() + it->second->getEncodedLength();
        }

        return sizeEncoded;
    } catch(const std::out_of_range& e) {
        throw EncoderException(L"Out of Range Exception");
    }
}

//\brief Get the octet boundary in the HLAvariantRecordImplementation object
//@return The octet boundary
unsigned int HLAvariantRecordImplementation::getOctetBoundary() const
{
    unsigned int maxOctetBoundary = 0;

    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++) {
        maxOctetBoundary = std::max(maxOctetBoundary, it->second->getOctetBoundary());
    }

    return maxOctetBoundary;
}

//\brief Compare type of discriminant of HLAvariantRecordImplementation with another object.
//@param a_rhs The HLAvariantRecordImplementation object which have to be compared
//@return True if the types are same
bool HLAvariantRecordImplementation::isSameTypeAs(const HLAvariantRecordImplementation &a_rhs)
{
    return _pDataElementDiscriminent->isSameTypeAs(*a_rhs._pDataElementDiscriminent);
}

//\brief Compare type inData with the variant which correspond to the discriminent in parameter.
//@param discriminant The discriminant which allow to access to the variant
//@param inData The data type to compare with the type stored in the HLAvariantRecordImplementation object
//@return True if the types are same
bool HLAvariantRecordImplementation::isSameTypeAs(const DataElement &discriminant, const DataElement &inData) const
throw (EncoderException)
{
    try {
        DataElement *pValue = this->c_at(discriminant).second;
        return pValue->isSameTypeAs(inData);
    } catch(const std::out_of_range& e) {
        throw EncoderException(L"Out of Range Exception");
    }
}

//\brief Search if a discriminent stored has the same type than the parameter a_dataElement.
//@param a_dataElement Search if a variant exist with the type of this parameter
//@return True if the types are same
bool HLAvariantRecordImplementation::hasMatchingDiscriminantTypeAs(const DataElement &a_dataElement) const
{
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++) {
       if(it->first->hash() == a_dataElement.hash())
           return true;
    }
    return false;
}

//\brief Add a variant with it's mapping discriminant.
//@param a_discriminant The discriminant of the variant. If the discriment exist, the variant is replaced.
//@param a_valuePrototype The variant to be added. A copy is store inside HLAvariantRecordImplementation object.
void HLAvariantRecordImplementation::addVariant(const DataElement &a_discriminant, const DataElement &a_valuePrototype)
throw (EncoderException)
{
    if(!_pDataElementDiscriminent->isSameTypeAs(a_discriminant))
        throw EncoderException(L"Discriminant must have the same type than prototype");
    if(_pDataElementDiscriminent) {
        if(!this->hasMatchingDiscriminantTypeAs(*_pDataElementDiscriminent)) {
            delete _pDataElementDiscriminent;
            _pDataElementDiscriminent = NULL;
        }
    }
    if(this->hasMatchingDiscriminantTypeAs(a_discriminant)) {
        DataElement *pDataElement = this->at(a_discriminant).second;
        if(!pDataElement) {
            delete pDataElement;
            pDataElement = NULL;
        }
        pDataElement = a_valuePrototype.clone().release();
        _pDataElementDiscriminent = this->at(a_discriminant).first;
    } else {
        _vectorpDataElement.emplace(a_discriminant.clone().release(), a_valuePrototype.clone().release());
        _pDataElementDiscriminent = this->at(a_discriminant).first;
    }

}

//\brief Add a variant with it's mapping discriminant.
//@param a_discriminant The discriminant of the variant. If the discriment exist, the variant is replaced.
//@param a_valuePtr A pointer to the variant to be added. The pointer is directly stored, there is no copy.
void HLAvariantRecordImplementation::addVariantPointer(const DataElement &a_discriminant, DataElement *a_valuePtr)
throw(EncoderException)
{
    if(a_valuePtr == NULL)
        throw EncoderException(L"Data Element pointer is NULL");
    if(!_pDataElementDiscriminent->isSameTypeAs(a_discriminant))
        throw EncoderException(L"Discriminant must have the same type than prototype");

    if(this->hasMatchingDiscriminantTypeAs(a_discriminant)) {
        DataElement *pDataElement = this->at(a_discriminant).second;
        if(!pDataElement) {
            delete pDataElement;
            pDataElement = NULL;
        }
        pDataElement = a_valuePtr;
        _pDataElementDiscriminent = this->at(a_discriminant).first;
    } else {
        _vectorpDataElement.emplace(a_discriminant.clone().release(), a_valuePtr);
        if(_pDataElementDiscriminent)
            delete _pDataElementDiscriminent;
        _pDataElementDiscriminent = this->at(a_discriminant).first;
    }
}

//\brief Select the current discriminant from stored discriminant.
//@param a_discriminant The discriminant of the variant. If the discriment dosn't exist, an exception is raised.
void HLAvariantRecordImplementation::setDiscriminant(const DataElement &a_discriminant)
throw (EncoderException)
{
    if(!_pDataElementDiscriminent->isSameTypeAs(a_discriminant))
        throw EncoderException(L"Discriminant must have the same type than prototype");

    DataElement *dataElemtoChange = NULL;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++) {
        if(it->first->hash() == a_discriminant.hash()){
            dataElemtoChange = it->first;
        }
    }
    if(dataElemtoChange) {
        _pDataElementDiscriminent = dataElemtoChange;
    } else {
        throw EncoderException(L"This discriminent doesn't exist");
    }
}

//\brief Update a variant with it's mapping discriminant.
//@param a_discriminant The discriminant of the variant. If the discriment dosn't exist, an exception is raised.
//@param a_value The variant to be added. A copy is store inside HLAvariantRecordImplementation object. The value
//                  type must correspond to the old one.
void HLAvariantRecordImplementation::setVariant(const DataElement &a_discriminant, const DataElement &a_value)
throw(EncoderException)
{
    if(!_pDataElementDiscriminent->isSameTypeAs(a_discriminant))
        throw EncoderException(L"Discriminant must have the same type than prototype");

    if(this->hasMatchingDiscriminantTypeAs(a_discriminant)) {
        DataElement *pElem = this->at(a_discriminant).second;
        if(!pElem->isSameTypeAs(a_value))
            throw EncoderException(L"Value doesn't have the same value than anterior value");
        delete pElem;
        pElem = a_value.clone().release();
        _pDataElementDiscriminent = this->at(a_discriminant).first;
    } else {
        throw EncoderException(L"This discriminent doesn't exist");
    }
}

//\brief Update a variant with it's mapping discriminant.
//@param a_discriminant The discriminant of the variant. If the discriment dosn't exist, an exception is raised.
//@param a_valuePtr A pointer to the variant to be added. The pointer is directly stored, there is no copy. The value
//                  type must correspond to the old one.
void HLAvariantRecordImplementation::setVariantPointer(const DataElement &a_discriminant, DataElement *a_valuePtr)
throw(EncoderException)
{
    if(a_valuePtr == NULL)
        throw EncoderException(L"Data Element pointer is NULL");
    if(!_pDataElementDiscriminent->isSameTypeAs(a_discriminant))
        throw EncoderException(L"Discriminant must have the same type than prototype");

    if(this->hasMatchingDiscriminantTypeAs(a_discriminant)) {
        DataElement *pElem = this->at(a_discriminant).second;
        if(!pElem->isSameTypeAs(*a_valuePtr))
            throw EncoderException(L"Value doesn't have the same value than anterior value");
        delete pElem;
        pElem = a_valuePtr;
        _pDataElementDiscriminent = this->at(a_discriminant).first;
    } else {
        throw EncoderException(L"This discriminent doesn't exist");
    }
}

//\brief Get the current discriment of the HLAvariantRecordImplementation object.
//@return the DataElement which is the current discriminant.
const DataElement &HLAvariantRecordImplementation::getDiscriminant() const
{
    return *_pDataElementDiscriminent;
}

//\brief Get the variant which correspond to the current discriminant.
//@return the DataElement which is the variant mapped by the current discriminant.
const DataElement &HLAvariantRecordImplementation::getVariant() const
throw(EncoderException)
{
    if(this->hasMatchingDiscriminantTypeAs(*_pDataElementDiscriminent)) {
        const DataElement* pElem = this->c_at(*_pDataElementDiscriminent).second;
        return *pElem;
    } else {
        throw EncoderException(L"This discriminent isn't mapped");
    }
}

//\brief Encode elements into a Byte buffer
//@param a_buffer The buffer wich will contain the result of encoding process
void HLAvariantRecordImplementation::encodeInto(std::vector<Octet> &a_buffer) const
throw(EncoderException)
{
    _pDataElementDiscriminent->encodeInto(a_buffer);

    unsigned int P = this->calculPaddingAfterDiscriminant();

    for(unsigned int i = 0; i < P; i++)
    {
        const Octet octet(0);
        a_buffer.push_back(octet);
    }

    this->c_at(*_pDataElementDiscriminent).second->encodeInto(a_buffer);

    PrintInfo<>(Encode::encode, &a_buffer[0], a_buffer.size());
}

//\brief decode elements from a Byte buffer
//@param a_buffer The buffer wich will contain the buffer to decode
//@param a_index The start index to decode
//@return The number of Byte in buffer decoded
size_t HLAvariantRecordImplementation::decodeFrom(const std::vector<Octet> &a_buffer, size_t a_index)
throw(EncoderException)
{
    a_index += _pDataElementDiscriminent->decodeFrom(a_buffer, a_index);
    unsigned int P = this->calculPaddingAfterDiscriminant();
    a_index += P;
    DataElement *pElemValue = this->at(*_pDataElementDiscriminent).second;
    pElemValue->decodeFrom(a_buffer, a_index);
    return this->getEncodedLength();
}

std::pair<DataElement*, DataElement*> HLAvariantRecordImplementation::at(const DataElement &a_dataElementIndex)
throw(EncoderException)
{
    bool finded = false;
    for(auto it = _vectorpDataElement.begin(); it != _vectorpDataElement.end(); it++) {
        if(it->first != NULL) {
            if(it->first->hash() == a_dataElementIndex.hash()) {
                finded = true;
                return std::pair<DataElement*, DataElement*>(it->first, it->second);
            }
        }
    }

    if(!finded)
        throw EncoderException(L"The discriminant doesn't exist !");
    return std::pair<DataElement*, DataElement*>(NULL, NULL);
}

const std::pair<DataElement*, DataElement*> HLAvariantRecordImplementation::c_at(const DataElement &a_dataElementIndex) const
throw(EncoderException)
{
    bool finded = false;
    for(auto it = _vectorpDataElement.cbegin(); it != _vectorpDataElement.cend(); it++) {
        if(it->first->hash() == a_dataElementIndex.hash()) {
            finded = true;
            return std::pair<DataElement*, DataElement*>(it->first, it->second);
        }
    }

    if(!finded)
        throw EncoderException(L"The discriminant doesn't exist !");
    return std::pair<DataElement*, DataElement*>(NULL, NULL);
}

unsigned int HLAvariantRecordImplementation::calculPaddingAfterDiscriminant() const
{
    //Calcul padding after nbElements indication
//    unsigned int P = 0;
//    unsigned int V = this->getOctetBoundary();
//    unsigned int S = _pDataElementDiscriminent->getEncodedLength();
//    unsigned int R = (S+P)%V;
////    R = (S+P)&(V-1); //Use this code to replace the modulo calculation by mask intead classic modulo
//    P = (R == 0) ? 0:(V-R);

    unsigned int P = padding::VariantRecord::calculPaddingAfterDiscriminant(_pDataElementDiscriminent->getEncodedLength(), this->getOctetBoundary());
    return P;
}



}
