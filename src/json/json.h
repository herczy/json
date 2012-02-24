/**
 * @file
 */
#ifndef JSON_JSON_H_INCLUDE
#define JSON_JSON_H_INCLUDE

#include <sstream>

#include <json/value.h>
#include <json/common.h>
#include <json/codec.h>

namespace Json
{

  DEFINE_EXCEPTION(ParseError);
  DEFINE_EXCEPTION_WITH_BASE(InvalidCharacter, ParseError);
  DEFINE_EXCEPTION_WITH_BASE(UnexpectedEof, ParseError);

  /**
   * JSON decoder/encoder. While the Value object only stores wide strings, the
   * JsonHandler object can handle normal strings. These strings will be transcoded
   * to the proper charsets prior to decoding and after encoding a Value object.
   */
  class JsonHandler
  {
  public:
    /**
     * Create and initialize a JsonHandler.
     *
     * @param encoding Encoding of non-multi byte strings.
     */
    JsonHandler(const char *encoding = "UTF-8");

    /**
     * Destroy JsonHandler.
     */
    ~JsonHandler();

    /**
     * Decode a JSON string. The string will be decoded with the given encoding.
     *
     * @param json The JSON data in the encoding given previously to JsonHandler.
     * @return The decoded JSON value.
     */
    Value decode(const std::string &json);

    /**
     * Decode a JSON string.
     *
     * @param json The JSON data.
     * @return The decoded JSON value.
     */
    Value decode(const std::wstring &json);

    /**
     * Encode a JSON string. The string will be encoded with the given encoding.
     *
     * @param dest Destination string.
     * @param value Value object to be encoded.
     */
    void encode(std::string &dest, const Value &value);

    /**
     * Encode a JSON string.
     *
     * @param dest Destination string.
     * @param value Value object to be encoded.
     */
    void encode(std::wstring &dest, const Value &value);

  private:
    Value decode_value(const wchar_t *data, int &pos);
    Value decode_string(const wchar_t *data, int &pos);
    Value decode_number(const wchar_t *data, int &pos);
    Value decode_array(const wchar_t *data, int &pos);
    Value decode_object(const wchar_t *data, int &pos);

    void encode(std::wstringstream &dest, const Value &value);

    wchar_t unescape(const wchar_t *data, int &pos);
    void escape(std::wstringstream &dest, const std::wstring &value);
    int decode_integer(const wchar_t *data, int &pos);

    bool compare_forward(const wchar_t *data, int &pos, const wchar_t *expect);

  private:
    template < class _Exception >
      void raise_error(const char *message, int position) JSON_NORETURN;

  private:
    Codec codec;
  };

  template < class _Exception >
    void JsonHandler::raise_error(const char *message, int position)
    {
      std::stringstream str;
      str << message << " at " << position;

      throw _Exception(str.str().c_str());
    }

} // namespace Json

#endif // JSON_H_INCLUDE
