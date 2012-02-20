includedir = $(rootincludedir)/json
pkglibdir = $(prefix)/src

include_HEADERS = json/json.h \
                  json/value.h \
                  json/codec.h \
                  json/exception.h

lib_LTLIBRARIES = libjson.la
libjson_la_SOURCES = codec.cpp \
                     exception.cpp \
                     value.cpp

libjson_la_CFLAGS = -Wall @CFLAGS@
libjson_la_LDFLAGS = -version-info 0:0:0 @LDFLAGS@
