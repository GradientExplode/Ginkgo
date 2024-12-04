#include <gkg-mri-reconstruction-core/Reconstructor.h>


template < class T >
gkg::Reconstructor< T >::Reconstructor()
{
}


template < class T >
gkg::Reconstructor< T >::~Reconstructor()
{
}


template class gkg::Reconstructor< float >;
template class gkg::Reconstructor< double >;
