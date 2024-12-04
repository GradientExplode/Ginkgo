#include <gkg-win32-feedback/RealTimeFeedbackFunctor.h>

#include "MrServers/MrVista/include/IrisICE.h"


IResult gkg::fFuncFeedbackInit( IFeedback::Pointer* pFeedbackSvc, IParcEnvironment* env )
{

  if ( !pFeedbackSvc )
  {

    ICE_RET_ERR( "Invalid pointer to feedback service, abort...", I_FAIL );

  }

  *pFeedbackSvc = Parc::cast< IFeedback >( env->Item( "FeedbackSvc" ) );

  if ( !*pFeedbackSvc )
  {

    ICE_RET_ERR( "Failed to retreive feedback service from the environment, abort...", I_FAIL );

  }

  return I_OK;

}


IResult gkg::funcSendAbort( IFeedback::Pointer feedbackSvc, SEQData& seqData )
{

  if ( !feedbackSvc )
  {

    ICE_RET_ERR( "Invalid pointer to feedback service, abort...", I_FAIL );

  }

  int abort = 1;
  seqData.setData( (void*)&abort, sizeof( int ) );
  seqData.setID( "GkgFeedback" );

  IResult res = feedbackSvc->Send( &seqData );

  if ( failed( res ) )
  {

    ICE_RET_ERR( "Failed to send feedback data, abort...", I_FAIL );

  }

  return I_OK;

}
