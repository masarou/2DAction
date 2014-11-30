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
	
	static GameMap *GameMap::CreateGameMap();
	~GameMap(void);

	virtual void Update();
	virtual void DrawUpdate();

	uint32_t GetTileHeight( const math::Vector2 &pos );
	uint32_t GetTileHeight( const uint32_t &posX, const uint32_t &posY );

private:

	struct TILE_INFO{
		int32_t		m_tileHandle;
		int32_t		m_tileTileKind;
		uint32_t	m_tileHeight;	// タイルの高さ(ユーザーが乗れるか否か)
		void Init(){
			m_tileHandle = INVALID_VALUE;
			m_tileTileKind = INVALID_VALUE;
			m_tileHeight = 0;
		}
	};

	GameMap(void);

	//!マップ情報読み込み
	void LoadTextureInfo( const char *jsonFile );
	void LoadMapInfo( const char *jsonFile );

	struct MAP_INFO{
		std::vector<uint32_t>	m_vTileKind;
		uint32_t				m_width;
		uint32_t				m_height;
		void Init(){
			m_vTileKind.clear();
			m_width		= INVALID_VALUE;
			m_height	= INVALID_VALUE;
		}
	};
	
	TEX_INIT_INFO				m_texInfo;
	MAP_INFO					m_mapInfo;		// マップ情報
	std::vector<TILE_INFO>		m_vTileInfo;	// タイル一枚当たりの情報
};

#endif