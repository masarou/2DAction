/* ====================================================================== */
/**
 * @brief  ���b�Z�[�W�C�x���g���Ǘ�����N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef MESSAGE_MANAGER
#define MESSAGE_MANAGER

#include <map>
#include "Common/CommonDefine.h"

class SystemMessageUnit;

class SystemMessageManager
{

public:

	static SystemMessageManager *Create();
	static SystemMessageManager *GetInstance();

	// ����̃^�X�N�Ƀ��b�Z�[�W�C�x���g��Push
	bool PushMessage( const uint32_t &uniqueId, const Common::CMN_EVENT &kind );

	// ����̃^�X�N���폜
	void AddMessagTask( SystemMessageUnit *unit );
	void RemoveMessagTask( SystemMessageUnit *unit );

	// �w��ID�����Ƃ��Ԃ��Ă��邩�`�F�b�N
	bool IsNumberOverlapID( const uint32_t &checkID ) const;

	// �o�^���Ă���MessageTask�̃C�x���g�����J�n
	void StartMessageEvent();
	
	// �q�̃N���X�̉��
	static void DeleteMessageManager();

private:

	SystemMessageManager(void);
	virtual ~SystemMessageManager(void);

	static SystemMessageManager *s_pInstance;

	std::map<uint32_t, SystemMessageUnit*>	m_messageUnitMap;

};

#endif