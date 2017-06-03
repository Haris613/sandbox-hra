//
// Created by haris on 21.5.17.
//

#include "lava.h"


void lava::saveState(ofstream & mapFile)const{
	mapFile << (uint8_t)LAVA;
}