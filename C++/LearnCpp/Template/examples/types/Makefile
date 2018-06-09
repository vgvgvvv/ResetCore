include ../Makefile.h

PROGS = 

OUTPROGS = type1test types

type1test.ott: type1test type1test.out
types.ott: types types.out

type1test.ctt: type1.hpp

typet.hpp: type1.hpp type2.hpp type3.hpp type4.hpp type5.hpp type6.hpp \
           type7.hpp type8.hpp typet.htt
	sed -f $(TTT2CPP) < $*.htt > $*.tmp
	../../bin/replinclude $*.tmp > $*.hpp

types.ctt: typet.hpp
types.ott: types.out

clean::
	rm -f typeop.tmp typet.tmp

