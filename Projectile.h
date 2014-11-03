#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <iostream>

/* {+FOLD} CLASS PROJECTILE */
class Projectile
{
	/* {+FOLD} PRIVATE */
	private:
		/* VARIABLES: */
		int MAXlifetime;
		int lifetime;
		int DMG;
		bool willGetDeleted;
		sf::Texture texture;
		sf::Vector2f direction;
		sf::Sprite sprite;
	/* {-FOLD} */

	/* {+FOLD} PUBLIC */
	public:
		Projectile(int,int,std::string,sf::Vector2f,sf::Vector2f);
		void move();
		void requestDeletion();
		void bounce();
		bool getsDeleted();
		sf::Vector2f getPosition();
		int getDMG();
		sf::Sprite* getSprite();
	/* {-FOLD} */
};
/* {-FOLD} */
#endif
