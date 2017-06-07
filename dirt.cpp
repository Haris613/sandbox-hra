//
// Created by haris on 21.5.17.
//

#include "dirt.h"

void dirt::saveState(ofstream & mapFile)const {
	uint8_t type = DIRT;
	mapFile.write((char*)&type, sizeof(type));
}


