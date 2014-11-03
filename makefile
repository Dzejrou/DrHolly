#  Makefile for DrHolly project.
#  Used by command: 'make'
#  (In case of name change: make -f FILENAME)
#  Created by dzejrou 2014.                     _____________________
#                                              /                    /
#  For future additions, the format is:       /    Attack On       /
#                                            /      The Evil      /
#     Target: [DEPENDENCIES]* <endl>        /     Doctor Holly   /
#           <TAB> [COMMAND]+ <endl>        /____________________/
#
#  Note: @ before command makes it invisible
#  	     in the command line.
#########################################################################

#Macros:
GR = -lsfml-graphics
WI = -lsfml-window
SY = -lsfml-system
AU = -lsfml-audio
SFML = $(GR) $(WI) $(SY) $(AU)
CPP = g++ 

#Targets:
DrHolly: main.o Game.o Character.o Projectile.o Menu.o
	@echo "Linking."
	@$(CPP) main.o Game.o Character.o Projectile.o Menu.o $(SFML) -o DrHolly
	@echo "Compilation complete."

main.o: main.cpp Game.h
	@echo "Compiling main.cpp"
	@$(CPP) -c main.cpp

Game.o: Game.h Game.cpp Character.h
	@echo "Compiling Game.cpp"
	@$(CPP) -c Game.cpp

Character.o: Character.h Character.cpp Projectile.h
	@echo "Compiling Character.cpp"
	@$(CPP) -c Character.cpp

Projectile.o: Projectile.h Projectile.cpp
	@echo "Compiling Projectile.cpp"
	@$(CPP) -c Projectile.cpp

Menu.o: Menu.h Menu.cpp
	@echo "Compiling Menu.cpp"
	@$(CPP) -c Menu.cpp

#Using the command 'make clean' removes all obj files.
clean: 
	@echo "Removing all obj files."
	@rm *.o
