//
// Created by haris on 21.5.17.
//

#ifndef SEMESTRALKA_DIRT_H
#define SEMESTRALKA_DIRT_H

#include "solid.h"

class dirt: public solid {
 public:
	dirt(){}
	~dirt(){}
	void saveState(ofstream & mapFile)const override;
};


#endif //SEMESTRALKA_DIRT_H
