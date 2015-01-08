/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_BBB
#define ENEMY_BBB

#include "EnemyBase.h"

class AttackGun;

class EnemyBBB : public EnemyBase
{
public:

	static EnemyBBB *Create( const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;		// �h����ł̏�����
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;
	virtual uint32_t GetEnemyDefaultHP() const override{return 100;}	// �G�N���X�̃f�t�H���gHP�擾

private:

	EnemyBBB( const uint32_t &uniqueID );
	~EnemyBBB(void);

};

#endif ENEMY_BBB