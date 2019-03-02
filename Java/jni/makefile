CC := clang++
CFLAGS := -g -std=c++11 -Wall -pedantic
LDFLAGS :=
SOURCES := src/com_keymagic_KeyMagicEngine.cpp $(wildcard ../cross-platform/libkeymagic/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
LIBRARY := libjni-keymagic.dylib
JAVA_HOME := `/usr/libexec/java_home`
INCLUDES := -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin -I../../cross-platform/libkeymagic/
CXXFLAGS := $(INCLUDES) $(CFLAGS)

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(CXXFLAGS) -o $@ $^