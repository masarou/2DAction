
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

		CMN_EVENT_MAX,
	};
	struct CMN_EVENT{
		EVENT_MESSAGE	m_event;		// �����N���������H
		uint32_t		m_eventValue;	// �ėp�ԍ�
	};

	enum ENEMY_KIND{
		KIND_AAA,
		KIND_BBB,
		KIND_CCC,
		KIND_DDD,

		KIND_MAX,
	};


	enum ENEMY_AI{
		AI_SEARCHING,	// �v���C���[��T���Ă���
		AI_MOVE_PLAYER,	// �v���C���[�ɋ߂Â�(�̓�����U��)
		AI_SHOOTING,	// �������U��(�������U��)

		AI_MAX,
	};

	struct SAVE_SCORE{
		uint32_t m_scoreTimeAttack[5];
		uint32_t m_scoreEndless[5];
	};
}
#endif