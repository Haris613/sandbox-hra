//
// Created by haris on 21.5.17.
//

#include "lava.h"

void lava::saveState(ofstream & mapFile)const{
	uint8_t type = LAVA;
	mapFile.write((char*)&type, sizeof(type));
}