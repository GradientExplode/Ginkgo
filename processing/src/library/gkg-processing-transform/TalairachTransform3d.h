#ifndef _gkg_processing_transform_TalairachTransform3d_h_
#define _gkg_processing_transform_TalairachTransform3d_h_


#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineWoShearingTransform3d.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class TalairachTransform3d : public CompositeTransform3d< T >
{

  public:
  
    
    template < class V1, class V2 >
    TalairachTransform3d( const Volume< V1 >& anyVolume,
                          const Volume< V2 >& talairachVolume );
    TalairachTransform3d( const TalairachTransform3d< T >& other );
    virtual ~TalairachTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    TalairachTransform3d< T >& 
      operator=( const TalairachTransform3d< T >& other );

    RCPointer< AffineWoShearingTransform3d< T > >
      getAffineWoShearingTransform3d() const;
    RCPointer< ParameterizedAffineWoShearingTransform3d< T > >
      getParameterizedAffineWoShearingTransform3d() const;

};


}


#endif

