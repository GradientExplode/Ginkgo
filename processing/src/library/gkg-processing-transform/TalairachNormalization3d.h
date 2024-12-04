#ifndef _gkg_processing_transform_TalairachNormalization3d_h_
#define _gkg_processing_transform_TalairachNormalization3d_h_


#include <gkg-processing-transform/Scaling3d.h>


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;
class HeaderedObject;
class AcPcIhCoordinates;


template < class T >
class TalairachNormalization3d : public Scaling3d< T >
{

  public:

    TalairachNormalization3d();
    template < class V > 
    TalairachNormalization3d( const Volume< V >& volume,
                              const TestFunction< V >& testFunction,
                              const AcPcIhCoordinates& acPcIhCoordinates );
    TalairachNormalization3d( const HeaderedObject& headeredObject );
    TalairachNormalization3d( const Vector3d< T >& scaling );
    TalairachNormalization3d( const TalairachNormalization3d< T >& other );
    virtual ~TalairachNormalization3d();

    TalairachNormalization3d< T >&
      operator=( const TalairachNormalization3d< T >& other );

    void addToHeaderedObject( HeaderedObject& other ) const;
    
    static void addSemantic( HeaderedObject& other );


};


}


#endif

