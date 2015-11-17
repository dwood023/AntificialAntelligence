#include <SFML/Graphics.hpp>
#include <string>
#include "Ant.h"
#include "TileMap.h"
#include "AssetLoader.h"
#include "Camera.h"
#include <unistd.h>
#include <stdio.h>


int main() {
	sf::RenderWindow gameWindow(sf::VideoMode(1024,1024), "Antificial Antelligence - [PRE-ALPHA]");
	
	sf::Time deltaTime = sf::seconds(1.0f/60.0f);

	sf::Clock clock;

	AssetLoader::loadTextures(); 



	sf::View view;

	view.reset(sf::FloatRect(0,0,200,200));
	view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	gameWindow.setView(view);

    
    //Set up tilemap
    std::vector<TileData> tileDataArray{
        TileData(AssetLoader::getSpriteEarth()),
        TileData(AssetLoader::getSpriteEarthHorizontalTunnel())
    };
    
    tileDataArray[1].localPathNetwork.createNewNode(sf::Vector2f(0, 154));
    tileDataArray[1].localPathNetwork.createNewNodeConnectedTo(sf::Vector2f(255, 154), 0);
    
    TileMap tileMap = TileMap();
    tileMap.setMapSize(200,200);
    tileMap.setTileSize(256);
	tileMap.setMapPosition(0,0);
    tileMap.setTileData(tileDataArray);
    //Finish Tilemap setup
    
    
	Ant ant = Ant(sf::Vector2f(100, 100));
    
    sf::Sprite testSprite;
    sf::Texture testTex;
    testTex.loadFromFile("madAnt.png");
    testSprite.setTexture(testTex);
    
    testSprite.setPosition(400, 300);
    
	while (gameWindow.isOpen()) {
		 
		sf::Event event;
		
		while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gameWindow.close();
            else if(event.type == sf::Event::Resized)
                view.setSize(gameWindow.getSize().x, gameWindow.getSize().y);
        }
		
		gameWindow.clear(sf::Color(225,225,225,225));

		tileMap.draw(gameWindow, view );
		ant.draw(gameWindow);
        
        gameWindow.draw(testSprite);

        updateCamera(deltaTime.asSeconds(), view, gameWindow);
        gameWindow.setView(view);
		gameWindow.display();
		deltaTime = clock.restart();
	}

	return 0;

}
