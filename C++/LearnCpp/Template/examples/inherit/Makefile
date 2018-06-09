include ../Makefile.h

PROGS = namedtmpl empty ebco1 ebco2 virtual testcounter
OUTPROGS = 

namedtmpl: namedtmpl.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
empty: empty.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
ebco1: ebco1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
ebco2: ebco2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
virtual: virtual.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
testcounter: testcounter.cpp objectcounter.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

