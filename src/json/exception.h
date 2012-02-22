#ifndef JSON_EXCEPTION_H_INCLUDE
#define JSON_EXCEPTION_H_INCLUDE

#include <exception>
#include <sstream>

#include <json/common.h>

namespace Json
{

  class Exception : public std::exception
  {
  public:
    Exception(const char *msg) throw();
    virtual ~Exception() throw();

    const char *what() const throw();

  private:
    char *msg;
  };

} // namespace Json

#define DEFINE_EXCEPTION_WITH_BASE(name, base) \
  class name : public base { \
  public: \
    name(const char *msg) throw() : base(msg) {} \
    virtual ~name() throw() {} \
  }

#define DEFINE_EXCEPTION(name) DEFINE_EXCEPTION_WITH_BASE(name, Json::Exception)

DEFINE_EXCEPTION(AssertionError);

#define assert(cond) do { \
    if (!(cond)) \
      { \
        std::stringstream str; \
        str << "Assertion " << #cond << "failed (file " << __FILE__ << " line " << __LINE__ << ")"; \
        throw AssertionError(str.str().c_str()); \
      } \
  } while (0)

#endif // JSON_EXCEPTION_H_INCLUDE
