#include <json/value.h>

#include "common.h"

#include <iostream>

using namespace Json;

void
test_null()
{
  Value v1, v2;

  ASSERT_EQ(v1.get_type(), Value::JSON_TYPE_NULL);
  ASSERT_EQ(v1, v2);
  ASSERT_EQ(v1, false);
  ASSERT_EQ(v1, 0);
}

void
test_bool()
{
  Value val(false);

  ASSERT_EQ(val.get_type(), Value::JSON_TYPE_BOOLEAN);
  ASSERT_EQ((bool)val, false);
  ASSERT_EQ(val, false);
  ASSERT_EQ(val, 0);
  ASSERT_EQ((bool)(val = Value(true)), true);
  ASSERT_EQ(val, true);
  ASSERT_EQ(val, 1);
}

void
test_integer()
{
  Value val(2);

  ASSERT_EQ(val.get_type(), Value::JSON_TYPE_INTEGER);
  ASSERT_EQ((int)val, 2);
  ASSERT_EQ(val, 2);
  ASSERT_EQ(val, true);
  ASSERT_EQ((int)(val = Value(0)), 0);
  ASSERT_EQ(val, 0);
  ASSERT_EQ(val, false);
}

void
test_float()
{
  Value val(2.5);

  ASSERT_EQ(val.get_type(), Value::JSON_TYPE_FLOAT);
  ASSERT_EQ((double)val, 2.5);
  ASSERT_EQ(val, 2.5);
  ASSERT_EQ((double)(val = Value(5.2)), 5.2);
  ASSERT_EQ(val, 5.2);
}

void
test_string()
{
  Value val          ( "Hello world");
  std::wstring etalon(L"Hello world");

  ASSERT_EQ(val.get_type(), Value::JSON_TYPE_STRING);
  ASSERT_EQ((const std::wstring &)val, etalon);
  ASSERT_EQ(val, etalon);
}

void
test_list()
{
  Value::List list;

  list.push_back(Value(1));
  list.push_back(Value(false));

  Value val(list);

  ASSERT_EQ(val.get_type(), Value::JSON_TYPE_LIST);
  ASSERT_EQ((const Value::List &)val, list);
  ASSERT_EQ(val, list);
}

void
test_object()
{
  Value::Object obj;
  obj[L"key1"] = Value(1);
  obj[L"key2"] = Value(2);

  Value val(obj);

  ASSERT_EQ(val.get_type(), Value::JSON_TYPE_OBJECT);
  ASSERT_EQ((const Value::Object &)val, obj);
  ASSERT_EQ(val, obj);
}

int
main()
{
  RUN0(test_null);
  RUN0(test_bool);
  RUN0(test_integer);
  RUN0(test_float);
  RUN0(test_string);
  RUN0(test_list);
  RUN0(test_object);
  return 0;
}
