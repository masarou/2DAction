
#ifndef _COMMON_DEFINE_
#define _COMMON_DEFINE_

#include "System/SystemDefine.h"

namespace Common{

	enum EVENT_MESSAGE{
		// �v���C���[�L�����N�^
		EVENT_HIT_ENEMY,	// �G�Ƃ̐ڐG
		EVENT_GET_ITEM,		// �A�C�e���擾

		// �G�L�����N�^�[
		EVENT_HIT_BULLET,	// �U���e�ɓ�������

		// ���̑�

		EVENT_MESSAGE_MAX,
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

	enum ENEMY_KIND{
		KIND_AAA,
		KIND_BBB,
		KIND_CCC,
		KIND_DDD,

		KIND_MAX,
	};


	enum ENEMY_AI{
		AI_NONE,		// �������Ȃ�
		AI_SEARCHING,	// �v���C���[��T���Ă���
		AI_MOVE_PLAYER,	// �v���C���[�ɋ߂Â�(�̓�����U��)
		AI_SHOOTING,	// �������U��(�������U��)

		AI_MAX,
	};


	const uint32_t RANKING_RECORD_MAX = 5;
	struct SAVE_SCORE{
		uint32_t m_scoreTimeAttack[RANKING_RECORD_MAX];
		uint32_t m_scoreEndless[RANKING_RECORD_MAX];
	};
}
#endif