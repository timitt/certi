#include <RTI/encoding/EncodingExceptions.h>

#include <string>

namespace rti1516e
{

EncoderException::EncoderException(std::wstring const& message) throw() :
  _msg(message)
{
}

std::wstring EncoderException::what () const throw()
{
  return _msg;
}

}
