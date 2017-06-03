//
// Created by haris on 21.5.17.
//

#include "stone.h"

void stone::saveState(ofstream & mapFile)const{
	mapFile << (uint8_t)STONE;
}