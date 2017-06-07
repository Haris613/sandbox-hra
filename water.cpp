//
// Created by haris on 21.5.17.
//

#include "water.h"

void water::saveState(ofstream & mapFile)const{
	uint8_t type = WATER;
	mapFile.write((char*)&type, sizeof(type));
}