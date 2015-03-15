#include "FlowEffectBase.h"


FlowEffectBase::FlowEffectBase(void)
: m_effectState( STATE_INIT )
{
}


FlowEffectBase::~FlowEffectBase(void)
{
}

void FlowEffectBase::SetStateNext()
{
	switch( m_effectState ){
	default:
		DEBUG_ASSERT( 0, "想定外のステータス" );
		break;
	case STATE_INIT:		// Init中
		m_effectState = STATE_UPDATE;
		break;
	case STATE_UPDATE:		// Update中
		m_effectState = STATE_CANDIE;
		break;
	case STATE_CANDIE:		// CanDie中
		m_effectState = STATE_FLOW_WAIT;
		break;
	case STATE_FLOW_WAIT:	// エフェクト終了。フローの削除待ち
		break;
	}
}

void FlowEffectBase::Exec()
{
	switch( GetState() ){
	default:
		DEBUG_ASSERT( 0, "想定外のステータス" );
		break;
	case STATE_INIT:		// Init中
		if( Init() ){
			SetStateNext();
		}
		break;
	case STATE_UPDATE:		// Update中
		Update();
		break;
	case STATE_CANDIE:		// CanDie中
		if( CanDie() ){
			SetStateNext();
		}
		break;
	case STATE_FLOW_WAIT:	// エフェクト終了。フローの削除待ち
		break;
	}

	CallPadEvent();
}