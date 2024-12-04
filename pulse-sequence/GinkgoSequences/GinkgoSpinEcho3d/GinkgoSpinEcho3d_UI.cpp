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

NLS_STATUS gkg::GinkgoSpinEcho3dUI::registerUI( SeqLim &seqLim ) 
{

  static const char* const ptModule = { 
                                    "gkg::GinkgoSpinEcho3dUI::registerUI" };

  return SEQU__NORMAL;

}

