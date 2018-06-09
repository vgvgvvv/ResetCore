include ../Makefile.h
all::
	@echo "Failure of compose3 is OK"

progs::	funcptr1.ott

PROGS =    functor1 funcwrap functordemo \
	   compose1 compose2 compose3 compose4 compose5 compose6 \
           bindtest
OUTPROGS = funcptr functorops

funcptr1.ott: funcptr.ott
	@echo "funcptr1.ott sollte der Output des EDG compilers sein"
	@echo "deshalb von Hand aus funcptr.ott erzeugen"
	@echo "aktueller diff:"
	diff funcptr.ott funcptr1.ott

typeslib: typet.hpp ifthenelse.hpp
forwardfiles: typeslib forwardparam.hpp typeop.hpp
typeop.hpp:
	(cd ../traits; make typeop.hpp)
	cp ../traits/typeop.hpp .
typet.hpp:
	(cd ../types; make typet.hpp)
	cp ../types/typet.hpp .
clean::
	rm -f typeop.hpp typet.hpp

functordemo: functordemo.cpp funcptr.hpp functionptr.hpp \
             functionptrt.hpp \
	     forwardfiles
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

functorparam.hpp: functorparam.htt functorparam1.hpp functorparam2.hpp

#types.hpp: ../traits/types.htt \
#           ../traits/type1.htt
#	(cd ../traits; make types.hpp)
#	sed '/type1.hpp/,$$d' ../traits/types.hpp > types.hpp
#	cat ../traits/type1.hpp >> types.hpp
#	sed -n '/type1.hpp/,/type6.hpp/p' ../traits/types.hpp >> types.hpp
#	cat ../traits/type6.hpp >> types.hpp
#	sed -n '/type6.hpp/,/type3.hpp/p' ../traits/types.hpp >> types.hpp
#	cat ../traits/type3.hpp >> types.hpp
#	sed -n '/type3.hpp/,/type4.hpp/p' ../traits/types.hpp >> types.hpp
#	cat ../traits/type4.hpp >> types.hpp
#	sed -n '/type4.hpp/,/type5.hpp/p' ../traits/types.hpp >> types.hpp
#	cat ../traits/type5.hpp >> types.hpp
#	sed -n '/type5.hpp/,/type7.hpp/p' ../traits/types.hpp >> types.hpp
#	cat ../traits/type7.hpp >> types.hpp
#	sed -n '/type7.hpp/,/type8.hpp/p' ../traits/types.hpp >> types.hpp
#	cat ../traits/type8.hpp >> types.hpp
#	sed -n '/type8.hpp/,/#endif/p' ../traits/types.hpp >> types.hpp
#	mv types.hpp qqq.hpp
#	sed -e 's/#include "type/\/\/#include "type/' < qqq.hpp > types.hpp

compose1: math1.hpp compose1.hpp compose1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
compose2: math1.hpp compose1.hpp composeconv.hpp compose2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
compose3: math1.hpp compose3.hpp composeconv.hpp compose3.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
	@echo "Failure of compose3 is OK"
compose4: math1.hpp compose4.hpp composeconv.hpp compose4.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
compose5: forwardfiles math3.hpp compose5.hpp composeconv.hpp compose5.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
compose6: forwardfiles funcptr.hpp functionptr.hpp functionptrt.hpp \
          math3.hpp compose6.hpp composeconv.hpp compose6.cpp \
	  functorparam.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

bindtest: forwardfiles functionptrt.hpp functionptr.hpp funcptr.hpp \
	boundval.hpp signselect.hpp binder5.hpp binderparams.hpp \
	bindconv.hpp bindtest.cpp functorparam.hpp \
	bindfp1.hpp bindfp2.hpp bindfp3.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp
functorops: functorops.cpp functorops.hpp forwardfiles functionptrt.hpp functionptr.hpp funcptr.hpp compose6.hpp \
	binder1.hpp bindconv.hpp functorparam.hpp binderparams.hpp \
	signselect.hpp bindfp1.hpp bindfp2.hpp bindfp3.hpp boundval.hpp binder5.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $@.cpp

all::
	@echo "Failure of compose3 is OK"
all::
	@echo ""
	@echo "binder1.htt echter Auszug von binder5.htt?:"
	@(diff binder2.htt binder5.htt | grep '^<' | grep -v dots; true)
	@echo "binder2.htt echter Auszug von binder5.htt?:"
	@(diff binder2.htt binder5.htt | grep '^<' | grep -v dots; true)
	@echo "binder3.htt echter Auszug von binder5.htt?:"
	@(diff binder3.htt binder5.htt | grep '^<' | grep -v dots; true)
	@echo "binder4.htt echter Auszug von binder5.htt?:"
	@(diff binder4.htt binder5.htt | grep '^<' | grep -v dots; true)

