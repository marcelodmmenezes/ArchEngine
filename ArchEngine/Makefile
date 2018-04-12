#*===========================================================================*
#* Arch Engine - "Makefile"                                                  *
#*                                                                           *
#* Makefile for project build on linux. Just a basic/test Makefile for now.  *
#* It will be upgraded during engine's development                           *
#*                                                                           *
#* Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
#* Created: 10/04/2018                                                       *
#* Last Modified: 10/04/2018                                                 *
#*===========================================================================*/


#------------------------------------------------------------ Compiler settings

CXX = g++
CXXFLAGS = -std=c++11
LIBS = -pthread -lSDL2

#---------------------------------------------------------------------- Folders

OUTDIR = GCCOutput
TESTOUTDIR = GCCOutput/Tests
OBJDIR = GCCOutput/Intermediate
TESTOBJDIR = $(OBJDIR)/Tests

CORE = ArchEngine/Core
TESTERS = ArchEngine/Testers
UTILS = ArchEngine/Utils


#------------------------------------------------------------------------ Rules


#------------------------------------------------------------------------ Tests

#--- Logger ---#
_OBJ = utils_logger_test.o serviceLocator.o
OBJ = $(patsubst %, $(TESTOBJDIR)/%, $(_OBJ))

$(TESTOBJDIR)/utils_logger_test.o : $(TESTERS)/utils_logger_test.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TESTOBJDIR)/serviceLocator.o: $(UTILS)/serviceLocator.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

utils_logger_test: $(OBJ)
	$(CXX) -o $(TESTOUTDIR)/$@ $^ $(CXXFLAGS) $(LIBS)

#--- Window ---#
_OBJ = core_window_test.o serviceLocator.o window.o
OBJ = $(patsubst %, $(TESTOBJDIR)/%, $(_OBJ))

$(TESTOBJDIR)/core_window_test.o : $(TESTERS)/core_window_test.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TESTOBJDIR)/window.o : $(CORE)/window.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

core_window_test: $(OBJ)
	$(CXX) -o $(TESTOUTDIR)/$@ $^ $(CXXFLAGS) $(LIBS)


#------------------------------------------------------------------------ Utils

create_directories:
	mkdir GCCOutput
	mkdir GCCOutput/Tests
	mkdir GCCOutput/Intermediate
	mkdir GCCOutput/Intermediate/Tests

install_dependencies:
	sudo apt-get install libsdl2-dev

clean:
	rm $(OBJDIR)/*.o