#include "Angle.h"

void Angle::Initialize() {
}

void Angle::Update() {
	angle1 = endPos1 - startPos1;
	angle2 = endPos2 - startPos2;

	resultRadians = atan2(angle1, angle2);
}

float Angle::GetRadians() {
	return resultRadians;
}

float Angle::PosForRadians(float startPos1, float startPos2, float endPos1, float endPos2) {
	angle1 = endPos1 - startPos1;
	angle2 = endPos2 - startPos2;

	resultRadians = atan2(angle1, angle2);

	return resultRadians;
}

float Angle::lR(float startPos1, float startPos2, float endPos1, float endPos2) {
	float px = endPos1 - startPos1;
	float py = endPos2 - startPos2;

	float l = sqrt(px * px + py * py);
	return l;
}

////����
//if (keyg[37]) {	//��
//	angle += 5;
//	//cos,sin�͔z����g�p���Ă���̂Ŋp�x��0�`359�ɂȂ�悤�ɒ�������K�v������
//	if (angle >= 360)
//		angle -= 360;
//	//angle%=360;�Ə�����
//}
//if (keyg[39]) {	//��
//	angle -= 5;
//
//	if (angle < 0)
//		angle += 360;
//	//angle=(angle+360)%360;�Ə�����(���Ԃ�
//}
//
////�����Ă���p�x�֐i��
//if (keyg[38]) {	//��
//	endPosX += fcos[angle] * mv;
//	endPosY -= fsin[angle] * mv;
//}