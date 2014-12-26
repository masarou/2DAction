
#ifndef CMN_GAME_UTILITY
#define CMN_GAME_UTILITY

#include "Common/CommonDefine.h"
#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "Math/MathUtility.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"


class EnemyAIBase;

void AddPlayerOffsetPos( math::Vector2 &pos );

const math::Vector2 &GetPlayerOffsetPos();

// ��̏�����Ԃ������蔻�肪�K�v���ǂ�������
const bool IsSameBelongArea( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// �`��ʒu���d�Ȃ��Ă��邩�ǂ���(�����蔻��)
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// �摜�̒��S�ʒu�����߂�
math::Vector2 GetCenterPos( const TEX_DRAW_INFO &drawInfo );

// �}�b�v��ł̏�����Ԃ����߂�
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex );

// �}�b�v��ł̍������擾
const uint32_t GetMapHeight( const uint32_t &posX, const uint32_t &posY );
const uint32_t GetMapHeight( const math::Vector2 &pos );

// ��ʏ�ɕ\�����Ă���I�u�W�F�N�g���ǂ���
bool IsPositionInWindowArea( const TEX_DRAW_INFO &texInfo );
bool IsPositionInWindowArea( const int32_t &xx, const int32_t &yy );

// �w���AI�N���X�̐���
EnemyAIBase *ChangeEnemyAI( Common::ENEMY_AI nextAI );

// �v���C���[�̈ʒu�����擾
math::Vector2 GetPlayerPos();

// �w��͈͓̔��̃����_���Ȑ�����Ԃ�
int32_t GetRandamValue( const int32_t &max, const int32_t &min);
float GetRandamValueFloat( const int32_t &max, const int32_t &min);

// �Z�[�u�f�[�^�̏����擾
bool GetSaveRanking( Common::SAVE_SCORE &saveData );

#endif