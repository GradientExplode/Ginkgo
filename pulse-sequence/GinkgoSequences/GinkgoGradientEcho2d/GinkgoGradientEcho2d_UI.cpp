#include <Core/SiemensInclude.h>

#include "GinkgoGradientEcho2d.h"
#include "GinkgoGradientEcho2d_UI.h"


#ifdef WIN32


gkg::GinkgoGradientEcho2d* getSeq( MrUILinkBase* const pThis )
{
	    
  return static_cast< gkg::GinkgoGradientEcho2d* >( 
                                                   pThis->sequence().getSeq() );
  
}


const gkg::GinkgoGradientEcho2dUI* getUI( MrUILinkBase* const pThis )
{
	  
  return static_cast< gkg::GinkgoGradientEcho2d*>(
                                          pThis->sequence().getSeq() )->getUI();
  
}


gkg::GinkgoGradientEcho2dUI::GinkgoGradientEcho2dUI()
{
}


gkg::GinkgoGradientEcho2dUI::~GinkgoGradientEcho2dUI()
{
}
  

NLS_STATUS gkg::GinkgoGradientEcho2dUI::registerUI( SeqLim & /*seqLim*/ )
{

  static const char* const ptModule = { 
                                    "gkg::GinkgoGradientEcho2dUI::registerUI" };

  return SEQU__NORMAL;

}


#endif

