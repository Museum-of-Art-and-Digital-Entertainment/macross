.SUFFIXES: .o .c .h .run

OBJECTS = builtins.o debugPrint.o errorStuff.o expr.o initialize.o\
instantiate.o link.o main.o map.o poke.o read.o relocate.o slinkyTables.o\
write.o

SOURCES = builtins.c debugPrint.c errorStuff.c expr.c initialize.c\
instantiate.c link.c main.c map.c poke.c read.c relocate.c slinkyTables.c\
write.c slinkyExpressions.h slinkyGlobals.h slinkyTypes.h y.tab.h

CFLAGS=-m32 # slinky is not 64 bit clean

.c.o:
	cc $(CFLAGS) -c -g $*.c

.c.run:
	cc $(CFLAGS) -o $* $*.c

slinky: $(OBJECTS)
	cc $(CFLAGS) -g -o slinky $(OBJECTS)

update: .mark
	kessel "(cd /u0/chip/macross/slinky; make slinky >&errorfyle)" &

move: .mark

.mark: $(SOURCES)
#	cp $? /net/kessel/u0/chip/macross/slinky
	cp $? /net/kessel/u0/chip/macross/slinky
#	cp $? /net/kessel/u0/chip/macross/slinky/prof
	cp $? /net/kessel/u0/chip/macross/slinky/prof
	cp $? opt
	date >.mark
#	date >/net/kessel/u0/chip/macross/slinky/.mark
	date >/net/kessel/u0/chip/macross/slinky/.mark
#	date >/net/kessel/u0/chip/macross/slinky/prof/.mark
	date >/net/kessel/u0/chip/macross/slinky/prof/.mark
	date >opt/.mark

install: slinky
	cp slinky /u1/gg/bin/slinky_tmp
	strip /u1/gg/bin/slinky_tmp
	mv /u1/gg/bin/slinky /u1/gg/bin/sliny.old
	mv /u1/gg/bin/slinky_tmp /u1/gg/bin/slinky

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

cleanup:
	/bin/rm -f *.o slinky

love:
	@echo "Not war?"

