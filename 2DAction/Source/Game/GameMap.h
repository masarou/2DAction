/* ====================================================================== */
/**
 * @brief  �}�b�v
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

// �����蔻����s���ۂ̃}�b�v�̕�����
static const uint32_t HIT_AREA_SPLIT_MAX = 5;

class GameMap : public TaskUnit
{
public:
	
	static GameMap *GameMap::CreateGameMap( const Common::GAME_FLOW &currentKind );
	~GameMap(void);

	virtual void Update();
	virtual void DrawUpdate();

	const math::Vector2 GetPlayerStartPos() const;	// �v���C���[�̃X�^�[�g�n�_�擾
	const int32_t GetTileHeight( const math::Vector2 &pos ) const;		// �w��ʒu�̏��̍������擾
	const uint32_t Get2DMortonNumber( const math::Vector2 &pos ) const;	// �����������蔻����s����Ԃ̉��Ԃɂ��邩�����߂�
	const uint32_t GetBelongArea( const math::Vector2 &pos ) const;		// �ʒu��񂩂瓖���蔻���Ԃ����߂�
	const uint32_t GetMapWidth() const;		// �}�b�v�T�C�Y�擾(����)
	const uint32_t GetMapHeight() const;	// �}�b�v�T�C�Y�擾(�c��)

private:

	struct TILE_INFO{
		int32_t		m_tileHandle;
		int32_t		m_tileTileKind;
		int32_t		m_tileHeight;	// �^�C���̍���(���[�U�[�����邩�ۂ�)
		void Init(){
			m_tileHandle = INVALID_VALUE;
			m_tileTileKind = INVALID_VALUE;
			m_tileHeight = 0;
		}
	};

	GameMap( const Common::GAME_FLOW &currentKind );

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
	
	math::Vector2				m_playerStartPos;	// �J�n�n�_
	TEX_INIT_INFO				m_texInfo;
	MAP_INFO					m_mapInfo;			// �}�b�v���
	std::vector<TILE_INFO>		m_vTileInfo;		// �^�C���ꖇ������̏��
};

#endif