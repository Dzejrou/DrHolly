#include "Character.h"

	/*******************************************/
	/* DEFINITION FOR THE PLAYER CLASS         */
	/*******************************************/

	/**
	 * Player class constructor, loads all media
	 * files and sets all necessary variables and
	 * objects.
	 * Parameters: 
	 */
	/* {+FOLD} PLAYER::PLAYER() */
	Player::Player(unsigned int w = 640, unsigned int h = 480)
	{
		/* Texture initialisation START */
		plrFront.loadFromFile("Media/Textures/playerFront.png");
		plrBack.loadFromFile("Media/Textures/playerBack.png");
		plrLeft.loadFromFile("Media/Textures/playerLeft.png");
		plrRight.loadFromFile("Media/Textures/playerRight.png");
		/* Texture initialisation END */

		sprite.setTexture(plrFront);
		float width = static_cast<float>(w);
		float height = static_cast<float>(h);
		sprite.setPosition(width,height);

		MaxHP = 100;
		HP = 100;
		minDMG = 10;
		maxDMG = 20;
		
		/* Cooldown clock initialisation: */
		currTime = sf::Time::Zero;
		clockCD.restart();
		
		/* Shot sound initialisation */
		shotBuffer.loadFromFile("Media/Sound/plrShot.wav");
		shotSound.setBuffer(shotBuffer);
	}
	/* {-FOLD} */

	/**
	 * Method that increases player's position by
	 * the movement vector, which it recieves from
	 * the Game object as a parameter.
	 * Parameters: #sf::Vector2f movement vector.
	 */
	/* {+FOLD} PLAYER::MOVE() */
	void Player::move(sf::Vector2f movement)
	{
		sprite.setPosition(sprite.getPosition() + movement);
	}
	/* {-FOLD} */

	/**
	 * A method that checks if it is possible to shoot
	 * a projectile and if it is, calculates the direction
	 * vector and creates the projectile, pointer to which
	 * it then returns.
	 * Parameters: #sf::Keyboard::Key code of the key
	 *				pressed.
	 */
	/* {+FOLD} PLAYER::SHOOT(SF::KEYBOARD::KEY) */
	Projectile* Player::shoot(sf::Keyboard::Key key)
	{
		/* {+FOLD} DIRECTION SETTING */
		sf::Vector2f direction(0.f,0.f);
		switch(key)
		{
			case sf::Keyboard::I:
				direction.y -= 10.f;
				break;
			case sf::Keyboard::K:
				direction.y += 10.f;
				break;
			case sf::Keyboard::J:
				direction.x -= 10.f;
				break;
			case sf::Keyboard::L:
				direction.x += 10.f;
				break;
		}
		/* {-FOLD} */

		/**
		 * Compensating the fact that the shot spawns to
		 * the top-left of the player.
		 */
		sf::Vector2f compensation(15.f,20.f);
		sf::Vector2f vfTemp = sprite.getPosition() + compensation;
		Projectile* tempPtr = new Projectile(50,getDMG(),"Media/Textures/plrShot.png",direction,vfTemp);

		shotSound.play();
		/* This shot will be added to CharProjList */
		return tempPtr;
	}
	/* {-FOLD} */

	/**
	 * A method that changes the player's sprite's
	 * texture according to his current movement,
	 * given to it by the orientation parameter.
	 * Parameters: #Int variable of the orientation.
	 */
	/* {+FOLD} PLAYER::CHANGESPRITE(INT) */
	void Player::changeSprite(int orientation)
	{
		switch(orientation)
		{
			case 0:
				// Do nothing, this is the default value.
				break;
			case 1:
				sprite.setTexture(plrBack);
				break;
			case 2:
				sprite.setTexture(plrLeft);
				break;
			case 3:
				sprite.setTexture(plrRight);
				break;
			case 4:
				sprite.setTexture(plrFront);
				break;
		}
	}
	/* {-FOLD} */

	/**
	 * A boolean method that returns true if the
	 * player cannot shoot yet and false if he can.
	 * Parameters: NONE
	 */
	/* {+FOLD} PLAYER::COOLDOWN() */
	bool Player::coolDown()
	{
		currTime = clockCD.getElapsedTime();
		float cooldown = sf::seconds(0.35f).asSeconds();

		if(currTime.asSeconds() > cooldown)
		{
			clockCD.restart();
			return false; // Not on cooldown.
		}
		else
			return true; // On cooldown.
	}
	/* {-FOLD} */

	/**
	 * A method handling the damage infliction
	 * and setting the status to 'dead' if
	 * needed. (By returning true.)
	 * Parameters: #Integer value of damage
	 *				inflicted.
	 */
	/* {+FOLD} PLAYER::INFLICTDMG(INT)*/
	bool Player::inflictDMG(int value)
	{
		HP -= value;
		if(HP <= 0)
			return true; // RIP
		else
			return false; // YAY!
	}
	/* {-FOLD} */

	/**
	 * A simple method that returns the player's
	 * sprite's position in the form of SFML's
	 * float 2-dimensional vector.
	 * Parameters: NONE
	 */
	/* {+FOLD} PLAYER::GETPOSITION() */
	sf::Vector2f Player::getPosition()
	{
		return sprite.getPosition();
	}
	/* {-FOLD} */

	/**
	 * A method that sets player's position
	 * to a given value.
	 * Parameters: #sf::Vector2f of the target
	 *				position.
	 */
	/* {+FOLD} PLAYER::SETPOSITION(SF::VECTOR2F) */
	void Player::setPosition(sf::Vector2f position)
	{
		sprite.setPosition(position);
	}
	/* {-FOLD} */

	/**
	 * A method that returns a pointer
	 * to the player's sprite.
	 * (Used for drawing.)
	 * Parameters: NONE
	 */
	/* {+FOLD} PLAYER::GETSPRITE() */
	sf::Sprite* Player::getSprite()
	{
		return &sprite;
	}
	/* {-FOLD} */

	/**
	 * A method used for damage calculation,
	 * returns a pseudo-random value between
	 * minDMG and maxDMG data members.
	 * Parameters: NONE
	 */
	/* {+FOLD} PLAYER::GETDMG() */
	int Player::getDMG()
	{
		srand(time(0));
		return rand() % maxDMG + minDMG;
	}
	/* {-FOLD} */

	/**
	 * A method that returns the percentage of
	 * player's remaining health.
	 * (For healthbar updates.)
	 * Parameters: NONE
	 */
	/* {+FOLD} PLAYER::GETHP() */
	float Player::getHP()
	{
		if(HP <= 0)
			return 0.f;
		return static_cast<float>(HP) / (static_cast<float>(MaxHP) / 100.f );
	}
	/* {-FOLD} */

	/**
	 * A method that restores player's
	 * health to it's max value.
	 * Parameters: NONE
	 */
	/* {+FOLD} PLAYER::RESTOREHP() */
	void Player::restoreHP()
	{
		HP = MaxHP;
	}
	/* {-FOLD} */

	/*******************************************/
	/* DEFINITION FOR THE ENEMY CLASS           */
	/*******************************************/

	/**
	 * Enemy class constructor, decided which values
	 * to assign to it's attributes based on the
	 * AI parameter it recieves and loads the enemy's
	 * texture.
	 * Parameters: #Integer number of the AI.
	 *				(AI is based on level)
	 */
	/* {+FOLD} ENEMY::ENEMY() */
	Enemy::Enemy(int aiValue)
	{
		switch(aiValue)
		{
			case 1:
				HP = 250;
				minDMG = 20;
				maxDMG = 40;
				texture.loadFromFile("Media/Textures/blob.png");
				break;
			case 2:
				HP = 200;
				minDMG = 10;
				maxDMG = 30;
				texture.loadFromFile("Media/Textures/blob.png");
				break;
			case 3:
				HP = 100;
				minDMG = 3;
				maxDMG = 8;
				texture.loadFromFile("Media/Textures/blob.png");
				break;
			case 4:
				// Dr Forstix.
				HP = 300;
				minDMG = 30;
				maxDMG = 50;
				texture.loadFromFile("Media/Textures/dr-forstix.png");
				break;
			case 5:
				// Doc Topferino.
				HP = 400;
				minDMG = 5;
				maxDMG = 20;
				texture.loadFromFile("Media/Textures/doc-topferino.png");
				break;
			case 6:
				// Dr Holly.
				HP = 500;
				minDMG = 10;
				maxDMG = 20;
				movDirection.x = 2.f;
				movDirection.y = 0.f;
				texture.loadFromFile("Media/Textures/dr-holly.png");
				break;
		}
		textureCharge.loadFromFile("Media/Textures/blob-charge.png");
		sprite.setTexture(texture);

		AI = aiValue;
		clock.restart();
		willGetDeleted = false;
		isInStun = false;
		isInCharge = false;

		stTime = chTime = cdTime = movTime = sf::Time::Zero;

		/* Last player position for AI #3. */
		plrPosition.x = 0;
		plrPosition.y = 0;

		/* Getting desktop properties. */
		sf::VideoMode currVid;
		w = currVid.getDesktopMode().width;
		h = currVid.getDesktopMode().height;

		/* Setting rail points. */
		if(AI == 6)
		{
			// A line at the top.
			P1.x = w / 6;
			P1.y = h / 10;
			P2.x = 5 * w / 6;
			P2.y = h / 10;
		}
		else
		{
			P1.x = w / 4;
			P1.y = h / 4;
			P2.x = 3 * w / 4;
			P2.y = h / 4;
		}
		P3.x = 3 * w / 4;
		P3.y = 3 * h / 4;
		P4.x = w / 4;
		P4.y = 3 * h / 4;


		canDoDMG = false; // No initial dmg.
	}
	/* {-FOLD} */

	/**
	 * Method that moves the enemy's sprite based
	 * on the movement vector it recieves as a 
	 * parameter from the Game object.
	 * Parameters: #sf::Vector2f movement vector.
	 */
	/* {+FOLD} ENEMY::MOVE(SF::VECTOR2F) */
	void Enemy::move(sf::Vector2f movement)
	{
		if(isStunned())
		{
			return; // You aren't running anywhere.
		}
		else if(isCharging())
		{
			sprite.setPosition(sprite.getPosition() + chargeDirection); // CHAAAAAARGE!!!!
			return;
		}

		/* Let's move */
		sf::Time currTime = clock.getElapsedTime();
		switch(AI)
		{
			case 1:
			case 2:
				if(currTime.asSeconds() - movTime.asSeconds() < 1.5f)
					sprite.setPosition(sprite.getPosition() + movDirection); // Cooldown for changing direction.
				movTime = currTime; // Setting last move time.
				movDirection = movement;
				// Intentional fall!
			case 3:
			case 4:
			case 6:
				sprite.setPosition(sprite.getPosition() + movement);
				break;
			case 5:
				if(currTime.asSeconds() - movTime.asSeconds() < 4.f)
					return;
				movTime = currTime; // Activate the teleport cooldown.
				sf::Vector2f vfTemp = sprite.getPosition();
				/* Teleportation! */
				if(vfTemp == P1)
					sprite.setPosition(P2);
				else if(vfTemp == P2)
					sprite.setPosition(P3);
				else if(vfTemp == P3)
					sprite.setPosition(P4);
				else if(vfTemp == P4)
					sprite.setPosition(P1);
				break;
		}
	}
	/* {-FOLD} */

	/**
	 * A method handling the damage infliction
	 * and setting the status to 'dead' if
	 * needed. (By returning true.)
	 * Parameters: #Integer value of damage
	 *				inflicted.
	 */
	/* {+FOLD} ENEMY::INFLICTDMG(INT) */
	bool Enemy::inflictDMG(int value)
	{
		if(isStunned()) // Immune to damage whilst stunned. Let's be fair.
			return false;
		HP -= value;
		if(HP <= 0)
			return true; // RIP
		else
			return false; // DAMN!
	}
	/* {-FOLD} */

	/**
	 * A method setting the enemy's sprite's
	 * position.
	 * Parameters: sf::Vector2f position vector.
	 */
	/* {+FOLD} ENEMY::SETPOSITION(SF::VECTOR2F) */
	void Enemy::setPosition(sf::Vector2f position)
	{
		sprite.setPosition(position);
	}
	/* {-FOLD} */

	/**
	 * A methot returning the enemy's sprite's
	 * position.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::GETPOSITION() */
	sf::Vector2f Enemy::getPosition()
	{
		return sprite.getPosition();
	}
	/* {-FOLD} */

	/**
	 * A method that, based on the relation
	 * of the enemy's and player's positions,
	 * calculates the direction vector (unitary).
	 * Parameters: #sf::Vector2f with player's
	 *				position.
	 */
	/* {+FOLD} ENEMY::GETDIRECTION(SF::VECTOR2F) */
	sf::Vector2f Enemy::getDirection(sf::Vector2f plrPosition)
	{
		/* In case the mob runs out of bounds whilst charging. */
		if(isInCharge)
			return chargeDirection;
		/* Vector from Enemy to the Player */
		sf::Vector2f vfTemp = plrPosition - sprite.getPosition();
		vfTemp = normalize(vfTemp);

		switch(AI)
		{
			case 1:
			case 4:
				/* Rush towards the player. */
				return vfTemp;
			case 2:
				/* Stay stationary and fire. */
				vfTemp.x = 0.f;
				vfTemp.y = 0.f;
				return vfTemp;
			/* {+FOLD} CASE 3 */
			case 3:
				{
					float x = sprite.getPosition().x;
					float y = sprite.getPosition().y;
					if(y == P1.y && x != P2.x)
					{
						// Line #1.
						vfTemp.x = 1.f;
						vfTemp.y = 0.f;
					}
					else if(x == P2.x && y != P3.y)
					{
						// Line #2.
						vfTemp.x = 0.f;
						vfTemp.y = 1.f;
					}
					else if(y == P3.y && x != P4.x)
					{
						// Line #3.
						vfTemp.x = -1.f;
						vfTemp.y = 0.f;
					}
					else if(x == P4.x)
					{
						// Line #4.
						vfTemp.x = 0.f;
						vfTemp.y = -1.f;
					}
					vfTemp *= 4.f;
					float futureX = x + vfTemp.x;
					float futureY = y + vfTemp.y;
					if(futureX > P2.x)
						return P2 - sprite.getPosition(); // Don't pass the corner, just go to it.
					if(futureX < P4.x)
						return P4 - sprite.getPosition();
					if(futureY < P1.y)
						return P1 - sprite.getPosition();
					if(futureY > P3.y)
						return P3 - sprite.getPosition();
					return vfTemp;
				}
			/* {-FOLD} */
			case 5:
				return vfTemp; // Just to return something, doesn't affect movement.
			case 6:
				{
					float x = sprite.getPosition().x;
					if(x == P1.x || x == P2.x)
					{
						movDirection.x *= -1.f; // Invert direction upon reaching the end point.
						return movDirection;
					}
					float futureX = x + movDirection.x;
					if(futureX < P1.x) // Don't pass the end points.
						return P1 - sprite.getPosition();
					if(futureX > P2.x)
						return P2 - sprite.getPosition();
					return movDirection;
				}
			default:
				return vfTemp; // Bad parameter, just rush...
		}
	}
	/* {-FOLD} */

	/**
	 * A method that is in charge of the enemy's
	 * attacking activities, based on the AI choses
	 * the right type of attack and executes it if
	 * the cooldown is off.
	 * Parameters: #sf::Vector2f of the target's
	 *				(read player's) position
	 */
	/* {+FOLD} ENEMY::SHOOT(SF::VECTOR2F) */
	Projectile* Enemy::shoot(sf::Vector2f position)
	{
		if(coolDown() || isInCharge || isInStun) // Not firing at anything.
		{
			plrPosition = position; // Keeping track of player's movements.
			return NULL;
		}
		sf::Vector2f vfTemp0(10.f,10.f); // Compensation
		sf::Vector2f vfTemp1 = position - sprite.getPosition(); // Direction vector.
		sf::Vector2f vfTemp2 = sprite.getPosition() + vfTemp0; // Position of the shot's spawnpoint.

		cdTime = clock.getElapsedTime();
		switch(AI)
		{
			/* {+FOLD} CASE 1 */
			case 1:
				vfTemp1 = normalize(vfTemp1);
				isInCharge = true;
				canDoDMG = true;
				chTime = clock.getElapsedTime();
				sprite.setTexture(textureCharge);
				chargeDirection = vfTemp1 * 2.f;
				return NULL; // Don't add anything to the ProjList.
			/* {-FOLD} */

			/* {+FOLD} CASE 2 + 5 */
			case 5:
			case 2:
			{
				int dir = rand() % 5; // 0..4 - five possible directions.
				sf::Vector2f vfTemp = position - sprite.getPosition();
				sf::Vector2f vects[5];
				switch(getPlrSector(position))
				{
					case 0:
						// Same axis.
						vfTemp /= std::max(vfTemp.x,vfTemp.y); // One of them is 0, other (the max)  is not.
						vects[1].x = vects[3].x = vfTemp.x;
						vects[1].y = vects[3].y = vfTemp.y;
						vects[0].y = vects[2].y = vfTemp.x;
						vects[0].x = vects[0].x = vfTemp.y;
						dir = rand() % 4; // 0..3 for this.
						break;
					case 1:
						vects[0].x = 0.f;
						vects[0].y = -1.f;
						vects[1].x = 1.f;
						vects[1].y = -2.f;
						vects[2].x = 1.f;
						vects[2].y = -1.f;
						vects[3].x = 2.f;
						vects[3].y = -1.f;
						vects[4].x = 1.f;
						vects[4].y = 0.f;
						break;
					case 2:
						vects[0].x = -1.f;
						vects[0].y = 0.f;
						vects[1].x = -2.f;
						vects[1].y = -1.f;
						vects[2].x = -1.f;
						vects[2].y = -1.f;
						vects[3].x = -1.f;
						vects[3].y = -2.f;
						vects[4].x = 0.f;
						vects[4].y = -1.f;
						break;
					case 3:
						vects[0].x = 0.f;
						vects[0].y = 1.f;
						vects[1].x = -1.f;
						vects[1].y = 2.f;
						vects[2].x = -1.f;
						vects[2].y = 1.f;
						vects[3].x = -2.f;
						vects[3].y = 1.f;
						vects[4].x = -1.f;
						vects[4].y = 0.f;
						break;
					case 4:
						vects[0].x = 1.f;
						vects[0].y = 0.f;
						vects[1].x = 2.f;
						vects[1].y = 1.f;
						vects[2].x = 1.f;
						vects[2].y = 1.f;
						vects[3].x = 1.f;
						vects[3].y = 2.f;
						vects[4].x = 0.f;
						vects[4].y = 1.f;
						break;
				}
				return new Projectile(800,getDMG(),"Media/Textures/blobShot.png",vects[dir],vfTemp2);
			}
		/* {-FOLD} */ 

		/* {+FOLD} CASE 3 */ 
		case 3:
			{
			sf::Vector2f direction = normalize(vfTemp1) * 5.f;
			return new Projectile(100,getDMG(),"Media/Textures/blobShot.png",direction,vfTemp2);
			}
		/* {-FOLD} */

		/* {+FOLD} CASE 4 */
		case 4:
			vfTemp1 = normalize(vfTemp1);
			isInCharge = true;
			canDoDMG = true;
			chTime = clock.getElapsedTime();
			chargeDirection = vfTemp1 * 4.f;
			return NULL;
		/* {-FOLD} */

		/* {+FOLD} CASE 6 */
		case 6:
			vfTemp1.x = 0.f;
			vfTemp1.y = 1.f;
			srand(time(0));
			float modifier = rand() % 5 + 2; // 1..3 * speed of the projectile.
			vfTemp1 *= modifier;
			return new Projectile(120,getDMG(),"Media/Textures/bossShot.png",vfTemp1,vfTemp2);
		/* {-FOLD} */
		}
	}
	/* {-FOLD} */

	/**
	 * A simple method that returns a pointer
	 * to the enemy's sprite.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::GETSPRITE() */
	sf::Sprite* Enemy::getSprite()
	{
		return &sprite;
	}
	/* {-FOLD} */

	/**
	 * Returns true if the enemy is
	 * supposed to die upon next
	 * Game::update call.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::GETSDELETED() */
	bool Enemy::getsDeleted()
	{
		return willGetDeleted;
	}
	/* {-FOLD} */

	/**
	 * A simple method that sets the
	 * enemy to be deleted upon next
	 * Game::update call.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::REQUESTDELETION() */
	void Enemy::requestDeletion()
	{
		willGetDeleted = true;
	}
	/* {-FOLD} */

	/**
	 * A simple method that stuns the enemy
	 * (e.g. after a contact with the player)
	 * and restarts the stun clock.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::STUN() */
	void Enemy::stun()
	{
		sf::Time tiTemp = clock.getElapsedTime();
		float stunCD = sf::seconds(4.f).asSeconds(); // 1 second stun immunity.
		if(tiTemp.asSeconds() - stTime.asSeconds() < stunCD) // stTime hols time of last stun.
			return; // No stun yet.
		stTime = clock.getElapsedTime();
		isInStun = true;
	}
	/* {-FOLD} */

	/**
	 * A method returning true if it is possible
	 * to fire a shot already (or charge) and
	 * false if the attack is on cooldown.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::COOLDOWN() */
	bool Enemy::coolDown()
	{
		sf::Time currTime = clock.getElapsedTime();
		float cooldown;
		switch(AI)
		{
			case 1:
				cooldown = sf::seconds(3.f).asSeconds();
				break;
			case 2:
				cooldown = sf::seconds(0.35f).asSeconds();
				break;
			case 3:
				cooldown = sf::seconds(0.30f).asSeconds();
				break;
			case 4:
				cooldown = sf::seconds(3.5f).asSeconds(); // 1 second between charges.
				break;
			case 5:
				cooldown = sf::seconds(0.2f).asSeconds();
				break;
			case 6:
				cooldown = sf::seconds(0.5f).asSeconds();
				break;
			default:
				return false; // Massive shootings indicate wrong AI.
		}
		
		if(currTime.asSeconds() - cdTime.asSeconds() > cooldown)
			return false; // Not on cooldown.
		else
			return true; // On cooldown.

	}
	/* {-FOLD} */

	/**
	 * A method that returns true if the enemy
	 * is STILL (automatical false if out of charge)
	 * charging and false, if the charge tim expired.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::ISCHARGING() */
	bool Enemy::isCharging()
	{
		if(!isInCharge)
			return false;
		sf::Time currTime = clock.getElapsedTime();
		float chargeTime;
		if(AI == 1)
			chargeTime = sf::seconds(2.f).asSeconds();
		else
			chargeTime = sf::seconds(2.5f).asSeconds();

		if(currTime.asSeconds() - chTime.asSeconds() > chargeTime)
		{
			sprite.setTexture(texture);
			isInCharge = false;
			canDoDMG = false;
			return false;
		}
		else
			return true;
	}
	/* {-FOLD} */

	/**
	 * A method returning true if the enemy is stunned
	 * and returning false + restarting the clock if
	 * it is not.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::ISSTUNNED() */
	bool Enemy::isStunned()
	{
		if(!isInStun)
			return false;
		sf::Time currTime = clock.getElapsedTime();
		float stunTime = sf::seconds(3.f).asSeconds();
		
		if(currTime.asSeconds() - stTime.asSeconds()> stunTime)
		{
			isInStun = false;
			return false;
		}
		else
			return true;
	}
	/* {-FOLD} */ 

	/**
	 * Returns a damage value for melee
	 * hits.
	 * Parameters: NONE
	 */
	/* {+FOLD} ENEMY::GETDMG() */
	int Enemy::getDMG()
	{
		if(!canDoDMG && (AI == 1 || AI == 4))
			// AI #1 and #4 can't deal damage all the time! 
			return 0;
		canDoDMG = false;
		srand(time(0));
		return rand() % maxDMG + minDMG;
	}
	/* {-FOLD} */

	/**
	 * A method that returns the number of the
	 * sector the player is located in in relation
	 * to the enemy.
	 * Parameters: #Player position vector.
	 */
	/* {+FOLD} ENEMY::GETPLRSECTOR(SF::VECTOR2F) */
	int Enemy::getPlrSector(sf::Vector2f plr)
	{
		/* Player */
		float pX = plr.x;
		float pY = plr.y;
		/* Enemy */
		float bX = sprite.getPosition().x;
		float bY = sprite.getPosition().y;

		if(pX == bX || pY == bY)
			return 0; // Same axis.
		else if(pX > bX)
		{
			if(pY < bY)
				return 1; // First sector.
			else
				return 4; // Fourth sector.
		}else if(pY < bY)
			return 2; // Second sector.
		else
			return 3; // Third sector.
	}
	/* {-FOLD} */

	/**
	 * A method that returns the distance between
	 * the enemy and a given target.
	 * Parameters: #Vector of target's position.
	 */
	/* {+FOLD} ENEMY::GETDISTANCE(SF::VECTOR2F) */
	float Enemy::getDistance(sf::Vector2f pos)
	{
		float x1 = pos.x; // Target.
		float y1 = pos.y;
		float x2 = sprite.getPosition().x; // Enemy.
		float y2 = sprite.getPosition().y;

		float a = std::max(y1,y2) - std::min(y1,y2); // Height line.
		float b = std::max(x1,x2) - std::min(x1,x2); // Width line.
		float c = std::sqrt(pow(a,2) + std::pow(b,2)); // Thank you Pythagoras.
		return c;
	}
	/* {-FOLD} */

	/**
	 * A simple method that returns a normalized
	 * form of a given vector.
	 * Parameters: #Vector that is to be normalized.
	 */
	/* {+FOLD} ENEMY::NORMALIZE(SF::VECTOR2F) */
	sf::Vector2f Enemy::normalize(sf::Vector2f vect)
	{
		float size = sqrt(pow(vect.x,2) + pow(vect.y,2));
		vect.x /= size;
		vect.y /= size;
		return vect;
	}
	/* {-FOLD} */
