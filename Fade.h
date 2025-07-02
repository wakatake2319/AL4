#pragma once
#include "KamataEngine.h"


using namespace KamataEngine;


class Fade {
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

private:

	// スプライト
	Sprite* sprite_ = nullptr;


};