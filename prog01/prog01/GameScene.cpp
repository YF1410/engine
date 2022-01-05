#include "GameScene.h"
#include <cassert>
#include "FbxLoader.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
	safe_delete(playerModel);
	safe_delete(groundModel);
	safe_delete(bossModel);
	safe_delete(Hpber);
	safe_delete(Hpgauge);
	safe_delete(title);
	safe_delete(gameclear);
	safe_delete(gameOver);
	safe_delete(player_attack_befor_Model);
	safe_delete(player_attack_after_Model);
	safe_delete(player_dash_Model);
	safe_delete(boss_rush_befor_Model);
	safe_delete(boss_rush_after_Model);
	safe_delete(boss_sweep_befor_Model);
	safe_delete(boss_sweep_after_Model);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->game_bgm = audio;
	this->attack_se = audio;
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	FbxLoader::GetInstance()->LoadModelFromFile("cube");

	// テクスチャ読み込み
	//if (!Sprite::LoadTexture(1, L"Resources/warningMark.png")) {
	//	assert(0);
	//}
	// 前景スプライト生成

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(2, L"Resources/title.png")) {
		assert(0);
	}
	// 前景スプライト生成
	//タイトル
	title = Sprite::Create(2, { 0.0f,0.0f });
	title->SetSize({ 1280.0f,720.0f });

	if (!Sprite::LoadTexture(3, L"Resources/HPgauge.png")) {
		assert(0);
	}
	// 前景スプライト生成
	//HPゲージ
	Hpgauge = Sprite::Create(3, { 0.0f,0.0f });
	Hpgauge->SetPosition({5 , 35 });
	Hpgauge->SetSize({ 30.0f,60.0f });

	if (!Sprite::LoadTexture(4, L"Resources/BossHP.png")) {
		assert(0);
	}
	// 前景スプライト生成
	//HPバー
	Hpber = Sprite::Create(4, { 0.0f,0.0f });
	Hpber->SetSize({ 306.0f,96.0f });

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(13, L"Resources/gameclear_color.png")) {
		assert(0);
	}
	//ゲームクリア
	gameclear = Sprite::Create(13, { 0.0f,0.0f });
	gameclear->SetSize({ 1280.0f,720.0f });

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(14, L"Resources/GameOver.png")) {
		assert(0);
	}
	//ゲームオーバー
	gameOver = Sprite::Create(14, { 0.0f,0.0f });
	gameOver->SetSize({ 1280.0f,720.0f });

	//.objの名前を指定してモデルを読み込む
	//player
	playerModel = playerModel->CreateFromObject("player");
	player_attack_befor_Model = player_attack_befor_Model->CreateFromObject("player_attack_befor");
	player_attack_after_Model = player_attack_after_Model->CreateFromObject("player_attack_after");
	player_dash_Model = player_dash_Model->CreateFromObject("player_dash");
	//boss
	bossModel = bossModel->CreateFromObject("boss");
	boss_rush_befor_Model = boss_rush_befor_Model->CreateFromObject("boss_rush_befor");
	boss_rush_after_Model = boss_rush_after_Model->CreateFromObject("boss_rush_after");
	boss_sweep_befor_Model = boss_sweep_befor_Model->CreateFromObject("boss_sweep_befor");
	boss_sweep_after_Model = boss_sweep_after_Model->CreateFromObject("boss_sweep_after");
	//stage
	groundModel = groundModel->CreateFromObject("ground");

	// 3Dオブジェクト生成
	playerObj = Object3d::Create();
	groundObj = Object3d::Create();
	bossObj = Object3d::Create();

	// 3Dオブジェクトにモデルを割り当てる

	playerObj->SetModel(playerModel);
	playerObj->SetPosition({ 25.0f, 0.0f, -25.0f });
	playerObj->SetScale({ 5.0f, 5.0f, 5.0f });
	playerObj->SetRotation({ 0, 0.0f, 0 });
	groundObj->SetModel(groundModel);
	groundObj->SetPosition({ 0.0f, -5.0f, 0.0f });
	groundObj->SetScale({ 5.0f, 5.0f, 5.0f });
	groundObj->SetRotation({ 0, 180.0f, 0 });
	bossObj->SetModel(bossModel);
	bossObj->SetPosition({ 25.0f, 0.0f, 125.0f });
	bossObj->SetScale({ 5.0f, 5.0f, 5.0f });
	bossObj->SetRotation({ 0, 180.0f, 0 });

	playerPos = playerObj->GetPosition();
	playerRot = playerObj->GetRotation();
	bossPos = bossObj->GetPosition();
	bossRot = bossObj->GetRotation();
	cameraEye = Object3d::GetEye();
	cameraTarget = Object3d::GetTarget();
}

void GameScene::Update() {
	if (nowScene == 0)//タイトル
	{
		if (input->TriggerKey(DIK_SPACE)) {
			game_bgm->PlayWave("Resources/BGM/game_bgm.wav", 255, 0.2);
			nowScene = 1;
		}

		if (input->TriggerPad(ButtonStickR)) {
			GameReset();
		}

		if (input->CheckUpLStick()) {
			GameReset();
		}
	}
	else if (nowScene == 1)//ゲームメイン
	{
		bossAttackCount--;
		Move();
		avoidance();
		playerAttack();
		collision();

		bossAttack();

		if (playerPos.x >= 275.0f) {
			playerPos.x = 275.0f;
			cameraEye.x = 340.0f;
			cameraTarget.x = 250.0f;
		}
		if (playerPos.x <= -275.0f) {
			playerPos.x = -275.0f;
			cameraEye.x = -210.0f;
			cameraTarget.x = -300.0f;
		}
		if (playerPos.z >= 250.0f) {
			playerPos.z = 250.0f;
			cameraEye.z = 185.0f;
			cameraTarget.z = 275.0f;
		}
		if (playerPos.z <= -150.0f) {
			playerPos.z = -150.0f;
			cameraEye.z = -215.0f;
			cameraTarget.z = -125.0f;
		}
		playerObj->Update();
		groundObj->Update();
		bossObj->Update();
		if (playerHp <= 0)
		{
			nowScene = 2;
			game_bgm->Stop();

		}
		else if (bossHp <= 0)
		{
			game_bgm->Stop();
			nowScene = 3;
		}
	}


	else if (nowScene == 2)//ゲームオーバー
	{

		if (input->TriggerKey(DIK_SPACE)) {

			GameReset();
		}
	}
	else if (nowScene == 3)//ゲームクリア
	{

		if (input->TriggerKey(DIK_SPACE)) {
			GameReset();
		}
	}
}

void GameScene::resetPos() {
	playerObj->SetModel(playerModel);
	playerObj->SetPosition({ 25.0f, 0.0f, -25.0f });
	playerObj->SetScale({ 5.0f, 5.0f, 5.0f });
	playerObj->SetRotation({ 0, 0.0f, 0 });
	groundObj->SetModel(groundModel);
	groundObj->SetPosition({ 0.0f, -5.0f, 0.0f });
	groundObj->SetScale({ 5.0f, 5.0f, 5.0f });
	groundObj->SetRotation({ 0, 180.0f, 0 });
	bossObj->SetModel(bossModel);
	bossObj->SetPosition({ 25.0f, 0.0f, 125.0f });
	bossObj->SetScale({ 5.0f, 5.0f, 5.0f });
	bossObj->SetRotation({ 0, 180.0f, 0 });


	playerPos = playerObj->GetPosition();
	playerRot = playerObj->GetRotation();
	bossPos = bossObj->GetPosition();
	bossRot = bossObj->GetRotation();

	cameraEye = Object3d::GetEye();
	cameraTarget = Object3d::GetTarget();
	cameraEye = { 90.0,135.0,-90.0 };
	cameraTarget = { 0,0,0 };
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	//XMFLOAT3 playerPos = playerObj->GetPosition();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// 背景スプライト描画
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3Dオブクジェクトの描画
	if (nowScene == 1)
	{
		if (charaPose == 0)
		{
			playerObj->SetModel(playerModel);
		}
		else if (charaPose == 1)
		{
			playerObj->SetModel(player_attack_befor_Model);
		}
		else if (charaPose == 2)
		{
			playerObj->SetModel(player_attack_after_Model);
		}
		else if (charaPose == 3)
		{
			playerObj->SetModel(player_dash_Model);
		}

		if (bossPose == 0)
		{
			bossObj->SetModel(bossModel);
		}
		else if (bossPose == 1)
		{
			bossObj->SetModel(boss_rush_befor_Model);
		}
		else if (bossPose == 2)
		{
			bossObj->SetModel(boss_rush_after_Model);
		}
		else if (bossPose == 3)
		{
			bossObj->SetModel(boss_sweep_befor_Model);
		}
		else if (bossPose == 4)
		{
			bossObj->SetModel(boss_sweep_after_Model);
		}

		if (playerHp > 0) {
			playerObj->Draw();
			playerFlag = true;
		}
		groundObj->Draw();
		if (bossHp > 0) {
			bossHpgauge = 100 * bossHp;
			bossObj->Draw();
			bossFlag = true;
		}
	}

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// 前景スプライトの描画
	if (nowScene == 0)
	{
		title->Draw();
	}
	if (nowScene == 1)
	{
		Hpber->Draw();
		Hpgauge->SetSize({ bossHpgauge,60 });
		Hpgauge->Draw();
	}
	else if (nowScene == 2)
	{
		gameOver->Draw();
	}
	else if (nowScene == 3)
	{
		gameclear->Draw();
	}
	// デバッグテキストの描画
	debugText.DrawAll(dxCommon->GetCommandList());
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void GameScene::Move() {
	// オブジェクト移動
	// 移動後の座標を計算
	if (playerMode == 0) {
		if (input->CheckUpLStick()) {
			playerPos.z += moveAmount;
			cameraEye.z += moveAmount;
			cameraTarget.z += moveAmount;
			playerPos.x -= moveAmount;
			cameraEye.x -= moveAmount;
			cameraTarget.x -= moveAmount;
			playerRot.y = -45.0f;
			if (input->CheckRightLStick()) {
				playerPos.z += moveAmount / 2;
				cameraEye.z += moveAmount / 2;
				cameraTarget.z += moveAmount / 2;
				playerPos.x += moveAmount;
				cameraEye.x += moveAmount;
				cameraTarget.x += moveAmount;
				playerRot.y = 0.0f;
			}
			else if (input->CheckLeftLStick()) {
				playerPos.x -= moveAmount / 2;
				cameraEye.x -= moveAmount / 2;
				cameraTarget.x -= moveAmount / 2;
				playerPos.z -= moveAmount;
				cameraEye.z -= moveAmount;
				cameraTarget.z -= moveAmount;
				playerRot.y = -90.0f;
			}
		}
		else if (input->CheckDownLStick()) {
			playerPos.z -= moveAmount;
			cameraEye.z -= moveAmount;
			cameraTarget.z -= moveAmount;
			playerPos.x += moveAmount;
			cameraEye.x += moveAmount;
			cameraTarget.x += moveAmount;
			playerRot.y = 135.0f;
			if (input->CheckRightLStick()) {
				playerPos.x += moveAmount / 2;
				cameraEye.x += moveAmount / 2;
				cameraTarget.x += moveAmount / 2;
				playerPos.z += moveAmount;
				cameraEye.z += moveAmount;
				cameraTarget.z += moveAmount;
				playerRot.y = 90.0f;
			}
			else if (input->CheckLeftLStick()) {
				playerPos.z -= moveAmount / 2;
				cameraEye.z -= moveAmount / 2;
				cameraTarget.z -= moveAmount / 2;
				playerPos.x -= moveAmount;
				cameraEye.x -= moveAmount;
				cameraTarget.x -= moveAmount;
				playerRot.y = 180.0f;
			}
		}
		else if (input->CheckRightLStick()) {
			playerPos.x += moveAmount;
			cameraEye.x += moveAmount;
			cameraTarget.x += moveAmount;
			playerPos.z += moveAmount;
			cameraEye.z += moveAmount;
			cameraTarget.z += moveAmount;
			playerRot.y = 45.0f;
		}
		else if (input->CheckLeftLStick()) {
			playerPos.x -= moveAmount;
			cameraEye.x -= moveAmount;
			cameraTarget.x -= moveAmount;
			playerPos.z -= moveAmount;
			cameraEye.z -= moveAmount;
			cameraTarget.z -= moveAmount;
			playerRot.y = -135.0f;
		}
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	playerObj->SetRotation(playerRot);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);
}

bool GameScene::avoidance() {
	if (input->TriggerKey(DIK_SPACE) && playerMode == 0) {
		playerMode = 1;
		startPlayerPos = playerPos;
		startCameraEye = cameraEye;
		startCameraTarget = cameraTarget;
		endPlayerPos = playerPos;
		endCameraEye = cameraEye;
		endCameraTarget = cameraTarget;
	}

	if (playerMode != 0) {
		avoidNowTime += 0.01f;
		avoidTimeRate = min(avoidNowTime / avoidEndTime, 1);
	}

	if (playerMode == 1) {
		if (input->PushKey(DIK_W)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;
			if (input->PushKey(DIK_D)) {
				endPlayerPos.x = startPlayerPos.x;
				endCameraEye.x = startCameraEye.x;
				endCameraTarget.x = startCameraTarget.x;
			}
			else if (input->PushKey(DIK_A)) {
				endPlayerPos.z = startPlayerPos.z;
				endCameraEye.z = startCameraEye.z;
				endCameraTarget.z = startCameraTarget.z;
			}
		}
		else if (input->PushKey(DIK_S)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
			if (input->PushKey(DIK_D)) {
				endPlayerPos.z = startPlayerPos.z;
				endCameraEye.z = startCameraEye.z;
				endCameraTarget.z = startCameraTarget.z;
			}
			else if (input->PushKey(DIK_A)) {
				endPlayerPos.x = startPlayerPos.x;
				endCameraEye.x = startCameraEye.x;
				endCameraTarget.x = startCameraTarget.x;
			}
		}
		else if (input->PushKey(DIK_D)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z + avoidMove;
			endCameraEye.z = startCameraEye.z + avoidMove;
			endCameraTarget.z = startCameraTarget.z + avoidMove;
			endPlayerPos.x = startPlayerPos.x + avoidMove;
			endCameraEye.x = startCameraEye.x + avoidMove;
			endCameraTarget.x = startCameraTarget.x + avoidMove;
		}
		else if (input->PushKey(DIK_A)) {
			playerMode = 2;
			endPlayerPos.z = startPlayerPos.z - avoidMove;
			endCameraEye.z = startCameraEye.z - avoidMove;
			endCameraTarget.z = startCameraTarget.z - avoidMove;
			endPlayerPos.x = startPlayerPos.x - avoidMove;
			endCameraEye.x = startCameraEye.x - avoidMove;
			endCameraTarget.x = startCameraTarget.x - avoidMove;
		}
	}
	if (playerMode == 2) {
		charaPose = 3;
		playerPos = easeOutQuint(startPlayerPos, endPlayerPos, avoidTimeRate);
		cameraEye = easeOutQuint(startCameraEye, endCameraEye, avoidTimeRate);
		cameraTarget = easeOutQuint(startCameraTarget, endCameraTarget, avoidTimeRate);
	}

	// 座標の変更を反映
	playerObj->SetPosition(playerPos);
	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);

	if (avoidTimeRate == 1) {
		avoidNowTime = 0;
		avoidTimeRate = 0;
		playerMode = 0;
		charaPose = 0;
	}

	if ((playerMode >= 1 && playerMode <= 2) && avoidTimeRate < 1) {
		return true;
	}
	return false;
}

void GameScene::playerAttack() {
	if ((input->TriggerKey(DIK_Q) || input->TriggerMouse(LeftButton)) && attackFlag == false)
	{
		attackFlag = true;
		playerMode = 3;
		//音声の読み込みと再生
		attack_se->PlayWave("Resources/SE/attack_se.wav", 0, 0.2);
	}
	if (attackFlag == true)
	{
		attackFrame++;
	}

	if (attackFrame < 24 && attackFlag == true)
	{
		charaPose = 1;
	}
	else if (attackFrame >= 24 && attackFrame < 60 && attackFlag == true) {
		charaPose = 2;

	}
	else if (attackFrame >= 60 && attackFlag == true) {
		attackFrame = 0;
		attackFlag = false;
		charaPose = 0;
	}

	if (attackFrame == 48 && collisionFlag == true) {
		bossHp -= 1;
	}
}

void GameScene::bossRush() {
	if (rushMode == 0) {
		bossRotation();
	}

	if (bossAttackCount == 230) {
		bossRushFlag = true;
	}

	if (bossRushFlag == true)
	{
		bossPose = 1;
		if (rushMode != 0) {
			rushNowTime += 0.01f;
			rushTimeRate = min(rushNowTime / rushEndTime, 1);
		}

		if (bossAttackCount <= 120 && rushMode == 0) {
			rushMode = 1;
			startBossPos = bossPos;
			endBossPos = playerPos;
		}

		if (rushMode == 1) {
			if (collisionFlag == true) {
				playerHp--;
			}
			bossPos = easeOutQuint(startBossPos, endBossPos, rushTimeRate);
			bossPose = 2;
		}
	}

	// 座標の変更を反映
	bossObj->SetPosition(bossPos);

	if (rushTimeRate == 1.0f) {
		rushNowTime = 0;
		rushTimeRate = 0;
		rushMode = 2;
		bossAttackCount = 420;
		bossRushFlag = false;
		bossRushStart = false;
	}
	if (rushMode == 2) {
		if (bossAttackCount == 300) {
			bossPose = 0;
			rushMode = 0;
		}
	}
}

void GameScene::bossSweep() {
	if (sweepMode == 0) {
		bossRotation();
	}

	if (bossAttackCount == 180) {
		bossSweepFlag = true;
	}

	if (bossSweepFlag == true)
	{
		bossPose = 3;
		if (sweepMode != 0) {
			rushNowTime += 0.01f;
			rushTimeRate = min(rushNowTime / rushEndTime, 1);
		}

		if (bossAttackCount <= 120 && sweepMode == 0) {
			sweepMode = 1;
		}

		if (sweepMode == 1) {
			if (collisionFlag == true) {
				playerHp--;
			}
			bossPose = 4;
		}
	}

	if (rushTimeRate == 1) {
		rushNowTime = 0;
		rushTimeRate = 0;
		sweepMode = 2;
		bossAttackCount = 420;
		bossSweepFlag = false;
		bossSweepStart = false;
	}
	if (sweepMode == 2) {
		if (bossAttackCount == 360) {
			bossPose = 0;
			sweepMode = 0;
		}
	}
}

void GameScene::bossAttack() {
	collisionX = playerPos.x - bossPos.x;
	collisionZ = playerPos.z - bossPos.z;
	Collision = sqrtf((collisionX * collisionX) + (collisionZ * collisionZ));
	if (Collision <= 75.0f && bossRushStart == false) {
		bossSweepStart = true;
	}
	else if (bossSweepStart == false) {
		bossRushStart = true;
	}

	if (bossRushStart == true) {
		bossRush();
	}
	else if (bossSweepStart == true) {
		bossSweep();
	}
}

void GameScene::GameReset()
{
	nowScene = 0;
	resetPos();
	charaPose = 0;
	bossPose = 0;
	playerHp = 1;
	bossHp = 3;
	bossAttackCount = 420;
	//mode
	playerMode = 0;
	rushMode = 0;
	sweepMode = 0;
	//Flag
	attackFlag = false;
	bossAttackFlag = false;
	bossRushFlag = false;
	bossSweepFlag = false;
	bossRushStart = false;
	bossSweepStart = false;
	collisionFlag = 0;
	//Frame
	attackFrame = 0;
	bossFrame = 0.0f;
	//collision
	collisionX = 0;
	collisionZ = 0;
	Collision = 0;
	//bgm
	game_bgm->Initialize();
	//timerate
	rushTimeRate = 0;
	rushNowTime = 0;
	avoidNowTime = 0;
	avoidTimeRate = 0;
}

void GameScene::collision() {
	if (!avoidance()) {
		collisionX = playerPos.x - bossPos.x;
		collisionZ = playerPos.z - bossPos.z;
		Collision = sqrtf((collisionX * collisionX) + (collisionZ * collisionZ));

		if (Collision <= playerRadius + bossRadius) {
			collisionFlag = true;
		}
		else {
			collisionFlag = false;
		}
	}
}

void GameScene::bossRotation() {
	XMFLOAT3 bossPos = bossObj->GetPosition();

	XMFLOAT3 playerPos = playerObj->GetPosition();

	float angleY = angle->PosForRadians(bossPos.x, bossPos.z, playerPos.x, playerPos.z);

	bossObj->SetRotation({ 0,XMConvertToDegrees(angleY), 0 });
}

