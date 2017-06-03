//
// Created by haris on 21.5.17.
//

#include "dirt.h"


void dirt::saveState(ofstream & mapFile)const {
	mapFile << (uint8_t)DIRT;
}


