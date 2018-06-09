include ../Makefile.h
all::
	@echo "Failure of apply1 is OK"

PROGS = accum3 accum4 accum5 accum6 accum8 accum9 \
	sizeof elementtype \
	apply1 apply2 \
	promote1 \
	rparam1 rparam2 \
	csm1

#OUTPROGS = accum1 accum2 accum7 type1test types
OUTPROGS = accum1 accum2 accum7 isclasst

accum1.ott: accum1 accum1.out
accum2.ott: accum2 accum2.out
accum7.ott: accum7 accum7.out
types.ott: types types.out

typeop.hpp: typeop.htt typeop1.hpp typeop2.hpp typeop3.hpp typeop4.hpp
	sed -f $(TTT2CPP) < $*.htt > $*.tmp
	../../bin/replinclude $*.tmp > $*.hpp
typet.hpp: type1.hpp type2.hpp type3.hpp type4.hpp type5.hpp type6.hpp \
           type7.hpp type8.hpp typet.htt
	sed -f $(TTT2CPP) < $*.htt > $*.tmp
	../../bin/replinclude $*.tmp > $*.hpp
clean::
	rm -f typeop.tmp typet.tmp

accum1: accum1.hpp accum1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum2: accum2.hpp accumtraits2.hpp accum2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum3: accum3.hpp accumtraits3.hpp accum3.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum4: accum4.hpp accumtraits4.hpp accum4.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum5: accum5.hpp accumtraits4.hpp accum5.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum6: accum6.hpp accumtraits4.hpp accum6.cpp sumpolicy1.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum7: accum6.hpp accumtraits4.hpp accum7.cpp sumpolicy1.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum8: accum8.hpp accumtraits4.hpp accum8.cpp sumpolicy2.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
accum9: accum9.hpp accumtraits4.hpp accum9.cpp sumpolicy3.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

apply1: apply1.cpp apply1.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
	@echo "Failure of apply1 is OK"
apply2: apply2.cpp apply2.hpp typeop.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

type1test: type1.hpp type1test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
types: typet.hpp types.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

isclasst.ctt: isclasst.hpp ifthenelse.hpp
isclasst.ott: isclasst isclasst.out

promote1: ifthenelse.hpp promote1.hpp promote2.hpp promote3.hpp promote4.hpp \
	  promotearray.hpp promote1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -c $@.cpp

#rparam1: rparam.hpp typet.hpp ifthenelse.hpp rparamcls.hpp rparam1.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
#rparam2: rparam.hpp typet.hpp ifthenelse.hpp rparamcls.hpp rparam2.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
rparam1: rparam.hpp isclasst.hpp ifthenelse.hpp rparamcls.hpp rparam1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
rparam2: rparam.hpp isclasst.hpp ifthenelse.hpp rparamcls.hpp rparam2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

csm1: isclasst.hpp csm1.hpp csm2.hpp csmtraits.hpp csm1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -c $@.cpp

all::
	@echo "Failure of apply1 is OK"
