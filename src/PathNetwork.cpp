//
//  PathNetwork.cpp
//  AntificialAntelligence
//
//  Created by Jonathan Wood on 14/11/2015.
//
//

#include <iostream>
#include <utility>
#include "PathNetwork.h"
#include "MathLib.h"

//Network ID method


bool PathNetwork::getConnectedNodes(unsigned int nodeID ,std::vector<PathNode*> & connectedNodesOut){
    if(!isIDValid(nodeID)){
        std::cout<<"invalid node ID in PathNetwork::getConnectedNode:"<<nodeID<<std::endl;
        return false;
    }
    
    const PathNode & node = allNodes[nodeID];
    
    for(int i = 0; i < node.connectedNodeNetIDs.size(); ++i){
        connectedNodesOut.push_back(&allNodes[node.connectedNodeNetIDs[i]]);
    }
    
    return true;
}



bool PathNetwork::createNewNode(sf::Vector2f newNodePos){
    allNodes.push_back(PathNode(newNodePos));
    allNodes[allNodes.size() - 1].setNetID(static_cast<int>(allNodes.size() -1));
    return true;
}


bool PathNetwork::createNewNodeConnectedTo(sf::Vector2f newNodePos,  unsigned int nodeIDToConnectTo){
    if(nodeIDToConnectTo > (allNodes.size() - 1)){
        return false;
    }

    allNodes.push_back(PathNode(newNodePos));
    allNodes[allNodes.size() - 1].setNetID(int(allNodes.size() -1));
    
    connectNodes(nodeIDToConnectTo, allNodes[allNodes.size() - 1].getNetID());

    return true;
}


bool PathNetwork::createNewNodeConnectedTo(sf::Vector2f newNodePos, std::initializer_list<unsigned int> nodeIDsToConnectTo){
    for(auto itr = nodeIDsToConnectTo.begin(); itr < nodeIDsToConnectTo.end(); ++itr){
        if(*itr > (allNodes.size() - 1)){
            return false;
        }
    }

    allNodes.push_back(PathNode(newNodePos));
    allNodes[allNodes.size() - 1].setNetID(int(allNodes.size() -1));

    for(auto itr = nodeIDsToConnectTo.begin(); itr < nodeIDsToConnectTo.end(); ++itr){
        connectNodes(*itr, allNodes[allNodes.size() - 1].getNetID());
    }
    
    return true;
}

bool PathNetwork::createNewNodeString(std::initializer_list<sf::Vector2f> newNodes){
    createNewNode(*newNodes.begin());
    for(auto itr = (newNodes.begin() + 1); itr < newNodes.end(); ++itr){
        createNewNodeConnectedTo(*itr, static_cast<int>(allNodes.size() - 1));
    }
    return true;
}

bool PathNetwork::createNewNodeStringConnectedTo(std::initializer_list<sf::Vector2f> newNodes, unsigned int nodeIDToConnectTo){
    if(nodeIDToConnectTo > (allNodes.size() - 1))
        return false;
    
    int firstNodeID= static_cast<int>(allNodes.size());
    createNewNodeString(newNodes);
    
    connectNodes(firstNodeID, nodeIDToConnectTo);
    return true;
}


void PathNetwork::moveNetwork(float x, float y){
    for(int i = 0; i < allNodes.size(); ++i){
        allNodes[i].move(x, y);
    }
}

void PathNetwork::moveNetwork(sf::Vector2f offset){
    for(int i = 0; i < allNodes.size(); ++i){
        allNodes[i].move(offset.x, offset.y);
    }
}

PathNode * PathNetwork::getNode(unsigned int nodeNetID){
    if(nodeNetID > (allNodes.size() - 1)){
        std::cout<<"invalid node ID in PathNetwork::getNode\n";
        return nullptr;
    }
    
    return &allNodes[nodeNetID];
}


unsigned long PathNetwork::numNodes() const{
    return allNodes.size();
}


void PathNetwork::assimilateNetwork(const PathNetwork &otherNetwork, sf::Vector2f offsetToNewNodes){
    const unsigned long IDoffset = allNodes.size();
    
    //copy each node from other network into this one, and update it's netID to it's new position in this->allNodes
    //Apply offsetToNewNodes after each node is added
    for(int i = 0; i < otherNetwork.allNodes.size(); ++i){
        allNodes.push_back(otherNetwork.allNodes[i]);
        PathNode & node = allNodes[allNodes.size() - 1];
        
        //Apply the offset, if there is one
        node.position += offsetToNewNodes;
        
        //Update the assimilated node's ID
        node.netID += IDoffset;
        
        //Update the ID of each connected node ID for node
        for(int j = 0; j < node.connectedNodeNetIDs.size(); ++j){
            node.connectedNodeNetIDs[j] += IDoffset;
        }
    }
    
    //Merge any adjacent nodes
    for(int i = 0; i < allNodes.size(); ++i){
        for(int j = (i + 1); j < allNodes.size(); ++j){
            if(MathLib::length(allNodes[i].getPosition() - allNodes[j].getPosition()) <= nodeMergeDistance){
                mergeNodes(i, j);
            }
        }
    }
}


bool PathNetwork::connectNodes(unsigned int Id1, unsigned int Id2){
    if(!isIDValid(Id1)   ||  !isIDValid(Id2)){
        return false;
    }
    
    allNodes[Id1].connectedNodeNetIDs.push_back(Id2);
    allNodes[Id2].connectedNodeNetIDs.push_back(Id1);
    
    return true;
}


void PathNetwork::mergeNodes(unsigned int Id1, unsigned int Id2){
    if(Id1 == Id2)
        return;
        
    if((!isIDValid(Id1)   ||  !isIDValid(Id2)))
        std::cout<<"error in PathNetwork::mergeNodes: node ID is invalid \n";
    
    //Ensure that Id2 is after Id1
    if(Id1 > Id2){
        std::swap(Id1, Id2);
    }
    
    //add node2's connected nodes to node 1
    for(int i = 0; i < allNodes[Id2].connectedNodeNetIDs.size(); ++i){
        //Don't connect node1 to itself, and don't connect node1 to node2 (as node 2 is about to be deleted)
        if(allNodes[Id2].connectedNodeNetIDs[i] != Id1  &&  allNodes[Id2].connectedNodeNetIDs[i] != Id2){
            bool alreadyConnected = false;
            //check if this node 1 is already connected to this node
            for(int j = 0; j < allNodes[Id1].connectedNodeNetIDs.size(); ++j){
                if(allNodes[Id1].connectedNodeNetIDs[j] == allNodes[Id2].connectedNodeNetIDs[i])
                    alreadyConnected = true;
            }
            
            if(!alreadyConnected)
                allNodes[Id1].connectedNodeNetIDs.push_back(allNodes[Id2].connectedNodeNetIDs[i]);
        }
    }
    
    //erase node 2
    allNodes.erase(allNodes.begin() + Id2);
    //Id2 is now GONE! everything after Id2 was moved back
    //Decrement all netIDs from the old position Id2 (which now contains the element previously after Id2)
    for(int i = Id2; i < allNodes.size(); ++i){
        --allNodes[i].netID;
    }
    
    //Decrement any connectedNodeIDs that refer to anything from (Id2 + 1) or over
    //Set any nodes that refer to Id2 to refer to Id2 instead, as this has replaced it
    for(int i = 0; i < allNodes.size(); ++i){
        for(int n = 0; n < allNodes[i].connectedNodeNetIDs.size(); ++n){
            if(allNodes[i].connectedNodeNetIDs[n] > Id2){
                --allNodes[i].connectedNodeNetIDs[n];
            }
            else if(allNodes[i].connectedNodeNetIDs[n] == Id2){
                allNodes[i].connectedNodeNetIDs[n] = Id1;
            }
        }
    }
}
