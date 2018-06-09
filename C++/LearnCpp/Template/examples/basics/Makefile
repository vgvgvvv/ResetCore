include ../Makefile.h
	

PROGS    = max2 max3 \
           stack1test stack2test stack3test \
	   stack4test addvaltest \
	   stack5test stack6test stack7test \
	   myfirst myfirst2 myfirstinst myfirst3 myfirst4 \
	   tracer printcolltest
OUTPROGS = max stack8test
all:: max2a.o max4.o

max.hpp: max.htt
max.cpp: max.hpp max.ctt
max.out: max

stack1test: stack1.hpp stack1test.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@

stack2test: stack1.hpp stack2.hpp stack2test.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@

stack3test: stack3.hpp stack3test.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@

stack4test: stack4.hpp stack4test.cpp
	cp stack4.hpp stack.hpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@
	rm stack.hpp

stack5test: stack5.hpp stack5decl.hpp stack5assign.hpp stack5test.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@

stack6test: stack6.hpp stack6decl.hpp stack6assign.hpp stack6test.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@

stack7test: stack7.hpp stack7decl.hpp stack7test.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@
	@echo "+ stack7test  error OK (siehe chmoretmpl.tex)"

stack8test: stack8.hpp stack8test.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@

addvaltest: addval.hpp addvaltest.cpp
	$(CXX) $(CXXFLAGS) $@.cpp $(LDFLAGS) -o $@
	
myfirst: myfirst.hpp myfirst.cpp myfirstmain.cpp
	$(CXX) $(CXXFLAGS) $@.cpp myfirstmain.cpp $(LDFLAGS) -o $@
	@echo "+ myfirst     should print link error: print_typeof() not defined"
	
myfirst2: myfirst2.hpp myfirst2main.cpp
	$(CXX) $(CXXFLAGS) myfirst2main.cpp $(LDFLAGS) -o $@

myfirstinst: myfirst.hpp myfirst.cpp myfirstinst.cpp myfirstmain.cpp
	$(CXX) $(CXXFLAGS) myfirst.cpp myfirstinst.cpp myfirstmain.cpp $(LDFLAGS) -o $@
	@echo "+ myfirstinst error OK with mystd"

myfirst3: myfirst3.hpp myfirst3.cpp myfirst3main.cpp
	$(CXX) $(CXXFLAGS) myfirst3main.cpp $(LDFLAGS) -o $@
	@echo "+ myfirst3    error OK if compiler can't export"

myfirst4: myfirst4.hpp myfirst.cpp myfirst4main.cpp
	$(CXX) $(CXXFLAGS) myfirst4main.cpp $(LDFLAGS) -o $@

tracer tracertest: tracer.hpp tracer.cpp tracertest.cpp
	$(CXX) $(CXXFLAGS) tracer.cpp tracertest.cpp $(LDFLAGS) -o $@
	@echo "+ tracer      error OK with mystd"

printcolltest: printcoll.hpp printcolltest.cpp
	$(CXX) $(CXXFLAGS) printcolltest.cpp $(LDFLAGS) -o $@

pow2.hpp: pow2.htt
pow2.cpp: pow2.hpp pow2.ctt

pow2b.hpp: pow2b.htt
pow2b.cpp: pow2b.hpp pow2b.ctt

log2.hpp: log2.htt
log2.cpp: log2.hpp pow2.hpp log2.ctt

sqrt.cpp: sqrt.hpp sqrt.ctt
sqrt.hpp: sqrt.htt

sqrt2.cpp: sqrt2.hpp sqrt2.ctt
sqrt2.hpp: sqrt2.htt

loop1.cpp: loop1.hpp loop1.ctt
loop1.hpp: loop1.htt
loop2.cpp: loop2.hpp loop2.ctt
loop2.hpp: loop2.htt

reader.cpp: reader.hpp reader.ctt
reader.hpp: reader.htt

merger1.cpp: merger1.hpp merger1char.hpp merger1int.hpp merger1.ctt
merger1.hpp: merger1.htt

merger2.cpp: merger2.hpp merger2traits.hpp merger2.ctt
merger2.hpp: merger2.htt

sarray.cpp: sarray.hpp sarrayops.hpp
sarray.hpp: sarray.htt
sarrayops.hpp: sarrayops.htt

sarray2.cpp: sarray2.hpp sarrayops.hpp sarrayops2.hpp
sarray2.hpp: sarray2.htt

unruh.out: unruh.cpp
	make unruh 2>&1 | grep conv | tee unruh.out
	echo '\begin{tttzeile}' > unruh.ott;
	cat unruh.out >> $*.ott;
	echo '\end{tttzeile}' >> unruh.ott;

exprmain.cpp: exprmain.ctt exprops1.hpp promotion.hpp exprscalar.hpp exprarray.hpp exprops2.hpp sarray.hpp
	sed -f $(TTT2CPP) < $< > $*.cpp

all::
	@echo "+ stack7test  error OK (siehe chmoretmpl.tex)"
	@echo "+ myfirst     should print link error: print_typeof() not defined"
	@echo "+ myfirstinst error OK with mystd"
	@echo "+ myfirst3    error OK if compiler can't export"
	@echo "+ tracer      error OK with mystd"

