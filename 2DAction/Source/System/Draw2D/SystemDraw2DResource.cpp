/* ====================================================================== */
/**
 * @brief  �Q�c�`����܂Ƃ߂čs���N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "SystemDraw2DResource.h"
#include "SystemDraw2DManager.h"

TextureResourceManager *TextureResourceManager::s_pInstance = NULL;

TextureResourceManager::TextureResourceManager(void)
{
}

TextureResourceManager::~TextureResourceManager(void)
{
	s_pInstance = NULL;
}

void TextureResourceManager::DeleteResourceManager()
{
	DEBUG_PRINT("/_/_/TextureResourceManager �폜/_/_/\n");

	//! ���������
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		for(uint32_t j = 0; j < NUMBEROF(m_vRecource2D.at(i).m_texHandle); ++j){
			if(m_vRecource2D.at(i).m_texHandle[j] == INVALID_VALUE){
				break;
			}
			DeleteGraph(m_vRecource2D.at(i).m_texHandle[j]);
		}
	}

	SAFE_DELETE( s_pInstance );
	
	DEBUG_PRINT("/_/_/TextureResourceManager �폜����/_/_/\n");
}

void *TextureResourceManager::Create()
{
	if(s_pInstance){
		DEBUG_ASSERT( 0, "TextureResourceManager is Already Created");
	}
	else{
		s_pInstance = NEW TextureResourceManager();
	}
	return s_pInstance;
}

TextureResourceManager *TextureResourceManager::GetInstance()
{
	if(!s_pInstance){
		DEBUG_ASSERT( 0, "m_pInstance is NULL");
		return NULL;
	}
	return s_pInstance;
}

/* ================================================ */
/**
 * @brief	json�t�@�C������`����擾
 */
/* ================================================ */
void TextureResourceManager::LoadTextureInfo( const char *jsonFile )
{
	//! �������łɓǂݍ���ł��邩�`�F�b�N
	for( uint32_t i = 0; i < m_vRecource2D.size(); ++i ){
		if( m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0 ){
			//! �ǂݍ��ݍς�
			++m_vRecource2D.at(i).m_readCounter;
			//DEBUG_PRINT("/_/_/���\�[�X�ēǂݍ��� counter = %d/_/_/\n", m_vRecource2D.at(i).m_readCounter);
			return;
		}
	}
	
	RESOURCE_TEX tex;
	tex.Init();

	std::string path = JSON_GAME2D_PATH;
	path += jsonFile;

	idxfstream ifs( path.c_str() );

	picojson::value root;
	picojson::value null;
	picojson::parse( root, ifs);
	
	//! ��{�ƂȂ���擾
	TEX_INIT_INFO basicInfo;
	basicInfo.Init();
	picojson::value sceneData = root.get("basicInfo");
	basicInfo.m_fileName	= sceneData.get(0).get("fileName").get<std::string>();
	basicInfo.m_animDefault	= sceneData.get(0).get("animDefault").get<std::string>();
	basicInfo.m_splitNum	= static_cast<uint32_t>(sceneData.get(0).get("total").get<double>());
	basicInfo.m_splitWidth	= static_cast<uint32_t>(sceneData.get(0).get("splitW").get<double>());
	basicInfo.m_splitHeight	= static_cast<uint32_t>(sceneData.get(0).get("splitH").get<double>());
	basicInfo.m_sizeWidth	= static_cast<uint32_t>(sceneData.get(0).get("sizeW").get<double>());
	basicInfo.m_sizeHeight	= static_cast<uint32_t>(sceneData.get(0).get("sizeH").get<double>());

	path = TEXTURE_PATH;
	path += basicInfo.m_fileName;
	LoadDivGraph( path.c_str(), basicInfo.m_splitNum
	, basicInfo.m_splitWidth, basicInfo.m_splitHeight
	, basicInfo.m_sizeWidth, basicInfo.m_sizeHeight, tex.m_texHandle);

	//! �A�j�����擾
	picojson::value animData = root.get("animeInfo");
	for( uint32_t i = 0;; ++i ){
		if( animData == null || animData.get(i) == null ){
			break;
		}
		ANIM_INFO data;
		data.Init();
		for( uint32_t j = 0;;++j ){
			if( animData.get(i).get("animIndex").get(j) == null ){
				break;
			}
			uint32_t animIndex = static_cast<uint32_t>(animData.get(i).get("animIndex").get(j).get<double>());
			data.m_vPlayIndex.push_back(animIndex);
		}
		data.m_animTag		= animData.get(i).get("animTag").get<std::string>();
		data.m_frameSpeed	= static_cast<uint32_t>(animData.get(i).get("frameSpeed").get<double>());
		data.m_nextAnimTag	= animData.get(i).get("nextAnim").get<std::string>();

		//! �ǂݍ��񂾃A�j������ێ�
		basicInfo.m_vAnimName.push_back(data);
	}

	// �����蔻��~���ݒ肳��Ă���Ύ擾
	picojson::value colData = root.get("collisionCircle");
	for( uint32_t i = 0;; ++i ){
		if( colData == null || colData.get(i) == null ){
			break;
		}
		CollisionCircle data;
		data.m_relativePos.x	= static_cast<float>( colData.get(i).get("posX").get<double>() );
		data.m_relativePos.y	= static_cast<float>( colData.get(i).get("posY").get<double>() );
		data.m_radius		= static_cast<float>( colData.get(i).get("radius").get<double>() );

		//! �ǂݍ��񂾃A�j������ێ�
		basicInfo.m_collisionArray.push_back(data);
	}

	++tex.m_readCounter;
	tex.m_jsonFile = jsonFile;
	tex.m_texInfo = basicInfo;

	m_vRecource2D.push_back(tex);
	//DEBUG_PRINT("/_/_/���\�[�X�ǂݍ��� : %s/_/_/\n", jsonFile);
}

/* ================================================ */
/**
 * @brief	�`����폜
 */
/* ================================================ */
void TextureResourceManager::DeleteTextureInfo( const char *jsonFile )
{
	//DEBUG_PRINT("/_/_/���\�[�X��� : %s/_/_/\n", jsonFile);
	std::vector<RESOURCE_TEX>::iterator it = m_vRecource2D.begin();
	for( uint32_t i = 0; i < m_vRecource2D.size(); ++i ){
		if(m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0){
			//! ����!!!
			--m_vRecource2D.at(i).m_readCounter;

			if( m_vRecource2D.at(i).m_readCounter <= 0 ){
				//! ���������
				for( uint32_t j = 0; j < NUMBEROF(it->m_texHandle); ++j ){
					if( it->m_texHandle[j] == INVALID_VALUE ){
						break;
					}

					// �ǂݍ��݉摜���폜
					DeleteGraph(it->m_texHandle[j]);

					// �`��\������Ă�����������Ȃ��̂ō폜
					Draw2DManager::GetInstance()->DeleteDrawInfo( jsonFile );
				}

				//DEBUG_PRINT("/_/_/���\�[�X������� : �y%s�z/_/_/\n", m_vRecource2D.at(i).m_jsonFile.c_str() );
				m_vRecource2D.erase(it);
			}
			else{
				//DEBUG_PRINT("/_/_/���\�[�X�ǂݍ��݃J�E���^�c�� %d/_/_/\n", m_vRecource2D.at(i).m_readCounter );
			}
			return;
		}
		++it;
	}
	DEBUG_PRINT("/_/_/���\�[�X��������Ȃ�����.../_/_/\n");
	DEBUG_ASSERT( 0, "jsonFile is Nothing");
}

/* ================================================ */
/**
 * @brief	�w���json�t�@�C������e�N�X�`���̉摜���擾
 */
/* ================================================ */
const TEX_INIT_INFO &TextureResourceManager::GetLoadTextureInfo( const char *jsonFile ) const
{
	for( uint32_t i = 0; i < m_vRecource2D.size(); ++i ){
		if( m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0 ){
			return m_vRecource2D.at(i).m_texInfo;
		}
	}
	DEBUG_ASSERT( 0, "�w���json�t�@�C���̏�񂪂Ȃ�!!!");
	return m_vRecource2D.at(0).m_texInfo;
}

/* ================================================ */
/**
 * @brief	�w���json�t�@�C������A�j���S�̂�Handle�����擾
 */
/* ================================================ */
void TextureResourceManager::GetTextureHandle( const char *jsonFile, std::vector<int32_t> &vHandle ) const
{
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		if( m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0 ){
			for( uint32_t j = 0; j < NUMBEROF(m_vRecource2D.at(i).m_texHandle);++j ){
				if( m_vRecource2D.at(i).m_texHandle[j] == INVALID_VALUE ){
					return;
				}
				vHandle.push_back(m_vRecource2D.at(i).m_texHandle[j]);
			}
			return;
		}
	}
}

/* ================================================ */
/**
 * @brief	�ǂݍ��񂾃e�N�X�`���̃f�t�H���g�A�j�����擾
 */
/* ================================================ */
const std::string TextureResourceManager::GetDefaultAnimName( const char *jsonFile ) const
{
	std::string retStr = "";
	const TEX_INIT_INFO &texInfo = GetLoadTextureInfo( jsonFile );
	retStr = texInfo.m_animDefault;
	return retStr;
}

/* ================================================ */
/**
 * @brief	�w���json�t�@�C������Đ��A�j�����擾
 */
/* ================================================ */
void TextureResourceManager::GetPlayAnimName( const char *jsonFile, std::vector<std::string> &vAnim ) const
{
	const TEX_INIT_INFO &texInfo = GetLoadTextureInfo( jsonFile );
	for(uint32_t j = 0; j < texInfo.m_vAnimName.size();++j){
		vAnim.push_back(texInfo.m_vAnimName.at(j).m_animTag);
	}
}

/* ================================================ */
/**
 * @brief	�w���json�t�@�C���A�A�j�����A�t���[��������
 *			�K�؂�Handle�z���Index�l��Ԃ�
 */
/* ================================================ */
const int32_t TextureResourceManager::GetAnimHandleIndex( const char *jsonFile, std::string &animName, uint32_t &frame ) const
{
	uint32_t drawIndex = INVALID_VALUE;

	const TEX_INIT_INFO &texInfo = GetLoadTextureInfo( jsonFile );
	for( uint32_t j = 0; j < texInfo.m_vAnimName.size(); ++j ){
		if(texInfo.m_vAnimName.at(j).m_animTag.compare(animName) == 0){
			//! �\�����ׂ��摜��Index�𒲂ׂ�
			const ANIM_INFO &animInfo = texInfo.m_vAnimName.at(j);
			uint32_t animIndex = 0;
			if(frame != 0){
				//! ���݂��Đ����A�j���̉����ڂ�`�悷�邩���߂�
				animIndex = frame / animInfo.m_frameSpeed;
			}

			//! �Đ���
			if(animIndex < animInfo.m_vPlayIndex.size()){
				//!�\������e�N�X�`���ݒ�
				drawIndex = animInfo.m_vPlayIndex.at(animIndex);
				return drawIndex;
			}
			//! �Đ����I�����Ă���
			else{

				// �t���[��������
				frame = 0;
				std::string nextAnimName = animInfo.m_nextAnimTag;

				//! ���A�j�����ݒ肳��Ă���Ȃ�ŏ�����A����ȊO�Ȃ疳���l
				if( animInfo.m_nextAnimTag.compare("") != 0 ){
					animName = animInfo.m_nextAnimTag;
					drawIndex = GetAnimHandleIndex( jsonFile, nextAnimName, frame );
					return drawIndex;
				}
				else{
					// �f�t�H���g�A�j�����ݒ肳��Ă��Ȃ��̂Ŗ����l��Ԃ�
					drawIndex = INVALID_VALUE;
				}
				return drawIndex;
			}
		}
	}
	return drawIndex;
}

