#include <gkg-win32-feedback/RealTimeFeedbackSequence.h>

#include "MrServers/MrMeasSrv/SeqIF/libRT/libRT.h"
#include "MrServers/MrMeasSrv/SeqIF/libRT/sSYNC.h"
#include "MrServers/MrMeasSrv/SeqIF/Sequence/sequmsg.h"
#include "MrServers/MrImaging/libSeqUtil/libSeqUtil.h"


static sSYNC_WAKEUP gkgWakeUp;
static bool gkgFeedbackReceived;
static bool gkgStopAcquisition;


NLS_STATUS gkg::fSEQFeedbackInit( sREADOUT& sADC )
{

  // ***** Reset the feedback flag
  gkgFeedbackReceived = false;
  gkgStopAcquisition = false;

  // ***** Require real time feedback from ICE program
  // VB17
  //sADC.Mdh.addToEvalInfoMask( sADC.Mdh.getEvalInfoMask() | MDH_RTFEEDBACK );
  // VE11
  sADC.getMDH().setEvalInfoMask( 
                             sADC.getMDH().getEvalInfoMask() | MDH_RTFEEDBACK );

  return SEQU_NORMAL;

}


void gkg::fRTEIFeedback( long startTime )
{

  fRTEI( startTime , 0, 0, 0, 0, 0, 0, 0 );

}


NLS_STATUS gkg::fRTWaitForFeedback( long pause_us )
{

  // ***** get synchroneous with real time
  // ***** (send an ´empty´ event block, containing a ´wait for wake up'...
  NLS_STATUS nStatus = fRunPause( pause_us );

  if ( nStatus != NLS_SUCCESS )
  { 

    return SEQU_ERROR;

  }

  // ***** ... and wait)
  nStatus = fRTWaitForWakeup();

  if ( nStatus != NLS_SUCCESS )
  {

    return SEQU_ERROR;

  }

  return nStatus;

}


NLS_STATUS gkg::fSEQReceive( SeqLim*, SeqExpo*, SEQData& rSEQData )
{

  // ***** check whether the received data object is relevant for us
  if ( strcmp( rSEQData.getID(), "GkgFeedback" ) )
  {

    return SEQU__NORMAL;

  }

  // ***** retrieve the feedback data
  void* ptr = rSEQData.getData();

  if ( !ptr || ( rSEQData.getLength() != (int)sizeof( int ) ) )
  { 

    return SEQU_ERROR;

  }

  gkgFeedbackReceived = true;
  gkgStopAcquisition = *( (int*)ptr ) ? true : false;

  return SEQU_NORMAL;

}


NLS_STATUS gkg::fRunPause( long duration_us )
{

  //fRTEBInit( &sROT_MATRIXUnity ); // VB17
  fRTEBInit( sROT_MATRIXUnity ); // VE11

  //- *******************************************************************
  //- * Start Time |  NCO  |  SRF |  ADC |   Gradient Events   | Sync
  //- *   (usec)   | Event | Event| Event| phase | read | slice| Event
  //- *******************************************************************
  fRTEI( 0, 0, 0, 0, 0, 0, 0, &gkgWakeUp );
  fRTEI( duration_us , 0, 0, 0, 0, 0, 0, 0 );

  NLS_STATUS nStatus = fRTEBFinish();

  if ( nStatus != NLS_SUCCESS )
  {

    return SEQU_ERROR;
  
  }

  return nStatus;

}

