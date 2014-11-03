#include "Projectile.h"

	/**
	 * Projectile class constructor, sets all of the variables and
	 * loads and sets the texture for the sprite. Also sets the position
	 * of the sprite in the game window.
	 * Parameters: #Integer value of maximal lifetime.
	 *			   #Integer value of the shot's damage.
	 *			   #String variable with the texture's
	 *			    address.
	 *			   #sf::Vector2f of the direction of
	 *			    the shot.
	 *			   #sf::Vector2f of the shot's position.
	 */
	/* {+FOLD} PROJECTILE::PROJECTILE(INT,INT,STD::STRING,SF::VECTOR2F,SF::VECTOR2F) */
	Projectile::Projectile(int lft, int dmg, std::string add, sf::Vector2f dir, sf::Vector2f pos)
	{
		MAXlifetime = lft;
		lifetime = 0;
		DMG = dmg;
		direction = dir;
		willGetDeleted = false;

		texture.loadFromFile(add);
		sprite.setTexture(texture);
		sprite.setPosition(pos);
	}
	/* {-FOLD} */

	/**
	 * A method that moves the projectile based on
	 * the direction vector that has been passed to
	 * it upon constructor call.
	 * Parameters: NONE
	 */
	/* {+FOLD} PROJECTILE::MOVE() */
	void Projectile::move()
	{
		sprite.setPosition(sprite.getPosition() + direction);
		lifetime++; // Used for monitoring of the shot's life span.
	}
	/* {-FOLD} */

	/**
	 * A boolean method that tells the caller
	 * if the shot is supposed to be deleted.
	 * Parameters: NONE
	 */
	/* {+FOLD} PROJECTILE::GETSDELETED() */
	bool Projectile::getsDeleted()
	{
		if(lifetime >= MAXlifetime || willGetDeleted)
			return true; // willGetDeleted for collision etc.
		else
			return false;
	}
	/* {-FOLD} */

	/**
	 * A method that bounces the shot from
	 * a wall.
	 * Parameters: NONE
	 */
	/* {+FOLD} PROJECTILE::BOUNCE() */
	void Projectile::bounce()
	{
		direction.x *= -1;
		direction.y *= -1;
	}
	/* {-FOLD} */

	/**
	 * A simple method that forces next call
	 * of the next getsDeleted() method to
	 * return true.
	 * Parameters: NONE
	 */
	/* {+FOLD} PROJECTILE::REQUESTDELETION() */
	void Projectile::requestDeletion()
	{
		willGetDeleted = true;
	}
	/* {-FOLD} */

	/**
	 * A method that returns the projectile's
	 * sprite's position.
	 * Parameters: NONE
	 */
	/* {+FOLD} PROJECTILE::GETPOSITION() */
	sf::Vector2f Projectile::getPosition()
	{
		return sprite.getPosition();
	}
	/* {-FOLD} */

	/**
	 * A simple method that returns the
	 * projectile's damage.
	 * Parameters: NONE
	 */
	/* {+FOLD} PROJECTILE::GETDMG() */
	int Projectile::getDMG()
	{
		return DMG;
	}
	/* {-FOLD} */

	/**
	 * A simple method that returns a pointer
	 * to the projectile's sprite for
	 * drawing purposes.
	 */
	/* {+FOLD} PROJECTILE::GETSPRITE() */
	sf::Sprite* Projectile::getSprite()
	{
		return &sprite;
	}
	/* {-FOLD} */
