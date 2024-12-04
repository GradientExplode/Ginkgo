#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperator_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperator_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMapOperator< L, Compare >::BundleMapOperator( bool verbose )
                       : _verbose( verbose )
{
}


template < class L, class Compare >
inline
gkg::BundleMapOperator< L, Compare >::~BundleMapOperator()
{
}


template < class L, class Compare >
inline
bool gkg::BundleMapOperator< L, Compare >::saveOutputBundleMap() const
{

  try
  {

    return true;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool gkg::BundleMapOperator< L, Compare >::"
             "saveOutputBundleMap() const" );

}


#endif

