#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "Projectile.h"

#ifndef PLAYER_H
#define PLAYER_H
/**
 * Player class, used to create an instance of the player's
 * character.
 */
 /* {+FOLD} CLASS PLAYER */
class Player
{
	/* {+FOLD} PRIVATE */
	private:
		/* Methods: */
		int getDMG();

		/* Variables: */
		int HP;
		int MaxHP;
		int minDMG, maxDMG;
		sf::Texture plrFront, plrBack, plrLeft, plrRight;
		sf::Clock clockCD;
		sf::Time currTime;
		sf::SoundBuffer shotBuffer;
		sf::Sound shotSound;
		sf::Sprite sprite;
	/* {-FOLD}  PRIVATE */
	
 	/* {+FOLD} PUBLIC */ 
	public:
		Player(unsigned int, unsigned int);
		void move(sf::Vector2f);
		void changeSprite(int);
		void restoreHP();
		void setPosition(sf::Vector2f);
		sf::Vector2f getPosition();
		Projectile* shoot(sf::Keyboard::Key);
		bool inflictDMG(int);
		bool coolDown();
		float getHP();
		sf::Sprite* getSprite();
	/* {-FOLD} */
};
/* {-FOLD} */ 
#endif

#ifndef ENEMY_H
#define ENEMY_H
/**
 * Enemy character class, used to create instances of both
 * blobs and bosses (with different AI numbers).
 */
/* {+FOLD} CLASS ENEMY */
class Enemy
{
	/* {+FOLD} PRIVATE */
	private:
		/* Methods: */
		float getDistance(sf::Vector2f);
		bool isCharging();
		int getPlrSector(sf::Vector2f);
		sf::Vector2f normalize(sf::Vector2f);

		/* Variables: */
		int HP,minDMG,maxDMG,AI;
		unsigned int w,h;
		bool canDoDMG, isInStun, isInCharge, willGetDeleted;
		sf::Texture texture, textureCharge;
		sf::Clock clock;
		sf::Time cdTime, chTime, stTime, movTime;
		sf::Vector2f chargeDirection, movDirection, plrPosition;
		sf::Vector2f P1,P2,P3,P4; // Rail corners.
		sf::Sprite sprite;
	/* {-FOLD} */

	/* {+FOLD} PUBLIC */
	public:
		Enemy(int);
		void move(sf::Vector2f);
		void requestDeletion();
		void stun();
		void setPosition(sf::Vector2f);
		sf::Vector2f getPosition();
		sf::Vector2f getDirection(sf::Vector2f);
		Projectile* shoot(sf::Vector2f);
		sf::Sprite* getSprite();
		bool inflictDMG(int);
		bool getsDeleted();
		bool coolDown();
		bool isStunned();
		int getDMG();
	/* {-FOLD} */
};
/* {-FOLD} */
#endif
