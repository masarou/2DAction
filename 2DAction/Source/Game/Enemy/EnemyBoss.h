/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_BOSS__
#define __ENEMY_BOSS__

#include "EnemyBase.h"

class EnemyBoss : public EnemyBase
{
public:

	static EnemyBoss *Create( const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;		// �h����ł̏�����
	virtual uint32_t GetEnemyDefaultHP() const override{return 7000;}	// �G�N���X�̃f�t�H���gHP�擾
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	EnemyBoss( const uint32_t &uniqueID );
	~EnemyBoss(void);

};

#endif //ENEMY_AAA