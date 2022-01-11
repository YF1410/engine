#pragma once

#include <math.h>

class Angle {
public:
	void Initialize();
	void Update();
	float GetRadians();
	float PosForRadians(float startPos1, float startPos2, float endPos1, float endPos2);
	float lR(float startPos1, float startPos2, float endPos1, float endPos2);

private:
	//    cx:座標　cy:座標  　mv:移動量   
	float startPos1 = 0;
	float startPos2 = 0;
	float endPos1 = 0;
	float endPos2 = 0;
	float angle1 = 0;
	float angle2 = 0;
	float resultRadians = 0;
};

