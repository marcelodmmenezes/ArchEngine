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


# Compiler settings

CXX = g++
CXXFLAGS = -std=c++11
LIBS = -pthread

# Folders

OUTDIR = GCCOutput
OBJDIR = GCCOutput/Intermediate

CORE = ArchEngine/Core
TESTERS = ArchEngine/Testers
UTILS = ArchEngine/Utils


# Rules


# Tests

_OBJ = utils_logger_test.o serviceLocator.o
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

$(OBJDIR)/utils_logger_test.o : $(TESTERS)/utils_logger_test.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJDIR)/serviceLocator.o: $(UTILS)/serviceLocator.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

utils_logger_test: $(OBJ)
	$(CXX) -o $(OUTDIR)/$@ $^ $(CXXFLAGS) $(LIBS)


# Utils

create_directories:
	mkdir GCCOutput
	mkdir GCCOutput/Intermediate

clean:
	rm $(OBJDIR)/*.o