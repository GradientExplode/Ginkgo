#ifndef _gkg_win32_feedback_RealTimeFeedbackSequence_h_
#define _gkg_win32_feedback_RealTimeFeedbackSequence_h_

//#define VB17
#define VE11

//#include "MrServers/MrImaCalcFramework/IceFramework/IceFeedbackPkg/MPCU/SeqFeedbackBuffer.h"
#ifdef VE11
#include "ProtBasic/Interfaces/SeqLim.h"
#include "MrServers/MrProtSrv/MrProt/SeqIF/SeqExpo.h"
#else
#include "MrServers/MrMeasSrv/SeqIF/SeqBuffer/SeqLim.h"
#include "MrServers/MrMeasSrv/SeqIF/SeqBuffer/SeqExpo.h"
#endif
#include "MrServers/MrMeasSrv/SeqIF/libRT/sREADOUT.h"
#include "MrServers/MrMeasSrv/SeqIF/libMES/SEQData.h"


namespace gkg
{


NLS_STATUS fSEQFeedbackInit( sREADOUT& );
void fRTEIFeedback( long );
NLS_STATUS fRTWaitForFeedback( long );
NLS_STATUS fSEQReceive( SeqLim*, SeqExpo*, SEQData& );
NLS_STATUS fRunPause( long );


}


#endif
