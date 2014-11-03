#include "Game.h"

	/**
	 * Game object constructor, creates all objects and sets their
	 * basic properties.
	 * Parameters: #Integer value of the window width.
	 * 			   #Integer value of the window heigth.
	 */
	/* {+FOLD} GAME::GAME(UNSIGNED INT,UNSIGNED INT) */ 
	Game::Game(unsigned int x = 640, unsigned int y = 480)
	:window(sf::VideoMode(x,y),"Dr.Holly",sf::Style::Fullscreen),
	 player(new Player(x,y)),
	 music(new sf::Music())
	{
		wWidth = x;
		wHeight = y;
		window.setMouseCursorVisible(false);

		/* Place the player in the middle of the room. */
		sf::Vector2f vfTemp(static_cast<float>(wWidth) / 2,static_cast<float>(wHeight) / 2);
		player->setPosition(vfTemp);

		if(!font.loadFromFile("Media/Fonts/COURIER.TTF"))
			std::cout << "[Error]Font not loaded." << std::endl;

		window.clear(sf::Color::White);
		music->openFromFile("Media/Music/room-theme.wav"); // TODO: Better music...
		music->setLoop(true);
		music->stop();

		/* Vector initialization START */
		CharList.resize(0);
		ProjList.resize(0);
		PlrProjList.resize(0);
		/* Vector initialization END */

		gameState = PAUSED;
		isMovingUp = isMovingDown = isMovingLeft = isMovingRight = false;


		/* {+FOLD} WALL CREATION */
		float w = static_cast<float>(wWidth);
		float h = static_cast<float>(wHeight);
		sf::Vector2f vfTemp1(0.f,0.f); // For position.
		sf::Vector2f vfTemp2(w,20.f); // For size.
		wall1.setPosition(vfTemp1);
		wall1.setSize(vfTemp2);
		wall1.setFillColor(sf::Color::Blue);

		vfTemp1.x = w - 20;
		vfTemp1.y = 0;
		vfTemp2.x = 20.f;
		vfTemp2.y = h;
		wall2.setPosition(vfTemp1);
		wall2.setSize(vfTemp2);
		wall2.setFillColor(sf::Color::Blue);

		vfTemp1.x = 0.f;
		vfTemp1.y = h - 20;
		vfTemp2.x = w;
		vfTemp2.y = 20.f;
		wall3.setPosition(vfTemp1);
		wall3.setSize(vfTemp2);
		wall3.setFillColor(sf::Color::Blue);

		vfTemp1.x = 0.f;
		vfTemp1.y = 0.f;
		vfTemp2.x = 20.f;
		vfTemp2.y = wHeight;
		wall4.setPosition(vfTemp1);
		wall4.setSize(vfTemp2);
		wall4.setFillColor(sf::Color::Blue);
		/* {-FOLD} */ 

		/* Health bar init. */
		sf::Vector2f vfTemp3(w / 6,50.f);
		sf::Vector2f vfTemp4(100.f,30.f);
		backHP.setPosition(vfTemp3);
		barHP.setPosition(vfTemp3);
		backHP.setSize(vfTemp4);
		barHP.setSize(vfTemp4);
		backHP.setFillColor(sf::Color::Black);
		barHP.setFillColor(sf::Color::Red);
		textHP.setString("HP");
		textHP.setFont(font);
		textHP.setColor(sf::Color::Black);
		textHP.setCharacterSize(40);
		textHP.setPosition(w / 6 - 50.f, 35.f);

		/* Level manager creation and initialization. */
		currentLevel = new Level;
		currentLevel->init(wWidth,wHeight);
		currentLevel->create();
	}
	/* {-FOLD} */

	/**
	 * Main game method managing the game loop as long as the window is opened
	 * and the game hasn't ended. Also guards the time between frames.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::RUN() */
	void Game::run()
	{
		sf::Time lastUpdate = sf::Time::Zero;
		sf::Time frameTime = sf::seconds(1.f/60.f);

		/* Initial menu */
		Menu* menu = new Menu(wWidth,wHeight);
		bool bTemp = menu->run(window,music);
		if(bTemp)
			gameState = ENDED;
		else
			gameState = RUNNING;

		/**
		 * Additional condition of state is used because
		 * of the possibility of window getting closed by
		 * the menu (or other object) and then another
		 * object would attempt to draw into it. This 
		 * second condition eliminates that possibility
		 * and enhances code readability.
		 */
		render();
		while(window.isOpen() && gameState != ENDED)
		{
			process();
			lastUpdate += clock.restart(); // Fixed framerate.
			while(lastUpdate > frameTime)
			{
				process();
				lastUpdate -= frameTime;

				/* If paused only accept input, do not update. */
				if(gameState != PAUSED)
					update(frameTime);
				/* Sad to hear this:( */
				if(gameState == LOST)
					getLostScreen();
				/* Congrats. */
				if(gameState == WON)
					getWinScreen();
			}
			render();
		}
	}
	/* {-FOLD} */

	/**
	 * Method that accepts player's input and decides what action
	 * should be taken.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::PROCESS() */
	void Game::process()
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::KeyPressed:
					handleInput(event.key.code,true);
					break;
				case sf::Event::KeyReleased:
					handleInput(event.key.code,false);
					break;
				case sf::Event::Closed:
					window.close();
					break;
			}
		}
	}
	/* {-FOLD} */

	/**
	 * Method that calls move() and shoot() methods of all entities
	 * currently in the game and also keeps track of collision
	 * between a character and enemy projectiles.
	 * Parameters: #sf::Time variable containing the time the frame took.
	 */
	/*  {+FOLD} GAME::UPDATE(sf::Time) */
	void Game::update(sf::Time frameTime)
	{
		sf::Vector2f movement(0.f,0.f);
		if(isMovingUp)
			movement.y -= 120.f;
		else if(isMovingDown)
			movement.y += 120.f;
		if(isMovingLeft)
			movement.x -= 120.f;
		else if(isMovingRight)
			movement.x += 120.f;

		changePlrSprite();
		/* {+FOLD} MOVING ENTITIES */
		player->move(movement*frameTime.asSeconds());
		if(outOfBounds(player->getSprite()) || (currentLevel->levelNumber == 6 && player->getPosition().y < wHeight / 4.f))
			player->move(movement*static_cast<float>(frameTime.asSeconds())*-1.f); // Get back, you!

		for(int i = 0; i < CharList.size(); i++)
		{
			CharList[i]->move(CharList[i]->getDirection(player->getPosition()));
			if(outOfBounds(CharList[i]->getSprite()))
			{
				CharList[i]->move(CharList[i]->getDirection(player->getPosition()) * -1.f);
				CharList[i]->stun(); // Crash to the wall.
			}
		}

		/* Move enemy projectiles and check for their life length. */
		std::vector<Projectile*>::iterator iterator0 = ProjList.begin();
		while(iterator0 != ProjList.end())
		{
			(*iterator0)->move(); // Iterator points to pointer to projectile.
			if(outOfBounds((*iterator0)->getSprite()) || (*iterator0)->getsDeleted())
			{
				delete *iterator0; // Deallocate memory.
				iterator0 = ProjList.erase(iterator0);
			}
			else
				iterator0++; // Standard increment.
		}

		/* Move player projectiles and check for their life lenght. */
		std::vector<Projectile*>::iterator iterator1 = PlrProjList.begin();
		while(iterator1 != PlrProjList.end())
		{
			(*iterator1)->move(); // Points to the pointer that points to the projectile object.
			if(outOfBounds((*iterator1)->getSprite()))
			{
				(*iterator1)->bounce(); // Get back, you.
				(*iterator1)->move();
			}
			if((*iterator1)->getsDeleted())
			{
				delete *iterator1; // Memory deallocation.
				iterator1 = PlrProjList.erase(iterator1); // Modifies the iterator.
			}
			else
				iterator1++; // Standar incremention if erase() not used.
		}
		/* {-FOLD} */ 

		/* {+FOLD} ENTITIES SHOOTING */
		for(int i = 0; i < CharList.size(); i++)
		{
			Projectile* prTempPtr = CharList[i]->shoot(player->getPosition());
			if(prTempPtr != NULL && ProjList.size() < 80) // Cooldown!
				ProjList.push_back(prTempPtr);
		}
		/* {-FOLD} */ 

		collisionSystem(); // Checks for collision between objects.

		/* Removing dead enemies. */
		std::vector<Enemy*>::iterator iterator2 = CharList.begin();
		while(iterator2 != CharList.end())
		{
			if((*iterator2)->getsDeleted())
			{
				delete *iterator2;
				iterator2 = CharList.erase(iterator2);
			}
			else
				iterator2++;
		}

		/* {+FOLD} LEVEL PROGRESSION */
		if(CharList.size() == 0 && currentLevel->levelNumber == 6)
			gameState = WON;
		if(CharList.size() == 0 && currentLevel->done[currentLevel->levelNumber] == false)
		{
			currentLevel->done[currentLevel->levelNumber] = true;
			currentLevel->setPortals();
			/* Delete all current enemy projectiles. */
			for(int i = 0; i < ProjList.size(); i++)
				delete ProjList[i];
			ProjList.resize(0);
		}
		if(currentLevel->done[currentLevel->levelNumber])
			currentLevel->pollPortal(player->getSprite(),wWidth,wHeight);
		if(currentLevel->freshlyCreated)
		{
			spawnEnemies();
			render();
			render(); // Fullscreen mode would sometimes still show the previous room after first render() call.
			currentLevel->getText(&window);
			clock.restart(); // Idling in intro would cause too many updates per second!
			currentLevel->done[currentLevel->levelNumber] = false;
			currentLevel->freshlyCreated = false;
			isMovingUp = isMovingDown = isMovingRight = isMovingLeft = false; // Stop the momentum.
		}
		/* {-FOLD} */
	}
	/* {-FOLD} */

	/**
	 * Method that clears the window, draws all the sprites and
	 * displays everything on the window.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::RENDER() */
	void Game::render()
	{
		window.clear(sf::Color::White);

		/* {+FOLD} DRAWING WALLS */
		window.draw(wall1);
		window.draw(wall2);
		window.draw(wall3);
		window.draw(wall4);
		/* {-FOLD} */

		/* {+FOLD} DRAWING ENTITIES */
		window.draw(*(player->getSprite()));
		for(int i = 0; i < CharList.size(); i++)
			window.draw(*(CharList[i]->getSprite()));

		for(int i = 0; i < ProjList.size(); i++)
			window.draw(*(ProjList[i]->getSprite()));

		for(int i = 0; i < PlrProjList.size(); i++)
			window.draw(*(PlrProjList[i]->getSprite()));
		/* {-FOLD} */

		currentLevel->draw(&window);

		/* Drawing health bar last to not be overdrawn by an entity. */
		getHealthBar();

		window.display();
	}
	/* {-FOLD} */ 

	/**
	 * Method deciding what action should be done based
	 * on the player's input.
	 * Parameters: #sf::Keyboard::Key variable with the key code.
	 * 			   #Boolean variable determining whether the key
	 * 			    was pressed or released.
	 */
	/* {+FOLD} GAME::HANDLEINPUT(sf::KEYBOARD::KEY,BOOL) */
	void Game::handleInput(sf::Keyboard::Key key, bool isPressed)
	{
		if(gameState == PAUSED && key != sf::Keyboard::Space)
			return;
		switch(key)
		{
			case sf::Keyboard::W:
				isMovingUp = isPressed;
				break;
			case sf::Keyboard::S:
				isMovingDown = isPressed;
				break;
			case sf::Keyboard::A:
				isMovingLeft = isPressed;
				break;
			case sf::Keyboard::D:
				isMovingRight = isPressed;
				break;
			case sf::Keyboard::Escape:
			{
				Menu* menu = new Menu(wWidth,wHeight);
				if(menu->run(window,music))
					gameState = ENDED;
				delete menu;
				break;
			}
			case sf::Keyboard::I:
			case sf::Keyboard::K:
			case sf::Keyboard::J:
			case sf::Keyboard::L:
				if(!player->coolDown() && PlrProjList.size() < 2)
				{
					Projectile* prTempPtr = player->shoot(key);
					if(prTempPtr != NULL)
						PlrProjList.push_back(prTempPtr);
				}
				break;
			case sf::Keyboard::Space:
				if(gameState == RUNNING && isPressed)
				{
					isMovingUp = isMovingDown = isMovingLeft = isMovingRight = false; // To cancel the momentum.
					gameState = PAUSED;
				}
				else if(gameState == PAUSED && isPressed)
					gameState = RUNNING;
				break;
			case sf::Keyboard::Home:
				/* Debug purposes. */
				window.close();
				break;
			case sf::Keyboard::Delete:
				/* Debug purposes */
				for(int i = 0; i < CharList.size(); i++)
					delete CharList[i];
				CharList.clear();
				break;
			default:
				// Do nothing. Without default, the compiler would scream because of enum cases.
				break;
		}
	}
	/* {-FOLD} */ 

	/**
	 * Method that determines how the player's
	 * sprite should be changed, based on the direction
	 * of his/her movement.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::CHANGEPLRSPRITE() */
	void Game::changePlrSprite()
	{
		int iTemp = 0;
		if(isMovingUp && !isMovingLeft && !isMovingRight)
			iTemp = 1;
		else if(isMovingLeft)
			iTemp = 2;
		else if(isMovingRight)
			iTemp = 3;
		else if(isMovingDown)
			iTemp = 4;
		player->changeSprite(iTemp);
	}
	/* {-FOLD} */

	/**
	 * A method that returns true if a sprite
	 * is out of the window or collides with a 
	 * wall.
	 * Parameters: #Pointer to said sprite.
	 */
	/* {+FOLD} GAME::OUTOFBOUNDS(SF::SPRITE*) */
	bool Game::outOfBounds(sf::Sprite* sprite)
	{
		bool bTemp1 = sprite->getPosition().x <= 0 || sprite->getPosition().x >= wWidth - 10;
		bool bTemp2 = sprite->getPosition().y <= 0 || sprite->getPosition().y >= wHeight - 10;
		return collisionWall(sprite) || bTemp1 || bTemp2;
	}
	/* {-FOLD} */

	/**
	 * A method that checks for a collision between
	 * two sprites using SFML Rectangle's intersection
	 * method.
	 * Parameters: #Pointer to first sprite.
	 *			   #Pointer to second sprite.
	 */
	/* {+FOLD} GAME::COLLIDE(SF::SPRITE*,SF::SPRITE*) */
	bool Game::collide(sf::Sprite* first, sf::Sprite* second)
	{
		sf::IntRect hitbox1;
		hitbox1.width = first->getGlobalBounds().width;
		hitbox1.height = first->getGlobalBounds().height;
		hitbox1.top = first->getPosition().x;
		hitbox1.left = first->getPosition().y;

		sf::IntRect hitbox2;
		hitbox2.width = second->getGlobalBounds().width;
		hitbox2.height = second->getGlobalBounds().height;
		hitbox2.top = second->getPosition().x;
		hitbox2.left = second->getPosition().y;

		return hitbox1.intersects(hitbox2);
	}
	/* {-FOLD} */

	/**
	 * The game's collision checking system,
	 * loops through projectiles and enemy
	 * entities to see if any type of bad
	 * (hurting) collision occurs and inflicts
	 * damage or destroys the object if necessary.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::COLLISIONSYSTEM() */
	void Game::collisionSystem()
	{
		/* Player x Enemy projectiles */
		for(int i = 0; i < ProjList.size(); i++)
			if(collide(player->getSprite(),ProjList[i]->getSprite()))
			{
				if(player->inflictDMG(ProjList[i]->getDMG()))
					gameState = LOST; // RIP
				ProjList[i]->requestDeletion(); // Will be destroyed in next update.
			}

		for(int i = 0; i < CharList.size(); i++)
		{
			/* Enemy x Player projectiles */
			for(int j = 0; j < PlrProjList.size(); j++)
				if(collide(CharList[i]->getSprite(),PlrProjList[j]->getSprite()))
				{
					if(CharList[i]->inflictDMG(PlrProjList[j]->getDMG()))
						CharList[i]->requestDeletion();
					PlrProjList[j]->requestDeletion();
				}
			/* Enemy x Player */
			if(collide(CharList[i]->getSprite(),player->getSprite()))
			{
				if(player->inflictDMG(CharList[i]->getDMG()))
					gameState = LOST; // RIP
				CharList[i]->stun(); // So the player can move away.
			}
		}
	}
	/* {-FOLD} */

	/**
	 * A method that checks if a sprite (passed
	 * as argument) collides with a wall.
	 * Parameters: #Pointer to the sprite.
	 */
	/* {+FOLD} GAME::COLLISIONWALL(SF::SPRITE*) */
	bool Game::collisionWall(sf::Sprite* sprite)
	{
		/* Sprite */
		sf::IntRect hitbox = sprite->getTextureRect();
		hitbox.left = sprite->getPosition().x;
		hitbox.top = sprite->getPosition().y;

		/* Wall 1 */
		sf::IntRect hitWall1;
		hitWall1.left = wall1.getPosition().x;
		hitWall1.top = wall1.getPosition().y;
		hitWall1.width = wall1.getSize().x;
		hitWall1.height = wall1.getSize().y;
		
		/* Wall 2 */
		sf::IntRect hitWall2;
		hitWall2.left = wall2.getPosition().x;
		hitWall2.top = wall2.getPosition().y;
		hitWall2.width = wall2.getSize().x;
		hitWall2.height = wall2.getSize().y;

		/* Wall 3 */
		sf::IntRect hitWall3;
		hitWall3.left = wall3.getPosition().x;
		hitWall3.top = wall3.getPosition().y;
		hitWall3.width = wall3.getSize().x;
		hitWall3.height = wall3.getSize().y;

		/* Wall 4 */
		sf::IntRect hitWall4;
		hitWall4.left = wall4.getPosition().x;
		hitWall4.top = wall4.getPosition().y;
		hitWall4.width = wall4.getSize().x;
		hitWall4.height = wall4.getSize().y;

		/* Temportary bool variables for better core readability. */
		bool tBool1 = hitbox.intersects(hitWall1);
		bool tBool2 = hitbox.intersects(hitWall2);
		bool tBool3 = hitbox.intersects(hitWall3);
		bool tBool4 = hitbox.intersects(hitWall4);

		return tBool1 || tBool2 || tBool3 || tBool4;
	}
	/* {-FOLD} */

	/**
	 * A method that spawns enemies in a freshly
	 * created level.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::SPAWNENEMIES() */
	void Game::spawnEnemies()
	{
		player->restoreHP(); // Let's be fair.

		sf::Vector2f vfTemp(0.f,0.f);
		switch(currentLevel->levelNumber)
		{
			case 0:
				// Do nothing, this is a peaceful room!
				break;
			case 1:
				// Three blobs in a horizontal line with AI #1.
				CharList.resize(3);
				for(int i = 0; i < 3; i++)
				{
					CharList[i] = new Enemy(1);
					vfTemp.x = wWidth / 4 + i * wWidth / 4;
					vfTemp.y = wHeight / 4;
					CharList[i]->setPosition(vfTemp);
				}
				break;
			case 2:
				// Dr Forstix.
				CharList.resize(1);
				CharList[0] = new Enemy(4);
				vfTemp.x = wWidth / 2.f;
				vfTemp.y = wHeight / 6.f;
				CharList[0]->setPosition(vfTemp);
				break;
			case 3:
				// Four blobs in the corners with AI #2.
				CharList.resize(4);
				PlrProjList.reserve(80);
				// One.
				CharList[0] = new Enemy(2);
				vfTemp.x = 30.f;
				vfTemp.y = 30.f;
				CharList[0]->setPosition(vfTemp);
				// Two.
				CharList[1] = new Enemy(2);
				vfTemp.x = wWidth - 80;
				vfTemp.y = 30;
				CharList[1]->setPosition(vfTemp);
				// Three.
				CharList[2] = new Enemy(2);
				vfTemp.x = 30.f;
				vfTemp.y = wHeight - 60;
				CharList[2]->setPosition(vfTemp);
				// And four!
				CharList[3] = new Enemy(2);
				vfTemp.x = wWidth - 80;
				vfTemp.y = wHeight - 60;
				CharList[3]->setPosition(vfTemp);
				break;
			case 4:
				// Doc Topferino.
				ProjList.reserve(30);
				CharList.resize(1);
				CharList[0] = new Enemy(5);
				vfTemp.x = wWidth / 4;
				vfTemp.y = wHeight / 4;
				CharList[0]->setPosition(vfTemp);
				break;
			case 5:
				CharList.resize(2);
				CharList[0] = new Enemy(3);
				vfTemp.x = wWidth / 4;
				vfTemp.y = wHeight / 2;
				CharList[0]->setPosition(vfTemp);

				CharList[1] = new Enemy(3);
				vfTemp.x = 3 * wWidth / 4; // vfTemp.y no change.
				CharList[1]->setPosition(vfTemp);
				break;
			case 6:
				// Dr Holly.
				ProjList.reserve(20);
				CharList.resize(1);
				CharList[0] = new Enemy(6);
				vfTemp.x = wWidth / 2;
				vfTemp.y = wHeight / 10;
				CharList[0]->setPosition(vfTemp);
				break;
		}
	}
	/* {-FOLD} */

	/**
	 * A method that updates and draws
	 * the health bar.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::GETHEALTHBAR */
	void Game::getHealthBar()
	{
		sf::Vector2f vfTemp(player->getHP(),30.f);
	 	barHP.setSize(vfTemp);
		window.draw(backHP); // Draw the health bar background.
		window.draw(barHP); // Draw the health progress bar.
		window.draw(textHP); // Draw the HP text.
	}
	/* {-FOLD} */

	/**
	 * A method that displays the 'LOST' screen
	 * after the player's health reaches 0.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::GETLOSTSCREEN() */
	void Game::getLostScreen()
	{
		window.clear(sf::Color::Black);
		sf::Text lostText("You lost the game.\nPress [ENTER] to exit.",font,40);
		sf::Vector2f vfTemp(wWidth / 3.f - 30.f, wHeight / 3.f + 20.f);
		lostText.setPosition(vfTemp);
		window.draw(lostText);
		window.display();
		pollForEnter(LOST);
	}
	/* {-FOLD} */

	/**
	 * A method that displays the 'WON' screen
	 * after the player beats the dark and evil 
	 * Dr Holly.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::GETWINSCREEN() */
	void Game::getWinScreen()
	{
		window.clear(sf::Color::Black);
		sf::Text winText("Congratulations,\nYou won the game!\nPress [ENTER] to exit.",font,40);
		sf::Vector2f vfTemp(wWidth / 3.f - 30.f, wHeight / 3.f + 20.f);
		winText.setPosition(vfTemp);
		window.draw(winText);
		window.display();
		pollForEnter(WON);
	}
	/* {-FOLD} */

	/**
	 * A simple method that polls the player for input
	 * until the input is the return key pressed.
	 * Parameters: #Status that the game is in until
	 *				the player presses the return key.
	 */
	/* {+FOLD} GAME::POLLFORENTER(STATE) */
	void Game::pollForEnter(STATE condition)
	{
		sf::Event tEvent;
		while(window.isOpen() && gameState == condition)
		{
			while(window.pollEvent(tEvent))
			{
				if(tEvent.type == sf::Event::KeyPressed)
					if(tEvent.key.code == sf::Keyboard::Return)
						gameState = ENDED; // Double if NECESSARY! Otherwise tEvent.type.code might be null!
			}
		}
		
	}
	/* {-FOLD} */

	/**
	 * Default Game class destructor, gets rid
	 * of all dynamically allocated variables and
	 * checks if the window is closed, if not, it 
	 * closes it before deleting.
	 * Parameters: NONE
	 */
	/* {+FOLD} GAME::~GAME() */
	Game::~Game()
	{
		music->stop();
		delete music;
		delete player;

		/**
		 * Note: Iterator not needed, because all members
		 * of all vectors are being deleted, thus there are no
		 * changes in indexes. (No use of erase() member method.)
		 * Vector pointers themselves are deleted by the vector 
		 * and it's destructor when this scope end.
		 */
		for(int i = 0; i < CharList.size(); i++)
			delete CharList[i];

		for(int i = 0; i < ProjList.size(); i++)
			delete ProjList[i];

		for(int i = 0; i < PlrProjList.size(); i++)
			delete PlrProjList[i];
	}
	/* {-FOLD} */
