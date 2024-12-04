#include <Core/SiemensInclude.h>

#include "GinkgoSpinEcho3d.h"
#include "GinkgoSpinEcho3d_UI.h"


#ifdef WIN32


gkg::GinkgoSpinEcho3d* getSeq( MrUILinkBase* const pThis )
{
	    
  return static_cast< gkg::GinkgoSpinEcho3d* >( 
                                                   pThis->sequence().getSeq() );
  
}


const gkg::GinkgoSpinEcho3dUI* getUI( MrUILinkBase* const pThis )
{
	  
  return static_cast< gkg::GinkgoSpinEcho3d*>(
                                          pThis->sequence().getSeq() )->getUI();
  
}


gkg::GinkgoSpinEcho3dUI::GinkgoSpinEcho3dUI()
{
}


gkg::GinkgoSpinEcho3dUI::~GinkgoSpinEcho3dUI()
{
}
  
#endif  

//~ NLS_STATUS gkg::GinkgoSpinEcho3dUI::registerUI( SeqLim &seqLim ) 
NLS_STATUS gkg::GinkgoSpinEcho3dUI::registerUI( 
                                          SeqLim &seqLim, 
                                          WPT_NAMESPACE::WIPParameterTool& tool)
{

  static const char* const ptModule = { 
                                    "gkg::GinkgoSpinEcho2dUI::registerUI" };
                                    
  tool.createDoubleParameter( 0, seqLim, "Fat saturation : flip angle", 
                              NULL, 1, 0.0, 180.0, 0.1, 90.0 );
  tool.createDoubleParameter( 1, seqLim, "Fat saturation : frequency offset", 
                              NULL, 1, -1000.0, 1000.0, 1.0, -420 );
  tool.createLongParameter( 2, seqLim, "Fat saturation : nb of iterations", 
                            NULL, 1, 3, 1, 3 );
  
  
  #ifdef WIN32
    tool.registerSolveHandler( 0, _solveHandlerDouble1 );
    tool.registerSolveHandler( 1, _solveHandlerDouble2 );
    tool.registerSolveHandler( 2, _solveHandlerLong );
  #endif

  return SEQU__NORMAL;

}

