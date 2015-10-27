
#ifndef __COMMON_DEFINE__
#define __COMMON_DEFINE__

#include "System/SystemDefine.h"

namespace Common{

	// �����蔻�肪����ł���\����
	enum TYPE_OBJECT{
		TYPE_PLAYER,
		TYPE_ENEMY_SLIME,
		TYPE_ENEMY_SLIME_ANOTHER,
		TYPE_ENEMY_AHRIMAN,
		TYPE_ENEMY_COW,
		TYPE_ENEMY_RANGER,
		TYPE_ENEMY_SLIME_KING,
		TYPE_ENEMY_WIZARD,
		TYPE_WIZARD_CRYSTAL,
		TYPE_DRAGON,
		TYPE_LAST_BOSS,
		TYPE_LAST_BOSS_LEFT,
		TYPE_LAST_BOSS_RIGHT,
		TYPE_ITEM_BULLET,
		TYPE_ITEM_LIFE,
		TYPE_ITEM_DAMAGE,
		TYPE_ITEM_ANTIDOTE,
		TYPE_BLADE_PLAYER,
		TYPE_BLADE_ENEMY,
		TYPE_BULLET_PLAYER,
		TYPE_BULLET_ENEMY,
		TYPE_FIRE,
		TYPE_FIRE_BALL,
		TYPE_POISON,
		TYPE_EXPLOSION_PLAYER,
		TYPE_EXPLOSION_ENEMY,

		TYPE_MAX,
	};

	// �I�u�W�F�N�g���m�̃C�x���g���b�Z�[�W
	enum EVENT_MESSAGE{
		EVENT_HIT_PLAYER,				// �v���C���[�L�����N�^
		EVENT_HIT_ENEMY_SLIME,			// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_SLIME_ANOTHER,	// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_AHRIMAN,		// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_COW,			// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_BOSS,			// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_SLIME_KING,		// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_WIZARD,			// �G�Ƃ̐ڐG
		EVENT_HIT_WIZARD_CRYSTAL,		// �G�Ƃ̐ڐG
		EVENT_HIT_DRAGON,				// �G�Ƃ̐ڐG
		EVENT_HIT_BOSS,					// �G�Ƃ̐ڐG
		EVENT_HIT_BOSS_LEFT,			// �G�Ƃ̐ڐG
		EVENT_HIT_BOSS_RIGHT,			// �G�Ƃ̐ڐG
		EVENT_GET_ITEM_BULLET,			// �A�C�e���擾
		EVENT_GET_ITEM_LIFE,			// �A�C�e���擾
		EVENT_GET_ITEM_DAMAGE,			// �A�C�e���擾
		EVENT_GET_ITEM_ANTIDOTE,		// �A�C�e���擾
		EVENT_HIT_BLADE_PLAYER,			// �v���C���[�̎a���ɓ�������
		EVENT_HIT_BLADE_ENEMY,			// �G�̎a���ɓ�������
		EVENT_HIT_BULLET_PLAYER,		// �v���C���[�̍U���e�ɓ�������
		EVENT_HIT_BULLET_ENEMY,			// �G�̍U���e�ɓ�������
		EVENT_HIT_FIRE,					// �Ή��e��̉���
		EVENT_HIT_FIRE_BALL,			// �Ή��e
		EVENT_HIT_POISON,				// ��
		EVENT_HIT_EXPLOSION_PLAYER,		// �v���C���[�̔����U���ɓ�������
		EVENT_HIT_EXPLOSION_ENEMY,		// �G�̔����U���ɓ�������

		EVENT_ADD_FORCE_MOVE,		// �_���[�W���ł̂ӂ��Ƃ�

		EVENT_MESSAGE_MAX,
	};

	// �����蔻��I�u�W�F�N�g�Ƃ���ɂ���Ĕ�������C�x���g���܂Ƃ߂�����
	struct COLLISION_OBJ
	{
		TYPE_OBJECT		m_type;
		EVENT_MESSAGE	m_eventMessage;
	} static const s_collisionInfo[ TYPE_MAX ] = {
		{	TYPE_PLAYER,				EVENT_HIT_PLAYER				},	// �v���C���[�L�����N�^
		{	TYPE_ENEMY_SLIME,			EVENT_HIT_ENEMY_SLIME			},	// �G�Ƃ̐ڐG
		{	TYPE_ENEMY_SLIME_ANOTHER,	EVENT_HIT_ENEMY_SLIME_ANOTHER	},	// �G�Ƃ̐ڐG
		{	TYPE_ENEMY_AHRIMAN,			EVENT_HIT_ENEMY_AHRIMAN			},	// �G�Ƃ̐ڐG
		{	TYPE_ENEMY_COW,				EVENT_HIT_ENEMY_COW				},	// �G�Ƃ̐ڐG
		{	TYPE_ENEMY_RANGER,			EVENT_HIT_ENEMY_BOSS			},	// �G�Ƃ̐ڐG
		{	TYPE_ENEMY_SLIME_KING,		EVENT_HIT_ENEMY_SLIME_KING		},	// �G�Ƃ̐ڐG
		{	TYPE_ENEMY_WIZARD,			EVENT_HIT_ENEMY_WIZARD			},	// �G�Ƃ̐ڐG
		{	TYPE_WIZARD_CRYSTAL,		EVENT_HIT_WIZARD_CRYSTAL		},	// �G�Ƃ̐ڐG
		{	TYPE_DRAGON,				EVENT_HIT_DRAGON				},	// �G�Ƃ̐ڐG
		{	TYPE_LAST_BOSS,				EVENT_HIT_BOSS					},	// �G�Ƃ̐ڐG
		{	TYPE_LAST_BOSS_LEFT,		EVENT_HIT_BOSS_LEFT				},	// �G�Ƃ̐ڐG
		{	TYPE_LAST_BOSS_RIGHT,		EVENT_HIT_BOSS_RIGHT			},	// �G�Ƃ̐ڐG
		{	TYPE_ITEM_BULLET,			EVENT_GET_ITEM_BULLET			},	// �A�C�e���擾
		{	TYPE_ITEM_LIFE,				EVENT_GET_ITEM_LIFE				},	// �A�C�e���擾
		{	TYPE_ITEM_DAMAGE,			EVENT_GET_ITEM_DAMAGE			},	// �A�C�e���擾
		{	TYPE_ITEM_ANTIDOTE,			EVENT_GET_ITEM_ANTIDOTE			},	// �A�C�e���擾
		{	TYPE_BLADE_PLAYER,			EVENT_HIT_BLADE_PLAYER			},	// �v���C���[�̎a���ɓ�������
		{	TYPE_BLADE_ENEMY,			EVENT_HIT_BLADE_ENEMY			},	// �G�̎a���ɓ�������
		{	TYPE_BULLET_PLAYER,			EVENT_HIT_BULLET_PLAYER			},	// �v���C���[�̍U���e�ɓ�������
		{	TYPE_BULLET_ENEMY,			EVENT_HIT_BULLET_ENEMY			},	// �G�̍U���e�ɓ�������
		{	TYPE_FIRE,					EVENT_HIT_FIRE					},	// �Ή��e��̉���
		{	TYPE_POISON,				EVENT_HIT_POISON				},	// ��
		{	TYPE_FIRE_BALL,				EVENT_HIT_FIRE_BALL				},	// �Ή��e
		{	TYPE_EXPLOSION_PLAYER,		EVENT_HIT_EXPLOSION_PLAYER		},	// �v���C���[�̔����U���ɓ�������
		{	TYPE_EXPLOSION_ENEMY,		EVENT_HIT_EXPLOSION_ENEMY		},	// �G�̔����U���ɓ�������
	};

	// �Q�[�����Ȃɏ�Ԃ�������
	enum GAME_FLOW{
		FLOW_TITLE,			// �^�C�g��
		FLOW_SCORE,			// �X�R�A
		FLOW_POWER_UP,		// ���[�U�[�p���[�A�b�v
		FLOW_STAGE_SEL,		// �J�n�X�e�[�W�I��
		FLOW_STAGE01,		// �X�e�[�W01
		FLOW_STAGE02,		// �X�e�[�W02
		FLOW_STAGE03,		// �X�e�[�W03
		FLOW_STAGE04,		// �X�e�[�W04
		FLOW_STAGE05,		// �X�e�[�W05
		FLOW_STAGE06,		// �X�e�[�W06
		FLOW_STAGE07,		// �X�e�[�W07
		FLOW_STAGE08,		// �X�e�[�W08
		FLOW_STAGE09,		// �X�e�[�W09
		FLOW_STAGE10,		// �X�e�[�W10
		FLOW_RESULT,		// ���U���g
		FLOW_INTERVAL,		// �x�e
		FLOW_RESULT_TOTAL,	// ���ׂẴX�e�[�W���I���Ẵ��U���g
		FLOW_GAME_CLEAR,	// �Q�[���I��
		FLOW_EXIT,			// �Q�[���I��

		FLOW_MAX
	};

	//�`��̎�� �D��
	enum PRIORITY{
		PRIORITY_LOWEST,
		PRIORITY_LOW,
		PRIORITY_BELOW_NORMAL,	// �W���ȉ�
		PRIORITY_NORMAL,		// �W��
		PRIORITY_ABOVE_NORMAL,	// �W���ȏ�
		PRIORITY_HIGH,
		PRIORITY_HIGHEST,
		PRIORITY_SCENE_FADE,
	
		PRIORITY_MAX,
	};

	enum EXTRA_KIND{
		KIND_ENEMY_POS,
		KIND_FORCE_MOVE,

		KIND_MAX,
	};

	struct EX_COMMON{
		float	m_posX;			// ����̃I�u�W�F�N�g�̈ʒu
		float	m_posY;			// ����̃I�u�W�F�N�g�̈ʒu
		void Init(){
			m_posX = 0.0f;
			m_posY = 0.0f;
		}
	};

	struct EX_FORCE_MOVE{
		float	m_posX;			// �����ړ��̕���
		float	m_posY;			// �����ړ��̕���
		float	m_forcePower;	// �����ړ��̑傫��
		void Init(){
			m_posX = 0.0f;
			m_posY = 0.0f;
			m_forcePower = 0.0f;
		}
	};

	union EXTRA_INFO{
		EX_COMMON		m_common;
		EX_FORCE_MOVE	m_forceMove;
	};

	struct CMN_EVENT{
	public:
		EVENT_MESSAGE	m_event;		// �����N���������H
		uint32_t		m_eventValue;	// �ėp�ԍ�
		uint32_t		m_delayTime;	// ���ۂɎ��s�����^�C�~���O(0=�����Ɏ��s)
		void Init(){
			m_event			= EVENT_MESSAGE_MAX;
			m_eventValue	= 0;
			m_delayTime		= 0;
			m_exkind		= KIND_MAX;
		}

		const EXTRA_KIND &GetExInfoKind() const{ return m_exkind; }

		void SetExInfoCmn( const EX_COMMON &info ){
			m_exkind = KIND_ENEMY_POS;
			m_exInfo.m_common = info;
		}
		void SetExInfoForceMove( const EX_FORCE_MOVE &info ){
			m_exkind = KIND_FORCE_MOVE;
			m_exInfo.m_forceMove = info;
		}

		const EX_COMMON &GetExInfoCmn() const{
			if( m_exkind == KIND_ENEMY_POS ){
				return m_exInfo.m_common;
			}
			DEBUG_ASSERT( 0, "���L�̂̎��̂Ɗ��҂��Ă���^���قȂ�܂�" );
			return m_exInfo.m_common;
		}
		const EX_FORCE_MOVE &GetExInfoForceMove() const{
			if( m_exkind == KIND_FORCE_MOVE ){
				return m_exInfo.m_forceMove;
			}
			DEBUG_ASSERT( 0, "���L�̂̎��̂Ɗ��҂��Ă���^���قȂ�܂�" );
			return m_exInfo.m_forceMove;
		}
	private:
		EXTRA_KIND		m_exkind;
		EXTRA_INFO		m_exInfo;		// ���̑����
	};

	// ��ʃN���X���̃p�[�c���
	enum PARTS_TYPE_MENU{
		PARTS_SINGLE_DRAW,	// �A�j�����������x�̃V���v���ȃp�[�c
		PARTS_NUM_COUNTER,	// �^����ꂽ���l��`�悵������p�[�c

		PARTS_MAX,
	};

	// ��ʃN���X�Ŏg�p����p�[�c�̏��\����
	struct PARTS_INFO{
		std::string		m_jsonStr;
		Common::PRIORITY		m_priority;
		math::Vector2	m_pos;
		PARTS_TYPE_MENU	m_type;
		void Init(){
			m_jsonStr = "";
			m_pos = math::Vector2(0,0);
			m_priority = PRIORITY_NORMAL;
			m_type = PARTS_SINGLE_DRAW;
		}
	};

	enum ENEMY_KIND{
		ENEMY_KIND_SLIME,
		ENEMY_KIND_SLIME_ANOTHER,
		ENEMY_KIND_AHRIMAN,
		ENEMY_KIND_COW,
		ENEMY_KIND_RANGER,
		ENEMY_KIND_SLIME_KING,
		ENEMY_KIND_WIZARD,
		ENEMY_KIND_DRAGON,
		ENEMY_KIND_LAST_BOSS,

		ENEMY_KIND_MAX,
	};

	// ���j�[�N�����X�^�[�z��(��{�I�Ɉ�̂������łȂ�)
	static ENEMY_KIND s_uniqueEnemyKind[] = {
		Common::ENEMY_KIND_WIZARD,
		Common::ENEMY_KIND_DRAGON,
		Common::ENEMY_KIND_RANGER,
	};
	
	// ���C�g�����X�^�[�z��(�����̑��݂��邱�Ƃ���)
	static ENEMY_KIND s_lightEnemyKind[] = {
		Common::ENEMY_KIND_SLIME,
		Common::ENEMY_KIND_SLIME_ANOTHER,
		Common::ENEMY_KIND_AHRIMAN,
		Common::ENEMY_KIND_COW,
	};

	enum ITEM_KIND{
		ITEM_KIND_RAPID_BULLET,	// �A�ˑ��xUP
		ITEM_KIND_LIFE_UP,		// ���C�t��
		ITEM_KIND_DAMAGE_UP,	// �e�̈З�UP
		ITEM_KIND_ANTIDOTE,		// ��ō�

		ITEM_KIND_MAX,
	};

	enum ENEMY_AI{
		AI_NONE,		// �������Ȃ�
		AI_SEARCHING,	// �v���C���[��T���Ă���
		AI_MOVE_PLAYER,	// �v���C���[�ɋ߂Â�(�̓�����U��)
		AI_DASH_TACKLE,	// �v���C���[�ɍ����̓�����
		AI_SHOOTING,	// �������U��(�������U��)

		// �ȉ��A�{�X�pAI
		AI_SEARCHING_SLIME_KING,	// �v���C���[�T��(�X���C���L���O)
		AI_MOVE_PLAYER_SLIME_KING,	// �v���C���[�ɋ߂Â�(�X���C���L���O)
		AI_ATTACK_NEAR,				// �����W���[AI
		AI_ATTACK_WIZARD,			// ���@�g��AI
		AI_ATTACK_DRAGON,			// �h���S��AI
		AI_ATTACK_LAST_BOSS,		// ���X�{�XAI
		AI_ATTACK_LAST_BOSS_HAND,	// ���X�{�X����AI
		AI_MAX,
	};
	
	// ���L��
	enum OWNER_TYPE{
		OWNER_PLAYER,
		OWNER_ENEMY,

		OWNER_MAX,
	};

	// �o�g���|�C���g���g�p���Ă����邱�Ƃ̂ł���v���C���[�X�e�[�^�X
	const uint32_t STATUS_LEVEL_MAX = 9;
	enum PLAYER_BASE_STATE{
		BASE_STATE_LIFE,		// ���C�t�̍ő�l�����߂�
		BASE_STATE_MOVE_SPEED,	// �_�b�V������
		BASE_STATE_DEFFENCE,	// ��_���[�W�����߂�
		BASE_STATE_BULLET_SPD,	// �}�V���K���̊Ԋu
		BASE_STATE_BULLET_DMG,	// �}�V���K���̃_���[�W
		BASE_STATE_BLADE_LEVEL,	// �a���̃_���[�W
		BASE_STATE_CONTINUE,	// �R���e�B�j���[(�r������ĊJ)

		BASE_STATE_MAX,
	};

	// �ȉ��A�Z�[�u�f�[�^�Ƃ��ĕێ�����
	const uint32_t RANKING_RECORD_MAX = 5;
	struct SAVE_DATA{
		bool		m_isFirst;			// ���߂Ẵp���[�A�b�v��ʂ��ǂ���
		bool		m_isFirstGamePlay;	// ���߂ẴQ�[���v���C���ǂ���
		bool		m_isClearPhase01;	// �t�F�[�Y1�N���A�t���O
		bool		m_isClearPhase02;	// �t�F�[�Y2�N���A�t���O
		uint32_t	m_battlePoint;	// �Q�[���v���C�Ŏ�ɂ��Ă��|�C���g(�p���[�A�b�v�ɕK�v)
		uint32_t	m_scoreRanking[RANKING_RECORD_MAX];
		uint32_t	m_playerBaseStateLv[BASE_STATE_MAX];
	};
}
#endif