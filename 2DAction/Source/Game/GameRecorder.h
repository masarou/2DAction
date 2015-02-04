/* ====================================================================== */
/**
 * @brief	�t���[���܂������f�[�^�u����
 *			�X�R�A��v���C���[�̃X�e�[�^�X���
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SCORE_RECORDER__
#define __SCORE_RECORDER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"

class GameRecorder : public TaskUnit
{

public:

	// ���_��������
	enum SCORE_KIND {
		ENEMY_AAA_DEATH,
		ENEMY_BBB_DEATH,
		ENEMY_CCC_DEATH,

		POINT_MAX
	};

	// �Q�[���̐i�s�x
	enum STATE_OF_PROGRESS{
		STATE_TITLE,
		STATE_STAGE01,
		STATE_STAGE02,
		STATE_STAGE03,

		STATE_MAX,
	};

	static GameRecorder *Create();
	static GameRecorder *GetInstance();
	~GameRecorder();

	// ������
	void InitRecord();

	// �X�R�A�̉��Z�Ǝ擾
	void ScoreEvent( const SCORE_KIND &kind );
	const int32_t GetScore() const;

	// �Q�[���i�s�󋵂̍X�V�Ǝ擾
	void SetGameStateOfProgress( STATE_OF_PROGRESS nextState );
	const STATE_OF_PROGRESS &GetGameStateOfProgress() const;

private:

	GameRecorder();

	static GameRecorder *s_pInstance;

	STATE_OF_PROGRESS	m_gameState;				// �Q�[���i�s�x

	uint32_t			m_userScore;				// ���[�U�[�X�R�A
	uint32_t			m_scoreDetail[POINT_MAX];	// �X�R�A�ڍ�
};
#endif