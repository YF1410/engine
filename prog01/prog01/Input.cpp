#include "Input.h"
#include <cassert>


#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

DIMOUSESTATE Input::currentMouseState;
DIMOUSESTATE Input::prevMouseState;
XMFLOAT2 Input::mousePos;
DIJOYSTATE Input::currentPadState;
DIJOYSTATE Input::prevPadState;

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd) {
	HRESULT result = S_FALSE;

	//初期化（一度だけ行う処理）
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr
	);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	this->hwnd = hwnd;

	CreateKeyDevice(hInstance, hwnd);
	CreateMouceDevice(hInstance, hwnd);
	CreatePadDevice(hInstance, hwnd);

	return true;
}

void Input::Update() {
	HRESULT result;

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
	// 前回のキー入力を保存
	memcpy(prevKeyState, keyState, sizeof(keyState));
	//全キーの入力状態を取得する
	result = devkeyboard->GetDeviceState(sizeof(keyState), keyState);

	if (padFlag == true) {
		//ゲームパッド情報の取得開始
		result = devpad->Acquire();

		prevPadState = currentPadState;

		result = devpad->GetDeviceState(sizeof(currentPadState), &currentPadState);
		if (FAILED(result)) {
			devpad->Poll();
		}
	}

	//マウス情報の取得開始
	result = devmouse->Acquire();
	prevMouseState = currentMouseState;
	// マウスの状態を取得
	result = devmouse->GetDeviceState(sizeof(currentMouseState), &currentMouseState);
	POINT p;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&p);
	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(hwnd, &p);
	// 変換した座標を保存
	mousePos.x = (float)p.x;
	mousePos.y = (float)p.y;
	//ShowCursor(false);
}

Input::MouseMove Input::GetMouseMove() {
	MouseMove tmp;
	tmp.lX = currentMouseState.lX;
	tmp.lY = currentMouseState.lY;
	tmp.lZ = currentMouseState.lZ;
	return tmp;
}


XMFLOAT2 Input::GetMousePosition() {
	return XMFLOAT2(mousePos);
}

bool Input::PushKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0でなければ押している
	if (keyState[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0で、今回が0でなければトリガー
	if (!prevKeyState[keyNumber] && keyState[keyNumber]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::PushMouse(MouseButton buttonType) {
	// 異常な引数を検出
	assert(0 <= buttonType <= 4);

	// 0でなければ押している
	if (currentMouseState.rgbButtons[buttonType]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerMouse(MouseButton buttonType) {
	// 異常な引数を検出
	assert(0 <= buttonType <= 4);

	// 前回が0で、今回が0でなければトリガー
	if (!currentMouseState.rgbButtons[buttonType] && prevMouseState.rgbButtons[buttonType]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::PushPad(ButtonKind padButtonType) {
	// 異常な引数を検出
	assert(0 <= padButtonType <= ButtonKindMax);

	// 0でなければ押している
	if (currentPadState.rgbButtons[padButtonType]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerPad(ButtonKind padButtonType) {
	// 異常な引数を検出
	assert(0 <= padButtonType <= ButtonKindMax);

	// 前回が0で、今回が0でなければトリガー
	if (!currentPadState.rgbButtons[padButtonType] && prevPadState.rgbButtons[padButtonType]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::CheckLeftTrigger() {
	if (currentPadState.lZ > unresponsiveRange) {
		return true;
	}
	return false;
}
bool Input::CheckRightTrigger() {
	if (currentPadState.lZ < -unresponsiveRange) {
		return true;
	}
	return false;
}


bool Input::CheckRightLStick() {
	if (currentPadState.lX > unresponsiveRange) {
		return true;
	}

	return false;
}

bool Input::CheckLeftLStick() {
	if (currentPadState.lX < -unresponsiveRange) {
		return true;
	}
	return false;
}

bool Input::CheckUpLStick() {
	if (currentPadState.lY < -unresponsiveRange) {
		return true;
	}
	return false;
}

bool Input::CheckDownLStick() {
	if (currentPadState.lY > unresponsiveRange) {
		return true;
	}
	return false;
}

bool Input::CheckRightRStick() {
	if (currentPadState.lRx > unresponsiveRange) {
		return true;
	}

	return false;
}

bool Input::CheckLeftRStick() {
	if (currentPadState.lRx < -unresponsiveRange) {
		return true;
	}
	return false;
}

bool Input::CheckUpRStick() {
	if (currentPadState.lRy < -unresponsiveRange) {
		return true;
	}
	return false;
}

bool Input::CheckDownRStick() {
	if (currentPadState.lRy > unresponsiveRange) {
		return true;
	}
	return false;
}

bool Input::PushRightCross() {
	if (currentPadState.rgdwPOV[0] == Right) {
		return true;
	}
	else if (currentPadState.rgdwPOV[0] == RightUp || currentPadState.rgdwPOV[0] == RightDown) {
		return true;
	}
	return false;
}

bool Input::PushLeftCross() {
	if (currentPadState.rgdwPOV[0] == Left) {
		return true;
	}
	else if (currentPadState.rgdwPOV[0] == LeftDown || currentPadState.rgdwPOV[0] == LeftUp) {
		return true;
	}
	return false;
}

bool Input::PushUpCross() {
	if (currentPadState.rgdwPOV[0] == Up) {
		return true;
	}
	else if (currentPadState.rgdwPOV[0] == RightUp || currentPadState.rgdwPOV[0] == LeftUp) {
		return true;
	}
	return false;
}

bool Input::PushDownCross() {
	if (currentPadState.rgdwPOV[0] == Down) {
		return true;
	}
	else if (currentPadState.rgdwPOV[0] == RightDown || currentPadState.rgdwPOV[0] == LeftDown) {
		return true;
	}
	return false;
}

bool Input::TriggerRightCross() {
	if (currentPadState.rgdwPOV[0] == Right && prevPadState.rgdwPOV[0] != Right) {
		return true;
	}
	if ((currentPadState.rgdwPOV[0] == RightUp && prevPadState.rgdwPOV[0] != RightUp) ||
		(currentPadState.rgdwPOV[0] == RightDown && prevPadState.rgdwPOV[0] != RightDown)) {
		return true;
	}
	return false;
}

bool Input::TriggerLeftCross() {
	if (currentPadState.rgdwPOV[0] == Left && prevPadState.rgdwPOV[0] != Left) {
		return true;
	}
	if ((currentPadState.rgdwPOV[0] == LeftUp && prevPadState.rgdwPOV[0] != LeftUp) ||
		(currentPadState.rgdwPOV[0] == LeftDown && prevPadState.rgdwPOV[0] != LeftDown)) {
		return true;
	}
	return false;
}

bool Input::TriggerUpCross() {
	if (currentPadState.rgdwPOV[0] == Up && prevPadState.rgdwPOV[0] != Up) {
		return true;
	}
	if ((currentPadState.rgdwPOV[0] == RightUp && prevPadState.rgdwPOV[0] != RightUp) ||
		(currentPadState.rgdwPOV[0] == LeftUp && prevPadState.rgdwPOV[0] != LeftUp)) {
		return true;
	}
	return false;
}

bool Input::TriggerDownCross() {
	if (currentPadState.rgdwPOV[0] == Down && prevPadState.rgdwPOV[0] != Down) {
		return true;
	}
	if ((currentPadState.rgdwPOV[0] == RightDown && prevPadState.rgdwPOV[0] != RightDown) ||
		(currentPadState.rgdwPOV[0] == LeftDown && prevPadState.rgdwPOV[0] != LeftDown)) {
		return true;
	}
	return false;
}



bool Input::CreateKeyDevice(HINSTANCE hInstance, HWND hwnd) {
	HRESULT result = S_FALSE;

	//キーボードデバイスの生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel
	(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}

bool Input::CreateMouceDevice(HINSTANCE hInstance, HWND hwnd) {
	HRESULT result = S_FALSE;

	//キーボードデバイスの生成
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//入力データ形式のセット
	result = devmouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//排他制御レベルのセット
	result = devmouse->SetCooperativeLevel
	(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}

bool Input::CreatePadDevice(HINSTANCE hInstance, HWND hwnd) {
	HRESULT result = S_FALSE;

	// デバイスの列挙
	result = dinput->EnumDevices(
		DI8DEVTYPE_GAMEPAD,
		DeviceFindCallBack,
		NULL,
		DIEDFL_ATTACHEDONLY
	);
	if (FAILED(result)) {
		assert(0);
		return result;
	}


	//パッドデバイスの生成
	result = dinput->CreateDevice(GUID_Joystick, &devpad, NULL);
	if (FAILED(result)) {
		padFlag = false;
	}

	if (padFlag == true) {
		//入力データ形式のセット
		result = devpad->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result)) {
			assert(0);
			return result;
		}

		// 軸モードを絶対値モードとして設定
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

		// 軸モードを変更
		if (FAILED(devpad->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
			return false;
		}

		// X軸の値の範囲設定
		DIPROPRANGE diprg;
		//ZeroMemory(&amp; diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;
		if (FAILED(devpad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return false;
		}

		// Y軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_Y;
		if (FAILED(devpad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return false;
		}

		//Z軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_Z;
		if (FAILED(devpad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return false;
		}

		//X軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_RX;
		if (FAILED(devpad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return false;
		}

		//Y軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_RY;
		if (FAILED(devpad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return false;
		}

		//Z軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_RZ;
		if (FAILED(devpad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return false;
		}

		//排他制御レベルのセット
		result = devmouse->SetCooperativeLevel
		(
			hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_FOREGROUND
		);
		if (FAILED(result)) {
			assert(0);
			return result;
		}
	}
	return true;
}

BOOL CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef) {
	return DIENUM_CONTINUE;
}