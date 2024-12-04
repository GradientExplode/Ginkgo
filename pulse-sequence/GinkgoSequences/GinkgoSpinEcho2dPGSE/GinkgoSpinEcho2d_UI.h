#ifndef _gkg_pulse_sequence_GinkgoSpinEcho2d_GinkgoSpinEcho2d_UI_h
#define _gkg_pulse_sequence_GinkgoSpinEcho2d_GinkgoSpinEcho2d_UI_h 1

#include "GinkgoSpinEcho2d.h"
#include <Core/SiemensInclude.h>

class MrProt;
class SeqLim;
class SeqExpo;
class Sequence;

namespace gkg
{


class GinkgoSpinEcho2dUI 
{

  public:

    GinkgoSpinEcho2dUI();
    virtual ~GinkgoSpinEcho2dUI();

    NLS_STATUS registerUI( SeqLim &seqLim ); 

};


}


#endif 

