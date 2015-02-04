/* ====================================================================== */
/**
 * @brief  ゲームで共有が必要な情報格納クラス
 *
 * @note
 *			クラスをまたいで情報を保持したい場合にセット
 */
/* ====================================================================== */

#ifndef __CMN_ACCESER__
#define __CMN_ACCESER__

#include "Math/MathVector.h"

class GameAccesser
{

public:

	static void Create();
	static GameAccesser *GetInstance();

	// 初期化
	void InitAll();

	// プレイヤー情報セット
	void SetPlayerOffSet(const math::Vector2 &setPos);
	void SetPlayerOffSet(const float &posX, const float &posY);
	void AddPlayerOffSet(const math::Vector2 &addPos);
	void AddPlayerOffSet(const float posX, const float posY);

	// プレイヤー情報取得
	const math::Vector2 &GetPlayerOffSet() const;
	void GetPlayerOffSet( float &posX, float &posY);

	// GameAccesser解放
	void DeleteGameAccesser();

private:

	GameAccesser();
	~GameAccesser();

	static GameAccesser *s_pInstance;

	//! 内部的なプレイヤー座標
	math::Vector2	m_offsetPlayer;

};

#endif