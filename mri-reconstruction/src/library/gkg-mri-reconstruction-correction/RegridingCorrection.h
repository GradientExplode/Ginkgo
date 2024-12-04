#ifndef _gkg_mri_reconstruction_correction_RegridingCorrection_h_
#define _gkg_mri_reconstruction_correction_RegridingCorrection_h_


#include <complex>
#include <vector>
#include <list>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class RegridingCorrection
{

  public:

    RegridingCorrection( int32_t inSizeX );
    RegridingCorrection( const std::vector< T >& inTrajectory );
    RegridingCorrection( const std::list< std::vector< T > >& inTrajectories );

    virtual ~RegridingCorrection();

    int32_t getTrajectoryCount() const;
    T getEffectiveSize( int32_t t ) const;
    T getLongestSize() const;

    void setOutputSampleCount( int32_t outSizeX, const T& newEffSize = -1.0 );
    void setOutputTrajectory( const std::vector< T >& outTrajectory );
    void setOutputTrajectories(
                         const std::list< std::vector< T > >& outTrajectories );

    void apply( const std::vector< std::complex< T > >& in,
                std::vector< std::complex< T > >& out,
                int32_t trajectory = 0 ) const;
    void apply( const Volume< std::complex< T > >& volumeIn,
                Volume< std::complex< T > >& volumeOut,
                const std::vector< int32_t >& trajectories,
                bool verbose ) const;
    void apply( const Volume< std::complex< T > >& volumeIn,
                Volume< std::complex< T > >& volumeOut,
                bool verbose ) const;

  private:

    void initializeIn( const std::list< std::vector< T > >& inTrajectories );
    void initializeOut( const std::list< std::vector< T > >& outTrajectories );
    T kernel( const T& value ) const;

    Volume< T > _inKSpace;
    Volume< T > _outKSpace;
    Volume< T > _weighting;
    Volume< int32_t > _left;
    Volume< int32_t > _right;
    std::vector< T > _effectiveSize;

};


}


#endif
