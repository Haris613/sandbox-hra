//
// Created by haris on 21.5.17.
//

#ifndef SEMESTRALKA_LIQUID_H
#define SEMESTRALKA_LIQUID_H


#include "tiles.h"

class liquid: public tiles {
 public:
	liquid(){}
	virtual ~liquid(){}
 private:
	unsigned int liquidHealth; //not a good name
};


#endif //SEMESTRALKA_LIQUID_H
