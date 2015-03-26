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

	static const uint32_t COMBO_COUNT_MAX = 10000;

	// ���_��������
	enum SCORE_KIND {
		ENEMY_AAA_DEATH,
		ENEMY_BBB_DEATH,
		ENEMY_CCC_DEATH,
		ENEMY_BOSS_DEATH,

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

	struct STAGE_CLEAR_INFO{
		float		m_userLifeRatio;				// ���[�U�[���C�t�̎c�芄��
		uint32_t	m_userScore;					// �N���A���̃X�R�A
		uint32_t	m_bonusScore;					// �{�[�i�X�X�R�A
		uint32_t	m_hitComboMaxOfStage;			// �e�X�e�[�W�̍ő�Hit��	
		uint32_t	m_scoreDetail[POINT_MAX];		// �X�R�A�ڍ�

		void Init(){
			m_userLifeRatio			= 0.0f;
			m_userScore				= 0;
			m_bonusScore			= 0;
			m_hitComboMaxOfStage	= 0;
			for( uint32_t i = 0; i < NUMBEROF(m_scoreDetail) ;++i ){
				m_scoreDetail[i] = 0;
			}
		}
	};

	static GameRecorder *Create();
	static GameRecorder *GetInstance();
	~GameRecorder();

	// ������
	void InitRecord();

	// �X�e�[�W�N���A���̃��[�U�[���C�t�̊����̃Z�b�g&�擾
	const void SetUserLifeRatio( const float &lifeRatio, const STATE_OF_PROGRESS &stage = STATE_MAX );
	const float GetUserLifeRatio( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// ���[�U�[���w��X�e�[�W���N���A�������ǂ���
	const bool IsUserClear( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// �X�R�A�̉��Z�Ǝ擾 GetScore�͈����Ȃ��Ȃ璼�߂̃X�e�[�W�X�R�A��Ԃ�
	void ScoreEvent( const SCORE_KIND &kind );
	void AddScoreBonus( uint32_t bonusScore, const STATE_OF_PROGRESS &stage = STATE_MAX );
	const int32_t GetScore( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetScoreBonus( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetStageScore( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetTotalScore() const;

	// �A�C�e���擾���̉��Z
	const void AddItem( ItemObject::ITEM_KIND kind );
	const uint32_t GetItemCount( ItemObject::ITEM_KIND kind ) const;

	// Hit����Inc�Ǝ擾
	void IncHitCounter();				// Hit���v���X
	uint32_t GetCurrentHitCounter();	// ���݂̘A��Hit���擾
	uint32_t GetRestHitTime();			// �A��Hit���L���ɂȂ�܂ł̎c����

	// �e�X�e�[�W�̍ő�R���{���擾
	uint32_t GetMaxComboNumOfStage( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// �R���{�p���L�����Ԏ擾
	uint32_t GetLeftTimeOfCombo() const;

	// �Q�[���i�s�󋵂̍X�V�Ǝ擾
	void SetGameStateOfProgress( STATE_OF_PROGRESS nextState );
	const STATE_OF_PROGRESS &GetGameStateOfProgress() const;

protected:
	
	virtual void Update() override;

private:

	GameRecorder();

	static GameRecorder *s_pInstance;

	STATE_OF_PROGRESS	m_gameState;					// �Q�[���i�s�x
	STAGE_CLEAR_INFO	m_clearStageInfo[STATE_MAX];	// �e�X�e�[�W�N���A���̏��
	
	// ���[�U�[�̎擾�A�C�e��(�Q�[������ł̐�����)
	uint32_t			m_getItem[ItemObject::ITEM_KIND_MAX];

	// �ȉ��A�X�e�[�W�N���A���ɃN���A�����
	// �A��HIT��
	uint32_t			m_hitComboNum;					// ���݂̘A��Hit��
	uint32_t			m_hitFailTime_ms;				// ���̘A��Hit���L���ɂȂ鎞��(100~30)
};
#endif