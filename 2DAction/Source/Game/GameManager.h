/* ====================================================================== */
/**
 * @brief  �G�̐�����A�C�e��������ݒ�ɂȂ���ĊǗ�����N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef GAME_MANAGER
#define GAME_MANAGER

#include "System/Task/SystemTaskUnit.h"

class GameManager : public TaskUnit
{
public:

	GameManager(void);
	~GameManager(void);

	static GameManager *CreateGameManager();

	// �������Ԃ��߂������ǂ���
	bool	IsGameOver() const;

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	
	void LoadGameSettings( const char *jsonFile );					// �Q�[�����̓ǂݍ���
	bool IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency );	// �G�̐������f
	bool IsCreateItem( uint32_t itemLimit, uint32_t frequency );	// �A�C�e���̐������f

	uint32_t		m_gameTimer;		// �Q�[���̃v���C����

	uint32_t		m_gameTimeMax;		// �Q�[���I���܂ł̎���(/fps)
	uint32_t		m_enemyMax;			// �ő�o���G��
	uint32_t		m_enemyFrequency;	// �G�̏o����(10�i�K0~9)
	uint32_t		m_itemMax;			// �ő�o���A�C�e����
	uint32_t		m_itemFrequency;	// �A�C�e���̏o����(10�i�K0~9)

};

#endif