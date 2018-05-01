#*===========================================================================*
#* Arch Engine - "Makefile"                                                  *
#*                                                                           *
#* Makefile for project build on Ubuntu.                                     *
#* Just a basic/test hard-coded Makefile for now.                            *
#* It will be upgraded during engine's development                           *
#*                                                                           *
#* Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
#* Created: 10/04/2018                                                       *
#* Last Modified: 27/04/2018                                                 *
#*===========================================================================*/


#---------------------------------------------------------------------- Folders

OUTDIR = GCCOutput
OBJDIR = GCCOutput/Intermediate

CORE = ArchEngine/Core
SCRIPT = ArchEngine/Script
TESTERS = ArchEngine/Testers
UTILS = ArchEngine/Utils

#------------------------------------------------------------ Compiler settings

CXX = g++
CXXFLAGS = -std=c++11 -I/usr/include/lua5.3
LIBS = -pthread -lSDL2 -llua5.3
VPATH = $(CORE):$(SCRIPT):$(TESTERS):$(UTILS)

#------------------------------------------------------------------------ Rules

info:
	@echo "Usage:"
	@echo "make install_dependencies -> install all the needed libraries"
	@echo "make create_directories -> \
creates the necessary directories for compilation"
	@echo "make build -> compiles the engine"


COMPILEOBJ = $(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJDIR)/$(CORE)/inputContext.o : $(CORE)/inputContext.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(CORE)/inputManager.o : $(CORE)/inputManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(CORE)/systemManager.o : $(CORE)/systemManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(CORE)/window.o : $(CORE)/window.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(SCRIPT)/luaScript.o : $(SCRIPT)/luaScript.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(TESTERS)/core_input_context_test.o : \
	$(TESTERS)/core_input_context_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/core_system_manager_test.o : \
	$(TESTERS)/core_system_manager_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/core_window_test.o : \
	$(TESTERS)/core_window_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/script_lua_script_test.o : \
	$(TESTERS)/script_lua_script_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/utils_logger_test.o : \
	$(TESTERS)/utils_logger_test.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(UTILS)/serviceLocator.o : $(UTILS)/serviceLocator.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(UTILS)/timer.o : $(UTILS)/timer.cpp
	$(COMPILEOBJ)

_OBJ = $(CORE)/inputContext.o $(CORE)/inputManager.o $(CORE)/systemManager.o \
	$(CORE)/window.o $(SCRIPT)/luaScript.o $(TESTERS)/core_input_context_test.o\
	$(TESTERS)/core_system_manager_test.o $(TESTERS)/core_window_test.o \
	$(TESTERS)/script_lua_script_test.o $(TESTERS)/utils_logger_test.o \
	$(UTILS)/serviceLocator.o $(UTILS)/timer.o
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

build: $(OBJ)
	$(CXX) -o $(OUTDIR)/Build/ArchEngine $^ $(CXXFLAGS) $(LIBS)


#------------------------------------------------------------------------ Utils

create_directories:
	if [ ! -d GCCOutput ]; then mkdir GCCOutput; fi
	if [ ! -d GCCOutput/Build ]; then mkdir GCCOutput/Build; fi
	if [ ! -d GCCOutput/Intermediate ]; then mkdir GCCOutput/Intermediate; fi
	if [ ! -d GCCOutput/Intermediate/ArchEngine ]; then \
		mkdir GCCOutput/Intermediate/ArchEngine; fi
	if [ ! -d GCCOutput/Intermediate/$(CORE) ]; then \
		mkdir GCCOutput/Intermediate/$(CORE); fi
	if [ ! -d GCCOutput/Intermediate/$(SCRIPT) ]; then \
		mkdir GCCOutput/Intermediate/$(SCRIPT); fi
	if [ ! -d GCCOutput/Intermediate/$(TESTERS) ]; then \
		mkdir GCCOutput/Intermediate/$(TESTERS); fi
	if [ ! -d GCCOutput/Intermediate/$(UTILS) ]; then \
		mkdir GCCOutput/Intermediate/$(UTILS); fi
	@echo ""

install_dependencies:
	if ! locate libsdl2-dev; then \
		sudo apt-get install libsdl2-dev; fi
	if ! locate liblua5.3-dev; then \
		sudo apt-get install liblua5.3-dev; fi

.PHONY: clean

clean:
	rm -rf $(OUTDIR)