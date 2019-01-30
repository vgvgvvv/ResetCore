# Recommended reading order:

## lmathlib.c, lstrlib.c: 
get familiar with the external C API. Don't bother with the pattern matcher though. Just the easy functions.

## lapi.c: 
Check how the API is implemented internally. Only skim this to get a feeling for the code. Cross-reference to lua.h and luaconf.h as needed.

## lobject.h: 
tagged values and object representation. skim through this first. you'll want to keep a window with this file open all the time.

## lstate.h: 
state objects. ditto.

## lopcodes.h: 
bytecode instruction format and opcode definitions. easy.

## lvm.c: 
scroll down to luaV_execute, the main interpreter loop. see how all of the instructions are implemented. skip the details for now. reread later.

## ldo.c: 
calls, stacks, exceptions, coroutines. tough read.

## lstring.c: 
string interning. cute, huh?

## ltable.c: 
hash tables and arrays. tricky code.

## ltm.c: 
metamethod handling, reread all of lvm.c now.

You may want to reread lapi.c now.

## ldebug.c: 
surprise waiting for you. abstract interpretation is used to find object names for tracebacks. does bytecode verification, too.

## lparser.c, lcode.c: 
recursive descent parser, targetting a register-based VM. start from chunk() and work your way through. read the expression parser and the code generator parts last.

## lgc.c: 
incremental garbage collector. take your time.

Read all the other files as you see references to them. Don't let your stack get too deep though.

If you're done before X-Mas and understood all of it, you're good. The information density of the code is rather high.