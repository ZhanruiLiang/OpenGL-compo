# Define executable name
BIN = main

# Define source files
SRCS = main.cpp gltools.cpp control.cpp compo.cpp body.cpp

# Define header file paths
INCPATH = -I./

# Define the -L library path(s)
LDFLAGS =

# Define the -l library name(s)
LIBS = -lGL -lglut -lm -lGLU -lpthread

# Only in special cases should anything be edited below this line
OBJS      = $(SRCS:.cpp=.o)
CXXFLAGS  = -std=c++11 -g -DDEBUG
DEP_FILE  = .depend

.PHONY = all clean distclean

# Main entry point
#
test: depend $(BIN)
	./$(BIN)

all: depend $(BIN)

# For linking object file(s) to produce the executable
#
$(BIN): $(OBJS)
	@echo Linking $@
	@$(CXX) $^ $(LDFLAGS) $(LIBS) -o $@

# For compiling source file(s)
#
.cpp.o:
	@echo Compiling $<
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) $<

# For cleaning up the project
#
clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(BIN)
	$(RM) $(DEP_FILE)

# For determining source file dependencies
#
depend: $(DEP_FILE)
	@touch $(DEP_FILE)

$(DEP_FILE):
	@echo Generating dependencies in $@
	@-$(CXX) -E -MM $(CXXFLAGS) $(INCPATH) $(SRCS) >> $(DEP_FILE)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring distclean,$(MAKECMDGOALS)))
-include $(DEP_FILE)
endif
endif
