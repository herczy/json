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
   *
   * The list of implicit casts is the following:
   * <ul>
   *   <li>A NULL can be cast to bool (false) and int (0)</li>
   *   <li>A boolean can be cast to bool and int (0 for false, 1 for true)</li>
   *   <li>An integer can be cast to bool (false if integer is 0, true otherwise) and int</li>
   *   <li>A float can be converted to float and int (its value rounded down)</li>
   * </ul>
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

    /**
     * List of values. JSON lists are decoded to this type.
     */
    typedef std::vector< Value > List;

    /**
     * Map of key-value pairs. JSON objects are decoded to this type.
     */
    typedef std::map< std::wstring, Value > Object;

  public:
    /**
     * Create a null JSON value.
     */
    Value();

    /**
     * Initialize value with the given type, if possible.
     *
     * @tparam _Type Initial value type.
     * @param value Initial value.
     */
    template < class _Type >
      Value(const _Type &value)
      {
        set(value);
      }

    /**
     * Copy the given value.
     */
    Value(const Value &other);

    /**
     * Destroy the Value and the contained JSON value.
     */
    ~Value();

    /**
     * Set the value to null.
     */
    void set();

    /**
     * Set the value to a boolean value.
     */
    void set(bool value);

    /**
     * Set the value to a number value.
     */
    void set(int value);

    /**
     * Set the value to a number value.
     */
    void set(double value);

    /**
     * Set the value to a string value.
     */
    void set(const std::wstring &value);

    /**
     * Set the value to a C-style string value.
     */
    void set(const wchar_t *value, int len = -1);

    /**
     * Set the value to a string value. The string is decoded with
     * the given codec.
     */
    void set(const std::string &value, const char *encoding = "UTF-8");

    /**
     * Set the value to a C-style string value. The string is decoded with
     * the given codec.
     */
    void set(const char *value, int len = -1, const char *encoding = "UTF-8");

    /**
     * Set the value to a JSON list.
     */
    void set(const List &value);

    /**
     * Set the value to a JSON object.
     */
    void set(const Object &value);

    /**
     * Swap values with other.
     */
    void swap(Value &other);

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

    /**
     * Get object value as a bool.
     */
    operator bool() const;

    /**
     * Get object value as an integer.
     */
    operator int() const;

    /**
     * Get object value as a floating point number.
     */
    operator double() const;

    /**
     * Get object value as a wide string.
     */
    operator const std::wstring &() const;

    /**
     * Get object value as a list of values.
     */
    operator const List &() const;

    /**
     * Get object value as a map of values.
     */
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
    union Values
    {
      bool v_boolean;
      int v_integer;
      double v_float;
      std::wstring *v_string;
      List *v_list;
      Object *v_object;
    } value;
  };

} // namespace Json

#endif // JSON_VALUE_H_INCLUDE
