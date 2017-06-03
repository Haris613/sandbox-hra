//
// Created by haris on 21.5.17.
//

#ifndef SEMESTRALKA_WATER_H
#define SEMESTRALKA_WATER_H

#include "liquid.h"

class water: public liquid {
 public:
	water(){}
	~water(){}
	void saveState(ofstream & mapFile)const override;
};


#endif //SEMESTRALKA_WATER_H
