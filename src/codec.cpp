#include "json/codec.h"

#include <iconv.h>
#include <errno.h>

#define BUFFER_SIZE 4096

using namespace Json;

namespace
{

  template < class _T_Char_Dest, class _T_Char_Src >
    void transcode(iconv_t handle, std::basic_string< _T_Char_Dest > &dest, const std::basic_string< _T_Char_Src > &src)
    {
      char outbuf_data[BUFFER_SIZE];

      char *inbuf = (char *)src.c_str();
      size_t inremain = src.size() + 1;

      while (inremain > 0)
        {
          size_t outremain = BUFFER_SIZE;
          char *outbuf = outbuf_data;
          size_t res = iconv(handle, (char **)&inbuf, &inremain, &outbuf, &outremain);

          if (res == (size_t)-1)
            {
              switch (errno)
                {
                case EILSEQ:
                  throw CodecException("Invalid multibyte sequence");
                  break;

                case EINVAL:
                  throw CodecException("Incomplete multibyte sequence");
                  break;
                  
                case E2BIG:
                  throw CodecException("No more room");
                  break;
                  
                default:
                  throw CodecException("Unknown error");
                  break;
                }
            }

          dest.append((_T_Char_Dest *)outbuf_data, (BUFFER_SIZE - outremain) / sizeof(_T_Char_Dest));
        }
    }

} // namespace

Codec::Codec(const char *encoding)
{
  internal_decode = iconv_open("WCHAR_T", encoding);

  if (!internal_decode)
    throw CodecException("Could not initialize iconv decoder");

  internal_encode = iconv_open(encoding, "WCHAR_T");

  if (!internal_encode)
    throw CodecException("Could not initialize iconv encoder");
}

Codec::~Codec()
{
  iconv_close(internal_decode);
  iconv_close(internal_encode);
}

void
Codec::decode(std::wstring &dest, const std::string &src)
{
  transcode(internal_decode, dest, src);
}

void
Codec::encode(std::string &dest, const std::wstring &src)
{
  transcode(internal_encode, dest, src);
}
