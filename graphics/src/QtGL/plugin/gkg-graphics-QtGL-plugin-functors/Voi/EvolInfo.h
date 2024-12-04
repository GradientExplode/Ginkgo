#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_EvolInfo_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_EvolInfo_h_


#include <QObject>


namespace gkg
{


class VoiEvolutionInfo : public QObject
{

  Q_OBJECT

 public:

    enum EvolDim
    {
    
      Dim2,
      Dim3
    };

    enum EvolConnex
    {
    
      Connex4 = 0,
      Connex8,
      Connex6,
      Connex18,
      Connex26
      
    };

    enum EvolDisp
    {
    
      Straight,
      StepByStep
      
    };

    VoiEvolutionInfo();
    virtual ~VoiEvolutionInfo();

    int32_t value();
    float ising();
    float kin();
    float kout();

    EvolDim dim();
    EvolConnex connex();
    EvolDisp disp();

  public slots:
 
    void setIsing( float );
    void setValue( int );

    void setDim( int );
    void setConnex( int );
    void setDisp( int );

  private:

    void computeCoefficients();

    int32_t cValue;
    float cIsing;
    float cKin;
    float cKout;
    float nComp;

    EvolDim    eDim;
    EvolConnex eConnex;
    EvolDisp   eDisp;

};


}


#endif
