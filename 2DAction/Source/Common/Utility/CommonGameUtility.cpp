
#include <random>
#include "System/Draw2D/SystemDraw2DResource.h"
#include "CommonGameUtility.h"
#include "Math/MathUtility.h"
#include "Game/Enemy/EnemyAIBase.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

void AddPlayerOffsetPos( math::Vector2 &pos )
{
	//!�v���C���[���擾
	pos += GameAccesser::GetInstance()->GetPlayerOffSet();
}
const math::Vector2 &GetPlayerOffsetPos()
{
	//!�v���C���[���擾
	return GameAccesser::GetInstance()->GetPlayerOffSet();
}

const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{
	if( texA.m_belongLv != texB.m_belongLv || texA.m_belongIndex != texB.m_belongIndex ){
		return false;
	}

	const TEX_INIT_INFO &texInfoA = TextureResourceManager::GetInstance()->GetLoadTextureInfo( texA.m_fileName.c_str() );
	const TEX_INIT_INFO &texInfoB = TextureResourceManager::GetInstance()->GetLoadTextureInfo( texB.m_fileName.c_str() );

	math::Vector2 diff = texA.m_pos - texB.m_pos;

	float inRangeX = math::Absf(texInfoA.m_sizeWidth/2.0f) + math::Absf(texInfoB.m_sizeWidth/2.0f);
	float inRangeY = math::Absf(texInfoA.m_sizeHeight/2.0f) + math::Absf(texInfoB.m_sizeHeight/2.0f);

	if( math::Absf( diff.x ) < inRangeX && math::Absf( diff.y ) < inRangeY ){
		return true;
	}

	return false;
}

// �}�b�v��ł̏�����Ԃ����߂�
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex )
{
	uint32_t retVal = 0;
	const TEX_INIT_INFO &texInfo	= TextureResourceManager::GetInstance()->GetLoadTextureInfo( tex.m_fileName.c_str() );
	const GameRegister *pRegister	= GameRegister::GetInstance();
	if( !pRegister ){ return; }
	const GameMap *pMap				= pRegister->GetGameMap();
	if( !pMap ){ return; }

	// �摜�̍���ƉE���̈ʒu�����߂�
	math::Vector2 upperLeft		= math::Vector2( tex.m_pos.x - (texInfo.m_sizeWidth/2.0f), tex.m_pos.y - (texInfo.m_sizeHeight/2.0f) );
	math::Vector2 underRight	= math::Vector2( tex.m_pos.x + (texInfo.m_sizeWidth/2.0f), tex.m_pos.y + (texInfo.m_sizeHeight/2.0f) );

	const uint32_t upper = pMap->GetBelongArea( upperLeft );
	const uint32_t under = pMap->GetBelongArea( underRight );

	// �r���I�_���a
	uint32_t areaNum = upper^under;
	uint32_t belongLv = 0;		// root��Lv0, �傫���ɂȂ�ɂ�Đe�A�q�A���ƂȂ�
	uint32_t belongIndex = 0;
	for( int32_t i = (HIT_AREA_SPLIT_MAX-1) ; i >= 0; --i ){
		if( (areaNum >> 2) == 0 ){
			tex.m_belongLv = i;
			tex.m_belongIndex = under >> ((HIT_AREA_SPLIT_MAX-1)-i)*2;
			break;
		}
		else{
			areaNum = areaNum >> 2;
		}
	}
}

EnemyAIBase *ChangeEnemyAI( Common::ENEMY_AI nextAI )
{
	EnemyAIBase *pRetAI = NULL;
	//switch( nextAI ){
	//case Common::AI_SEARCHING:	// �v���C���[��T���Ă���
	//	pRetAI = NEW EnemyAIBase();
	//	break;
	//case Common::AI_MOVE_PLAYER:	// �v���C���[�ɋ߂Â�(�̓�����U��)
	//	pRetAI = NEW EnemyAIBase();
	//	break;
	//case Common::AI_SHOOTING:	// �������U��(�������U��)
	//	pRetAI = NEW EnemyAIBase();
	//	break;
	//}
	return pRetAI;
}

int32_t GetRandamValue( const int32_t &max, const int32_t &min)
{
	//std::mt19937 rand;
	//std::uniform_int_distribution<int> distribution( min, max );
	//return distribution(rand);

	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

bool GetSaveRanking( Common::SAVE_SCORE &saveData )
{
	// �t�@�C�����Ȃ��ꍇ������̂ň�x�J���č쐬���Ă���
	FILE *fpCheck = fopen( "playLog.dat", "r" );
	if( !fpCheck ){
		// �t�@�C�����Ȃ��ꍇ������̂ň�x�J���č쐬���Ă���
		FILE *fpCreate = fopen( "playLog.dat", "a" );
		fclose( fpCreate );

		// �f�t�H���g�̒l���l�߂Ă���
		Common::SAVE_SCORE scoreLog = {
			{ 1000, 500, 300, 100, 0},
			{ 1000, 500, 300, 100, 0},
		};
		FILE *fpWriteDef = fopen( "playLog.dat", "wb" );
		if( fpWriteDef == NULL ){
			return false;
		}
		fwrite( &scoreLog, sizeof(scoreLog), 1, fpWriteDef );
		fclose( fpWriteDef );
	}
	else{
		// �t�@�C���͑��݂��Ă���
		fclose( fpCheck );
	}

	// �v���C���O�ǂݍ���
	FILE *fpRead = fopen( "playLog.dat", "rb" );
	if( fpRead == NULL ){
		return 0;
	}
	fread( &saveData, sizeof(saveData), 1, fpRead );
	fclose( fpRead );

	return true;
}
