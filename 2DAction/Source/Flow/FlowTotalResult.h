/* ====================================================================== */
/**
 * @brief  
 *		�Q�[�����ʉ�ʃN���X
 * @note
 *		�e�X�e�[�W�̃X�R�A��\�����āA���Z���o�����
 */
/* ====================================================================== */
#ifndef __FLOW_TOTAL_RESULT__
#define __FLOW_TOTAL_RESULT__

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class PartsCounter;

class FlowTotalResult : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual bool FinishFlow() override;

	// �����ɍ���̃v���C�f�[�^�𔽉f���������L���O���Z�b�g
	void RecordGameResult();
	void UpdateSortRanking( Common::SAVE_DATA &scoreData );
	
	FlowTotalResult( const std::string &fileName );
	~FlowTotalResult(void);

};


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�\������X�R�A���̂���
 */
/* ====================================================================== */
class TotalResult2D : public MenuWindow
{
public:

	static TotalResult2D *CreateTotalResult2D( const std::string &readMenuJson );

	bool ProceedNext(){ return m_dispState == DISP_MAX ? true : false ;}		// �S�ĕ\�����I���Ď��ɐi��ł������ǂ���

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;

private:

	enum DISP_STATE{
		DISP_SCENE01,	// �X�e�[�W1~4
		DISP_SCENE02,	// �X�e�[�W5~8
		DISP_SCENE03,	// �X�e�[�W9~12
		DISP_TOTAL,

		DISP_MAX,
	};

	// ��ʂ̕\���X�V���ڂ����̂��̂�
	bool ChangeDispNextState();

	TotalResult2D( const std::string &readMenuJson );
	virtual ~TotalResult2D(void);

	DISP_STATE			m_dispState;						// �ǂ��܂ŕ\�����Ă��邩
	PartsCounter		*m_pNumCounter[DISP_MAX];			// �X�R�A�\��
};


#endif