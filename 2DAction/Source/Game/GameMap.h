/* ====================================================================== */
/**
 * @brief  マップ
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_GAME_MAP__
#define __SYSTEM_GAME_MAP__

#include "Common/CommonDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "Game2DBase.h"

// 当たり判定を行う際のマップの分割数
static const uint32_t HIT_AREA_SPLIT_MAX = 5;

class GameMap : public TaskUnit
{
public:
	
	static GameMap *GameMap::CreateGameMap( const Common::GAME_FLOW &currentKind );
	~GameMap(void);

	virtual void Update();
	virtual void DrawUpdate();

	const math::Vector2 GetPlayerStartPos() const;	// プライヤーのスタート地点取得
	const int32_t GetTileHeight( const math::Vector2 &pos ) const;		// 指定位置の床の高さを取得
	const uint32_t Get2DMortonNumber( const math::Vector2 &pos ) const;	// 引数が当たり判定を行う空間の何番にいるかを求める
	const uint32_t GetBelongArea( const math::Vector2 &pos ) const;		// 位置情報から当たり判定空間を求める
	const uint32_t GetMapWidth() const;		// マップサイズ取得(横幅)
	const uint32_t GetMapHeight() const;	// マップサイズ取得(縦幅)

private:

	struct TILE_INFO{
		int32_t		m_tileHandle;
		int32_t		m_tileTileKind;
		int32_t		m_tileHeight;	// タイルの高さ(ユーザーが乗れるか否か)
		void Init(){
			m_tileHandle = INVALID_VALUE;
			m_tileTileKind = INVALID_VALUE;
			m_tileHeight = 0;
		}
	};

	GameMap( const Common::GAME_FLOW &currentKind );

	//!マップ情報読み込み
	void LoadTextureInfo( const char *jsonFile );
	void LoadMapInfo( const char *jsonFile );

	struct MAP_INFO{
		std::vector<uint32_t>	m_vTileKind;
		uint32_t				m_width;		// 横にタイルが何枚あるか
		uint32_t				m_height;		// 縦にタイルが何枚あるか
		void Init(){
			m_vTileKind.clear();
			m_width		= INVALID_VALUE;
			m_height	= INVALID_VALUE;
		}
	};
	
	math::Vector2				m_playerStartPos;	// 開始地点
	TEX_INIT_INFO				m_texInfo;
	MAP_INFO					m_mapInfo;			// マップ情報
	std::vector<TILE_INFO>		m_vTileInfo;		// タイル一枚当たりの情報
};

#endif