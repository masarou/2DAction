/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ステージ準備クラス
 */
/* ====================================================================== */
#ifndef __FLOW_PROCESS_INIT_LAST_STAGE__
#define __FLOW_PROCESS_INIT_LAST_STAGE__

#include "FlowProcessBase.h"

class StageStart : public ProcessBase
{
public:

	static StageStart *Create();

	~StageStart(void);

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool CanDie() override;
	virtual void PadEventDecide() override;
	virtual bool IsNeedPauseFilter() const override{ return false; }

private:

	void FadeUpdate( TEX_DRAW_INFO &info );
	std::string GetStageAnimStr();
	std::string GetClearAnimStr();

	StageStart(void);

	uint32_t		m_counter;			// 演出を開始してからのカウンタ
	int32_t			m_texSizeW;			// テクスチャサイズ画面から出たかどうかの判定で使用
	bool			m_IsFadeOut;		// フェードアウトを開始してもよいかどうか
	Texture2D		m_stagePhaseTex;	// ステージがいくつかを表す画像
	Texture2D		m_stageClearTex;	// クリア条件を表す画像
};

#endif