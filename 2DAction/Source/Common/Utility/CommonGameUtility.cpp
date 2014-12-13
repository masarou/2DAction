
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

const bool IsSameBelongArea( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{
	bool retVal = false;

	// �ǂ��炩�̋��Lv��0=��ʑS�̂Ȃ瓖���蔻��͕K���K�v
	// �������Lv,�ԍ��Ȃ瓖���蔻����s��
	if( (texA.m_belongLv == 0 || texB.m_belongLv == 0)
		|| (texA.m_belongLv == texB.m_belongLv && texA.m_belongIndex == texB.m_belongIndex) ){
		retVal = true;
	}
	else{
		// ���Lv�������Ȃ̂�Index���Ⴄ�Ȃ画��Ȃ�
		if( texA.m_belongLv == texB.m_belongLv ){
			retVal = false;
		}

		uint32_t smallLv	= 0;
		uint32_t smallIndex	= 0;
		uint32_t bigLv		= 0;
		uint32_t bigIndex	= 0;

		if( texA.m_belongLv < texB.m_belongLv ){
			smallLv		= texA.m_belongLv;
			smallIndex	= texA.m_belongIndex;
			bigLv		= texB.m_belongLv;
			bigIndex	= texB.m_belongIndex;
		}
		else{
			smallLv		= texB.m_belongLv;
			smallIndex	= texB.m_belongIndex;
			bigLv		= texA.m_belongLv;
			bigIndex	= texA.m_belongIndex;
		}
		uint32_t diffLv = bigLv - smallLv;
		uint32_t convIndex = bigIndex >> diffLv*2;
		if( convIndex == smallIndex ){
			retVal = true;
		}
	}
	return retVal;
}

const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{
	if( !IsSameBelongArea( texA, texB ) ){
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
	float offsetX = tex.m_pos.x;
	float offsetY = tex.m_pos.y;

	// �I�t�Z�b�g���g�p���Ă��Ȃ�
	// ��ɉ�ʏ�ɌŒ�ŕ\������Ă���`�敨�̓����蔻��̏ꍇ
	// �v���C���[�̏����ʒu���l�����Ēl��ύX���Ă��K�v������
	if( !tex.m_usePlayerOffset ){
		//!�v���C���[���擾
		float offsetx = 0.0f;
		float offsety = 0.0f;
		GameAccesser::GetInstance()->GetPlayerOffSet(offsetx, offsety);
		offsetX = WINDOW_WIDTH/2.0f + offsetx;
		offsetY = WINDOW_HEIGHT/2.0f + offsety;
	}

	math::Vector2 upperLeft		= math::Vector2( offsetX - (texInfo.m_sizeWidth/2.0f), offsetY - (texInfo.m_sizeHeight/2.0f) );
	math::Vector2 underRight	= math::Vector2( offsetX + (texInfo.m_sizeWidth/2.0f), offsetY + (texInfo.m_sizeHeight/2.0f) );

	const uint32_t upper = pMap->GetBelongArea( upperLeft );
	const uint32_t under = pMap->GetBelongArea( underRight );

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
	//DEBUG_PRINT( " m_belongLv = %d, m_belongIndex = %d\n", tex.m_belongLv, tex.m_belongIndex );
}


bool IsPositionInWindowArea( const TEX_DRAW_INFO &texInfo )
{
	if( !texInfo.m_usePlayerOffset ){
		return true;
	}
	return IsPositionInWindowArea( static_cast<int32_t>(texInfo.m_pos.x), static_cast<int32_t>(texInfo.m_pos.y) );
}
bool IsPositionInWindowArea( const int32_t &xx, const int32_t &yy )
{
	bool retVal = false;

	//!�v���C���[���擾
	float offsetx = 0.0f;
	float offsety = 0.0f;
	GameAccesser::GetInstance()->GetPlayerOffSet(offsetx, offsety);

	//!���ݕ`�悵�����[(��ʊO�ł���}�X���ł͕`��)
	int32_t WidthLower	= -1 * 50;
	int32_t WidthUpper	= WINDOW_WIDTH + 50;
	int32_t HeightLower = -1 * 50;
	int32_t HeightUpper = WINDOW_HEIGHT + 50;

	//!�`�悵�悤�Ƃ��Ă���ʒu
	int32_t posY = yy - static_cast<int32_t>(offsety);
	int32_t posX = xx - static_cast<int32_t>(offsetx);

	if(posX < WidthUpper && posX > WidthLower
	&& posY < HeightUpper && posY > HeightLower){
		retVal = true;
	}
	return retVal;
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

// �v���C���[�̈ʒu�����擾
math::Vector2 GetPlayerPos()
{
	math::Vector2 offset;
	math::Vector2 defaultPos;
	GameAccesser::GetInstance()->GetPlayerOffSet( offset.x, offset.y );
	defaultPos.x = WINDOW_WIDTH/2.0f;
	defaultPos.y = WINDOW_HEIGHT/2.0f;

	return defaultPos + offset;
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
