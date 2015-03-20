
#ifndef __COMMON_DEFINE__
#define __COMMON_DEFINE__

#include "System/SystemDefine.h"

namespace Common{

	// �����蔻�肪����ł���\����
	enum TYPE_OBJECT{
		TYPE_PLAYER,
		TYPE_EVENMY_AAA,
		TYPE_EVENMY_BBB,
		TYPE_EVENMY_CCC,

		TYPE_ITEM_BULLET,
		TYPE_ITEM_LIFE,
		TYPE_ITEM_DAMAGE,
		TYPE_BLADE_PLAYER,
		TYPE_BLADE_ENEMY,
		TYPE_BULLET_PLAYER,
		TYPE_BULLET_ENEMY,

		TYPE_MAX,
	};

	enum EVENT_MESSAGE{
		EVENT_HIT_PLAYER,		// �v���C���[�L�����N�^
		EVENT_HIT_ENEMY_AAA,	// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_BBB,	// �G�Ƃ̐ڐG
		EVENT_HIT_ENEMY_CCC,	// �G�Ƃ̐ڐG

		EVENT_GET_ITEM_BULLET,	// �A�C�e���擾
		EVENT_GET_ITEM_LIFE,	// �A�C�e���擾
		EVENT_GET_ITEM_DAMAGE,	// �A�C�e���擾
		EVENT_HIT_BLADE_PLAYER,	// �v���C���[�̎a���ɓ�������
		EVENT_HIT_BLADE_ENEMY,	// �G�̎a���ɓ�������
		EVENT_HIT_BULLET_PLAYER,// �v���C���[�̍U���e�ɓ�������
		EVENT_HIT_BULLET_ENEMY,	// �G�̍U���e�ɓ�������
		EVENT_SHOOT_BULLET,		// �U���e�𔭎�

		// ���̑�

		EVENT_MESSAGE_MAX,
	};

	// �Q�[�����Ȃɏ�Ԃ�������
	enum GAME_FLOW{
		FLOW_TITLE,			// �^�C�g��
		FLOW_SCORE,			// �X�R�A
		FLOW_STAGE01,		// �X�e�[�W01
		FLOW_STAGE02,		// �X�e�[�W02
		FLOW_STAGE03,		// �X�e�[�W03
		FLOW_RESULT,		// ���U���g
		FLOW_INTERVAL,		// �x�e
		FLOW_RESULT_TOTAL,	// ���ׂẴX�e�[�W���I���Ẵ��U���g
		FLOW_EXIT,			// �Q�[���I��

		FLOW_MAX
	};

	struct CMN_EVENT{
		EVENT_MESSAGE	m_event;		// �����N���������H
		uint32_t		m_eventValue;	// �ėp�ԍ�
		uint32_t		m_delayTime;	// ���ۂɎ��s�����^�C�~���O(0=�����Ɏ��s)
		void			*m_exInfo;		// ���ɕK�v�ȏ�񂪂����
		void Init(){
			m_event			= EVENT_MESSAGE_MAX;
			m_eventValue	= INVALID_VALUE;
			m_delayTime		= 0;
			m_exInfo		= NULL;
		}
	};

	// ��ʃN���X�Ŏg�p����p�[�c�̏��\����
	struct PARTS_INFO{
		math::Vector2 m_pos;
		void Init(){
			m_pos = math::Vector2(0,0);
		}
	};

	enum ENEMY_KIND{
		ENEMY_KIND_AAA,
		ENEMY_KIND_BBB,
		ENEMY_KIND_CCC,
		ENEMY_KIND_DDD,

		ENEMY_KIND_MAX,
	};


	enum ENEMY_AI{
		AI_NONE,		// �������Ȃ�
		AI_SEARCHING,	// �v���C���[��T���Ă���
		AI_MOVE_PLAYER,	// �v���C���[�ɋ߂Â�(�̓�����U��)
		AI_SHOOTING,	// �������U��(�������U��)

		AI_MAX,
	};
	
	// ���L��
	enum OWNER_TYPE{
		OWNER_PLAYER,
		OWNER_ENEMY,

		OWNER_MAX,
	};

	const uint32_t RANKING_RECORD_MAX = 5;
	struct SAVE_SCORE{
		uint32_t m_scoreTimeAttack[RANKING_RECORD_MAX];
		uint32_t m_scoreEndless[RANKING_RECORD_MAX];
	};
}
#endif