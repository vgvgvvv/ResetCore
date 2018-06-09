include ../Makefile.h

PROGS = holdertest refmemtest truletest \
	countingptrtest

OUTPROGS = 

holdertest: holder.hpp holdertest.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
refmemtest: holder.hpp refmem2.hpp refmemtest.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
truletest: holder2.hpp trule.hpp truletest.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

countingptrtest: countingptr.hpp stdobjpolicy.hpp stdarraypolicy.hpp \
		 simplerefcount.hpp allocator.hpp \
		 countingptrtest.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
