# to make for another target CPU, redefine PROC to the name of the target
# processor, e.g., 68000
PROC =6502

SLINKY_OBJECTS = builtins.o debugPrint.o errorStuff.o expr.o		\
                 globals.o initialize.o instantiate.o link.o main.o	\
                 map.o poke.o read.o relocate.o slinkyTables.o		\
                 write.o

CFLAGS=-m32 -g -ansi -DYYDEBUG -DTARGET_CPU=CPU_$(PROC)
LDFLAGS=-m32

# If yacc is notionally present on a system, it's usually actually
# bison in a compatibility mode. bison is available by name more often
# than yacc itself is.
YACC=bison -y
#YACC=yacc

CC=gcc
#CC=clang

slinky: $(SLINKY_OBJECTS)
	$(CC) $(LDFLAGS) -o slinky $(SLINKY_OBJECTS)

clean:
	/bin/rm -f *.o slinky

love:
	@echo "Not war?"

# Slinky needs to use the parser headers created by Macross. If they
# aren't available, we dip down and make them ourselves.
../y.tab.h: ../macross_$(PROC).y
	cd .. && $(YACC) -d macross_$(PROC).y && cd slinky

.c.o:
	$(CC) $(CFLAGS) -c $<

# Dependencies below created with makedepend -Y *.c
# DO NOT DELETE

builtins.o: slinkyTypes.h slinkyGlobals.h slinkyExpressions.h builtins.h
builtins.o: errorStuff.h expr.h link.h relocate.h
debugPrint.o: slinkyTypes.h slinkyGlobals.h debugPrint.h
errorStuff.o: slinkyTypes.h slinkyGlobals.h errorStuff.h
expr.o: slinkyTypes.h slinkyGlobals.h slinkyExpressions.h expr.h errorStuff.h
expr.o: initialize.h ../y.tab.h
globals.o: slinkyTypes.h slinkyGlobals.h
initialize.o: slinkyTypes.h slinkyGlobals.h initialize.h errorStuff.h main.h
instantiate.o: slinkyTypes.h slinkyGlobals.h slinkyExpressions.h errorStuff.h
instantiate.o: expr.h instantiate.h ../y.tab.h
link.o: slinkyTypes.h slinkyGlobals.h link.h debugPrint.h errorStuff.h
link.o: instantiate.h poke.h read.h relocate.h write.h
main.o: slinkyTypes.h slinkyGlobals.h main.h initialize.h link.h
map.o: slinkyTypes.h slinkyGlobals.h map.h debugPrint.h link.h
poke.o: slinkyTypes.h slinkyGlobals.h poke.h debugPrint.h errorStuff.h expr.h
read.o: slinkyTypes.h slinkyGlobals.h read.h debugPrint.h errorStuff.h
read.o: initialize.h instantiate.h link.h
relocate.o: slinkyTypes.h slinkyGlobals.h relocate.h debugPrint.h
relocate.o: errorStuff.h link.h map.h
slinkyTables.o: slinkyTypes.h builtins.h
write.o: slinkyTypes.h slinkyGlobals.h write.h
