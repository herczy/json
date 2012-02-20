#include "json/exception.h"

using namespace Json;

Exception::Exception(const char *msg) throw()
  : msg(msg)
{
}

Exception::~Exception() throw()
{
}

const char *
Exception::what() const throw()
{
  return msg;
}
