
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

// �`��ʒu���d�Ȃ��Ă��邩�ǂ���(�����蔻��)
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// �}�b�v��ł̏�����Ԃ����߂�
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex );

// �w���AI�N���X�̐���
EnemyAIBase *ChangeEnemyAI( Common::ENEMY_AI nextAI );

// �w��͈͓̔��̃����_���Ȑ�����Ԃ�
int32_t GetRandamValue( const int32_t &max, const int32_t &min);

// �Z�[�u�f�[�^�̏����擾
bool GetSaveRanking( Common::SAVE_SCORE &saveData );

#endif