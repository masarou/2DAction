
/* ====================================================================== */
/**
 * @brief  2D画像貼り付けクラス
 *
 * @note
 *		基本的にはこれを継承、または保持して使用
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "Game2DBase.h"
#include "System/Draw2D/SystemDraw2DResource.h"

Game2DBase::Game2DBase(const char *jsonFile)
: m_animCounter(0)
, m_jsonFile(jsonFile)
, m_currentAnimTag("")
{
	m_drawInfo.Init();

	//!jsonから情報取得
	LoadTextureInfo(m_jsonFile.c_str());
}

Game2DBase::~Game2DBase(void)
{
	DeleteTextureInfo(m_jsonFile.c_str());
}

/* ================================================ */
/**
 * @brief	設定アニメの更新
 */
/* ================================================ */
void Game2DBase::DrawUpdate2D()
{
	if(m_currentAnimTag.compare("") == 0){
		//!アニメが何も設定されていない
		return ;		
	}

	// 描画の要素番号取得
	uint32_t drawIndex = TextureResourceManager::GetInstance()->GetAnimHandleIndex( m_jsonFile.c_str(), m_currentAnimTag, m_animCounter);

	// 無効値が帰ってきた場合はアニメになにもセットしない(デフォルトアニメになにもセットされていない等)
	if( drawIndex == INVALID_VALUE ){
		m_currentAnimTag = "";
		return;
	}

	// 配列要素より大きかった場合も何もしない
	if(m_vTexHandle.size() <= drawIndex){
		DEBUG_ASSERT("m_vTexHandle.size() <= drawIndex");
		return;
	}

	// 無事取得できたなら描画するようにpushしておく
	if(drawIndex != INVALID_VALUE){
		Draw2DManager::GetInstance()->PushDrawInfo( m_drawInfo, m_vTexHandle.at(drawIndex));
		++m_animCounter;
	}

}

/* ================================================ */
/**
 * @brief	再生アニメ変更
 */
/* ================================================ */
void Game2DBase::SetAnim( const std::string &animTag )
{
	if(m_currentAnimTag.compare(animTag.c_str()) == 0){
		//!再生中アニメなら無視
		return;
	}

	//!指定されたアニメがあるか調査
	for(uint32_t i = 0; i < m_vAnimName.size(); ++i){
		if(m_vAnimName.at(i).compare(animTag) == 0){
			m_currentAnimTag = animTag;
			m_animCounter = 0;
			return;
		}
	}

	DEBUG_PRINT("【指定されたアニメタグがありません！】\n");
}

/* ================================================ */
/**
 * @brief	描画の際に必要となる情報更新
 */
/* ================================================ */
void Game2DBase::SetDrawInfo( TEX_DRAW_INFO &info)
{
	if( info.m_fileName.compare("") == 0 ){
		info.m_fileName = m_jsonFile;
	}
	m_drawInfo = info;
}

/* ================================================ */
/**
 * @brief	jsonファイルからアニメ情報まとめて取得
 */
/* ================================================ */
void Game2DBase::LoadTextureInfo(const char *jsonFile)
{
	// 読み込み
	TextureResourceManager::GetInstance()->LoadTextureInfo(jsonFile);

	// 読み込んだファイルから情報取得
	TextureResourceManager::GetInstance()->GetTextureHandle( jsonFile, m_vTexHandle);
	TextureResourceManager::GetInstance()->GetPlayAnimName( jsonFile, m_vAnimName);
	m_currentAnimTag = TextureResourceManager::GetInstance()->GetDefaultAnimName( jsonFile );
}

/* ================================================ */
/**
 * @brief	テクスチャアンロード
 */
/* ================================================ */
void Game2DBase::DeleteTextureInfo(const char *jsonFile)
{
	TextureResourceManager::GetInstance()->DeleteTextureInfo(jsonFile);
}