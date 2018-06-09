include Makefile.h

all::

CODE_DIRS = basics details \
            poly traits inherit meta exprtmpl \
            types pointers tuples functors
all::
	@for DIR in $(CODE_DIRS); \
        do \
            (cd $$DIR; make all) \
        done
clean::
	@for DIR in $(CODE_DIRS); \
        do \
            (cd $$DIR; make clean) \
        done

