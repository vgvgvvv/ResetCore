include ../Makefile.h

PROGS =    pow3 pow3b sqrt1 sqrt2 sqrt3 loop2
OUTPROGS = loop1 unruh sqrt4

pow3.hpp: pow3.htt
pow3.cpp: pow3.hpp pow3.ctt

pow3b.hpp: pow3b.htt
pow3b.cpp: pow3b.hpp pow3b.ctt

sqrt1.cpp: sqrt1.hpp sqrt1.ctt
sqrt1.hpp: sqrt1.htt

sqrt2.cpp: sqrt2.hpp sqrt2.ctt ifthenelse.hpp
sqrt2.hpp: sqrt2.htt

sqrt3.cpp: sqrt3.hpp sqrt3.ctt
sqrt3.hpp: sqrt3.htt

sqrt4.cpp: sqrt4.hpp sqrt4.ctt ifthenelse.hpp
sqrt4.hpp: sqrt4.htt

loop1.cpp: loop1.hpp loop1.ctt
loop1.hpp: loop1.htt
loop2.cpp: loop2.hpp loop2.ctt
loop2.hpp: loop2.htt

unruh.out: unruh.cpp
	make unruh 2>&1 | grep conv | tee unruh.out
	echo '\begin{tttzeile}' > unruh.ott;
	cat unruh.out >> $*.ott;
	echo '\end{tttzeile}' >> unruh.ott;

