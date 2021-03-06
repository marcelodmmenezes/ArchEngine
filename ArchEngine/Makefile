#*===========================================================================*
#* Arch Engine - "Makefile"                                                  *
#*                                                                           *
#* Makefile for project build on Ubuntu.                                     *
#* Just a basic/test hard-coded Makefile for now.                            *
#* It will be upgraded during engine's development                           *
#*                                                                           *
#* Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
#* Created: 10/04/2018                                                       *
#* Last Modified: 22/06/2018                                                 *
#*===========================================================================*/


#------------------------------------------------------------- Additional paths

CPLUS_INCLUDE_PATH = /usr/Bullet
export CPLUS_INCLUDE_PATH

LIBRARY_PATH = /usr/Bullet/lib
export LIBRARY_PATH

#---------------------------------------------------------------------- Folders

OUTDIR = GCCOutput
OBJDIR = GCCOutput/Intermediate

API = ArchEngine/API
CORE = ArchEngine/Core
GRAPHICS = ArchEngine/Graphics
GUI = ArchEngine/GUI
OS = ArchEngine/OS
PHYSICS = ArchEngine/Physics
SCRIPT = ArchEngine/Script
TESTERS = ArchEngine/Testers
UTILS = ArchEngine/Utils

#------------------------------------------------------------ Compiler settings

CXX = g++
CXXFLAGS = -std=c++11 -I/usr/include/lua5.3 -I/usr/include/freetype2
LIBS = -pthread -ldl -lSDL2 -llua5.3 -lBulletSoftBody -lBulletDynamics \
	-lBulletCollision -lLinearMath -lassimp -lfreetype

#------------------------------------------------------------------------ Rules

info:
	@echo "Usage:"
	@echo "make install_dependencies -> install all the needed libraries"
	@echo "make create_directories -> \
creates the necessary directories for compilation"
	@echo "make build -> compiles the engine"


COMPILEOBJ = $(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJDIR)/$(API)/entity.o : $(API)/entity.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(CORE)/engine.o : $(CORE)/engine.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(CORE)/event.o : $(CORE)/event.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(CORE)/eventManager.o : $(CORE)/eventManager.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(GRAPHICS)/animation.o : $(GRAPHICS)/animation.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/assimpLoader.o : $(GRAPHICS)/assimpLoader.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/bone.o : $(GRAPHICS)/bone.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/camera.o : $(GRAPHICS)/camera.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/debugCamera.o : $(GRAPHICS)/debugCamera.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/framebuffer.o : $(GRAPHICS)/framebuffer.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/glad_3_3_core.o : $(GRAPHICS)/glad_3_3_core.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/graphicsManager.o : $(GRAPHICS)/graphicsManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/materialManager.o : $(GRAPHICS)/materialManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/mesh.o : $(GRAPHICS)/mesh.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/shader.o : $(GRAPHICS)/shader.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/skeleton.o : $(GRAPHICS)/skeleton.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/stb_image.o : $(GRAPHICS)/stb_image.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/terrainGenerator.o : $(GRAPHICS)/terrainGenerator.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/thirdPersonCamera.o : $(GRAPHICS)/thirdPersonCamera.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GRAPHICS)/uniforms.o : $(GRAPHICS)/uniforms.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(GUI)/guiComponent.o : $(GUI)/guiComponent.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GUI)/guiManager.o : $(GUI)/guiManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GUI)/pushButton.o : $(GUI)/pushButton.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GUI)/renderableComponent.o : $(GUI)/renderableComponent.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(GUI)/writableComponent.o : $(GUI)/writableComponent.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(OS)/inputContext.o : $(OS)/inputContext.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(OS)/inputManager.o : $(OS)/inputManager.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(OS)/window.o : $(OS)/window.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(PHYSICS)/debugDraw.o : $(PHYSICS)/debugDraw.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(PHYSICS)/physicsManager.o : $(PHYSICS)/physicsManager.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(SCRIPT)/engineLuaAPI.o : $(SCRIPT)/engineLuaAPI.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(SCRIPT)/luaScript.o : $(SCRIPT)/luaScript.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(TESTERS)/test.o : \
	$(TESTERS)/test.cpp
	$(COMPILEOBJ)

$(OBJDIR)/$(UTILS)/debugging.o : $(UTILS)/debugging.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(UTILS)/fileWatcher.o : $(UTILS)/fileWatcher.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(UTILS)/serviceLocator.o : $(UTILS)/serviceLocator.cpp
	$(COMPILEOBJ)
$(OBJDIR)/$(UTILS)/timer.o : $(UTILS)/timer.cpp
	$(COMPILEOBJ)

_OBJ = $(API)/entity.o $(CORE)/engine.o $(CORE)/event.o \
	$(CORE)/eventManager.o $(GRAPHICS)/animation.o \
	$(GRAPHICS)/assimpLoader.o $(GRAPHICS)/bone.o $(GRAPHICS)/camera.o \
	$(GRAPHICS)/debugCamera.o $(GRAPHICS)/framebuffer.o \
	$(GRAPHICS)/glad_3_3_core.o $(GRAPHICS)/graphicsManager.o \
	$(GRAPHICS)/materialManager.o $(GRAPHICS)/mesh.o \
	$(GRAPHICS)/shader.o  $(GRAPHICS)/skeleton.o $(GRAPHICS)/stb_image.o \
	$(GRAPHICS)/terrainGenerator.o $(GRAPHICS)/thirdPersonCamera.o \
	$(GRAPHICS)/uniforms.o $(GUI)/guiComponent.o $(GUI)/guiManager.o \
	$(GUI)/pushButton.o $(GUI)/renderableComponent.o \
	$(GUI)/writableComponent.o $(OS)/inputContext.o $(OS)/inputManager.o \
	$(OS)/window.o $(PHYSICS)/debugDraw.o $(PHYSICS)/physicsManager.o \
	$(SCRIPT)/engineLuaAPI.o $(SCRIPT)/luaScript.o $(TESTERS)/test.o \
	$(UTILS)/debugging.o $(UTILS)/fileWatcher.o $(UTILS)/serviceLocator.o \
	$(UTILS)/timer.o
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

build: $(OBJ)
	$(CXX) -o $(OUTDIR)/Build/ArchEngine $^ $(CXXFLAGS) $(LIBS)

release: $(OBJ)
	$(CXX) -Ofast -o $(OUTDIR)/Release/ArchEngine $^ $(CXXFLAGS) $(LIBS)

#------------------------------------------------------------------------ Utils

create_directories:
	if [ ! -d GCCOutput ]; then mkdir GCCOutput; fi
	if [ ! -d GCCOutput/Build ]; then mkdir GCCOutput/Build; fi
	if [ ! -d GCCOutput/Release ]; then mkdir GCCOutput/Release; fi
	if [ ! -d GCCOutput/Intermediate ]; then mkdir GCCOutput/Intermediate; fi
	if [ ! -d GCCOutput/Intermediate/ArchEngine ]; then \
		mkdir GCCOutput/Intermediate/ArchEngine; fi
	if [ ! -d GCCOutput/Intermediate/$(API) ]; then \
		mkdir GCCOutput/Intermediate/$(API); fi
	if [ ! -d GCCOutput/Intermediate/$(CORE) ]; then \
		mkdir GCCOutput/Intermediate/$(CORE); fi
	if [ ! -d GCCOutput/Intermediate/$(ECS) ]; then \
		mkdir GCCOutput/Intermediate/$(ECS); fi
	if [ ! -d GCCOutput/Intermediate/$(GRAPHICS) ]; then \
		mkdir GCCOutput/Intermediate/$(GRAPHICS); fi
	if [ ! -d GCCOutput/Intermediate/$(GUI) ]; then \
		mkdir GCCOutput/Intermediate/$(GUI); fi
	if [ ! -d GCCOutput/Intermediate/$(OS) ]; then \
		mkdir GCCOutput/Intermediate/$(OS); fi
	if [ ! -d GCCOutput/Intermediate/$(PHYSICS) ]; then \
		mkdir GCCOutput/Intermediate/$(PHYSICS); fi
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
	if ! locate libglm-dev; then \
		sudo apt-get install libglm-dev; fi
	if ! locate libassimp-dev; then \
		sudo apt-get install libassimp-dev; fi
	if ! locate libfreetype6-dev; then \
		sudo apt-get install libfreetype6-dev; fi

.PHONY: clean

clean:
	rm -rf $(OUTDIR)