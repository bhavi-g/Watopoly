CXX = g++-14.2.0
CXXFLAGS = -std=c++20 -fmodules-ts -Wall -g
HEADERS = cctype ctime fstream iomanip iostream algorithm map optional random set sstream utility vector string

ORDER_FILE = order.txt
EXEC = watopoly

OBJECTS = $(patsubst %.cc,%.o,$(shell cat $(ORDER_FILE)))
HEADER_OBJS = $(patsubst %,%.gcm,$(HEADERS))

all: precompile-headers $(EXEC)

# Compile the program using the object files in order
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

# Compile .cc files into .o files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile standard headers into .gcm (module interface)
%.gcm:
	g++-14.2.0 -std=c++20 -fmodules-ts -c -x c++-system-header $*

# Rule to precompile all standard headers
precompile-headers: $(HEADER_OBJS)

clean:
	rm -f *.o *.gcm $(EXEC)
