//
// Created by haris on 7.5.17.
//

#ifndef SEMESTRALKA_TILES_H
#define SEMESTRALKA_TILES_H


class tiles {//todo derive this class maybe - from class grouping tiles and humanoids
 public:
	tiles();
	virtual void drawTile(unsigned int posX, unsigned int posY) = 0;
};


#endif //SEMESTRALKA_TILES_H
