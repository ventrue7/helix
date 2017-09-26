EXECUTABLES=TestSimulation

#compilers
cc=g++

#flags
debug=-g
std=-std=c++11
#std=-std=gnu++11
thread=-pthread
#numeric=-fext-numeric-literals #enable numeric literals

CPPFLAGS=$(debug)

EXPORT=win_x64
#script
all: ${EXECUTABLES}

#$@: left%, $^: right(all), $<: right(first)
# %: %.cpp Matrix.h Rand.h Simulation.h Simulation.cpp G2Enums.h RateModel.h G2PP.h G2PP.cpp
#	${cc} ${debug} ${std} ${thread} -I. -o $@ $^

TestSimulation: TestSimulation.o Curve.o Simulation.o G2PP.o
	${cc} ${debug} ${std} ${thread} -I. -o $@ $^

TestSimulation.cpp: G2PP.h

G2PP.h: Simulation.h Curve.h RateModel.h

G2PP.cpp: G2PP.h

RateModel.h: G2Enums.h

Curve.cpp: Curve.h

Simulation.h: Matrix.h Rand.h

Simulation.cpp: Simulation.h

.PHONY: clean purge import export extract run
clean:
	rm -f *.o *.exe

purge:
	rm -f *.h *.cpp

import:
	cp include/* src/* .

export:
	mv *.exe ${EXPORT}

extract:
	make purge clean import && make all export purge clean

run:
	${EXPORT}/${EXECUTABLES}.exe
