/* ====================================================================== */
/**
 * @brief  jsonに紐づいたTexリソース管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_DRAW_RESOURCE_MANAGER__
#define __SYSTEM_DRAW_RESOURCE_MANAGER__

#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class TextureResourceManager
{
public:


	static void *Create();
	static TextureResourceManager *GetInstance();

	void LoadTextureInfo( const char *jsonFile );
	void DeleteTextureInfo( const char *jsonFile );

	// 読み込んだテクスチャの画像情報取得
	const TEX_INIT_INFO &GetLoadTextureInfo( const char *jsonFile ) const;
	// 読み込んだテクスチャの描画Handle取得
	void GetTextureHandle( const char *jsonFile, std::vector<int32_t> &vHandle ) const;
	// 読み込んだテクスチャのデフォルトアニメ名取得
	const std::string GetDefaultAnimName( const char *jsonFile ) const;
	// 読み込んだテクスチャの再生アニメ名取得
	void GetPlayAnimName( const char *jsonFile, std::vector<std::string> &vAnim ) const;
	// frameの更新とframe数から適切なHandleの要素番号を返す
	const int32_t GetAnimHandleIndex( const char *jsonFile, std::string &animName, uint32_t &frame ) const;

	// クラスの解放
	void DeleteResourceManager();

private:

	struct RESOURCE_TEX{
		std::string		m_jsonFile;
		int32_t			m_readCounter;
		TEX_INIT_INFO	m_texInfo;
		int32_t			m_texHandle[SPLIT_MAX];
		void Init(){
			m_jsonFile = "";
			m_readCounter = 0;
			m_texInfo.Init();
			for(uint32_t i = 0; i < NUMBEROF(m_texHandle); ++i){
				m_texHandle[i] = INVALID_VALUE;
			}
		}
	};
	
	TextureResourceManager(void);
	~TextureResourceManager(void);

	static TextureResourceManager	*s_pInstance;
	std::vector<RESOURCE_TEX>		m_vRecource2D;

};
#endif //SYSTEM_DRAW_RESOURCE_MANAGER
