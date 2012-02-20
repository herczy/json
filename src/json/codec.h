#ifndef JSON_CODEC_H_INCLUDE
#define JSON_CODEC_H_INCLUDE

#include <string>

#include <json/exception.h>

namespace Json
{

  DEFINE_EXCEPTION(CodecException);

  class Codec
  {
  public:
    Codec(const char *encoding);
    ~Codec();

    void decode(std::wstring &dest, const std::string &src);
    void encode(std::string &dest, const std::wstring &src);

  private:
    void *internal_decode;
    void *internal_encode;
  };

} // namespace Json

#endif // JSON_CODEC_H_INCLUDE
