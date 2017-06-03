//
// Created by haris on 21.5.17.
//

#include "water.h"


void water::saveState(ofstream & mapFile)const{
	mapFile << (uint8_t)WATER;
}