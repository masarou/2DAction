/* ====================================================================== */
/**
 * @brief  ���b�Z�[�W�C�x���g���Ǘ�����N���X
 *
 * @note
 *		SystemMessageUnit���p�������N���X���o�^�����Manager�N���X
 *		���̃N���X����̃C�x���g��ς�ŁA����̃^�C�~���O��
 *		SystemMessageUnit��EventUpdate���Ă�Ŋe�N���X�ɒm�点�Ă�����
 */
/* ====================================================================== */

#ifndef __MESSAGE_MANAGER__
#define __MESSAGE_MANAGER__

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

	// ����̃^�X�N�̓o�^�ƍ폜
	void AddMessagTask( SystemMessageUnit *unit );
	void RemoveMessagTask( SystemMessageUnit *unit );

	// �w��ID�����Ƃ��Ԃ��Ă��邩�`�F�b�N
	bool IsNumberOverlapID( const uint32_t &checkID ) const;

	// �o�^���Ă���MessageTask�̃C�x���g�����J�n
	void StartMessageEvent();
	
	// ���̃N���X�̉��
	static void DeleteMessageManager();

private:

	SystemMessageManager(void);
	virtual ~SystemMessageManager(void);

	static SystemMessageManager *s_pInstance;

	std::map<uint32_t, SystemMessageUnit*>	m_messageUnitMap;

};

#endif