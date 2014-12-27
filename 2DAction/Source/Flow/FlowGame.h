/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[����ʊǗ��N���X
 */
/* ====================================================================== */
#ifndef FLOW_GAME
#define FLOW_GAME

#include "FlowBase.h"

class FlowGame : public FlowBase
{

public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void UpdateFlow() override;

	FlowGame( const std::string &fileName );
	virtual ~FlowGame(void);

	void LoadGameSettings( const char *jsonFile );

	uint32_t					m_gameTimer;		// �Q�[���̃v���C����

	uint32_t					m_gameTimeMax;		// �Q�[���I���܂ł̎���(/fps)
	uint32_t					m_enemyMax;			// �ő�o���G��
	uint32_t					m_enemyFrequency;	// �G�̏o����(10�i�K)
	uint32_t					m_itemMax;			// �ő�o���A�C�e����
	uint32_t					m_itemFrequency;	// �A�C�e���̏o����(10�i�K)
};
#endif

