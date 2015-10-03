/* ====================================================================== */
/**
 * @brief  �Q�[���S��utility�N���X
 *
 * @note
 *
 */
/* ====================================================================== */
#ifndef __CMN_GAME_UTILITY__
#define __CMN_GAME_UTILITY__

#include "Common/CommonDefine.h"
#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "Math/MathUtility.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "System/Input/SystemInputWatcher.h"

class EnemyAIBase;

namespace Utility
{

// �Q�[���|�[�Y�̃Z�b�g�Ɗm�F
void StartGamePause();
void EndGamePause();
bool IsGamePause();

// �Q�[���X�g�b�v
void StartGameStop( bool withFileter = false );	// �����͍���(BlackFilter)�̗L��
void EndGameStop();
bool IsGameStop();
bool IsShowPauseFilter();

// �Q�[����������ďI���Ƃ��A�^�C�g���ɖ߂�Ƃ��ȂǂɃv���C�L�^���Z�b�g
void GameInitALL();

// �v���C���[�I�t�Z�b�g�̕ύX�Ǝ擾
void AddPlayerOffsetPos( math::Vector2 &pos );
const math::Vector2 &GetPlayerOffsetPos();

// ��̏�����Ԃ������蔻�肪�K�v���ǂ�������
const bool IsSameBelongArea( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// �`��ʒu���d�Ȃ��Ă��邩�ǂ���(�����蔻��)
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// ����������������܂ł̃x�N�g�������߂�(�傫����1)
math::Vector2 GetDirAToB( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// �}�b�v��ł̏�����Ԃ����߂�
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex );

// �}�b�v��ł̍������擾
const uint32_t GetMapHeight( const uint32_t &posX, const uint32_t &posY );
const uint32_t GetMapHeight( const math::Vector2 &pos );

// �}�b�v��œK���Ȑ��l��Ԃ�
math::Vector2 GetMapRandamPos( bool allowInWindow = true, uint32_t mapHeight = 0 );

// ��ʏ�ɕ\�����Ă���I�u�W�F�N�g���ǂ���
bool IsPositionInWindowArea( const TEX_DRAW_INFO &texInfo );
bool IsPositionInWindowArea( const float &xx, const float &yy );

// �v���C���[�ɏd�Ȃ�Ȃ��ʒu���ǂ���
bool IsPositionInPlayerPos( const float &xx, const float &yy );

// �w���AI�N���X�̐���
EnemyAIBase *CreateEnemyAI( Common::ENEMY_AI nextAI );

// �v���C���[�̈ʒu�����擾
math::Vector2 GetPlayerPos();

// Window��̓���̈ʒu�ɏ�ɂ�����̂̈ʒu���Q�[����̈ʒu�ɕϊ�
math::Vector2 ConvertWindowPosToGamePos( const math::Vector2 &windowPos );

// �����������㉺���E����I��
InputWatcher::BUTTON_KIND GetDirection( const float dirX, const float dirY );

// �w��͈͓̔��̃����_���Ȑ�����Ԃ�(max��3, min��0����ꂽ��u0,1,2�v�̂����ꂩ)
int32_t GetRandamValue( const int32_t &max, const int32_t &min);
float GetRandamValueFloat( const int32_t &max, const int32_t &min);

// �Z�[�u�f�[�^�̏����擾
bool GetSaveData( Common::SAVE_DATA &saveData );
bool OverWriteSaveData( Common::SAVE_DATA &saveData );

// �v���C���[�̃��x��������ۂɃZ�b�g����l�֕ϊ�
uint32_t ConvertLevelToBaseState( Common::PLAYER_BASE_STATE stateKind, uint32_t level );

// ��ʂ�json����p�[�c�����擾("partsInfo")
void GetPartsInfoFromJson( const std::string &jsonStr, std::map< std::string, Common::PARTS_INFO > &vParts );

// ������̕\��
void DrawStringOnWindow( const std::string &str, const math::Vector2 &pos, uint32_t color = GetColor( 255, 255, 255 ) );

// �w��̏ꏊ�͈ړ��ł���ꏊ���ǂ���
bool IsMovable( const std::string &resourceJson, const math::Vector2 &pos );

void DrawDebugCircle( math::Vector2 drawPos );

} // Utility

#endif