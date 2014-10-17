/* ====================================================================== */
/**
 * @brief  マップ
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_GAME_MAP
#define SYSTEM_GAME_MAP

#include "System/Task/SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "Game2DBase.h"

class GameMap : public TaskUnit
{
public:

	GameMap(void);
	~GameMap(void);

	virtual void Update();
	virtual void DrawUpdate();

private:

	struct TILE_INFO{
		int32_t m_tileHandle;
		bool m_walkOver;
		void Init(){
			m_tileHandle = INVALID_VALUE;
			m_walkOver = true;
		}
	};

	//!マップ情報読み込み
	void LoadTextureInfo( const char *jsonFile );

	
	TEX_INIT_INFO				m_texInfo;
	std::vector<TILE_INFO>		m_vTileInfo;
};

#endif