//
// Created by haris on 21.5.17.
//

#ifndef SEMESTRALKA_STONE_H
#define SEMESTRALKA_STONE_H


#include "solid.h"

class stone: public solid {
 public:
	stone(){}
	~stone(){}
	void saveState(ofstream & mapFile)const override;
};


#endif //SEMESTRALKA_STONE_H
