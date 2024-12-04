#ifndef _gkg_mri_reconstruction_parallel_ReductionStrategy_h_
#define _gkg_mri_reconstruction_parallel_ReductionStrategy_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


template < class T >
class ReductionStrategy
{

  public:

    ReductionStrategy( const T& reductionFactor = 1.0 );
    ReductionStrategy( const ReductionStrategy< T >& other );
    virtual ~ReductionStrategy();

    ReductionStrategy< T >& operator=( const ReductionStrategy< T >& other );

    const T& getReductionFactor() const;
    int32_t getUnfoldedPositionCount() const;
    void getUnfoldedPositions( const T& foldedPosition,
                               int32_t fullSize,
                               std::vector< T >& unfoldedPositions ) const;

  protected:

    T _reductionFactor;

};


}


#endif
