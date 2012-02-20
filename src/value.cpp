#include "json/value.h"
#include "json/codec.h"

using namespace Json;

Value::Value()
  : type(JSON_TYPE_NULL)
{
}

Value::Value(bool value)
  : type(JSON_TYPE_BOOLEAN),
    v_boolean(value)
{
}

Value::Value(int value)
  : type(JSON_TYPE_INTEGER),
    v_integer(value)
{
}

Value::Value(double value)
  : type(JSON_TYPE_FLOAT),
    v_float(value)
{
}

Value::Value(const wchar_t *value, int len)
  : type(JSON_TYPE_STRING),
    v_string(new std::wstring)
{
  if (len == -1)
    v_string->append(value);

  else
    v_string->append(value, len);
}

Value::Value(const std::wstring &value)
  : type(JSON_TYPE_STRING),
    v_string(new std::wstring(value))
{
}

Value::Value(const char *value, int len, const char *encoding)
  : type(JSON_TYPE_STRING),
    v_string(new std::wstring)
{
  Codec codec(encoding);

  std::string str;

  if (len == -1)
    str.append(value);

  else
    str.append(value, len);

  codec.decode(*v_string, str);
}

Value::Value(const std::string &value, const char *encoding)
  : type(JSON_TYPE_STRING),
    v_string(new std::wstring)
{
  Codec codec(encoding);

  codec.decode(*v_string, value);
}

Value::Value(const List &list)
  : type(JSON_TYPE_LIST),
    v_list(new List(list))
{
}

Value::Value(const Object &object)
  : type(JSON_TYPE_OBJECT),
    v_object(new Object(object))
{
}

Value::Value(const Value &other)
  : type(JSON_TYPE_NULL)
{
  (*this) = other;
}

Value &
Value::operator=(const Value &other)
{
  clear();

  type = other.type;
  switch (type)
    {
    case JSON_TYPE_NULL:
      break;

    case JSON_TYPE_BOOLEAN:
      v_boolean = other.v_boolean;
      break;

    case JSON_TYPE_INTEGER:
      v_integer = other.v_integer;
      break;

    case JSON_TYPE_FLOAT:
      v_float = other.v_float;
      break;

    case JSON_TYPE_STRING:
      v_string = new std::wstring(*other.v_string);
      break;

    case JSON_TYPE_LIST:
      v_list = new List(*other.v_list);
      break;

    case JSON_TYPE_OBJECT:
      v_object = new Object(*other.v_object);
      break;
    }
}

bool
Value::operator==(const Value &other) const
{
  if (type != other.type)
    return false;

  switch (type)
    {
    case JSON_TYPE_NULL:
      return true;

    case JSON_TYPE_BOOLEAN:
      return v_boolean == other.v_boolean;

    case JSON_TYPE_INTEGER:
      return v_integer == other.v_integer;

    case JSON_TYPE_FLOAT:
      return v_float == other.v_float;

    case JSON_TYPE_STRING:
      return *v_string == *other.v_string;

    case JSON_TYPE_LIST:
      return *v_list == *other.v_list;

    case JSON_TYPE_OBJECT:
      return *v_object == *other.v_object;
    }
}

Value::~Value()
{
  clear();
}

void
Value::clear()
{
  type = JSON_TYPE_NULL;

  switch (type)
    {
    case JSON_TYPE_NULL:
    case JSON_TYPE_BOOLEAN:
    case JSON_TYPE_INTEGER:
    case JSON_TYPE_FLOAT:
      break;

    case JSON_TYPE_STRING:
      delete v_string;
      break;

    case JSON_TYPE_LIST:
      delete v_list;
      break;

    case JSON_TYPE_OBJECT:
      delete v_object;
      break;
    }
}

Value::operator bool() const
{
  switch (type)
    {
    case JSON_TYPE_NULL:
      return false;

    case JSON_TYPE_BOOLEAN:
      return v_boolean;

    case JSON_TYPE_INTEGER:
      return (bool)v_integer;

    default:
      throw ValueException("Invalid value type");
    }
}

Value::operator int() const
{
  switch (type)
    {
    case JSON_TYPE_NULL:
      return 0;

    case JSON_TYPE_BOOLEAN:
      return (v_boolean ? 1 : 0);

    case JSON_TYPE_INTEGER:
      return v_integer;

    case JSON_TYPE_FLOAT:
      return v_float;

    default:
      throw ValueException("Invalid value type");
    }
}

Value::operator double() const
{
  check_type(JSON_TYPE_FLOAT);
  return v_float;
}

Value::operator const std::wstring &() const
{
  check_type(JSON_TYPE_STRING);
  return *v_string;
}

Value::operator const List &() const
{
  check_type(JSON_TYPE_LIST);
  return *v_list;
}

Value::operator const Object &() const
{
  check_type(JSON_TYPE_OBJECT);
  return *v_object;
}
