CPPFLAGS = -Wall -pedantic -pedantic-errors -O3

CXX = g++

all: cache

# This bit of trickery is to create ".d" files which describe the dependencies
# of each .cpp file, which are then included in this makefile (see the
# "-include" directive below). See the makefile info page for more info on
# this technique.
%.d:%.cpp
	set -e; $(CXX) -MM $(CPPFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

SOURCES = main.cpp CacheSimUnit.cpp TraceParser.cpp
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDENCIES = $(SOURCES:.cpp=.d)

# always make main
main.o:
	$(CXX)  -Wall -pedantic -pedantic-errors -o main.o -c main.cpp

# If the target is not "clean", then include the dependencies (which also makes
# them as necessary) 
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDENCIES)
endif

cache: $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(OBJECTS) -o cache_sim
	cp cache_sim ../

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDENCIES) core cache_sim

