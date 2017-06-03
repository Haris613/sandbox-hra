//
// Created by haris on 2.6.17.
//

#ifndef SEMESTRALKA_AIR_H
#define SEMESTRALKA_AIR_H

#include "tiles.h"

class air: public tiles {
 public:
	air(){}
	~air(){}
	void saveState(ofstream & mapFile)const override;
};


#endif //SEMESTRALKA_AIR_H
