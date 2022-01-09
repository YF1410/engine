#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <DirectXMath.h>
#include "WinApp.h"

using namespace DirectX;

enum MouseButton {
	LeftButton,			//!< ��
	RightButton,		//!< �E
	CenterButton,		//!< �^��
};

enum ButtonKind {
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonL,
	ButtonR,
	Select,
	Start,
	ButtonStickL,
	ButtonStickR,
	ButtonKindMax,
};

enum ButtonState {
	ButtonStateNone,
	ButtonStateDown,
	ButtonStatePush,
	ButtonStateUp,
	ButtonStateMax,
};

enum CrossAngle {
	Up = 0,
	RightUp = 4500,
	Right = 9000,
	RightDown = 13500,
	Down = 18000,
	LeftDown = 22500,
	Left = 27000,
	LeftUp = 31500,
};


class Input
{
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
private:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�����o�֐�
	//������
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	//�X�V
	void Update();
	// �L�[�̉������`�F�b�N
	bool PushKey(BYTE keyNumber);
	// �L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE keyNumber);

	//�L�[�{�[�h�f�o�C�X����
	bool CreateKeyDevice(HINSTANCE hInstance, HWND hwnd);
	//�}�E�X�f�o�C�X����
	bool CreateMouceDevice(HINSTANCE hInstance, HWND hwnd);
	//�R���g���[���[�f�o�C�X����
	bool CreatePadDevice(HINSTANCE hInstance, HWND hwnd);

	// �}�E�X�̉������`�F�b�N
	bool PushMouse(MouseButton mouseButtonType);
	// �}�E�X�̃g���K�[���`�F�b�N
	bool TriggerMouse(MouseButton mouseButtonType);
	//�}�E�X�̈ړ��ʎ擾
	XMFLOAT2 GetMouseVelocity();
	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();
	//�}�E�X�̈ʒu�擾
	XMFLOAT2 GetMousePosition();

	// �p�b�h�{�^���̉������`�F�b�N
	bool PushPad(ButtonKind padButtonType);
	// �p�b�h�{�^���̃g���K�[���`�F�b�N
	bool TriggerPad(ButtonKind padButtonType);
	//�p�b�h�{�^����LR�g���K�[�`�F�b�N
	bool CheckLeftTrigger();
	bool CheckRightTrigger();

	//�p�b�h�X�e�B�b�N�`�F�b�N
	bool CheckRightLStick();
	bool CheckLeftLStick();
	bool CheckUpLStick();
	bool CheckDownLStick();

	bool CheckRightRStick();
	bool CheckLeftRStick();
	bool CheckUpRStick();
	bool CheckDownRStick();

	bool PushRightCross();
	bool PushLeftCross();
	bool PushUpCross();
	bool PushDownCross();
	bool TriggerRightCross();
	bool TriggerLeftCross();
	bool TriggerUpCross();
	bool TriggerDownCross();

	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);

private: //�����o�ϐ�
	HWND hwnd;
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;
	ComPtr<IDirectInputDevice8> devmouse;
	ComPtr<IDirectInputDevice8> devpad;
	LPDIRECTINPUTDEVICE devicePad;
	BYTE keyState[256] = {};
	BYTE prevKeyState[256] = {};
	float unresponsiveRange = 200;

private:
	static DIMOUSESTATE currentMouseState;
	static DIMOUSESTATE prevMouseState;
	static DIJOYSTATE currentPadState;
	static DIJOYSTATE prevPadState;
	static XMFLOAT2 mousePos;
};