#include <memory>
#include <RTI/encoding/DataElement.h>

namespace rti1516e
{

class HLAarrayImplementation
{
public:
    HLAarrayImplementation() = delete;
    HLAarrayImplementation(const DataElement& a_prototype);
    HLAarrayImplementation(const HLAarrayImplementation &a_rhs);
    HLAarrayImplementation& operator=(const HLAarrayImplementation &a_rhs) = delete;
    virtual ~HLAarrayImplementation();

    virtual void encodeInto(std::vector<Octet> &a_buffer) const throw(EncoderException) = 0;
    virtual size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index) throw (EncoderException) = 0;

    size_t size() const;

    const DataElement& get(size_t a_index) const throw(EncoderException);
    void set(size_t a_index, const DataElement& a_dataElement) throw (EncoderException);
    void setElementPointer(size_t a_index, DataElement *a_dataElement);

    unsigned int getOctetBoundary() const;
    size_t getEncodedLength() const;

    bool isSameTypeAs(const HLAarrayImplementation& a_rhs);

protected:
    unsigned int calculPaddingAfterNbElements() const;
    unsigned int calculPaddingAfterEachElements(const DataElement &a_dataElement) const;

protected:
    std::unique_ptr<DataElement> _pDataElementPrototype;
    std::vector<DataElement*> _vectorpDataElement;
};

}
