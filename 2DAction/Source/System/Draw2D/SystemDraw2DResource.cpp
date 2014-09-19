/* ====================================================================== */
/**
 * @brief  ２Ｄ描画をまとめて行うクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "SystemDraw2DResource.h"

TextureResourceManager *TextureResourceManager::s_pInstance = NULL;

TextureResourceManager::TextureResourceManager(void)
{
}


TextureResourceManager::~TextureResourceManager(void)
{
	DEBUG_PRINT("/_/_/TextureResourceManager 削除/_/_/\n");

	//! メモリ解放
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		for(uint32_t j = 0; j < NUMBEROF(m_vRecource2D.at(i).m_texHandle); ++j){
			if(m_vRecource2D.at(i).m_texHandle[j] == INVALID_VALUE){
				break;
			}
			DeleteGraph(m_vRecource2D.at(i).m_texHandle[j]);
		}
	}
	
	DEBUG_PRINT("/_/_/TextureResourceManager 削除完了/_/_/\n");
}

void *TextureResourceManager::Create()
{
	if(s_pInstance){
		DEBUG_ASSERT("m_pInstance is Already Created");
	}
	else{
		s_pInstance = NEW TextureResourceManager();
	}
	return s_pInstance;
}

TextureResourceManager *TextureResourceManager::GetInstance()
{
	if(!s_pInstance){
		DEBUG_ASSERT("m_pInstance is NULL");
		return NULL;
	}
	return s_pInstance;
}

/* ================================================ */
/**
 * @brief	jsonファイルから描画情報取得
 */
/* ================================================ */
void TextureResourceManager::LoadTextureInfo(const char *jsonFile)
{
	//! もうすでに読み込んでいるかチェック
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		if(m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0){
			//! 読み込み済み
			++m_vRecource2D.at(i).m_readCounter;
			DEBUG_PRINT("/_/_/リソース再読み込み counter = %d/_/_/\n", m_vRecource2D.at(i).m_readCounter);
			return;
		}
	}
	
	RESOURCE_TEX tex;
	tex.Init();

	std::string path = JSON_GAME2D_PATH;
	path += jsonFile;
	std::ifstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	//! 基本となる情報取得
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

	//! アニメ情報取得
	picojson::value animData = root.get("animeInfo");
	for(uint32_t i = 0;; ++i){
		picojson::value null;
		if(animData.get(i) == null){
			break;
		}
		ANIM_INFO data;
		data.Init();
		for(uint32_t j = 0;;++j){
			if(animData.get(i).get("animIndex").get(j) == null){
				break;
			}
			uint32_t animIndex = static_cast<uint32_t>(animData.get(i).get("animIndex").get(j).get<double>());
			data.m_vPlayIndex.push_back(animIndex);
		}
		data.m_animTag		= animData.get(i).get("animTag").get<std::string>();
		data.m_frameSpeed	= static_cast<uint32_t>(animData.get(i).get("frameSpeed").get<double>());
		data.m_isLoop		= animData.get(i).get("loop").get<bool>();

		//! 読み込んだアニメ情報を保持
		basicInfo.m_vAnimName.push_back(data);
	}

	++tex.m_readCounter;
	tex.m_jsonFile = jsonFile;
	tex.m_texInfo = basicInfo;

	m_vRecource2D.push_back(tex);
	DEBUG_PRINT("/_/_/リソース読み込み : %s/_/_/\n", jsonFile);
}

/* ================================================ */
/**
 * @brief	描画情報削除
 */
/* ================================================ */
void TextureResourceManager::DeleteTextureInfo(const char *jsonFile)
{
	DEBUG_PRINT("/_/_/リソース解放 : %s/_/_/\n", jsonFile);
	std::vector<RESOURCE_TEX>::iterator it = m_vRecource2D.begin();
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		if(m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0){
			//! 発見!!!
			--m_vRecource2D.at(i).m_readCounter;

			if(m_vRecource2D.at(i).m_readCounter <= 0){
				//! メモリ解放
				for(uint32_t j = 0; j < NUMBEROF(it->m_texHandle); ++j){
					if(it->m_texHandle[j] == INVALID_VALUE){
						break;
					}
					DeleteGraph(it->m_texHandle[j]);
				}
				DEBUG_PRINT("/_/_/リソース解放完了 : 【%s】/_/_/\n", m_vRecource2D.at(i).m_jsonFile.c_str() );
				m_vRecource2D.erase(it);
			}
			else{
				DEBUG_PRINT("/_/_/リソース読み込みカウンタ残り %d/_/_/\n", m_vRecource2D.at(i).m_readCounter );
			}
			return;
		}
		++it;
	}
	DEBUG_PRINT("/_/_/リソースが見つからなかった.../_/_/\n");
	DEBUG_ASSERT("jsonFile is Nothing");
}

/* ================================================ */
/**
 * @brief	指定のjsonファイルからアニメ全体のHandle情報を取得
 */
/* ================================================ */
void TextureResourceManager::GetTextureHandle( const char *jsonFile, std::vector<int32_t> &vHandle)
{
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		if(m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0){
			for(uint32_t j = 0; j < NUMBEROF(m_vRecource2D.at(i).m_texHandle);++j){
				if(m_vRecource2D.at(i).m_texHandle[j] == INVALID_VALUE){
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
 * @brief	読み込んだテクスチャのデフォルトアニメ名取得
 */
/* ================================================ */
void TextureResourceManager::GetDefaultAnimName( const char *jsonFile, std::string &defaultAnim)
{
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		if(m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0){
			TEX_INIT_INFO &texInfo = m_vRecource2D.at(i).m_texInfo;
			defaultAnim = texInfo.m_animDefault;
			return;
		}
		return;
	}
}

/* ================================================ */
/**
 * @brief	指定のjsonファイルから再生アニメ名取得
 */
/* ================================================ */
void TextureResourceManager::GetPlayAnimName( const char *jsonFile, std::vector<std::string> &vAnim)
{
	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		if(m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0){
			TEX_INIT_INFO &texInfo = m_vRecource2D.at(i).m_texInfo;
			for(uint32_t j = 0; j < texInfo.m_vAnimName.size();++j){
				vAnim.push_back(texInfo.m_vAnimName.at(j).m_animTag);
			}
			return;
		}
	}
}

/* ================================================ */
/**
 * @brief	指定のjsonファイル、アニメ名、フレーム数から
 *			適切なHandle配列のIndex値を返す
 */
/* ================================================ */
int32_t TextureResourceManager::GetAnimHandleIndex( const char *jsonFile, const char *animName, uint32_t &frame)
{
	uint32_t drawIndex = INVALID_VALUE;

	for(uint32_t i = 0; i < m_vRecource2D.size(); ++i){
		if(m_vRecource2D.at(i).m_jsonFile.compare(jsonFile) == 0){
			//! jsonファイルからアニメ情報を調べる
			TEX_INIT_INFO &texInfo = m_vRecource2D.at(i).m_texInfo;
			for(uint32_t j = 0; j < texInfo.m_vAnimName.size(); ++j){
				if(texInfo.m_vAnimName.at(j).m_animTag.compare(animName) == 0){
					//! 表示すべき画像のIndexを調べる
					ANIM_INFO &animInfo = texInfo.m_vAnimName.at(j);
					uint32_t animIndex = 0;
					if(frame != 0){
						//! 現在が再生中アニメの何枚目を描画するか求める
						animIndex = frame / animInfo.m_frameSpeed;
					}

					//! 再生中
					if(animIndex < animInfo.m_vPlayIndex.size()){
						//!表示するテクスチャ設定
						drawIndex = animInfo.m_vPlayIndex.at(animIndex);
						return drawIndex;
					}
					//! 再生が終了している
					else{

						//! 再生を最初からにしてやる
						frame = 0;

						//! loop設定なら最初から、それ以外ならデフォルトアニメに戻す
						if(animInfo.m_isLoop){
							drawIndex = animInfo.m_vPlayIndex.at(0);
							return drawIndex;
						}
						else{
							//! デフォルトアニメの先頭Indexを返す
							if( texInfo.m_animDefault.compare("") != 0 ){
								drawIndex = GetAnimHandleIndex( jsonFile, texInfo.m_animDefault.c_str(), frame);
							}
							else{
								// デフォルトアニメが設定されていないので無効値を返す
								drawIndex = INVALID_VALUE;
							}
							return drawIndex;
						}
					}
				}
			}
		}
	}
	return drawIndex;
}

