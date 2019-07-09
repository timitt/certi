#include <memory>
#include <RTI/encoding/DataElement.h>

namespace rti1516e
{

class HLAvariableArrayImplementation
{
public:
    HLAvariableArrayImplementation() = delete;
    HLAvariableArrayImplementation(const DataElement& a_prototype);
    HLAvariableArrayImplementation(const HLAvariableArrayImplementation &a_rhs);
    HLAvariableArrayImplementation& operator=(const HLAvariableArrayImplementation &a_rhs) = delete;
    virtual ~HLAvariableArrayImplementation();

    void encodeInto(std::vector<Octet> &a_buffer) const throw(EncoderException);
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index) throw (EncoderException);

    size_t size() const;

    void addElement(const DataElement &a_dataElement) throw (EncoderException);
    void addElementPointer(DataElement* a_dataElement) throw (EncoderException);

    const DataElement& get(size_t a_index) const throw(EncoderException);
    void set(size_t a_index, const DataElement& a_dataElement) throw (EncoderException);
    void setElementPointer(size_t a_index, DataElement *a_dataElement);

    unsigned int getOctetBoundary() const;
    size_t getEncodedLength() const;

    bool isSameTypeAs(const HLAvariableArrayImplementation& a_rhs);

private:
    uint calculPaddingAfterNbElements() const;
    uint calculPaddingAfterEachElements(const DataElement &a_dataElement) const;

private:
    std::unique_ptr<DataElement> _pDataElementPrototype;
    std::vector<DataElement*> _vectorpDataElement;
};

}
