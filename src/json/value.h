#ifndef JSON_VALUE_H_INCLUDE
#define JSON_VALUE_H_INCLUDE

#include <vector>
#include <map>

namespace Json
{

  class Value
  {
  public:
    enum Type
    {
      JSON_TYPE_NULL,
      JSON_TYPE_BOOLEAN,
      JSON_TYPE_INTEGER,
      JSON_TYPE_FLOAT,
      JSON_TYPE_STRING,
      JSON_TYPE_LIST,
      JSON_TYPE_OBJECT,
    };

    typedef std::vector< Value > List;
    typedef std::map< std::wstring, Value > Object;

  public:
    Value();
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const char *value);
    Value(const wchar_t *value);
  };

} // namespace Json

#endif // JSON_VALUE_H_INCLUDE
