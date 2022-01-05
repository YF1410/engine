#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "Model.h"
#include "stdlib.h"
#include "time.h"
#include "Vector3.h"
#include "Angle.h"

using namespace DirectX;

class GameScene {
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数
	// コンストクラタ
	GameScene();
	// デストラクタ
	~GameScene();
	// 初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);
	// 毎フレーム処理
	void Update();
	//pos初期化
	void resetPos();
	// 描画
	void Draw();
	//移動
	void Move();
	//回避
	bool avoidance();
	//攻撃
	void playerAttack();
	//当たり判定
	void collision();
	//boss攻撃
	void bossRush();
	void bossSweep();
	//boss向き調整
	void bossRotation();

	void bossAttack();

	void GameReset();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Angle* angle = new Angle();
	Audio* game_bgm = nullptr;
	Audio* attack_se = nullptr;
	DebugText debugText;

	// ゲームシーン用
	Sprite* sprite = nullptr;
	Sprite* back1 = nullptr;
	Sprite* back2 = nullptr;
	Sprite* back3 = nullptr;
	Sprite* Hpgauge = nullptr;
	Sprite* Hpber = nullptr;
	Sprite* title = nullptr;
	Sprite* gameclear = nullptr;
	Sprite* gameOver = nullptr;
	Model* playerModel = nullptr;
	Model* groundModel = nullptr;
	Model* bossModel = nullptr;
	Model* player_attack_befor_Model = nullptr;
	Model* player_attack_after_Model = nullptr;
	Model* player_dash_Model = nullptr;
	Model* boss_rush_befor_Model = nullptr;
	Model* boss_rush_after_Model = nullptr;
	Model* boss_sweep_befor_Model = nullptr;
	Model* boss_sweep_after_Model = nullptr;

	Object3d* playerObj = nullptr;
	Object3d* groundObj = nullptr;
	Object3d* bossObj = nullptr;

	XMFLOAT3 playerPos;
	XMFLOAT3 playerRot;
	XMFLOAT3 startPlayerPos;
	XMFLOAT3 endPlayerPos;

	XMFLOAT3 cameraEye;
	XMFLOAT3 cameraTarget;
	XMFLOAT3 startCameraEye;
	XMFLOAT3 endCameraEye;
	XMFLOAT3 startCameraTarget;
	XMFLOAT3 endCameraTarget;

	XMFLOAT3 bossPos;
	XMFLOAT3 bossRot;
	XMFLOAT3 startBossPos;
	XMFLOAT3 endBossPos;

	int nowScene = 0;
	int charaPose = 0;
	int bossPose = 0;

	bool attackFlag = false;
	bool bossAttackFlag = false;
	bool bossRushFlag = false;
	bool bossSweepFlag = false;
	bool bossRushStart = false;
	bool bossSweepStart = false;
	float bossFrame = 0.0f;
	float attackFrame = 0.0f;

	const float playerScale = 1.0f;
	const float groundScale = 15.0f;
	const float largeCarScale = 3.0f;

	float avoidNowTime = 0;
	const float avoidEndTime = 0.5f;
	float avoidTimeRate = 0;

	const float moveAmount = 1.0f;

	const float avoidMove = 40.0f;

	int playerMode = 0;

	int bossAttackCount = 420;

	int rushMode = 0;
	float rushNowTime = 0;
	const float rushEndTime = 0.5;
	float rushTimeRate = 0;

	int sweepMode = 0;

	const float playerRadius = 15.0f;
	const float bossRadius = 15.0f;
	float collisionX = 0;
	float collisionZ = 0;
	float Collision = 0;
	bool collisionFlag = 0;

	int playerHp = 1;
	int bossHp = 3;
	float bossHpgauge = 30;
	bool playerFlag = false;
	bool bossFlag = false;
};