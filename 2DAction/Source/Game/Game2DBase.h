/* ====================================================================== */
/**
 * @brief  2D画像貼り付けクラス
 *
 * @note
 *		基本的にはこれを継承、または保持して使用
 */
/* ====================================================================== */

#ifndef __SYSTEM_GAME_2D__
#define __SYSTEM_GAME_2D__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "System/Draw2D/SystemDraw2DManager.h"

class Game2DBase
{
public:

	// CreateWithCheckは指定されたJsonがGame2D用のものかどうかのチェックも行う
	static Game2DBase *Create( const char *jsonFile );
	static Game2DBase *CreateWithCheck( const char *jsonFile );
	
	virtual ~Game2DBase(void);

	//!再生中アニメの変更、取得
	void SetAnim( const std::string &animTag );
	const char *GetPlayAnim() const{return m_currentAnimTag.c_str();}
	
	//!描画設定更新
	void SetDrawInfo( TEX_DRAW_INFO &info );
	
	//!設定アニメの更新
	void DrawUpdate2D();

	//!現在のアニメ再生フレーム数取得
	const uint32_t &GetCurrentAnimFrame() const{ return m_animCounter; }

	const TEX_DRAW_INFO &GetDrawInfo() const{ return m_drawInfo; }
	TEX_DRAW_INFO &UpdateDrawInfo(){ return m_drawInfo; }
	
private:

	Game2DBase( const char *jsonFile );

	//!テクスチャロード
	void LoadTextureInfo(const char *jsonFile);
	//!テクスチャアンロード
	void DeleteTextureInfo(const char *jsonFile);

	// デフォルトアニメ、jsonファイル名、アニメの種類数

	uint32_t						m_animCounter;		//!<アニメの際に使用するカウンタ
	TEX_DRAW_INFO					m_drawInfo;

	std::string						m_currentAnimTag;	//!<再生中のアニメ
	std::vector<int32_t>			m_vTexHandle;		// 描画Handle
	std::vector<std::string>		m_vAnimName;		// 再生できるアニメ

};

struct Texture2D{
	Game2DBase			*m_pTex2D;			// 画像管理
	//TEX_DRAW_INFO		m_texInfo;			// 描画情報

	Texture2D(){
		Init();
	}

	void Init(){
		m_pTex2D = NULL;
		//m_texInfo.Init();
	}
	void DeleteAndInit(){
		SAFE_DELETE(m_pTex2D);
		//m_texInfo.Init();
	}
};

#endif