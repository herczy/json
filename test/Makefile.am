CXXFLAGS=@CXXFLAGS@ -I../src
LDFLAGS=@LDFLAGS@ ../src/libjson.la

TESTS = codec value json
noinst_PROGRAMS = $(TESTS) performance

codec_SOURCES = codec.cpp
value_SOURCES = value.cpp
json_SOURCES = json.cpp

performance_SOURCES = performance.cpp
