#include "json/exception.h"

#include <stdlib.h>
#include <string.h>

using namespace Json;

Exception::Exception(const char *msg) throw()
  : msg(strdup(msg))
{
}

Exception::~Exception() throw()
{
  free(msg);
}

const char *
Exception::what() const throw()
{
  return msg;
}
