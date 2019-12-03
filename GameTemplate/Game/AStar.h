#pragma once
#include "Navimesh.h"

class AStar
{
public:
	AStar();
	~AStar();
	void AstarSearch(Cell startcell,Cell endcell);
};

