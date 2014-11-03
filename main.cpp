#include "Game.h"
#include <SFML/Graphics.hpp>

int main(){
	/* Getting info on resolution: */
	sf::VideoMode currentVideoMode;
	unsigned int width = currentVideoMode.getDesktopMode().width;
	unsigned int height = currentVideoMode.getDesktopMode().height;
    
	Game game(width,height);
    game.run();

	std::exit(EXIT_SUCCESS);
}
