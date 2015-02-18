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
#include "Item/ItemObject.h"

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

	// �X�R�A�̉��Z�Ǝ擾 GetScore�͈����Ȃ��Ȃ璼�߂̃X�e�[�W�X�R�A��Ԃ�
	void ScoreEvent( const SCORE_KIND &kind );
	const int32_t GetScore( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetTotalScore() const;

	// �A�C�e���擾���̉��Z
	const void AddItem( ItemObject::ITEM_KIND kind );
	const uint32_t GetItemCount( ItemObject::ITEM_KIND kind );

	// �Q�[���i�s�󋵂̍X�V�Ǝ擾
	void SetGameStateOfProgress( STATE_OF_PROGRESS nextState );
	const STATE_OF_PROGRESS &GetGameStateOfProgress() const;

private:

	GameRecorder();

	static GameRecorder *s_pInstance;

	STATE_OF_PROGRESS	m_gameState;				// �Q�[���i�s�x

	uint32_t			m_userScore[STATE_MAX];		// ���[�U�[�X�R�A
	uint32_t			m_scoreDetail[POINT_MAX];	// �X�R�A�ڍ�
	uint32_t			m_getItem[ItemObject::ITEM_KIND_MAX];	// ���[�U�[�̎擾�A�C�e��
};
#endif