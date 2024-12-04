#include <Core/SiemensInclude.h>

#include "GinkgoGradientEcho3d.h"
#include "GinkgoGradientEcho3d_UI.h"


#ifdef WIN32


gkg::GinkgoGradientEcho3d* getSeq( MrUILinkBase* const pThis )
{
	    
  return static_cast< gkg::GinkgoGradientEcho3d* >( 
                                                   pThis->sequence().getSeq() );
  
}


const gkg::GinkgoGradientEcho3dUI* getUI( MrUILinkBase* const pThis )
{
	  
  return static_cast< gkg::GinkgoGradientEcho3d*>(
                                          pThis->sequence().getSeq() )->getUI();
  
}


gkg::GinkgoGradientEcho3dUI::GinkgoGradientEcho3dUI()
{
}


gkg::GinkgoGradientEcho3dUI::~GinkgoGradientEcho3dUI()
{
}
  

NLS_STATUS gkg::GinkgoGradientEcho3dUI::registerUI( SeqLim & /*seqLim*/ )
{

  static const char* const ptModule = { 
                                    "gkg::GinkgoGradientEcho3dUI::registerUI" };

#ifdef WIN32

  // Registration of set and solve handler methods

#endif

return SEQU__NORMAL;

}


#endif

