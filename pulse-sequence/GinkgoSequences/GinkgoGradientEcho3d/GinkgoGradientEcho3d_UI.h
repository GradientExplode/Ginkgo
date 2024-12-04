#ifndef _gkg_pulse_sequence_GinkgoGradientEcho3d_GinkgoGradientEcho3d_UI_h
#define _gkg_pulse_sequence_GinkgoGradientEcho3d_GinkgoGradientEcho3d_UI_h 1

#include "GinkgoGradientEcho3d.h"
#include <Core/SiemensInclude.h>

class MrProt;
class SeqLim;
class SeqExpo;
class Sequence;

namespace gkg
{


class GinkgoGradientEcho3dUI 
{

  public:

    GinkgoGradientEcho3dUI();
    virtual ~GinkgoGradientEcho3dUI();

    NLS_STATUS registerUI( SeqLim &seqLim );

#ifdef WIN32

    //UI_ELEMENT_LONG       m_Contrast;   // Example for set-handler
    //UI_ELEMENT_SELECTION  m_GradMode;   // Example for solve-handler

#endif 

};


}


#endif 

