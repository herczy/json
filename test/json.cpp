#include <json/json.h>

#include "common.h"

using namespace Json;

void
test_decode_null()
{
  JsonHandler handler;
  Value nullval;
  
  nullval = handler.decode("null");
  ASSERT_EQ(nullval.get_type(), Value::JSON_TYPE_NULL);
}

void
test_decode_bool()
{
  JsonHandler handler;
  Value boolval;

  boolval = handler.decode("false");
  ASSERT_EQ(boolval.get_type(), Value::JSON_TYPE_BOOLEAN);
  ASSERT_EQ(boolval, false);

  boolval = handler.decode("true");
  ASSERT_EQ(boolval.get_type(), Value::JSON_TYPE_BOOLEAN);
  ASSERT_EQ(boolval, true);
}

void
test_decode_string(const char *jstr, const wchar_t *expected)
{
  JsonHandler handler;
  Value strval;

  strval = handler.decode(jstr);
  ASSERT_EQ(strval.get_type(), Value::JSON_TYPE_STRING);
  ASSERT_EQ(strval, std::wstring(expected));
}

void
test_decode_integer(const char *jstr, int expected)
{
  JsonHandler handler;
  Value intval;

  intval = handler.decode(jstr);
  ASSERT_EQ(intval.get_type(), Value::JSON_TYPE_INTEGER);
  ASSERT_EQ(intval, expected);
}

void
test_decode_float(const char *jstr, double expected)
{
  JsonHandler handler;
  Value floatval;

  floatval = handler.decode(jstr);
  ASSERT_EQ(floatval.get_type(), Value::JSON_TYPE_FLOAT);
  ASSERT_EQ(floatval, expected);
}

void
test_decode_array()
{
  JsonHandler handler;
  Value listval;

  // Test empty list
  GUARD(listval = handler.decode("[]"));
  ASSERT_EQ(listval.get_type(), Value::JSON_TYPE_LIST);
  ASSERT_EQ(((const Value::List &)listval).size(), 0);

  // Test list with one item
  GUARD(listval = handler.decode("[ 1 ]"));
  ASSERT_EQ(listval.get_type(), Value::JSON_TYPE_LIST);

  {
    const Value::List &list = listval;
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list[0], 1);
  }

  // Test list with multiple items
  GUARD(listval = handler.decode("[ 1, 1.2, \"x\" ]"));
  ASSERT_EQ(listval.get_type(), Value::JSON_TYPE_LIST);

  {
    const Value::List &list = listval;
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list[0], 1);
    ASSERT_EQ(list[1], 1.2);
    ASSERT_EQ(list[2], std::wstring(L"x"));
  }

  // Test nested list
  GUARD(listval = handler.decode("[ 1, 1.2, [] ]"));
  ASSERT_EQ(listval.get_type(), Value::JSON_TYPE_LIST);

  {
    const Value::List &list = listval;
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list[0], 1);
    ASSERT_EQ(list[1], 1.2);
    ASSERT_EQ(list[2].get_type(), Value::JSON_TYPE_LIST);

    const Value::List &embeded = list[2];
    ASSERT_EQ(embeded.size(), 0);
  }

  // Negative cases
  ASSERT_THROW(handler.decode("[,]"), ParseError);
  ASSERT_THROW(handler.decode("["), ParseError);
  ASSERT_THROW(handler.decode("[1,]"), ParseError);
  ASSERT_THROW(handler.decode("[1,,1]"), ParseError);
}

void
test_decode_object()
{
  JsonHandler handler;
  Value objval;

  // Test empty object
  GUARD(objval = handler.decode("{}"));
  ASSERT_EQ(objval.get_type(), Value::JSON_TYPE_OBJECT);
  ASSERT_EQ(((const Value::Object &)objval).size(), 0);

  // Test object with one item
  GUARD(objval = handler.decode("{ \"key\" : 1 }"));
  ASSERT_EQ(objval.get_type(), Value::JSON_TYPE_OBJECT);

  {
    const Value::Object &obj = objval;
    ASSERT_EQ(obj.size(), 1);

    const Value::Object::const_iterator pos = obj.find(L"key");
    ASSERT_NE(pos, obj.end());
    ASSERT_EQ(pos->second, 1);
  }

  // Negative cases
  ASSERT_THROW(handler.decode("{"), ParseError);
  ASSERT_THROW(handler.decode("{:}"), ParseError);
  ASSERT_THROW(handler.decode("{\"\":}"), ParseError);
  ASSERT_THROW(handler.decode("{:1}"), ParseError);
  ASSERT_THROW(handler.decode("{,}"), ParseError);
}

int
main()
{
  // Positive tests
  RUN0(test_decode_null);
  RUN0(test_decode_bool);
  RUN2(test_decode_string, "\"hello world\"", L"hello world");
  RUN2(test_decode_string, "\"\\\\\\//\\b\\f\\n\\r\\t\\u20ac\"", L"\\//\b\f\n\r\t\u20ac");
  RUN2(test_decode_integer, "0", 0);
  RUN2(test_decode_integer, "-0", 0);
  RUN2(test_decode_integer, "123", 123);
  RUN2(test_decode_integer, "-123", -123);
  RUN2(test_decode_float, "0.1", 0.1);
  RUN2(test_decode_float, "-0.1", -0.1);
  RUN2(test_decode_float, "1e2", 100);
  RUN2(test_decode_float, "-1e2", -100);
  RUN2(test_decode_float, "1e+2", 100);
  RUN2(test_decode_float, "-1e+2", -100);
  RUN2(test_decode_float, "1e-2", 0.01);
  RUN2(test_decode_float, "-1e-2", -0.01);
  RUN2(test_decode_float, "1.6e2", 160);
  RUN2(test_decode_float, "-1.6e2", -160);
  RUN2(test_decode_float, "1.6e+2", 160);
  RUN2(test_decode_float, "-1.6e+2", -160);
  RUN2(test_decode_float, "1.6e-2", 0.016);
  RUN2(test_decode_float, "-1.6e-2", -0.016);
  RUN0(test_decode_array);
  RUN0(test_decode_object);
  return 0;
}
