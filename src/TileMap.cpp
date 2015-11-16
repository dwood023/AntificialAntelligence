//
//  TileMap.cpp
//  AntificialAntelligence
//
//  Created by Jonathan Wood on 09/11/2015.
//
//

#include "TileMap.h"
#include <iostream>

void TileMap::setMapSize(unsigned int x, unsigned int y){
    //rows
    map.resize(y);
    
    //columns
    for(int i = 0; i < map.size(); ++i){
        map[i].resize(x, 0);
    }
}

sf::Vector2u TileMap::getMapSize() const{
    if(map.size()  && map[0].size()){
        return sf::Vector2u(map[0].size(),map.size());
    }
    return sf::Vector2u(0, 0);
}

//Get the world space position of the tile
sf::Vector2f TileMap::getTilePosition(sf::Vector2u tileMapIndex) const{
    return getMapPosition() + sf::Vector2f(tileMapIndex * tileSize);
}
sf::Vector2f TileMap::getTilePosition(unsigned int x, unsigned int y) const{
    return getTilePosition(sf::Vector2u(x ,y));
}

sf::Vector2f TileMap::getMapPosition() const{
    return position;
}

inline uint8_t TileMap::getTile(unsigned int x, unsigned int y){
    return map[y][x];
}

void TileMap::draw(sf::RenderWindow & window, sf::View & view){
    const sf::FloatRect rect = sf::FloatRect(view.getCenter().x - view.getSize().x/2.0f, view.getCenter().y - view.getSize().y/2.0f, view.getSize().x, view.getSize().y);
    
    sf::Vector2i drawStart = sf::Vector2i(getMapIndexAtPosition(rect.left, rect.top));
    sf::Vector2i drawEnd = sf::Vector2i(getMapIndexAtPosition(rect.left + rect.width, rect.top + rect.height));
    
    if(drawStart.x < 0)
        drawStart.x = 0;
    if(drawStart.y < 0)
        drawStart.y = 0;
    if(drawEnd.x >= getMapSize().x)
        drawEnd.x = getMapSize().x - 1;
    if(drawEnd.y >= getMapSize().y)
        drawEnd.y = getMapSize().y - 1;
    
    //If the view is below the x or y limits of the tilemap, so that no tiles are visible
    else if(drawEnd.x < 0 || drawEnd.y < 0)
        return;
    //If the view is beyond the x or y limits of the tilemap, so that no tiles are visible
    else if(drawStart.x > getMapSize().x  ||  drawStart.y > getMapSize().y)
        return;
    
    
    for(int y = drawStart.y; y <= drawEnd.y; ++y){
        for(int x = drawStart.x; x <= drawEnd.x; ++x){
            tileDataArray[getTile(x, y)].sprite.setPosition(getPositionOfTile(x, y));
            window.draw(tileDataArray[getTile(x, y)].sprite);
        }
    }
}

void TileMap::setTileData(const std::vector<TileData> & newData){
    tileDataArray = newData;
    
}


void TileMap::setTileSize(unsigned int newSize){
    tileSize = newSize;
}

void TileMap::setMapPosition(float x, float y){
    position = sf::Vector2f(x, y);
}


void TileMap::setTile(sf::Vector2u gridPos, uint8_t newValue){
    map[gridPos.y][gridPos.x] = newValue;
}



void TileMap::randomizeMap(){
    for(int y = 0; y < map.size(); ++y){
        for(int x = 0; x < map[y].size(); ++x){
            uint8_t tileIndex;
            if(x % 2 == 0){
                tileIndex = 0;
            }
            else{
                tileIndex = 1;
            }
            map[y][x] = tileIndex;
        }
    }
}


void TileMap::constructPathNetworkInArea(sf::Vector2u start, sf::Vector2u end){
    if(start.x > end.x  ||  start.y > end.y){
        std::cout<<"error in constructPathNetworkInArea: start coordinates greater than end coordinates \n";
        return;
    }
    if(end.x > getMapSize().x  || end.y > getMapSize().y){
        std::cout<<"error in constructPathNetworkInArea: coordinates are outside map \n";
        return;
    }
    
    for(int x = start.x; x <= end.x; ++x){
        for(int y = start.y; y <= end.y; ++y){
            if(tileDataArray[getTile(x, y)].localPathNetwork.numNodes()){
                worldPathNetwork.assimilateNetwork(tileDataArray[getTile(x, y)].localPathNetwork, sf::Vector2f(getTilePosition(x, y)));
            }
        }
    }
}


sf::Vector2u TileMap::getMapIndexAtPosition(float x, float y){
    //Set x and y to map local space
    x -= position.x;
    y -= position.y;
    //convert from pixel units to tile units
    x /= tileSize;
    y /= tileSize;
    
    const unsigned int xIndex = x;
    const unsigned int yIndex = y;
    return sf::Vector2u(xIndex, yIndex);
}

inline sf::Vector2f TileMap::getPositionOfTile(unsigned int x, unsigned int y){
    return sf::Vector2f(position.x + x * tileSize, position.y + y * tileSize);
}
