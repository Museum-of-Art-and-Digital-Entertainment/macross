.SUFFIXES: .o .c .h .run

OBJECTS = builtins.o debugPrint.o errorStuff.o expr.o globals.o initialize.o\
instantiate.o link.o main.o map.o poke.o read.o relocate.o slinkyTables.o\
write.o

SOURCES = builtins.c debugPrint.c errorStuff.c expr.c globals.c initialize.c\
instantiate.c link.c main.c map.c poke.c read.c relocate.c slinkyTables.c\
write.c slinkyExpressions.h slinkyGlobals.h slinkyTypes.h y.tab.h

CFLAGS= -m32 # slinky is not 64 bit clean

.c.o:
	cc $(CFLAGS) -c $*.c

.c.run:
	cc $(CFLAGS) -o $* $*.c

slinky: $(OBJECTS)
	cc $(CFLAGS) -g -o slinky $(OBJECTS)

builtins.o: builtins.c slinkyGlobals.h slinkyTypes.h slinkyExpressions.h

debugPrint.o: debugPrint.c slinkyGlobals.h slinkyTypes.h

errorStuff.o: errorStuff.c slinkyGlobals.h slinkyTypes.h

expr.o: expr.c slinkyExpressions.h slinkyGlobals.h slinkyTypes.h y.tab.h

initialize.o: initialize.c slinkyGlobals.h slinkyTypes.h

instantiate.o: instantiate.c slinkyGlobals.h slinkyTypes.h slinkyExpressions.h

link.o: link.c slinkyGlobals.h slinkyTypes.h

main.o: main.c slinkyGlobals.h slinkyTypes.h

map.o: map.c slinkyGlobals.h slinkyTypes.h

poke.o: poke.c slinkyGlobals.h slinkyTypes.h

read.o: read.c slinkyGlobals.h slinkyTypes.h

relocate.o: relocate.c slinkyGlobals.h slinkyTypes.h

slinkyTables.o: slinkyTables.c slinkyTypes.h

write.o: write.c slinkyGlobals.h slinkyTypes.h

clean:
	/bin/rm -f *.o slinky

love:
	@echo "Not war?"

