#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Character.h"
#include "Menu.h"
#include <vector>
#include <iostream>

/**
 * Enumerated list of game states.
 * States: RUNNING, PAUSED, ENDED.
 */
/* {+FOLD} GAME::STATE */
	enum STATE{ RUNNING, PAUSED, ENDED, LOST, WON};
/* {-FOLD} */

/* Forward declaration for struct Level. */
struct Level;

/** Main class of the game, containing all the entity objects as well as the
  * main methods the game needs to run.
  */
/* {+FOLD} Class GAME */
class Game
{   
	/* {+FOLD} PRIVATE */
	private:
		/* Methods: */
		void process();
        void update(sf::Time);
        void render();
        void handleInput(sf::Keyboard::Key,bool);
        void changePlrSprite();
		void collisionSystem();
		void spawnEnemies();
		void getHealthBar();
		void getWinScreen();
		void getLostScreen();
		void pollForEnter(STATE);
		bool outOfBounds(sf::Sprite*);
		bool collisionWall(sf::Sprite*);
		bool collide(sf::Sprite*,sf::Sprite*);

		/* Variables: */
        bool isMovingUp,isMovingDown,isMovingLeft,isMovingRight;
		unsigned int wWidth,wHeight; // Window size.
		Menu* menu;
		Player* player;
		sf::Clock clock;
		std::vector<Enemy*> CharList;
		std::vector<Projectile*> ProjList;
		std::vector<Projectile*> PlrProjList;
		sf::RenderWindow window;
		sf::Music* music;
		sf::RectangleShape wall1,wall2,wall3,wall4; // Starts at top and goes clock-wise.
		sf::RectangleShape barHP,backHP;
		sf::Text textHP;
		sf::Font font;
		STATE gameState;
		Level* currentLevel;
     /* {-FOLD} */

     /* {+FOLD} PUBLIC */
     public:
		Game(unsigned int,unsigned int);
		void run();
		~Game();
     /* {-FOLD} */
};
/* {-FOLD} */
#endif

/**
 * A struct used to manage levels, their draw
 * process, enemy spawning and movement between
 * them.
 */
/* {+FOLD} GAME::LEVEL */
struct Level
{
	/* {+FOLD} DECLARATION */
	int levelNumber; // Current level.
	bool freshlyCreated; // To spawn enemies.
	bool done[6]; // Level done?
	bool portal[3]; // Portal to be drawn?
	bool portalOpen[3]; // Portal accessible?
	sf::Sprite portals[3];
	sf::Texture textureOPEN;
	sf::Texture textureCLOSED;
	sf::Texture textureDONE;
	sf::Font font;
	sf::Text text;
	bool started;
	sf::RectangleShape textbox;
	/* {-FOLD} */

	/* {+FOLD} INIT(INT,INT) */
	void init(int wWidth, int wHeight)
	{
		textureOPEN.loadFromFile("Media/Textures/portal-open.png");
		textureCLOSED.loadFromFile("Media/Textures/portal-closed.png");
		textureDONE.loadFromFile("Media/Textures/portal-done.png");
		levelNumber = 0;
		freshlyCreated = false;
		started = true;
		if(!font.loadFromFile("Media/Fonts/COURIER.TTF"))
			std::cout << "[Error]Font not loaded." << std::endl;

		/* Sets portal positions. */
		float w = static_cast<float>(wWidth);
		float h = static_cast<float>(wHeight);
		portals[0].setPosition(50.f,h / 2);
		portals[1].setPosition(w / 2 - 30,50.f);
		portals[2].setPosition(w - 100,h / 2);

		/* Sets the text box. */
		textbox.setFillColor(sf::Color::Yellow);
		textbox.setOutlineColor(sf::Color::Black);
		textbox.setOutlineThickness(5);
		sf::Vector2f vfTemp1(w / 3,4 * h / 6); // Position.
		sf::Vector2f vfTemp2(w / 3,h / 6); // Size.
		textbox.setPosition(vfTemp1);
		textbox.setSize(vfTemp2);

		text.setFont(font);
		text.setPosition(vfTemp1);
		text.setCharacterSize(15);
		text.setColor(sf::Color::Black);

		done[0] = true;
		for(int i = 1; i < 6; i++)
			done[i] = false;
	}
	/* {-FOLD} */

	/* {+FOLD} CREATE() */
	void create()
	{
		portalOpen[0] = portalOpen[1] = portalOpen[2] = false;
		/* Which portals are open? */
		switch(levelNumber)
		{
			case 0:
				/* LEFT PORTAL */
				if(done[2])
					portals[0].setTexture(textureDONE);
				else
				{
					portals[0].setTexture(textureOPEN);
					portalOpen[0] = true;
				}
				/* TOP PORTAL */
				if(done[5])
					portals[1].setTexture(textureDONE);
				else if(done[4])
				{
					portals[1].setTexture(textureOPEN);
					portalOpen[1] = true;
				}
				else
					portals[1].setTexture(textureCLOSED);
				/* RIGHT PORTAL */
				if(done[4])
					portals[2].setTexture(textureDONE);
				else if(done[2])
				{
					portals[2].setTexture(textureOPEN);
					portalOpen[2] = true;
				}
				else
					portals[2].setTexture(textureCLOSED);
				break;
			case 1:
			case 2:
				portals[0].setTexture(textureOPEN);
				break;
			case 3:
			case 4:
				portals[2].setTexture(textureOPEN);
				break;
			case 5:
			case 6:
				portals[1].setTexture(textureOPEN);
				break;
		}
		setPortals();
		freshlyCreated = true;
	}
	/* {-FOLD} */

	/* {+FOLD} SETPORTALS() */
	void setPortals()
	{
		/* Which portals are to be drawn? */
		portal[0] = false;
		portal[1] = false;
		portal[2] = false;
		switch(levelNumber)
		{
			case 0:
				// All three portals at level 0.
				portal[0] = true;
				portal[1] = true;
				portal[2] = true;
				break;
			case 1:
			case 2:
				if(done[levelNumber])
					portal[0] = true;
				break;
			case 3:
			case 4:
				if(done[levelNumber])
					portal[2] = true;
				break;
			case 5:
			case 6:
				if(done[levelNumber])
					portal[1] = true;
				break;
		}
	}
	/* {-FOLD} */ 

	/* {+FOLD} COLLIDE(SF::SPRITE*,SF::SPRITE*) */
	bool collide(sf::Sprite* first, sf::Sprite* second)
	{
		/* Same as Game::collide */
		sf::IntRect hitbox1 = first->getTextureRect();
		hitbox1.left = first->getPosition().x;
		hitbox1.top = first->getPosition().y;

		sf::IntRect hitbox2 = second->getTextureRect();
		hitbox2.left = second->getPosition().x;
		hitbox2.top = second->getPosition().y;

		return hitbox1.intersects(hitbox2);
	}
	/* {-FOLD} */

	/* {+FOLD} POLLPORTAL(SF::SPRITE*,INT,INT) */
	void pollPortal(sf::Sprite* sprite,int w,int h)
	{
		if(!done[levelNumber]) // Oops, level not completed!
			return;

		/* Checks if player entered a portal. */
		if(collide(&portals[0],sprite))
		{
			if(levelNumber == 0 && !portalOpen[0])
				return;
			levelNumber = (levelNumber + 1) % 3; // Left portal leads to 1,2 and back 0.
			create();
			sf::Vector2f vfTemp(static_cast<float>(w) / 2,static_cast<float>(h) / 2);
			sprite->setPosition(vfTemp);
			portal[0] = portal[1] = portal[2] = false;
		}
		if(collide(&portals[1],sprite))
		{
			if(levelNumber == 0)
			{
				if(!portalOpen[1])
					return;
				levelNumber = 5;
			}
			else if(levelNumber == 5)
				levelNumber = 6;
			else if(levelNumber == 6)
				levelNumber = 0; // Top portal leads to 5,6 and back 0.
			create();
			sf::Vector2f vfTemp(static_cast<float>(w) / 2,static_cast<float>(h) / 2);
			sprite->setPosition(vfTemp);
			portal[0] = portal[1] = portal[2] = false;
		}
		if(collide(&portals[2],sprite))
		{
			if(levelNumber == 0)
			{
				if(!portalOpen[2])
					return;
				levelNumber = 3;
			}
			else if(levelNumber == 3)
				levelNumber = 4;
			else if(levelNumber == 4)
				levelNumber = 0; // Right portal leads to 3,4 and back 0.
			create();
			sf::Vector2f vfTemp(static_cast<float>(w) / 2,static_cast<float>(h) / 2);
			sprite->setPosition(vfTemp);
			portal[0] = portal[1] = portal[2] = false;
		}
	}
	/* {-FOLD} */ 

	/* {+FOLD} GETTEXT(SF::RENDERWINDOW*) */
	void getText(sf::RenderWindow* window)
	{
		switch(levelNumber)
		{
			case 0:
				text.setString("Welcome to the 'Atttack on Dr Holly' game.\nYou can start by entering the portal to your left.\nBlue color means that the portal is enterable,\ngreen that it is done and red that it is not\nopened yet.\n\n\t\t\t\t\t[ENTER]");
				break;
			case 1:
				text.setString("[Guide]\nThese are blobs, your enemies.\nThis kind will try to charge at you, so don't\nlet them to stack up or they can one shot you!\n\n\n\t\t\t\t\t[ENTER]");
				break;
			case 2:
				text.setString("[Dr Forstix]\nSo you think you can beat me? You don't even know\nhow to use g/re/p!\n\n\n\t\t\t\t\t[ENTER]");
				break;
			case 3:
				text.setString("[Guide]\nThis is the Super Mega Deathzone 3000, be sure\nto dodge those shots or you might get toasted\npretty quick!\n\n\n\t\t\t\t\t[ENTER]");
				break;
			case 4:
				text.setString("[Doc Topferino]\nRemember, dog goes 'woof',\nbut big dogs go 'WOOF!'...\n\n\n\n\t\t\t\t\t[ENTER]");
				break;
			case 5:
				text.setString("[Guide]\nBe careful! These blobs have low damage,\nbut can shoot pretty fast!\nBe sure to kill them before you get\nhit too many times and remember your jukes!\n\n\t\t\t\t\t[ENTER]");
				break;
			case 6:
				text.setString("[Dr Holly]\nSo.. you managed to get past my associates?\nWell, get ready to face a deadly dose of\nhomework,then.\n\n\n\t\t\t\t\t[ENTER]");
				break;
		}

		if(levelNumber == 0)
			if(!started)
				return;
		started = false;
		bool tDone = false;
		sf::Event tEvent;

		/* Drawing the textbox. */
		window->draw(textbox);
		window->draw(text);
		window->display(); // The previous render is still present underneath.

		while(window->isOpen() && !tDone)
		{
			while(window->pollEvent(tEvent) && !tDone)
			{
				switch(tEvent.type)
				{
					case sf::Event::KeyPressed:
						if(tEvent.key.code == sf::Keyboard::Return)
							tDone = true;
						break;
					case sf::Event::Closed:
						window->close();
						break;
				}
			}
		}
		
	}
	/* {-FOLD} */

	/* {+FOLD} DRAW(SF::RENDERWINDOW*) */
	void draw(sf::RenderWindow* window)
	{
		/* Just draws the portals */
		for(int i = 0; i < 3; i++)
			if(portal[i] && done[levelNumber])
				window->draw(portals[i]);
	}
	/* {-FOLD} */
};
/* {-FOLD} */
