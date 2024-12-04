#ifndef _gkg_processing_container_FrontEvolutionRoi_h_
#define _gkg_processing_container_FrontEvolutionRoi_h_


#include <gkg-processing-container/Roi.h>
#include <gkg-processing-container/Front.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


template < class T >
class FrontEvolutionRoi : public Roi< T >
{

  public:

    FrontEvolutionRoi( RCPointer< Volume< T > > labelVolume,
                       const T& label,
                       int32_t stride = 1 );
    FrontEvolutionRoi( const FrontEvolutionRoi< T >& other );
    virtual ~FrontEvolutionRoi();

    const Front< T >& getFront() const;

    void evaluateSiteAddition( const Vector3d< int32_t >& site );
    void evaluateSiteRemoval( const Vector3d< int32_t >& site );
    void resetEvaluation();

    void addSite( const Vector3d< int32_t >& site );
    void removeSite( const Vector3d< int32_t >& site );

    void getRandomSite( Vector3d< int32_t >& site ) const;

  protected:

    Front< T > _front;

};


}


#endif
