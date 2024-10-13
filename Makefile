CXX = g++
CXXFLAGS = -std=c++17 -MMD -g
OBJECTS = Action.o BCSSelector.o Block.o Level.o Main.o BlockCluster.o BlockClusterShape.o BlockStyle.o DisplayManager.o InputManager.o Session.o SessionConstants.o
DEPENDS = ${OBJECTS:.o=.d}
PROGRAM = quadris

main: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${PROGRAM}

-include ${DEPENDS}

clean:
	rm *.o *.d ${PROGRAM}

all: main
