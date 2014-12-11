/* ====================================================================== */
/**
 * @brief  �X�R�A�L�^�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SCORE_RECORDER
#define SCORE_RECORDER

#include "Game/Game2DBase.h"
#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Common/CmnNumberCounter.h"

class ScoreRecorder : public TaskUnit
{

public:

	// ���_��������
	enum SCORE_KIND {
		ENEMY_AAA_DEATH,
		ENEMY_BBB_DEATH,
		ENEMY_CCC_DEATH,

		POINT_MAX
	};

	static ScoreRecorder *Create();
	static ScoreRecorder *GetInstance();
	~ScoreRecorder();

	// ������
	void ScoreReset();
	// �X�R�A�ǉ�
	void ScoreEvent( const SCORE_KIND &kind );
	// �X�R�A�擾
	const int32_t GetScore() const;

private:

	ScoreRecorder();

	static ScoreRecorder *s_pInstance;
	uint32_t	m_scoreDetail[POINT_MAX];
	NumberCounter	*m_pNumCounter;
};
#endif