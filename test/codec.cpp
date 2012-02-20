#include <json/codec.h>

#include "common.h"

#include <iostream>

using namespace Json;

void
test_utf8_decode()
{
  Codec utf8("UTF8");

  std::wstring dest;
  std::string src("xyz\xe2\x82\xacxyz");

  utf8.decode(dest, src);

  ASSERT_WSTREQ(dest.c_str(), L"xyz\u20ACxyz");
}

int
main()
{
  RUN0(test_utf8_decode);
  return 0;
}
