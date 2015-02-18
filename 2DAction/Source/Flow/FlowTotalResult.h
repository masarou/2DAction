/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[�����ʉ�ʃN���X
 */
/* ====================================================================== */
#ifndef __FLOW_TOTAL_RESULT__
#define __FLOW_TOTAL_RESULT__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"
#include "System/Task/SystemTaskUnit.h"

class TotalResult2D;

class FlowTotalResult : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual bool FinishFlow() override;
	virtual void PadEventDecide() override;

	// �����ɍ���̃v���C�f�[�^�𔽉f���������L���O���Z�b�g
	void UpdateSortRanking( Common::SAVE_SCORE &scoreData );
	
	FlowTotalResult( const std::string &fileName );
	~FlowTotalResult(void);

	TotalResult2D	*m_pResultTex;
};


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���I�����N���X
 */
/* ====================================================================== */
class TotalResult2D : public TaskUnit, InputWatcher
{
public:

	static TotalResult2D *CreateTotalResult2D();

	bool ProceedNext(){ return m_dispState == DISP_MAX ? true : false ;}		// �S�ĕ\�����I���Ď��ɐi��ł������ǂ���

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// �`��X�V
	virtual void PadEventDecide() override;

private:

	enum DISP_STATE{
		DISP_STAGE01,
		DISP_STAGE02,
		DISP_STAGE03,
		DISP_TOTAL,

		DISP_MAX,
	};

	TotalResult2D();
	virtual ~TotalResult2D(void);
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;

	DISP_STATE			m_dispState;						// �ǂ��܂ŕ\�����Ă��邩
	Texture2D			m_textureBG;						// �w�i�ꖇ�G
	Texture2D			m_textureResult;					// �w�i�t���[��
	TEX_DRAW_INFO		m_numberInfo;						// �`��֌W���
	
	NumberCounter		*m_pNumCounter[DISP_MAX];			// �X�R�A�\��

	std::map< std::string, Common::PARTS_INFO >	m_partsMap;	// �p�[�c�}�b�v
};


#endif