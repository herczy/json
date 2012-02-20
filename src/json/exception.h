#ifndef JSON_EXCEPTION_H_INCLUDE
#define JSON_EXCEPTION_H_INCLUDE

#include <exception>

namespace Json
{

  class Exception : public std::exception
  {
  public:
    Exception(const char *msg) throw();
    virtual ~Exception() throw();

    const char *what() const throw();

  private:
    const char *msg;
  };

} // namespace Json

#define DEFINE_EXCEPTION_WITH_BASE(name, base) \
  class name : public base { \
  public: \
    name(const char *msg) throw() : base(msg) {} \
    virtual ~name() throw() {} \
  }

#define DEFINE_EXCEPTION(name) DEFINE_EXCEPTION_WITH_BASE(name, Json::Exception)

#endif // JSON_EXCEPTION_H_INCLUDE
