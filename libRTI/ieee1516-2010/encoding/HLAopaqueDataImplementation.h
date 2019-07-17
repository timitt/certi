#include <vector>
#include <RTI/encoding/EncodingConfig.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <cstring>
#include <memory>


namespace rti1516e {

class HLAopaqueDataImplementation {

public:
    HLAopaqueDataImplementation();
    HLAopaqueDataImplementation(const HLAopaqueDataImplementation &a_rhs);
    HLAopaqueDataImplementation(const Octet *a_inData, size_t a_dataSize);
    HLAopaqueDataImplementation(Octet **a_inData, size_t a_bufferSize, size_t a_dataSize) throw (EncoderException);

    virtual ~HLAopaqueDataImplementation();

    size_t dataSize() const;
    size_t bufferSize() const;

    void setDataPointer(Octet **a_inData, size_t a_bufferSize, size_t a_dataSize) throw (EncoderException);
    void set(const Octet* a_inData, size_t a_dataSize);
    const Octet* get () const;

    void encodeInto(std::vector<Octet>& a_buffer) const throw (EncoderException);
    size_t decodeFrom(std::vector<Octet> const & a_buffer, size_t a_index) throw (EncoderException);

private:
    Octet* _tabOctet;
    size_t _dataSize;
    size_t _bufferSize;
    bool _handleMemory;

};

}
