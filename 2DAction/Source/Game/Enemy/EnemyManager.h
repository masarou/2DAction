/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/Task/SystemTaskUnit.h"
#include "System/SystemDefine.h"
#include "EnemyBase.h"

class EnemyManager : TaskUnit
{
public:

	static EnemyManager *GetInstance();
	void CreateEnemy( Common::ENEMY_KIND kind );	// �G�L��������

protected:

	bool Init() override;
	void Update() override;
	bool DieMain() override;

private:

	EnemyManager(void);
	~EnemyManager(void);



	static EnemyManager		*m_pInstance;
	std::vector<EnemyBase*> m_enemyArray;	// �G�Ǘ�

};

