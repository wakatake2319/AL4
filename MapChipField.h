#pragma once
#include "KamataEngine.h"
#include "vector"

class MapChipField {
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

public:
	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);

	// マップチップ種別の取得
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	// マップチップ座標の取得
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);


private:
	MapChipData mapChipData_;
};

enum class MapChipType {
	// 空白
	kBlank,
	// ブロック
	kBlock
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

