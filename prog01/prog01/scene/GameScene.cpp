#include "GameScene.h"
#include "FbxLoader.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "imgui.h"

using namespace DirectX;

GameScene::GameScene() {
}

GameScene::~GameScene() {
	safe_delete(object1);
	safe_delete(model1);
	safe_delete(spriteBG);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,0,-1000.0f });

	// モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	// カメラをセット
	FbxObject3d::SetCamera(camera);
	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	object1 = new FbxObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->SetPosition({ 0,y,0 });
}

void GameScene::Update() {

	if (input->TriggerKey(DIK_C) && (!falling && !shooting)) {
		if (nowMode == drop) {
			object1->SetPosition({ defCannonPos });
			vy = cannonVy;
			y = defCannonPos.y;
			nowMode = cannon;
		}
		else {
			object1->SetPosition({ defDropPos });
			y = defDropPos.y;
			nowMode = drop;
		}
	}

	switch (nowMode)
	{
	case drop:
		if (input->TriggerKey(DIK_SPACE)) {
			falling = true;
		}

		if (falling) {
			vy += gravity;
			y += vy;
			object1->SetPosition({ 0,y,0 });
			if (y <= -500.0f) {
				falling = false;
				vy = 0;
				y = defDropPos.y;
				object1->SetPosition({ 0,y,0 });
			}
		}
		object1->Update();
		break;
	case cannon:
		if (input->TriggerKey(DIK_SPACE)) {
			shooting = true;
		}

		if (shooting) {
			vy += gravity;
			y += vy;
			x += 10;
			object1->SetPosition({ x,y,0 });
			if (y <= -500.0f) {
				shooting = false;
				vy = cannonVy;
				y = defCannonPos.y;
				x = defCannonPos.x;
				object1->SetPosition({ x,y,0 });
			}
		}

		object1->Update();
		break;
	default:
		break;
	}

	camera->Update();
	particleMan->Update();
}

void GameScene::Draw() {
	/*ImGui::Begin("Light");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));*/
	//ImGui::SliderFloat3("object2Pos", object2Pos,-1000,1000);
	//ImGui::DragFloat3("object2Pos", object2Pos);
	//ImGui::End();

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	object1->Draw(cmdList);

	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	switch (nowMode)
	{
	case drop:
		debugText->GetInstance()->Print("Space:Drop", 0, 0, 3);
		debugText->GetInstance()->Print("C:ModeChange", 0, 40, 3);
		break;
	case cannon:
		debugText->GetInstance()->Print("Space:Shot", 0, 0, 3);
		debugText->GetInstance()->Print("C:ModeChange", 0, 40, 3);
		break;
	default:
		break;
	}

	// デバッグテキストの描画
	debugText->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
