#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <DirectXMath.h>
#include "WinApp.h"

using namespace DirectX;

enum MouseButton {
	LeftButton,			//!< 左
	RightButton,		//!< 右
	CenterButton,		//!< 真ん中
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


class Input {
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
private:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ関数
	//初期化
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	//更新
	void Update();
	// キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

	//キーボードデバイス生成
	bool CreateKeyDevice(HINSTANCE hInstance, HWND hwnd);
	//マウスデバイス生成
	bool CreateMouceDevice(HINSTANCE hInstance, HWND hwnd);
	//コントローラーデバイス生成
	bool CreatePadDevice(HINSTANCE hInstance, HWND hwnd);

	// マウスの押下をチェック
	bool PushMouse(MouseButton mouseButtonType);
	// マウスのトリガーをチェック
	bool TriggerMouse(MouseButton mouseButtonType);
	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();
	//マウスの位置取得
	XMFLOAT2 GetMousePosition();

	// パッドボタンの押下をチェック
	bool PushPad(ButtonKind padButtonType);
	// パッドボタンのトリガーをチェック
	bool TriggerPad(ButtonKind padButtonType);
	//パッドボタンのLRトリガーチェック
	bool CheckLeftTrigger();
	bool CheckRightTrigger();

	//パッドスティックチェック
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

private: //メンバ変数
	HWND hwnd;
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;
	ComPtr<IDirectInputDevice8> devmouse;
	ComPtr<IDirectInputDevice8> devpad;
	LPDIRECTINPUTDEVICE devicePad;
	BYTE keyState[256] = {};
	BYTE prevKeyState[256] = {};
	float unresponsiveRange = 200;
	bool padFlag = false;

private:
	static DIMOUSESTATE currentMouseState;
	static DIMOUSESTATE prevMouseState;
	static DIJOYSTATE currentPadState;
	static DIJOYSTATE prevPadState;
	static XMFLOAT2 mousePos;
};