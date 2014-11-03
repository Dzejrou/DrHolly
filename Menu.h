#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

/**
 * Class defining the menu objects, used in the game to 
 * displaye help, play the game, turn music on/off and
 * exit the program.
 */
/* {+FOLD} CLASS MENU */
class Menu
{
	/* {+FOLD} PRIVATE */
	private:
		/* Methods: */
		void poll(int,sf::RenderWindow&);
		void handleInput(sf::Keyboard::Key,int);
		void drawOptions(std::vector<sf::Text*>,bool,sf::RenderWindow&);
		void subMenuHelp(sf::RenderWindow&);

		/* Variables: */
		sf::Texture texture;
		sf::Sprite check;
		sf::Font font;
		sf::Vector2f position;
		int choice;
		int optCount;
		unsigned int width;
		unsigned int height;
		bool willExit;
		bool done;
		std::vector<sf::Text*> options;
		std::vector<sf::Text*> helpText;
	/* {-FOLD} */

	/* {+FOLD} PUBLIC */
	public:
		Menu(unsigned int,unsigned int);
		bool run(sf::RenderWindow&,sf::Music*);
		~Menu();
	/* {-FOLD} */
};
/* {-FOLD} */
#endif
