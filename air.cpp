//
// Created by haris on 2.6.17.
//

#include "air.h"

void air::saveState(ofstream &mapFile)const{
	uint8_t type = AIR;
	mapFile.write((char*)&type, sizeof(type));
}

