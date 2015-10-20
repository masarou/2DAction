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
		ENEMY_SLIME_DEATH,
		ENEMY_SLIME_ANOTHER_DEATH,
		ENEMY_AHRIMAN_DEATH,
		ENEMY_COW_DEATH,
		ENEMY_BOSS_DEATH,
		ENEMY_SLIME_KING_DEATH,
		ENEMY_WIZARD_DEATH,
		ENEMY_DRAGON_DEATH,
		ENEMY_LAST_BOSS_DEATH,
		ENEMY_LAST_BOSS_RIGHT_DEATH,
		ENEMY_LAST_BOSS_LEFT_DEATH,

		POINT_MAX
	};

	// �Q�[���̐i�s�x
	enum STATE_OF_PROGRESS{
		STATE_STAGE01,
		STATE_STAGE02,
		STATE_STAGE03,
		STATE_STAGE04,	// �{�X1
		STATE_STAGE05,
		STATE_STAGE06,
		STATE_STAGE07,
		STATE_STAGE08,	// �{�X2
		STATE_STAGE09,
		STATE_STAGE10,	// �{�X3

		STATE_MAX,
	};

	struct BATTLE_POINT_INFO{
		uint32_t m_scoreUnder;
		uint32_t m_battlePoint;

		void Init(){
			m_scoreUnder = 0;
			m_battlePoint = 0;
		}
	};

	struct STAGE_CLEAR_INFO{
		float		m_userLifeRatio;				// ���[�U�[���C�t�̎c�芄��
		uint32_t	m_userScore;					// �N���A���̃X�R�A
		uint32_t	m_bonusScore;					// �{�[�i�X�X�R�A
		uint32_t	m_hitComboMaxOfStage;			// �e�X�e�[�W�̍ő�Hit��	
		uint32_t	m_scoreDetail[POINT_MAX];		// �X�R�A�ڍ�
		std::vector< BATTLE_POINT_INFO > m_clearPoint;				// �N���A�[���ɖႦ��o�g���|�C���g

		void Init(){
			m_userLifeRatio			= 0.0f;
			m_userScore				= 0;
			m_bonusScore			= 0;
			m_hitComboMaxOfStage	= 0;
			for( uint32_t i = 0; i < NUMBEROF(m_scoreDetail) ;++i ){
				m_scoreDetail[i] = 0;
			}
			m_clearPoint.clear();
		}
	};

	static GameRecorder *Create();
	static GameRecorder *GetInstance();
	~GameRecorder();

	// ������
	void InitRecord();
	
	// �r������v���C���ǂ���
	void SetContinueFlag(){ m_isContinuePlay = true; }
	const bool &IsContinue() const{ return m_isContinuePlay; }

	// �S�N���A�t���OON
	void SetClearAll(){ m_isAllClear = true; }
	const bool &IsClearAll() const{ return m_isAllClear; }

	// �X�e�[�W�N���A���̃��[�U�[���C�t�̊����̃Z�b�g&�擾
	const void SetUserLifeRatio( const float &lifeRatio, const STATE_OF_PROGRESS &stage = STATE_MAX );
	const float GetUserLifeRatio( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// ���[�U�[���w��X�e�[�W���N���A�������ǂ���
	const bool IsUserClear( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// �X�R�A�̉��Z�Ǝ擾 GetScore�͈����Ȃ��Ȃ璼�߂̃X�e�[�W�X�R�A��Ԃ�
	void ScoreEvent( const SCORE_KIND &kind, const uint32_t &tmpInfo = 0 );
	void AddScoreBonus( uint32_t bonusScore, const STATE_OF_PROGRESS &stage = STATE_MAX );
	const int32_t GetScore( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetScoreBonus( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetStageScore( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetTotalScore() const;

	// �A�C�e���擾���̉��Z
	const void AddItem( Common::ITEM_KIND kind );
	const uint32_t GetItemCount( Common::ITEM_KIND kind ) const;

	// Hit����Inc�Ǝ擾
	void IncHitCounter();				// Hit���v���X
	uint32_t GetCurrentHitCounter();	// ���݂̘A��Hit���擾

	// �e�X�e�[�W�̍ő�R���{���擾
	uint32_t GetMaxComboNumOfStage( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// �e�X�e�[�W�N���A���ɖႦ��o�g���|�C���g�̏��
	void SetClearBattlePoint( const uint32_t &score, const uint32_t &point, const STATE_OF_PROGRESS &stage = STATE_MAX );
	uint32_t GetClearBattlePoint( const uint32_t &point, const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// �ۑ�����Ă��邷�ׂẴV�[���̎擾�|�C���g��Ԃ�
	uint32_t GetClearBattlePointAll() const;

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

	bool				m_isContinuePlay;				// �r������̃v���C���ǂ���
	bool				m_isAllClear;					// �S�N���A�t���O
	STATE_OF_PROGRESS	m_gameState;					// �Q�[���i�s�x
	STAGE_CLEAR_INFO	m_clearStageInfo[STATE_MAX];	// �e�X�e�[�W�N���A���̏��
	
	// ���[�U�[�̎擾�A�C�e��(�Q�[������ł̐�����)
	uint32_t			m_getItem[Common::ITEM_KIND_MAX];

	// �ȉ��A�X�e�[�W�N���A���ɃN���A�����
	// �A��HIT��
	uint32_t			m_hitComboNum;					// ���݂̘A��Hit��
	uint32_t			m_hitFailTime_ms;				// ���̘A��Hit���L���ɂȂ鎞��(100~30)
};
#endif