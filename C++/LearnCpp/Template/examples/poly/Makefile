include ../Makefile.h

PROGS =    dynapoly staticpoly printmax
OUTPROGS = 

dynapoly: coord.hpp dynahier.hpp dynapoly.cpp dynahier.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp dynahier.cpp
staticpoly: coord.hpp statichier.hpp staticpoly.cpp statichier.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp statichier.cpp

printmax.ctt: MyClass.hpp
