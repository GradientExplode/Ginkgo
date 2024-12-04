#include <Core/SiemensInclude.h>

#include "GinkgoSpinEcho2d.h"
#include "GinkgoSpinEcho2d_UI.h"


#ifdef WIN32


gkg::GinkgoSpinEcho2d* getSeq( MrUILinkBase* const pThis )
{
	    
  return static_cast< gkg::GinkgoSpinEcho2d* >( pThis->sequence().getSeq() );
  
}


const gkg::GinkgoSpinEcho2dUI* getUI( MrUILinkBase* const pThis )
{
	  
  return static_cast< gkg::GinkgoSpinEcho2d*>(
                                          pThis->sequence().getSeq() )->getUI();
  
}


gkg::GinkgoSpinEcho2dUI::GinkgoSpinEcho2dUI()
{
}


gkg::GinkgoSpinEcho2dUI::~GinkgoSpinEcho2dUI()
{
}
  

NLS_STATUS gkg::GinkgoSpinEcho2dUI::registerUI( SeqLim & /*seqLim*/ )
{

  static const char* const ptModule = { "gkg::GinkgoSpinEcho2dUI::registerUI" };

  return SEQU__NORMAL;

}


#endif

