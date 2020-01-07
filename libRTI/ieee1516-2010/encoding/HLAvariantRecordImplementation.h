#include <RTI/encoding/DataElement.h>
#include <map>
#include <typeindex>

namespace rti1516e
{

class HLAvariantRecordImplementation
{
public:
    HLAvariantRecordImplementation() = delete;
    HLAvariantRecordImplementation(const DataElement& a_prototype);
    HLAvariantRecordImplementation(const HLAvariantRecordImplementation &a_rhs);
    virtual ~HLAvariantRecordImplementation();
    HLAvariantRecordImplementation& operator=(const HLAvariantRecordImplementation &a_rhs) = delete;

    size_t getEncodedLength() const throw (EncoderException);
    unsigned int getOctetBoundary () const;

    bool isSameTypeAs(const HLAvariantRecordImplementation& a_rhs);
    bool isSameTypeAs(DataElement const& a_discriminant, DataElement const& a_inData ) const
        throw (EncoderException);
    bool hasMatchingDiscriminantTypeAs(DataElement const& a_dataElement ) const;

    void addVariant(const DataElement& a_discriminant, const DataElement& a_valuePrototype)
       throw (EncoderException);
    void addVariantPointer(const DataElement& a_discriminant, DataElement* a_valuePtr)
       throw (EncoderException);
    void setDiscriminant(const DataElement& a_discriminant)
       throw (EncoderException);
    void setVariant(const DataElement& a_discriminant, DataElement const& a_value)
       throw (EncoderException);
    void setVariantPointer(const DataElement& a_discriminant, DataElement* a_valuePtr)
       throw (EncoderException);

    const DataElement& getDiscriminant () const;
    const DataElement& getVariant() const
       throw (EncoderException);

    void encodeInto(std::vector<Octet>& a_buffer) const
       throw (EncoderException);
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index)
       throw (EncoderException);

private:
    std::pair<DataElement*, DataElement*> at(const DataElement &a_dataElementIndex) throw(EncoderException);
    const std::pair<DataElement*, DataElement*> c_at(const DataElement &a_dataElementIndex) const throw(EncoderException);
    unsigned int calculPaddingAfterDiscriminant() const;

private:
    DataElement *_pDataElementDiscriminent;
    std::map<DataElement*, DataElement*> _vectorpDataElement;
};

}
