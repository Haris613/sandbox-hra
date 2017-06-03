//
// Created by haris on 21.5.17.
//

#ifndef SEMESTRALKA_LAVA_H
#define SEMESTRALKA_LAVA_H


#include "liquid.h"

class lava: public liquid {
 public:
	lava(){}
	~lava(){}
	void saveState(ofstream & mapFile)const override;
};


#endif //SEMESTRALKA_LAVA_H
