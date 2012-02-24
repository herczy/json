#include <stdio.h>
#include <time.h>
#include <math.h>

#include <json/json.h>

using namespace Json;

double humanize(double v, const char *&prefix)
{
  static const char *prefixes[] = { "", "m", "u", "n", NULL };

  double val = v;
  int i = 0;

  while (val < 1.0 && prefixes[i + 1])
    {
      val *= 1000.0;
      i++;
    }

  prefix = prefixes[i];
  return val;
}

double gettime(clock_t start, clock_t end)
{
  if (end >= start)
    return (double)(end - start) / (double)CLOCKS_PER_SEC;

  // Wrapover, recalculate value accordingly
  return (pow(256, sizeof(clock_t)) - (double)(start - end)) / (double)CLOCKS_PER_SEC;
}

#define RUNPERF(call) do { \
    printf(# call " "); \
    fflush(stdout); \
    double runtime = call; \
    const char *prefix; \
    double humanized = humanize(runtime, prefix); \
    printf("%.3lf %ss\n", humanized, prefix); \
  } while (0)

#define PERF_BEGIN \
    clock_t start = clock(); \
    double result; \
    int count = 0; \
    while (count == 0 || gettime(start, clock()) < 1.5) \
    { \
      ++count;

#define PERF_END \
    } \
    clock_t end = clock(); \
    result = gettime(start, end) / (double)count;

double ParseEmptyObject()
{
  JsonHandler handler;

  PERF_BEGIN
    handler.decode("{}");
  PERF_END

  return result;
}

double ParseEmptyList()
{
  JsonHandler handler;

  PERF_BEGIN
    handler.decode("[]");
  PERF_END

  return result;
}

double ParseListIntegers(int values)
{
  JsonHandler handler;
  std::wstring input;

  {
    std::wstringstream str;

    str << L"[";
    for (int i = 0; i < values; ++i)
      {
        if (i > 0)
          str << L" , ";
        str << i;
      }
    str << L"]";

    input = str.str();
  }

  PERF_BEGIN
    handler.decode(input);
  PERF_END

  return result;
}

double ParseObjectIntegers(int values)
{
  JsonHandler handler;
  std::wstring input;

  {
    std::wstringstream str;

    str << L"{";
    for (int i = 0; i < values; ++i)
      {
        if (i > 0)
          str << L" , ";
        str << L"\"value" << i << L'"' << L':' << i;
      }
    str << L"}";

    input = str.str();
  }

  PERF_BEGIN
    handler.decode(input);
  PERF_END

  return result;
}

int
main()
{
  RUNPERF(ParseEmptyObject());
  RUNPERF(ParseEmptyObject());

  RUNPERF(ParseListIntegers(100));
  RUNPERF(ParseListIntegers(1000));
  RUNPERF(ParseListIntegers(10000));
  RUNPERF(ParseListIntegers(100000));
  RUNPERF(ParseListIntegers(1000000));
  RUNPERF(ParseObjectIntegers(100));
  RUNPERF(ParseObjectIntegers(1000));
  RUNPERF(ParseObjectIntegers(10000));
  RUNPERF(ParseObjectIntegers(100000));
  RUNPERF(ParseObjectIntegers(1000000));
  return 0;
}
