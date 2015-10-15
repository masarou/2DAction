/* ====================================================================== */
/**
 * @brief  �Q�c�`��DEFINE
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_DRAW2D_DEFINE__
#define __SYSTEM_DRAW2D_DEFINE__

#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "Common/CommonDefine.h"
#include "System/SystemDefine.h"

#define SPLIT_MAX 1024

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

// �����蔻��~
struct CollisionCircle{
	math::Vector2	m_relativePos;		// �摜���S�_����̑��Έʒu
	float			m_radius;			// �~�̔��a����
	CollisionCircle::CollisionCircle(){
		m_relativePos = math::Vector2();
		m_radius = 0.0f;
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
	std::vector< CollisionCircle >	m_collisionArray;	// �����蔻��~
	void Init(){
		m_fileName		= "";
		m_splitNum		= 0;
		m_splitWidth	= 0;
		m_splitHeight	= 0;
		m_sizeWidth		= 0;
		m_sizeHeight	= 0;
		m_animDefault	= "";
		m_vAnimName.clear();
		m_collisionArray.clear();
	}
};

//�`��X�V�̍ۂ̏��
struct TEX_DRAW_INFO{
	std::string			m_fileName;
	math::Vector2		m_scale;			//!<�g�嗦
	math::Vector2		m_posOrigin;		//!<�ʒu���(������W)
	math::Vector2		m_arrangeOrigin;	//!<�g��A��]���̌��_
	bool				m_usePlayerOffset;	//!<�ʒu�␳�̎g�p�L��
	math::Angle			m_rot;				//!<��]���
	uint32_t			m_alpha;			//!<���ߏ��
	Common::PRIORITY	m_prioity;			//!<�`��D��x
	bool				m_drawForce;		//!<�`���K�����邩���ǂ���
	uint32_t			m_belongLv;			// �}�b�v��ł̏������(�����蔻��)
	uint32_t			m_belongIndex;		// ������Ԃ̔ԍ�(�����蔻��)
	TEX_DRAW_INFO::TEX_DRAW_INFO(){
		Init();
	}
	void Init(){
		m_fileName = "";
		m_scale = math::Vector2( 1.0f, 1.0f );
		m_posOrigin = DEFAULT_VECTOR2;
		m_arrangeOrigin = math::Vector2( INVALID_FVALUE, INVALID_FVALUE );
		m_usePlayerOffset = true;
		m_rot = math::Angle();
		m_alpha = 255;
		m_prioity = Common::PRIORITY_NORMAL;
		m_drawForce = false;
		m_belongLv = INVALID_VALUE;
		m_belongIndex = INVALID_VALUE;
	}
};


#endif