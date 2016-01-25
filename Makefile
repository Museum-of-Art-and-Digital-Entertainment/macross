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
CFLAGS=-m32 -g -ansi -DYYDEBUG -DTARGET_CPU=CPU_$(PROC)
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

slinky/slinky: $(SLINKY_OBJECTS)
	$(CC) $(LDFLAGS) -o slinky/slinky $(SLINKY_OBJECTS)

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

# Dependencies below created with makedepend -Y -DTARGET_CPU=CPU_6502 *.c
# DO NOT DELETE

actions_6502.o: macrossTypes.h operandDefs_6502.h conditionDefs_6502.h
actions_6502.o: macrossGlobals.h actions.h macrossTypes.h emitStuff.h
actions_6502.o: errorStuff.h semanticMisc.h
actions_68000.o: macrossTypes.h macrossGlobals.h
buildStuff1.o: macrossTypes.h macrossGlobals.h errorStuff.h lookups.h
buildStuff1.o: parserMisc.h
buildStuff2.o: macrossTypes.h macrossGlobals.h lookups.h parserMisc.h
buildStuff3.o: macrossTypes.h macrossGlobals.h lookups.h
builtInFunctions.o: macrossTypes.h macrossGlobals.h buildStuff.h errorStuff.h
builtInFunctions.o: expressionSemantics.h garbage.h lexer.h lookups.h
builtInFunctions.o: operandStuff.h semanticMisc.h statementSemantics.h
builtInFunsSD_6502.o: macrossTypes.h macrossGlobals.h builtInFunctions.h
builtInFunsSD_6502.o: operandStuff.h
builtInFunsSD_68000.o: macrossTypes.h macrossGlobals.h
debugPrint.o: macrossTypes.h macrossGlobals.h debugPrint.h y.tab.h
debugPrintSD_6502.o: macrossTypes.h macrossGlobals.h debugPrint.h y.tab.h
debugPrintSD_68000.o: macrossTypes.h macrossGlobals.h y.tab.h
emitBranch_6502.o: macrossTypes.h macrossGlobals.h buildStuff.h emitStuff.h
emitBranch_6502.o: parserMisc.h semanticMisc.h
emitBranch_68000.o: macrossTypes.h macrossGlobals.h
emitStuff.o: macrossTypes.h macrossGlobals.h actions.h debugPrint.h
emitStuff.o: errorStuff.h semanticMisc.h
encode.o: macrossTypes.h macrossGlobals.h y.tab.h encode.h debugPrint.h
encode.o: errorStuff.h parserMisc.h semanticMisc.h slinkyExpressions.h
errorStuff.o: macrossTypes.h macrossGlobals.h errorStuff.h initialize.h
expressionSemantics.o: macrossTypes.h macrossGlobals.h y.tab.h
expressionSemantics.o: builtInFunctions.h errorStuff.h expressionSemantics.h
expressionSemantics.o: fixups.h listing.h lookups.h operandStuff.h
expressionSemantics.o: parserMisc.h semanticMisc.h statementSemantics.h
expressionSemantics.o: tokenStrings.h
fixups.o: macrossTypes.h macrossGlobals.h errorStuff.h expressionSemantics.h
fixups.o: fixups.h listing.h operandStuff.h parserMisc.h semanticMisc.h
fixups.o: tokenStrings.h
garbage.o: macrossTypes.h macrossGlobals.h garbage.h operandStuff.h
garbage.o: parserMisc.h y.tab.h
globals.o: macrossTypes.h macrossGlobals.h
initialize.o: macrossTypes.h macrossGlobals.h initialize.h errorStuff.h
initialize.o: lexer.h lookups.h semanticMisc.h
lexer.o: macrossTypes.h macrossGlobals.h y.tab.h debugPrint.h errorStuff.h
lexer.o: lexer.h lexerTables.h listing.h lookups.h parserMisc.h
listing.o: macrossTypes.h macrossGlobals.h emitStuff.h lexer.h listing.h
listing.o: semanticMisc.h
lookups.o: macrossTypes.h macrossGlobals.h buildStuff.h errorStuff.h
lookups.o: garbage.h listing.h lookups.h operandStuff.h parserMisc.h
lookups.o: semanticMisc.h
macrossTables_6502.o: macrossTypes.h actions.h builtInFunctions.h y.tab.h
macrossTables_68000.o: macrossTypes.h y.tab.h
main.o: macrossTypes.h macrossGlobals.h initialize.h semanticMisc.h y.tab.h
object.o: macrossTypes.h macrossGlobals.h debugPrint.h encode.h
object.o: expressionSemantics.h fixups.h garbage.h lookups.h object.h
object.o: semanticMisc.h
operandStuffSD_6502.o: macrossTypes.h macrossGlobals.h errorStuff.h
operandStuffSD_6502.o: expressionSemantics.h fixups.h garbage.h listing.h
operandStuffSD_6502.o: operandStuff.h parserMisc.h semanticMisc.h
operandStuffSD_6502.o: statementSemantics.h
operandStuffSD_68000.o: macrossTypes.h macrossGlobals.h
parserMisc.o: macrossTypes.h macrossGlobals.h y.tab.h buildStuff.h fixups.h
parserMisc.o: initialize.h errorStuff.h parserMisc.h
semanticMisc.o: macrossTypes.h macrossGlobals.h y.tab.h semanticMisc.h
semanticMisc.o: buildStuff.h debugPrint.h emitStuff.h errorStuff.h
semanticMisc.o: expressionSemantics.h fixups.h listing.h lookups.h object.h
semanticMisc.o: operandStuff.h parserMisc.h
statementSemantics.o: macrossTypes.h macrossGlobals.h actions.h debugPrint.h
statementSemantics.o: emitBranch.h emitStuff.h errorStuff.h
statementSemantics.o: expressionSemantics.h fixups.h garbage.h lexer.h
statementSemantics.o: listing.h lookups.h operandStuff.h parserMisc.h
statementSemantics.o: semanticMisc.h statementSemantics.h structSemantics.h
statementSemantics.o: tokenStrings.h
structSemantics.o: macrossTypes.h macrossGlobals.h emitStuff.h errorStuff.h
structSemantics.o: listing.h parserMisc.h semanticMisc.h statementSemantics.h
structSemantics.o: structSemantics.h
tokenStrings_6502.o: macrossTypes.h macrossGlobals.h
tokenStrings_68000.o: macrossTypes.h macrossGlobals.h
y.tab.o: macrossTypes.h macrossGlobals.h buildStuff.h errorStuff.h lexer.h
y.tab.o: lookups.h operandStuff.h parserMisc.h semanticMisc.h
y.tab.o: statementSemantics.h
