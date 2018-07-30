#pragma once
#ifndef GAMEMAP
#define GAMEMAP

class GameMap {
public:
	GameMap();
	
private:
	enum class GridType {//might not keep this here, just doodling it out
		NONE = -1, HEX = 0, QUAD = 1
	};
	//2D vector(container) of tiles
};

#endif