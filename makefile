CXX=gcc
CXXFLAG=-Wall -Wcast-align

INCLUDE=
OBJDIR=./src/
OBJ=$(OBJDIR)*.c
EXECDIR=./bin/
EXECUTABLE=unitconv

all: $(EXECDIR)$(EXECUTABLE)

$(EXECDIR)$(EXECUTABLE): $(OBJ)
	mkdir -p $(EXECDIR)
	$(CXX) $(CXXFLAG) $(INCLUDE) $(OBJ) -o $@

debug: CXXFLAG+=-g
debug: $(EXECDIR)$(EXECUTABLE)

clean:
	rm -rf $(EXECDIR)
