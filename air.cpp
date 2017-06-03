//
// Created by haris on 2.6.17.
//

#include "air.h"

void air::saveState(ofstream &mapFile)const{
	mapFile <<(uint8_t) AIR;
}

