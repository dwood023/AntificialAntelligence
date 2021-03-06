#include <SFML/Graphics.hpp>
#include "Ant.h"
#include "AssetLoader.h"
#include "MathLib.h"
#include <iostream>

Ant::Ant()
: sprite{}
, idleAnimation{}
, walkAnimation{}
, pathNetMoveComp{}
, brain{}
{
}

Ant::Ant(sf::Vector2f newPosition)
	:idleAnimation(AssetLoader::getAntIdleAnimation())
	,walkAnimation(AssetLoader::getAntWalkAnimation())
{
    sprite = AssetLoader::getSpriteAnt();
    setPosition(newPosition);
    pathNetMoveComp.setPosition(getPosition());
}

void Ant::update(float deltaTime){
    brain.think(deltaTime, *this);
    setPosition(pathNetMoveComp.getPosition());

	if (isMoving()) {

		walkAnimation.update(sprite, deltaTime);

		int angle = pathNetMoveComp.calcRotation();
		int yScale = sprite.getScale().y;

		// If the ant is upside down and isn't already flipped, flip it on Y
		if ((abs(angle) > 90 && yScale > 0) || (abs(angle) < 90 && yScale < 0)) 
			sprite.setScale(sprite.getScale().x, -yScale);

		sprite.setRotation(angle);


	}
	else {
		idleAnimation.update(sprite, deltaTime);
	}
}


void Ant::draw(sf::RenderWindow &window) {
     
    window.draw(sprite);

}

void Ant::setPosition(sf::Vector2f newPosition) {
    sprite.setPosition(newPosition);
}


void Ant::move(sf::Vector2f offset){
    setPosition(getPosition() + offset);
}


sf::Vector2f Ant::getPosition() {
     return sprite.getPosition();
}


void Ant::joinPathNetwork(PathNetwork * newNetwork, int nodeID){
    pathNetMoveComp.joinNetwork(newNetwork, nodeID);
}

bool Ant::isMoving() {
	 return !pathNetMoveComp.isAtTarget();
}
