#ifndef _gkg_win32_feedback_RealTimeFeedbackFunctor_h_
#define _gkg_win32_feedback_RealTimeFeedbackFunctor_h_


#include "MrServers/MrVista/include/Iris/Services/IFeedback.h"
#include "MrServers/MrMeasSrv/SeqIF/libMES/SEQData.h"


namespace gkg
{


IResult fFuncFeedbackInit( IFeedback::Pointer*, IParcEnvironment* );
IResult funcSendAbort( IFeedback::Pointer, SEQData& );


}


#endif
