#ifndef JSON_VALUE_H_INCLUDE
#define JSON_VALUE_H_INCLUDE

#include <vector>
#include <map>
#include <string>

#include <json/exception.h>

namespace Json
{

  DEFINE_EXCEPTION(ValueException);

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
    Value(const wchar_t *value, int len = -1);
    Value(const std::wstring &value);
    Value(const char *value, int len = -1, const char *encoding = "UTF-8");
    Value(const std::string &value, const char *encoding = "UTF-8");
    
    Value(const List &list);
    Value(const Object &object);

    Value(const Value &other);

    ~Value();

    Value &operator=(const Value &other);
    bool operator==(const Value &other) const;

    template < typename _Other >
      bool operator==(const _Other &other) const
      {
        return (const _Other &)(*this) == other;
      }

    template < typename _Other >
      bool operator!=(const _Other &other) const
      {
        return !((*this) == other);
      }

    inline const Type get_type() const
    { return type; }

    inline bool is_null() const
    { return type == JSON_TYPE_NULL; }

    operator bool() const;
    operator int() const;
    operator double() const;
    operator const std::wstring &() const;
    operator const List &() const;
    operator const Object &() const;

  private:
    void clear();

    inline void check_type(Type type) const
    {
      if (this->type != type)
        throw ValueException("Invalid value type");
    }

  private:
    Type type;
    union
    {
      bool v_boolean;
      int v_integer;
      double v_float;
      std::wstring *v_string;
      List *v_list;
      Object *v_object;
    };
  };

} // namespace Json

#endif // JSON_VALUE_H_INCLUDE
