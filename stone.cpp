//
// Created by haris on 21.5.17.
//

#include "stone.h"

void stone::saveState(ofstream & mapFile)const{
	uint8_t type = STONE;
	mapFile.write((char*)&type, sizeof(type));
}