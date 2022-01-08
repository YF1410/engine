#include "Camera.h"

using namespace DirectX;

void Camera::Initialize() {
	// �r���[�s��̐���
	matView = XMMatrixLookAtLH
	(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);


	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(60.0f),
		(float)window_width / (float)window_height,
		0.1f, 1000.0f
	);
}
