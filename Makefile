# Tester Makefile

#---------------------------------------------------------------------------------
#
#                CONSTANTS
#
#---------------------------------------------------------------------------------
CC = gcc
CFLAGS = -Wall -g

## Source Locations
OTHER = ./Other
ALM = ./Almica_Integer_Algorithm
LINT = ./Lahiri_Integer_Algorithm
LINL = ./Lahiri_Linear_Algorithm
## End Locations

SubInt_DEP = ./Other/delay.o ./Other/utvpiInterpreter.o ./Other/constraint.o ./Other/voidRefList.o ./Other/halfint.o
SubLin_DEP = ./Other/utvpiInterpreter.o ./Other/constraint.o ./Other/voidRefList.o ./Other/halfint.o
Lahiri_DEP = ./Other/utvpiInterpreter.o ./Other/constraint.o ./Other/voidRefList.o ./Other/delay.o
BF_DEP = utvpiInterpreter.o constraint.o voidRefList

DEPH = $(OTHER)/delay.h $(Other)/halfint.h $(OTHER)/utvpiInterpreter.h $(OTHER)/constraint.h $(OTHER)/voidRefList.h

DEPC = $(OTHER)/delay.c $(Other)/halfint.c $(OTHER)/utvpiInterpreter.c $(OTHER)/constraint.c $(OTHER)/voidRefList.c

OBJ = delay.o hellofunc.o

#---------------------------------------------------------------------------------
#
#                MAIN Executables
#            (/home/andersonm/Desktop/almicautvpiemp/code/)
#    Almica Loc (subWojInt):    ./Almica Integer Algorithm
#    LintLoc (Lahiri):    ./Lahiri Integer Algorithm
#    LLin (mine, subWojLin):    ./Lahiri Linear Algorithm
#    Other (BellmanFord):    ./Other/
#----------------------------------------------------------------------------------
All: mine.exe SubWojLin.exe Lahiri.exe LahiriCO1.exe LahiriCO2.exe LahiriFIFO.exe LahiriFIFOtrace.exe subWojInt.exe subWojIntCO1.exe subWojIntCO2.exe subWojIntFIFO.exe subWojIntFIFOtrace.exe halfIntTester.exe utvpiInterpreterTester.exe

mine.exe: $(SubLin_DEP) $(LINL)/mine.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

SubWojLin.exe: $(SubLin_DEP) $(LINL)/subWojLin.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

Lahiri.exe: $(Lahiri_DEP) $(LINT)/lahiri.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS) -lm

LahiriCO1.exe: $(Lahiri_DEP) $(LINT)/lahiriCO1.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS) -lm

LahiriCO2.exe: $(Lahiri_DEP) $(LINT)/lahiriCO2.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS) -lm

LahiriFIFO.exe: $(Lahiri_DEP) $(LINT)/lahiriFIFO.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS) -lm

LahiriFIFOtrace.exe: $(Lahiri_DEP) $(LINT)/lahiriFIFOtrace.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS) -lm

subWojInt.exe: $(SubInt_DEP) $(ALM)/subWojInt.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

subWojIntCO1.exe: $(SubInt_DEP) $(ALM)/subWojIntCO1.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

subWojIntCO2.exe: $(SubInt_DEP) $(ALM)/subWojIntCO2.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

subWojIntFIFO.exe: $(SubInt_DEP) $(ALM)/subWojIntFIFO.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

subWojIntFIFOtrace.exe: $(SubInt_DEP) $(ALM)/subWojIntFIFOtrace.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS) -lm

#---------------------------------------------------------------------------------
#
#		TESTERS
#
#---------------------------------------------------------------------------------
halfIntTester.exe: ./Other/halfint.o ./Other/halfIntTester.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

utvpiInterpreterTester.exe: ./Other/utvpiInterpreter.o ./Other/utvpiInterpreterTester.c
	$(CC) -I$(OTHER) -o $@ $^ $(CFLAGS)

#----------------------------------------------------------------------------------
#
#                DEPENDENCIES
#    Loc: /home/andersonm/Desktop/almicautvpiemp/code/Other/
#
#----------------------------------------------------------------------------------
./Other/%.o: ./Other/%.c
	$(CC) -c $(CFLAGS) $< -o $@


#------------------------------------------------------------------------------------
#
#		CLEAN UP
#
#------------------------------------------------------------------------------------
.PHONY : clean
clean: 
	rm ./Other/*.o *.exe
