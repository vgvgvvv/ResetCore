include ../Makefile.h

PROGS =    sarray1 sarray2 exprmain
OUTPROGS = 

sarray1.cpp: sarray1.hpp sarrayops1.hpp

sarray2.cpp: sarray2.hpp sarrayops1.hpp sarrayops2.hpp

exprmain.cpp: exprmain.ctt exprops1.hpp exprops1a.hpp exprscalar.hpp exprarray.hpp exprops2.hpp sarray1.hpp
	sed -f $(TTT2CPP) < $< > $*.cpp

