.SUFFIXES: .o .c .h .run

# to make for another target CPU, redefine PROC to the name of the target
# processor, e.g., 68000
PROC =6502

OBJECTS = y.tab.o actions.o buildStuff1.o buildStuff2.o\
buildStuff3.o builtInFunctions.o builtInFunsSD.o debugPrint.o debugPrintSD.o\
emitBranch.o emitStuff.o encode.o errorStuff.o expressionSemantics.o fixups.o\
garbage.o initialize.o lexer.o listing.o lookups.o macrossTables.o main.o\
malloc.o object.o operandStuffSD.o parserMisc.o semanticMisc.o\
statementSemantics.o structSemantics.o tokenStrings.o

SOURCES = macross_$(PROC).y actions_$(PROC).c buildStuff1.c buildStuff2.c\
buildStuff3.c builtInFunctions.c builtInFunsSD_$(PROC).c debugPrint.c\
debugPrintSD_$(PROC).c emitBranch_$(PROC).c emitStuff.c encode.c errorStuff.c\
expressionSemantics.c fixups.c garbage.c initialize.c lexer.c listing.c\
lookups.c macrossTables_$(PROC).c main.c malloc.c object.c\
operandStuffSD_$(PROC).c parserMisc.c semanticMisc.c statementSemantics.c\
structSemantics.c tokenStrings_$(PROC).c lexerTables.h macrossGlobals.h\
macrossTypes.h operandDefs_$(PROC).h operandBody_$(PROC).h\
conditionDefs_$(PROC).h driver.c slinkyExpressions.h\
operandStuff.h statementSemantics.h listing.h parserMisc.h\
emitBranch.h semanticMisc.h expressionSemantics.h

HEADERS = macrossTypes.h macrossGlobals.h

CFLAGS=-m32 # macross is not 64 bit clean
# If yacc is notionally present on a system, it's usually actually
# bison in a compatibility mode. bison is available by name more often
# than bison itself is.
YACC=bison -o y.tab.c

.c.o:
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) $*.c

.c.run:
	cc $(CFLAGS) -o $* $*.c

macross: $(OBJECTS)
	cc $(CFLAGS) -g -o macross $(OBJECTS)

driver: driver.c
	cc $(CFLAGS) -o driver driver.c

update: .mark
	kessel "(cd /u0/chip/macross; make macross >&errorfyle)" &

install: macross
	cp macross /u1/gg/bin/macross_tmp
	strip /u1/gg/bin/macross_tmp
	mv /u1/gg/bin/macross_$(PROC) /u1/gg/bin/macross_$(PROC).old
	mv /u1/gg/bin/macross_tmp /u1/gg/bin/macross_$(PROC)
	cp /u1/gg/bin/macross_$(PROC) /net/mycroft/u1/gg/bin
	cp /u1/gg/bin/macross_$(PROC) /net/shem/u1/gg/bin
	cp /u1/gg/bin/macross_$(PROC) /net/weyr/u1/gg/bin

dinstall: driver
	cp driver /u1/gg/bin/driver_tmp
	strip /u1/gg/bin/driver_tmp
	mv /u1/gg/bin/driver_tmp /u1/gg/bin/driver/macross
	cp /u1/gg/bin/driver /net/mycroft/u1/gg/bin/macross
	cp /u1/gg/bin/driver /net/shem/u1/gg/bin/macross
	cp /u1/gg/bin/driver /net/weyr/u1/gg/bin/macross

change:
	rm *.o
	rm *.tab.*
	cp Makefile_68000 Makefile

move: .mark

.mark: $(SOURCES)
	cp $? /net/kessel/u0/chip/macross
	cp $? /net/kessel/u0/chip/macross/prof
	cp $? opt
	date >.mark
	date >/net/kessel/u0/chip/macross/.mark
	date >/net/kessel/u0/chip/macross/prof/.mark
	date >opt/.mark

macrossTypes.h: operandDefs_$(PROC).h operandBody_$(PROC).h\
conditionDefs_$(PROC).h

actions.o: actions_$(PROC).c $(HEADERS)
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) actions_$(PROC).c
	mv actions_$(PROC).o actions.o

buildStuff1.o: buildStuff1.c $(HEADERS)

buildStuff2.o: buildStuff2.c $(HEADERS)

buildStuff3.o: buildStuff3.c $(HEADERS)

builtInFunctions.o: builtInFunctions.c $(HEADERS) expressionSemantics.h operandStuff.h semanticMisc.h

builtInFunsSD.o: builtInFunsSD_$(PROC).c $(HEADERS)
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) builtInFunsSD_$(PROC).c
	mv builtInFunsSD_$(PROC).o builtInFunsSD.o

debugPrint.o: debugPrint.c y.tab.h $(HEADERS)

debugPrintSD.o: debugPrintSD_$(PROC).c y.tab.h $(HEADERS)
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) debugPrintSD_$(PROC).c
	mv debugPrintSD_$(PROC).o debugPrintSD.o

emitBranch.o: emitBranch_$(PROC).c $(HEADERS)
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) emitBranch_$(PROC).c
	mv emitBranch_$(PROC).o emitBranch.o

emitStuff.o: emitStuff.c $(HEADERS)
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) emitStuff.c

encode.o: encode.c $(HEADERS) y.tab.h semanticMisc.h slinkyExpressions.h

errorStuff.o: errorStuff.c $(HEADERS)

expressionSemantics.o: expressionSemantics.c y.tab.h $(HEADERS) expressionSemantics.h semanticMisc.h

fixups.o: fixups.c $(HEADERS)

garbage.o: garbage.c y.tab.h $(HEADERS)

initialize.o: initialize.c $(HEADERS)

lexer.o: lexer.c lexerTables.h y.tab.h $(HEADERS) parserMisc.h

listing.o: listing.c $(HEADERS) listing.h

lookups.o: lookups.c $(HEADERS)

macrossTables.o: macrossTables_$(PROC).c y.tab.h macrossTypes.h
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) macrossTables_$(PROC).c
	mv macrossTables_$(PROC).o macrossTables.o

malloc.o: malloc.c

main.o: main.c $(HEADERS)

object.o: object.c $(HEADERS)

operandStuffSD.o: operandStuffSD_$(PROC).c $(HEADERS)
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) operandStuffSD_$(PROC).c
	mv operandStuffSD_$(PROC).o operandStuffSD.o

parserMisc.o: parserMisc.c y.tab.h $(HEADERS) parserMisc.h

semanticMisc.o: semanticMisc.c $(HEADERS) semanticMisc.h expressionSemantics.h

statementSemantics.o: statementSemantics.c $(HEADERS) emitBranch.h expressionSemantics.h operandStuff.h parserMisc.h semanticMisc.h statementSemantics.h

structSemantics.o: structSemantics.c $(HEADERS)

tokenStrings.o: tokenStrings_$(PROC).c $(HEADERS)
	cc $(CFLAGS) -c -g -DTARGET_CPU=CPU_$(PROC) tokenStrings_$(PROC).c
	mv tokenStrings_$(PROC).o tokenStrings.o

y.tab.o: y.tab.c $(HEADERS)
	cc $(CFLAGS) -c -g -DYYDEBUG -DTARGET_CPU=CPU_$(PROC) y.tab.c

y.tab.c y.tab.h: macross_$(PROC).y
	$(YACC) -d macross_$(PROC).y

y.output: macross_$(PROC).y
	$(YACC) -vd macross_$(PROC).y

cleanup:
	/bin/rm -f *.o y.output y.tab.c y.tab.h macross

love:
	@echo "Not war?"

