#include "Camera.h"

using namespace DirectX;

void Camera::Initialize() {
	// ビュー行列の生成
	matView = XMMatrixLookAtLH
	(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);


	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(60.0f),
		(float)window_width / (float)window_height,
		0.1f, 1000.0f
	);
}
