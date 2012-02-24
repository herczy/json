#include "json/value.h"
#include "json/codec.h"

#include <wchar.h>
#include <string.h>

using namespace Json;

namespace
{
  void memxor(void *p1, const void *p2, int len)
  {
    unsigned char *ch1 = (unsigned char *)p1;
    const unsigned char *ch2 = (const unsigned char *)p2;

    while (len)
      {
        *ch1++ ^= *ch2++;
        len--;
      }
  }

  void memswap(void *p1, void *p2, int len)
  {
    memxor(p1, p2, len);
    memxor(p2, p1, len);
    memxor(p1, p2, len);
  }

  template < class _T >
    void swap(_T &v1, _T &v2)
    {
      memswap(&v1, &v2, sizeof(_T));
    }
}

Value::Value()
  : type(JSON_TYPE_NULL)
{
}

Value::Value(const Value &other)
  : type(JSON_TYPE_NULL)
{
  (*this) = other;
}

void
Value::set()
{
  clear();
  type = JSON_TYPE_NULL;
}

void
Value::set(bool value)
{
  clear();
  type = JSON_TYPE_BOOLEAN;
  this->value.v_boolean = value;
}

void
Value::set(int value)
{
  clear();
  type = JSON_TYPE_INTEGER;
  this->value.v_integer = value;
}

void
Value::set(double value)
{
  clear();
  type = JSON_TYPE_FLOAT;
  this->value.v_float = value;
}

void
Value::set(const std::wstring &value)
{
  clear();
  type = JSON_TYPE_STRING;
  this->value.v_string = new std::wstring(value);
}

void
Value::set(const wchar_t *value, int len)
{
  std::wstring temp(value, (len < 0 ? wcslen(value) : len));
  set(temp);
}

void
Value::set(const std::string &value, const char *encoding)
{
  Codec codec(encoding);
  std::wstring temp;
  codec.decode(temp, value);
  set(temp);
}

void
Value::set(const char *value, int len, const char *encoding)
{
  std::string temp(value, (len < 0 ? strlen(value) : len));
  set(temp, encoding);
}

void
Value::set(const List &value)
{
  clear();
  type = JSON_TYPE_LIST;
  this->value.v_list = new List(value);
}

void
Value::set(const Object &value)
{
  clear();
  type = JSON_TYPE_OBJECT;
  this->value.v_object = new Object(value);
}

void Value::swap(Value &other)
{
  ::swap(type, other.type);
  ::swap(value, other.value);
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
      value.v_boolean = other.value.v_boolean;
      break;

    case JSON_TYPE_INTEGER:
      value.v_integer = other.value.v_integer;
      break;

    case JSON_TYPE_FLOAT:
      value.v_float = other.value.v_float;
      break;

    case JSON_TYPE_STRING:
      value.v_string = new std::wstring(*other.value.v_string);
      break;

    case JSON_TYPE_LIST:
      value.v_list = new List(*other.value.v_list);
      break;

    case JSON_TYPE_OBJECT:
      value.v_object = new Object(*other.value.v_object);
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
      return value.v_boolean == other.value.v_boolean;

    case JSON_TYPE_INTEGER:
      return value.v_integer == other.value.v_integer;

    case JSON_TYPE_FLOAT:
      return value.v_float == other.value.v_float;

    case JSON_TYPE_STRING:
      return *value.v_string == *other.value.v_string;

    case JSON_TYPE_LIST:
      return *value.v_list == *other.value.v_list;

    case JSON_TYPE_OBJECT:
      return *value.v_object == *other.value.v_object;
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
      delete value.v_string;
      break;

    case JSON_TYPE_LIST:
      delete value.v_list;
      break;

    case JSON_TYPE_OBJECT:
      delete value.v_object;
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
      return value.v_boolean;

    case JSON_TYPE_INTEGER:
      return (bool)value.v_integer;

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
      return (value.v_boolean ? 1 : 0);

    case JSON_TYPE_INTEGER:
      return value.v_integer;

    case JSON_TYPE_FLOAT:
      return value.v_float;

    default:
      throw ValueException("Invalid value type");
    }
}

Value::operator double() const
{
  check_type(JSON_TYPE_FLOAT);
  return value.v_float;
}

Value::operator const std::wstring &() const
{
  check_type(JSON_TYPE_STRING);
  return *value.v_string;
}

Value::operator const List &() const
{
  check_type(JSON_TYPE_LIST);
  return *value.v_list;
}

Value::operator const Object &() const
{
  check_type(JSON_TYPE_OBJECT);
  return *value.v_object;
}
