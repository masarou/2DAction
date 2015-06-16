#include "FlowMenuBase.h"


FlowMenuBase::FlowMenuBase( const std::string &fileName )
: FlowBase( fileName )
{
}


FlowMenuBase::~FlowMenuBase(void)
{
}

void FlowMenuBase::CheckNextFlow()
{
	// ŽŸ‚Ì‘JˆÚæ‚ðí‚ÉŠÄŽ‹
	if( m_pMenuWindow && !m_pMenuWindow->GetNextFlowStr().empty() ){
		StartFade( m_pMenuWindow->GetNextFlowStr().c_str() );
	}
}
