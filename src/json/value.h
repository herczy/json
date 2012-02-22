#ifndef JSON_VALUE_H_INCLUDE
#define JSON_VALUE_H_INCLUDE

#include <vector>
#include <map>
#include <string>

#include <json/exception.h>

namespace Json
{

  DEFINE_EXCEPTION(ValueException);

  /**
   * Represents a JSON value. Can be casted to the STL appropriate JSON types.
   */
  class Value
  {
  public:
    /**
     * Type enumeration.
     */
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
    /**
     * Create a null JSON value.
     */
    Value();

    /**
     * Create a boolean JSON value.
     */
    Value(bool value);

    /**
     * Create a number JSON value.
     */
    Value(int value);

    /**
     * Create a number JSON value.
     */
    Value(double value);

    /**
     * Create a string JSON value.
     */
    Value(const wchar_t *value, int len = -1);

    /**
     * Create a string JSON value.
     */
    Value(const std::wstring &value);

    /**
     * Create a string JSON value. The string is decoded with
     * the given codec.
     */
    Value(const char *value, int len = -1, const char *encoding = "UTF-8");

    /**
     * Create a string JSON value. The string is decoded with
     * the given codec.
     */
    Value(const std::string &value, const char *encoding = "UTF-8");

    /**
     * Create a JSON list.
     */
    Value(const List &list);

    /**
     * Create a JSON object.
     */
    Value(const Object &object);

    /**
     * Copy the given value.
     */
    Value(const Value &other);

    /**
     * Destroy the Value and the contained JSON value.
     */
    ~Value();

    /**
     * Assign another JSON value to this one.
     */
    Value &operator=(const Value &other);

    /**
     * Compare another JSON value with this one.
     */
    bool operator==(const Value &other) const;

    /**
     * Try to compare an arbitary value to this.
     */
    template < typename _Other >
      bool operator==(const _Other &other) const
      {
        return (const _Other &)(*this) == other;
      }

    /**
     * Same as operator== but negated.
     */
    template < typename _Other >
      bool operator!=(const _Other &other) const
      {
        return !((*this) == other);
      }

    /**
     * Get value type.
     */
    inline const Type get_type() const
    { return type; }

    /**
     * Check if value is null.
     */
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
