CXX = g++
CXXFLAGS = -g -Wall -MMD
SOURCES=Main.cc Board.cc Piece.cc Point.cc
OBJECTS=$(SOURCES:.cc=.o)
DEPENDS = ${OBJECTS:.o=.d}
EXEC = quadris

${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}
