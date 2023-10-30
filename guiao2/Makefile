CXX = g++

# Flags for compilation
CXXFLAGS = $(shell pkg-config --cflags opencv4)
# Flags for linking
LDFLAGS = $(shell pkg-config --libs opencv4)

SOURCES = $(wildcard *.cpp)

OUTPUTS = $(SOURCES:.cpp=)

all: $(OUTPUTS)

%: %.cpp
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(OUTPUTS)