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

Makefile.depend:
	makedepend -Y -f - *.c > Makefile.depend

depend: Makefile.depend

include Makefile.depend
