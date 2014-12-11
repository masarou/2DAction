/* ====================================================================== */
/**
 * @brief  �}�b�v
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

// �����蔻����s���ۂ̃}�b�v�̕�����
static const uint32_t HIT_AREA_SPLIT_MAX = 6;

class GameMap : public TaskUnit
{
public:
	
	static GameMap *GameMap::CreateGameMap();
	~GameMap(void);

	virtual void Update();
	virtual void DrawUpdate();

	const uint32_t GetTileHeight( const math::Vector2 &pos ) const;
	const uint32_t GetTileHeight( const uint32_t &posX, const uint32_t &posY ) const;
	
	const uint32_t Get2DMortonNumber( const math::Vector2 &pos ) const;	// �����������蔻����s����Ԃ̉��Ԃɂ��邩�����߂�
	const uint32_t GetBelongArea( const math::Vector2 &pos ) const;		// �ʒu��񂩂瓖���蔻���Ԃ����߂�

private:

	struct TILE_INFO{
		int32_t		m_tileHandle;
		int32_t		m_tileTileKind;
		uint32_t	m_tileHeight;	// �^�C���̍���(���[�U�[�����邩�ۂ�)
		void Init(){
			m_tileHandle = INVALID_VALUE;
			m_tileTileKind = INVALID_VALUE;
			m_tileHeight = 0;
		}
	};

	GameMap(void);

	//!�}�b�v���ǂݍ���
	void LoadTextureInfo( const char *jsonFile );
	void LoadMapInfo( const char *jsonFile );

	struct MAP_INFO{
		std::vector<uint32_t>	m_vTileKind;
		uint32_t				m_width;		// ���Ƀ^�C�����������邩
		uint32_t				m_height;		// �c�Ƀ^�C�����������邩
		void Init(){
			m_vTileKind.clear();
			m_width		= INVALID_VALUE;
			m_height	= INVALID_VALUE;
		}
	};
	
	TEX_INIT_INFO				m_texInfo;
	MAP_INFO					m_mapInfo;		// �}�b�v���
	std::vector<TILE_INFO>		m_vTileInfo;	// �^�C���ꖇ������̏��
};

#endif