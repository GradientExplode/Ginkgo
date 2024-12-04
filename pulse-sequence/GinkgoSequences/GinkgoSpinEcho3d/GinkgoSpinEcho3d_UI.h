#ifndef _gkg_pulse_sequence_GinkgoSpinEcho3d_GinkgoSpinEcho3d_UI_h
#define _gkg_pulse_sequence_GinkgoSpinEcho3d_GinkgoSpinEcho3d_UI_h 1

#include "GinkgoSpinEcho3d.h"
#include <Core/SiemensInclude.h>

class MrProt;
class SeqLim;
class SeqExpo;
class Sequence;

namespace gkg
{


class GinkgoSpinEcho3dUI 
{

  public:

    GinkgoSpinEcho3dUI();
    virtual ~GinkgoSpinEcho3dUI();

    NLS_STATUS registerUI( SeqLim &seqLim ); 
  
};


}


#endif 

