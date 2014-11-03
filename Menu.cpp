#include "Menu.h"

	/**
	 * Default menu constructor, sets all basic variables, loads
	 * textures and reserves memory for options vector.
	 * Parameters: #Unsigned integer of the window width.
	 *			   #Unsigned integer og the window height.
	 */
	/* {+FOLD} MENU::MENU(UNSIGNED INT, UNSIGNED INT,BOOL*) */
	Menu::Menu(unsigned int w,unsigned int h)
	{
		done = false;
		choice = 0;
		optCount = 4;

		width = w;
		height = h;
	
		font.loadFromFile("Media/Fonts/COURIER.TTF");
		texture.loadFromFile("Media/Textures/blob.png");
		
		check.setTexture(texture);
		
		options.reserve(4); // Easy option adding: Change the size and create new sf::Text object.
		options.resize(4);
	}
	/* {-FOLD} */
	
	/**
	 * Creates and displays menu options along with the menu check sprite,
	 * then starts polling for input (Up/W, Down/S and Enter).
	 * Parameters: #The game window passes by reference for
	 *				rendering purposes.
	 *			   #Pointer to the music object to change it's status.
	 */
	/* {+FOLD} MENU::RUN() */
	bool Menu::run(sf::RenderWindow& window, sf::Music* music)
	{
		// Creating option texts.
		options[0] = new sf::Text("Play",font,40);
		options[1] = new sf::Text("Help",font,40);
		options[2] = new sf::Text("Music on/off",font,40);
		options[3] = new sf::Text("Exit",font,40);
		// Setting position of menu check.
		position.x = width / 3 - 70;
		position.y = height / 3 + 10;
	
		check.setPosition(position);
		
		// Setting position of menu options and drawing them.
		for(int i = 0; i < options.size(); i++)
		{
			position.x = width / 3;
    		position.y = height / 3 + i*40;
    		options[i]->setPosition(position);
    		window.draw(*options[i]);
		}
		
    	// Menu loop.
		choice = 0;
		willExit = false;
    	do
    	{
    		poll(optCount,window);
    		switch(choice)
    		{
    			case 0:
    				// If the game is running, just end the menu scope.
    				break;
    			case 1:
    				subMenuHelp(window);
					done = false;
    				break;
    			case 2:
					if(music->getStatus() == sf::SoundSource::Playing)
						music->stop();
					else
						music->play();
					done = false;
					break;
    			case 3: 
    				willExit = true;
    				break;		
    		}
    	}while(choice != 0 && !willExit); // Because of sub menus.	

		return willExit; // To change the gameState outside this scope.
	}
	/* {-FOLD} */
	
	/**
	 * Loops through user input and either calls procedure handleInput
	 * or sets the willExit variable to true and stops.
	 * Parameters: #Integer variable containing the number of choices.
	 *			   #The game window passes by reference for rendering
	 *			    purposes.
	 */
	/* {+FOLD} MENU::POLL(INT,SF::RENDERWINDOW&) */
	void Menu::poll(int optCount,sf::RenderWindow& window)
	{
		// Additional menu render additions.
		drawOptions(options,true,window);

    	// The poll loop.
    	sf::Event event;
		while(window.isOpen() && !done)
		{
			while(window.pollEvent(event))
        	{
        	   	switch(event.type)
            	{
            	    case sf::Event::KeyPressed:
            	        handleInput(event.key.code,optCount);
            	        break;
            	    case sf::Event::Closed:
            	        willExit = true;
            	        done = true;
            	        break;
            	}
			if(!done)
				drawOptions(options,true,window);
        	}
        }
	}
	/* {-FOLD} */
	
	/**
	 * Method that handles player's input in the menu screen.
	 * Parameters: #Key code of user's input.
	 *			   #Integer variable containing the number of choices. 
	 */
	/* {+FOLD} MENU::HANDLEINPUT(SF::KEYBOARD::KEY,INT) */
	void Menu::handleInput(sf::Keyboard::Key key, int optCount)
	{
		position = check.getPosition();
		switch(key)
		{
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				if(optCount == 0)
					break;
				choice = (choice - 1 + optCount) % optCount;
				position.y = (height / 3) + choice*40 + 10;
				check.setPosition(position); // Move the check sprite.
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				if(optCount == 0)
					break;
				choice = (choice + 1) % optCount;
				position.y = (height / 3) + choice*40 + 10;
				check.setPosition(position); // Move the check sprite.
				break;
			case sf::Keyboard::Return:
				done = true;
				break;
			case sf::Keyboard::Home: // Emergency :)
				willExit = true;
				done = true;
				break;
		}
	}
	/* {-FOLD} */
	
	/**
	 * Clears the screen, draws all menu options (plus the check icon)
	 * and displays the render window.
	 * Parameters: #Vector of option text.
	 *             #Boolean variable determining whether to draw check icon.
	 *             #The game window passes by reference for rendering purposes.
	 */
	/* {+FOLD} MENU::DRAWOPTIONS(STD::VECTOR<SF::TEXT*>,BOOL,SF::RENDERWINDOW&) */
	void Menu::drawOptions(std::vector<sf::Text*> options, bool drawCheck, sf::RenderWindow& window)
	{
		window.clear(sf::Color::Black);
		if(drawCheck)
			window.draw(check);
		for(int i = 0; i < options.size(); i++)
			window.draw(*options[i]);
		window.display();
	}
	/* {-FOLD} */
	
	/**
	 * A brief ingame help sub menu, showing movement/shoot keys
	 * and the goal of the game.
	 * Parameters: #The game window passed by reference for
	 *				rendering purposes.
	 */
	/* {+FOLD} MENU::SUBMENUHELP(SF::RENDERWINDOW&) */
	void Menu::subMenuHelp(sf::RenderWindow& window)
	{
		bool tDone = false;
		int lines = 6; // For easy line count changes.
		helpText.reserve(lines);
		helpText.resize(lines);
		
		/* Creating the help text START */
		helpText[0] = new sf::Text("Help:",font,20);
		helpText[1] = new sf::Text("Movement keys: W (up), S (down), A (left) and D (right)",font,20);
		helpText[2] = new sf::Text("Shooting keys: I (up), K (down), J (left) and L (right)",font,20);
		helpText[3] = new sf::Text("Menu: Escape, Pause: Space",font,20);
		helpText[4] = new sf::Text("Goal of the game: Enter all three portals in the main room",font,20);
		helpText[5] = new sf::Text("                  and finnish them by killing the boss.",font,20);
		/* Creating the help text END */

		for(int i = 0; i < lines; i++)
		{
			position.x = width / 3 - 80;
    		position.y = height / 3 + i*20;
    		helpText[i]->setPosition(position);
		}

		drawOptions(helpText,false,window);
		
		// Polling the player.
		sf::Event tEvent;
		while(!tDone)
		{
			while(window.pollEvent(tEvent))
			{
				switch(tEvent.type)
				{
            	    case sf::Event::KeyPressed:
						if(tEvent.key.code == sf::Keyboard::Return || tEvent.key.code == sf::Keyboard::Escape)
							tDone = true;
            	        break;
            	    case sf::Event::Closed:
            	        willExit = true;
            	        tDone = true;
            	        break;				
				}
			}
		}
		for(int i = 0; i < lines; i++) // Cleanup.
			delete helpText[i];
	}
	/* {-FOLD} */
		
	/**
	 * Menu class destructors, deletes all dynamically
	 * allocated variables (that weren't already deleted).
	 * Parameters: NONE
	 */
	/* {+FOLD} MENU::~MENU() */
	Menu::~Menu()
	{
		for(int i = 0; i < options.size(); i++)
			delete options[i];
	}
	/* {-FOLD} */
