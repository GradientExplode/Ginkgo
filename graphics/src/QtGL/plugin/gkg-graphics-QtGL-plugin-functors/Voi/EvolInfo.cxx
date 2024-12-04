#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolInfo.h>


gkg::VoiEvolutionInfo::VoiEvolutionInfo() 
                     : QObject(),
                       cValue( 10 ),
                       cIsing( 1.0 ),
                       cKin( 2.1 ),
                       cKout( 4.0 ),
                       nComp( 4.0 ),
                       eDim( gkg::VoiEvolutionInfo::Dim2 ),
                       eConnex( gkg::VoiEvolutionInfo::Connex4 ),
                       eDisp( gkg::VoiEvolutionInfo::Straight )		      
{
}


gkg::VoiEvolutionInfo::~VoiEvolutionInfo()
{
}


int32_t gkg::VoiEvolutionInfo::value()
{

  return cValue;
  
}


float gkg::VoiEvolutionInfo::ising()
{

  return cIsing;
  
}


float gkg::VoiEvolutionInfo::kin()
{

  return cKin;
  
}


float gkg::VoiEvolutionInfo::kout()
{

  return cKout;
  
}


gkg::VoiEvolutionInfo::EvolDim gkg::VoiEvolutionInfo::dim()
{ 

  return eDim;
  
}


gkg::VoiEvolutionInfo::EvolConnex gkg::VoiEvolutionInfo::connex()
{

  return eConnex;
  
}


gkg::VoiEvolutionInfo::EvolDisp gkg::VoiEvolutionInfo::disp()
{

  return eDisp;
  
}


void gkg::VoiEvolutionInfo::setIsing( float val )
{

  cIsing = val;

  computeCoefficients();

}


void gkg::VoiEvolutionInfo::setValue( int val )
{

  cValue = val;

}


void gkg::VoiEvolutionInfo::setDim( int id )
{

  eDim = (gkg::VoiEvolutionInfo::EvolDim)( gkg::VoiEvolutionInfo::Dim2 + id );

  if ( eDim == gkg::VoiEvolutionInfo::Dim2 )
  {
  
    nComp = 4.0;
    
  }
  else
  {
  
    nComp = 6.0;
    
  }

  computeCoefficients();

}


void gkg::VoiEvolutionInfo::setConnex( int id )
{

  float nC[] = { 4.0, 8.0, 6.0, 18.0, 26.0 };

  if ( eDim == gkg::VoiEvolutionInfo::Dim2 )
  {
  
    eConnex = 
          (gkg::VoiEvolutionInfo::EvolConnex)( gkg::VoiEvolutionInfo::Connex4 + 
                                               id );
    
  }  
  else
  {
  
    eConnex = 
          (gkg::VoiEvolutionInfo::EvolConnex)( gkg::VoiEvolutionInfo::Connex6 + 
                                               id );

  }
  
  nComp = nC[ (int32_t)eConnex ];

  computeCoefficients();
  
}


void gkg::VoiEvolutionInfo::setDisp( int id )
{

  eDisp = (gkg::VoiEvolutionInfo::EvolDisp)( gkg::VoiEvolutionInfo::Straight + 
                                             id );

}


void gkg::VoiEvolutionInfo::computeCoefficients()
{

  cKout = nComp * cIsing;
  cKin = ( nComp - 2.0 ) * cIsing + 0.1;

}
