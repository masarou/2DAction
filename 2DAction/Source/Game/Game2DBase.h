/* ====================================================================== */
/**
 * @brief  2D画像貼り付けクラス
 *
 * @note
 *		基本的にはこれを継承、または保持して使用
 */
/* ====================================================================== */

#ifndef SYSTEM_GAME_2D
#define SYSTEM_GAME_2D

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "System/Draw2D/SystemDraw2DManager.h"

class Game2DBase
{
public:

	Game2DBase( const char *jsonFile );
	virtual ~Game2DBase(void);

	//!再生中アニメの変更、取得
	void SetAnim( const std::string &animTag );							
	const char *GetPlayAnim(){return m_currentAnimTag.c_str();}
	
	//!描画設定更新
	void SetDrawInfo( TEX_DRAW_INFO &info );
	
	//!設定アニメの更新
	void DrawUpdate2D();

	const TEX_DRAW_INFO &GetDrawInfo(){ return m_drawInfo; }

protected:



private:

	//!テクスチャロード
	void LoadTextureInfo(const char *jsonFile);
	//!テクスチャアンロード
	void DeleteTextureInfo(const char *jsonFile);

	// デフォルトアニメ、jsonファイル名、アニメの種類数

	uint32_t						m_animCounter;		//!<アニメの際に使用するカウンタ
	std::string						m_jsonFile;			// 読み込んだjsonファイル名
	TEX_DRAW_INFO					m_drawInfo;

	std::string						m_currentAnimTag;	//!<再生中のアニメ
	std::vector<int32_t>			m_vTexHandle;		// 描画Handle
	std::vector<std::string>		m_vAnimName;		// 再生できるアニメ
	
};

#endif