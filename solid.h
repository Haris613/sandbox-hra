//
// Created by haris on 21.5.17.
//

#ifndef SEMESTRALKA_SOLID_H
#define SEMESTRALKA_SOLID_H


#include "tiles.h"

class solid: public tiles {
 public:
	solid(){}
	virtual ~solid(){}
	virtual void saveState(ofstream & mapFile)const override = 0;
};


#endif //SEMESTRALKA_SOLID_H
