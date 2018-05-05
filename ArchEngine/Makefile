#*===========================================================================*
#* Arch Engine - "Makefile"                                                  *
#*                                                                           *
#* Makefile for project build on Ubuntu.                                     *
#* Just a basic/test hard-coded Makefile for now.                            *
#* It will be upgraded during engine's development                           *
#*                                                                           *
#* Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
#* Created: 10/04/2018                                                       *
#* Last Modified: 05/05/2018                                                 *
#*===========================================================================*/


#---------------------------------------------------------------------- Folders

OUTDIR = GCCOutput
OBJDIR = GCCOutput/Intermediate

CORE = ArchEngine/Core
ECS = ArchEngine/ECS
OS = ArchEngine/OS
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

$(OBJDIR)/$(CORE)/concurrentEventQueue.o : $(CORE)/concurrentEventQueue.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(CORE)/engine.o : $(CORE)/engine.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(CORE)/eventManager.o : $(CORE)/eventManager.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(ECS)/idGenerator.o : $(ECS)/idGenerator.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(OS)/inputContext.o : $(OS)/inputContext.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(OS)/inputManager.o : $(OS)/inputManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(OS)/systemManager.o : $(OS)/systemManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(OS)/window.o : $(OS)/window.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(SCRIPT)/luaScript.o : $(SCRIPT)/luaScript.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(TESTERS)/core_engine_test.o : \
	$(TESTERS)/core_engine_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/ecs_ecs_test.o : \
	$(TESTERS)/ecs_ecs_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/event_system_test.o : \
	$(TESTERS)/event_system_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/os_input_context_test.o : \
	$(TESTERS)/os_input_context_test.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(TESTERS)/os_window_test.o : \
	$(TESTERS)/os_window_test.cpp
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

_OBJ = $(CORE)/concurrentEventQueue.o $(CORE)/engine.o $(CORE)/eventManager.o \
	$(ECS)/idGenerator.o $(OS)/inputContext.o $(OS)/inputManager.o \
	$(OS)/window.o $(SCRIPT)/luaScript.o $(TESTERS)/core_engine_test.o\
	$(TESTERS)/ecs_ecs_test.o $(TESTERS)/event_system_test.o \
	$(TESTERS)/os_input_context_test.o $(TESTERS)/os_window_test.o \
	#(TESTERS)/script_lua_script_test.o $(TESTERS)/utils_logger_test.o \
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
	if [ ! -d GCCOutput/Intermediate/$(ECS) ]; then \
		mkdir GCCOutput/Intermediate/$(ECS); fi
	if [ ! -d GCCOutput/Intermediate/$(OS) ]; then \
		mkdir GCCOutput/Intermediate/$(OS); fi
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