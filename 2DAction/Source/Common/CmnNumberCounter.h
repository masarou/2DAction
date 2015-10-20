/* ====================================================================== */
/**
 * @brief  スコア表示クラス
 *
 * @note
 *		ゲーム中に左上に出すステージスコア
 */
/* ====================================================================== */
#ifndef __SCORE_COUNTER__
#define __SCORE_COUNTER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Game/Game2DBase.h"

class NumberCounter : public TaskUnit
{
public:

	~NumberCounter(void);
	
	static NumberCounter *Create( const char *readJson );

	// 初期化
	void Reset();
	// スコア追加,セット
	void AddValue( const int32_t &addValue );
	void SetValue( const uint32_t &setValue );
	// スコア取得
	const int32_t &GetValue() const{ return m_value; }
	// 描画情報セット
	void SetDrawInfo( const TEX_DRAW_INFO &drawInfo );
	void SetDrawPos( const math::Vector2 &pos );
	// カウントアニメをやめる
	void CountAnimEnd();
	// カウントアニメ中かどうか
	bool IsPlayCountAnim();
	// 描画無効にするかフラグセット
	void SetDrawInvalidFlag( const bool &flg ){ m_invalidDraw = flg; }
	// 数値が変わる時にSEを鳴らさないかフラグセット
	void SetCountUpSeInvalidFlag( const bool &flg ){ m_invalidCallCountUpSE = flg; }

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

private:

	NumberCounter( const char *readJson );

	void UpdateScore( const uint32_t &score );

	bool		m_invalidDraw;			// 数字の描画を行うかどうか(trueで描画しない)
	bool		m_invalidCallCountUpSE;	// 数字が変化するときにSEを鳴らすかどうか(trueで鳴らさない)
	std::string	m_readFile;				// 読み込んでいるjson
	uint32_t	m_counter;				// 数値が上がっているときにSEを定期的にならすカウンタ
	int32_t		m_value;
	int32_t		m_currDispValue;

	std::vector<Game2DBase*>	m_pNumber2DArray;	// 得点の画像等管理
	TEX_DRAW_INFO				m_numberInfo;		// 描画関係情報

};

#endif