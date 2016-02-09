# to make for another target CPU, redefine PROC to the name of the target
# processor, e.g., 68000
PROC =6502

MACROSS_OBJECTS = actions_$(PROC).o buildStuff1.o buildStuff2.o		\
                  buildStuff3.o builtInFunctions.o			\
                  builtInFunsSD_$(PROC).o debugPrint.o			\
                  debugPrintSD_$(PROC).o emitBranch_$(PROC).o		\
                  emitStuff.o encode.o errorStuff.o			\
                  expressionSemantics.o fixups.o garbage.o globals.o	\
                  initialize.o lexer.o listing.o lookups.o		\
                  macrossTables_$(PROC).o main.o object.o		\
                  operandStuffSD_$(PROC).o parserMisc.o			\
                  semanticMisc.o statementSemantics.o			\
                  structSemantics.o tokenStrings_$(PROC).o y.tab.o

# Macross is not 64-bit clean and it does a lot of silent downcasting
# to simulate subclasses and uses int and void * interchangably a
# bunch. gcc calls these the int-conversion and
# incompatible-pointer-types warnings.
CFLAGS=-m32 -O2 -ansi -DYYDEBUG -DTARGET_CPU=CPU_$(PROC)
LDFLAGS=-m32

# If yacc is notionally present on a system, it's usually actually
# bison in a compatibility mode. bison is available by name more often
# than yacc itself is.
YACC=bison -y
#YACC=yacc

CC=gcc
#CC=clang

macross: $(MACROSS_OBJECTS)
	$(CC) $(LDFLAGS) -o macross $(MACROSS_OBJECTS)

# This is a switcher program between multiple versions of Macross, not
# really a build product
# driver: driver.c
#	cc $(CFLAGS) -o driver driver.c

y.tab.c y.tab.h: macross_$(PROC).y
	$(YACC) -d macross_$(PROC).y

clean:
	rm -f *.o y.tab.c y.tab.h macross

love:
	@echo "Not war?"

.c.o:
	$(CC) $(CFLAGS) -c $<

Makefile.depend:
	makedepend -Y -DTARGET_CPU=CPU_$(PROC) -f - *.c > Makefile.depend

depend: Makefile.depend

include Makefile.depend
