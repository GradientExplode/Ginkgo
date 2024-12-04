#include <gkg-mri-reconstruction-parallel/ParallelReconstructor.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-mri-reconstruction-parallel/ReductionStrategy.h>


template < class T >
gkg::ParallelReconstructor< T >::ParallelReconstructor()
{
}


template < class T >
gkg::ParallelReconstructor< T >::~ParallelReconstructor()
{
}


template < class T >
void gkg::ParallelReconstructor< T >::setReductionStrategy(
                            typename gkg::ParallelReconstructor< T >::Axis axis,
                            const gkg::ReductionStrategy< T >& strategy )
{

  try
  {

    switch ( axis )
    {

      case gkg::ParallelReconstructor< T >::XAxis:

        _xReductionStrategy = strategy;
        break;

      case gkg::ParallelReconstructor< T >::YAxis:

        _yReductionStrategy = strategy;
        break;

      case gkg::ParallelReconstructor< T >::ZAxis:

        _zReductionStrategy = strategy;
        break;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ParallelReconstructor< T >::setReductionStrategy( "
             "typename gkg::ParallelReconstructor< T >::ReductionAxis axis, "
             "const gkg::ReductionStrategy< T >& strategy ) const" );

}


template class gkg::ParallelReconstructor< float >;
template class gkg::ParallelReconstructor< double >;
