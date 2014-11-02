/* ====================================================================== */
/**
 * @brief  �Q�c�`��DEFINE
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_DRAW2D_DEFINE
#define SYSTEM_DRAW2D_DEFINE

#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "System/SystemDefine.h"

#define SPLIT_MAX 80

//�`��̎�� �D��
enum KIND_2D{
	KIND_BACK,
	KIND_NORMAL,
	KIND_FILTER,

	KIND_MAX,
};

enum PRIORITY{
	PRIORITY_LOWEST,
	PRIORITY_LOW,
	PRIORITY_BELOW_NORMAL,	// �W���ȉ�
	PRIORITY_NORMAL,		// �W��
	PRIORITY_ABOVE_NORMAL,	// �W���ȏ�
	PRIORITY_HIGH,
	PRIORITY_HIGHEST,

	PRIORITY_MAX,
};

//!�A�j���[�V�����̏��\����
struct ANIM_INFO{
	std::string				m_animTag;		// �A�j�����ʎq
	std::string				m_nextAnimTag;	// �Đ��I�����Ɏ��ɍĐ��������A�j��������ΐݒ�
	std::vector<uint32_t>	m_vPlayIndex;	// �A�j���̍Đ��G�̔ԍ�
	uint32_t				m_frameSpeed;	// �Đ��X�s�[�h
	void Init(){
		m_animTag = "";
		m_nextAnimTag = "";
		m_vPlayIndex.clear();
		m_frameSpeed = 10;
	}
};

//�e�N�X�`����ǂݍ��ނƂ��̏��
struct TEX_INIT_INFO{
	std::string m_fileName;
	uint32_t	m_splitNum;
	uint32_t	m_splitWidth;	//��������
	uint32_t	m_splitHeight;	//�c������
	uint32_t	m_sizeWidth;	//���T�C�Y
	uint32_t	m_sizeHeight;	//�c�T�C�Y
	std::string m_animDefault;	//�f�t�H���g�A�j��
	std::vector<ANIM_INFO> m_vAnimName;	//�A�j�����
	void Init(){
		m_fileName		= "";
		m_splitNum		= 0;
		m_splitWidth	= 0;
		m_splitHeight	= 0;
		m_sizeWidth		= 0;
		m_sizeHeight	= 0;
		m_animDefault	= "";
		m_vAnimName.clear();
	}
};

//�`��X�V�̍ۂ̏��
struct TEX_DRAW_INFO{
	std::string		m_fileName;
	math::Vector2	m_scale;			//!<�g�嗦
	math::Vector2	m_pos;				//!<�ʒu���
	math::Vector2	m_arrangeOrigin;	//!<�g��A��]���̌��_
	bool			m_usePlayerOffset;	//!<�ʒu�␳�̎g�p�L��
	math::Angle		m_rot;				//!<��]���
	uint32_t		m_alpha;			//!<���ߏ��
	KIND_2D			m_kind2D;			//!<2D�̎��
	PRIORITY		m_prioity;			//!<�`��D��x
	void Init(){
		m_fileName = "";
		m_scale = math::Vector2( 1.0f, 1.0f );
		m_pos = math::Vector2();
		m_arrangeOrigin = math::Vector2( INVALID_FVALUE, INVALID_FVALUE );
		m_usePlayerOffset = true;
		m_rot = math::Angle();
		m_alpha = 255;
		m_kind2D = KIND_NORMAL;
		m_prioity = PRIORITY_NORMAL;
	}
};


#endif