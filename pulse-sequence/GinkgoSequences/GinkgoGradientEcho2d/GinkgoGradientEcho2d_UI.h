#ifndef _gkg_pulse_sequence_GinkgoGradientEcho2d_GinkgoGradientEcho2d_UI_h
#define _gkg_pulse_sequence_GinkgoGradientEcho2d_GinkgoGradientEcho2d_UI_h 1

#include "GinkgoGradientEcho2d.h"
#include <Core/SiemensInclude.h>

class MrProt;
class SeqLim;
class SeqExpo;
class Sequence;

namespace gkg
{


class GinkgoGradientEcho2dUI 
{

  public:

    GinkgoGradientEcho2dUI();
    virtual ~GinkgoGradientEcho2dUI();

    NLS_STATUS registerUI( SeqLim &seqLim );

};


}


#endif 

