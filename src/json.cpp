#include "json/json.h"
#include "json/codec.h"

#include <wctype.h>
#include <math.h>
#include <stdio.h>

using namespace Json;

JsonHandler::JsonHandler()
{
}

JsonHandler::~JsonHandler()
{
}

Value
JsonHandler::decode(const std::string &json, const char *encoding)
{
  Codec codec(encoding);
  std::wstring data;

  codec.decode(data, json);
  return decode(data);
}

Value
JsonHandler::decode(const std::wstring &json)
{
  int pos = 0;
  return decode_value(json.c_str(), pos);
}

Value
JsonHandler::decode_value(const wchar_t *data, int &pos)
{
  skip_spaces(data, pos);

  switch (data[pos])
    {
    case L't':
      if (compare_forward(data, pos, L"true"))
        return Value(true);

      raise_error< InvalidCharacter >("Invalid token found", pos);

    case L'f':
      if (compare_forward(data, pos, L"false"))
        return Value(false);

      raise_error< InvalidCharacter >("Invalid token found", pos);

    case L'n':
      if (compare_forward(data, pos, L"null"))
        return Value();

      raise_error< InvalidCharacter >("Invalid token found", pos);

    case L'"':
      return decode_string(data, pos);

    case L'{':
      return decode_object(data, pos);

    case L'[':
      return decode_array(data, pos);

    default:
      if (iswdigit(data[pos]) || data[pos] == L'-')
        return decode_number(data, pos);

      raise_error< InvalidCharacter >("Invalid character found", pos);
    }
}

Value
JsonHandler::decode_string(const wchar_t *data, int &pos)
{
  std::wstring str;
  bool escape = false;

  assert(data[pos] == L'"');
  ++pos;

  while (data[pos] && (escape || data[pos] != L'"'))
    {
      if (escape)
        {
          str.push_back(unescape(data, pos));
          escape = false;
          continue;
        }

      if (data[pos] == L'\\')
        {
          escape = true;
          ++pos;
          continue;
        }

      str.push_back(data[pos]);
      ++pos;
    }

  if (!data[pos])
    raise_error< UnexpectedEof >("Unexpected end of input", pos);

  assert(data[pos] == L'"');
  ++pos;

  return Value(str);
}

Value
JsonHandler::decode_number(const wchar_t *data, int &pos)
{
  int integer = 0;
  int remainder = 0;
  int remainder_count = 0;
  bool neg_mantissa = false;

  int exponent = 0;
  bool neg_exponent = false;

  // See if there is a signature
  if (data[pos] == L'-')
    {
      neg_mantissa = true;
      ++pos;
    }

  if (!iswdigit(data[pos]))
    raise_error< InvalidCharacter >("Invalid digit", pos);

  // Zero is a separate case
  if (data[pos] != L'0')
    {
      integer = decode_integer(data, pos);
    }
  else
    {
      ++pos;
    }

  if (data[pos] == L'.')
    {
      ++pos;

      if (!iswdigit(data[pos]))
        raise_error< InvalidCharacter >("Invalid digit", pos);

      int oldpos = pos;
      remainder = decode_integer(data, pos);
      remainder_count = pos - oldpos;
    }

  if (data[pos] == L'e' || data[pos] == L'E')
    {
      ++pos;

      if (data[pos] == L'+')
        {
          ++pos;
        }
      else if (data[pos] == L'-')
        {
          neg_exponent = true;
          ++pos;
        }

      if (!iswdigit(data[pos]))
        raise_error< InvalidCharacter >("Invalid digit", pos);

      exponent = decode_integer(data, pos);
    }

  if (remainder_count == 0 && exponent == 0)
    {
      if (neg_mantissa)
        integer = -integer;

      return Value(integer);
    }

  double res = integer + (double)remainder / pow(10, (double)remainder_count);

  if (neg_mantissa)
    res = -res;

  if (neg_exponent)
    exponent = -exponent;

  res = res * pow(10, (double)exponent);

  return Value(res);
}

Value
JsonHandler::decode_array(const wchar_t *data, int &pos)
{
  assert(data[pos] == L'[');
  ++pos;

  Value::List list;
  bool last = false;

  skip_spaces(data, pos);
  if (data[pos] != L']')
    {
      while (data[pos] && !last && data[pos] != L']')
        {
          skip_spaces(data, pos);
          list.push_back(decode_value(data, pos));

          skip_spaces(data, pos);
          if (data[pos] == L',')
            {
              ++pos;
              last = false;
            }
          else
            {
              last = true;
            }

          skip_spaces(data, pos);
        }
    }
  else
    last = true;

  if (!last || data[pos] != L']')
    raise_error< InvalidCharacter >("List ended with an invalid character", pos);

  return Value(list);
}

Value
JsonHandler::decode_object(const wchar_t *data, int &pos)
{
  assert(data[pos] == L'{');
  ++pos;

  Value::Object object;
  bool last = false;

  skip_spaces(data, pos);
  if (data[pos] != L'}')
    {
      while (data[pos] && !last && data[pos] != L'}')
        {
          skip_spaces(data, pos);
          if (data[pos] != L'"')
            raise_error< InvalidCharacter >("Expected string for key", pos);
          Value key = decode_string(data, pos);

          skip_spaces(data, pos);
          if (data[pos] != L':')
            raise_error< InvalidCharacter >("Expected ':'", pos);
          ++pos;

          skip_spaces(data, pos);
          Value value = decode_value(data, pos);

          object.insert(std::make_pair((const std::wstring &)key, value));

          skip_spaces(data, pos);
          if (data[pos] == L',')
            {
              ++pos;
              last = false;
            }
          else
            {
              last = true;
            }

          skip_spaces(data, pos);
        }
    }
  else
    last = true;

  if (!last || data[pos] != L'}')
    raise_error< InvalidCharacter >("Object ended with invalid character", pos);

  return Value(object);
}

wchar_t
JsonHandler::unescape(const wchar_t *data, int &pos)
{
  switch (data[pos])
    {
    case L'b':
      ++pos;
      return L'\b';

    case L'f':
      ++pos;
      return L'\f';

    case L'n':
      ++pos;
      return L'\n';

    case L'r':
      ++pos;
      return L'\r';

    case L't':
      ++pos;
      return L'\t';

    case L'"':
    case L'\\':
    case L'/':
      ++pos;
      return data[pos - 1];

    case L'u':
      ++pos;
      break;

    default:
      raise_error< InvalidCharacter >("Invalid escape character found", pos);
    }

  unsigned code = 0;
  for (int i = 0; i < 4; ++i)
    {
      code <<= 4;

      if (!iswxdigit(data[pos + i]))
        raise_error< InvalidCharacter >("Invalid hex code", pos + i);

      if (iswdigit(data[pos + i]))
        code += data[pos + i] - '0';
      else if (isupper(data[pos + i]))
        code += data[pos + i] - 'A' + 10;
      else
        code += data[pos + i] - 'a' + 10;
    }

  pos += 4;
  return (wchar_t)code;
}

int
JsonHandler::decode_integer(const wchar_t *data, int &pos)
{
  int res = 0;

  while (data[pos] && iswdigit(data[pos]))
    {
      res = res * 10 + data[pos] - '0';
      ++pos;
    }

  return res;
}

void
JsonHandler::skip_spaces(const wchar_t *data, int &pos)
{
  while (data[pos] && iswspace(data[pos]))
    pos++;
}

bool
JsonHandler::compare_forward(const wchar_t *data, int &pos, const wchar_t *expected)
{
  int datapos = pos;
  int exppos = 0;

  while (data[datapos] && expected[exppos] && data[datapos] == expected[exppos])
    {
      datapos++;
      exppos++;
    }

  if (expected[exppos] == L'\0')
    {
      pos = datapos;
      return true;
    }

  return false;
}
