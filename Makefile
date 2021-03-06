CXX=clang++
CFLAGS=-c -Wall -pedantic -std=c++11
LDFLAGS=-ljsoncpp -lmicrohttpd -llua
SOURCES=main.cpp scriptapi.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BINARY=baldrapi
OUTDIR=bin

all: $(SOURCES) $(BINARY)

$(BINARY): $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(CXX) $(LDFLAGS) -o $(OUTDIR)/$(BINARY) $(OBJECTS)

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(OUTDIR)/$(BINARY)
