include ../Makefile.h

PROGS =    duo1 duo5 tuple1
OUTPROGS = 

duo1.cpp: duo1.hpp duo1.ctt

duo5.cpp: duo5.ctt duo1.hpp duo2.hpp duo3.hpp duo4.hpp duo5.hpp duo6.hpp \
	  typeop.hpp
duo2.hpp: duo1.hpp
	sed -f $(TTT2CPP) < $*.htt | sed '/private:/,$$d' > $*.hpp
	sed -n '/private/,/};/p' duo1.hpp >> $*.hpp

tuple1.cpp: tuple1.ctt tuple1.hpp \
          duo1.hpp duo2.hpp duo3.hpp duo4.hpp duo5.hpp duo6.hpp \
	  typeop.hpp

typeop.hpp:
	(cd ../traits; make typeop.hpp)
	cp ../traits/typeop.hpp .

clean::
	rm -f typeop*.hpp
