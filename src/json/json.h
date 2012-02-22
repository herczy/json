#ifndef JSON_JSON_H_INCLUDE
#define JSON_JSON_H_INCLUDE

#include <sstream>

#include <json/value.h>
#include <json/common.h>

namespace Json
{

  DEFINE_EXCEPTION(ParseError);
  DEFINE_EXCEPTION_WITH_BASE(InvalidCharacter, ParseError);
  DEFINE_EXCEPTION_WITH_BASE(UnexpectedEof, ParseError);

  class JsonHandler
  {
  public:
    JsonHandler();
    ~JsonHandler();

    Value decode(const std::string &json, const char *encoding = "UTF-8");
    Value decode(const std::wstring &json);

    void encode(std::wstring &dest, const Value &value);
    void encode(std::string &dest, const Value &value, const char *encoding = "UTF-8");

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

    void skip_spaces(const wchar_t *data, int &pos);

  private:
    template < class _Exception >
      void raise_error(const char *message, int position) JSON_NORETURN;
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
