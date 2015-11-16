//
//  PathNode.cpp
//  AntificialAntelligence
//
//  Created by Jonathan Wood on 14/11/2015.
//
//

#include "PathNode.h"
#include <iostream>

//Node pointer method
//
//PathNode::PathNode(float posX, float posY){
//    position.x = posX;
//    position.y = posY;
//}
//
//PathNode::PathNode(sf::Vector2f pos){
//    position = pos;
//}
//
//
//const std::vector<PathNode*> & PathNode::getConnectedNodes(){
//    return connectedNodes;
//}



//Network ID method

int PathNode::getNetID(){
    return netID;
}

sf::Vector2f PathNode::getPosition() const{
    return position;
}


PathNode::PathNode(float posX, float posY){
    position.x = posX;
    position.y = posY;
}

PathNode::PathNode(sf::Vector2f pos){
    position = pos;
}

void PathNode::setNetID(int newID){
    netID = newID;
    std::cout<<"set node's net ID to: "<<netID<<std::endl;
}

void PathNode::move(float x, float y){
    position.x += x;
    position.y += y;
}