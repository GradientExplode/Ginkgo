#ifndef _gkg_dmri_connectivity_matrix_VolumeToVolumeConnectivityMatrixFunctor_i_h_
#define _gkg_dmri_connectivity_matrix_VolumeToVolumeConnectivityMatrixFunctor_i_h_

#include <gkg-dmri-connectivity-matrix/VolumeToVolumeConnectivityMatrixFunctor.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::VolumeToVolumeConnectivityMatrixFunctor< L, Compare >::
                                       VolumeToVolumeConnectivityMatrixFunctor(
                        const std::vector< double >& /*scalarParameters*/,
                        const std::vector< std::string >& /*stringParameters*/ )
                                : gkg::ConnectivityMatrixFunctor< L, Compare >()
{
}


template < class L, class Compare >
inline
gkg::VolumeToVolumeConnectivityMatrixFunctor< L, Compare >::
                                      ~VolumeToVolumeConnectivityMatrixFunctor()
{
}


template < class L, class Compare >
inline
void gkg::VolumeToVolumeConnectivityMatrixFunctor< L, Compare >::apply(
                const std::list< std::string >& /*fileNameBundleMaps*/,
                const std::list< std::string >& /*fileNameListOfROIs1*/,
                const std::string& /*fileNameROIs1ToBundleMapTransform3d*/,
                const std::list< std::string >& /*fileNameListOfROIs2*/,
                const std::string& /*fileNameROIs2ToBundleMapTransform3d*/,
                const std::list< std::string >& /*functorNames*/,
                const std::list< std::string >& /*outputFileNames*/,
                const std::string& /*theOutputBundleMapFormat*/,
                const std::string& /*theOutputTextureMapFormat*/,
                bool /*ascii*/,
                bool /*verbose*/ ) const
{

  try
  {




  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::VolumeToVolumeConnectivityMatrixFunctor< L, Compare >::"
             "apply( "
             "const std::list< std::string >& fileNameBundleMaps, "
             "const std::list< std::string >& fileNameListOfROIs1, "
             "const std::string& fileNameROIs1ToBundleMapTransform3d, "
             "const std::list< std::string >& fileNameListOfROIs2, "
             "const std::string& fileNameROIs2ToBundleMapTransform3d, "
             "const std::list< std::string >& functorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::string& theOutputBundleMapFormat, "
             "const std::string& theOutputTextureMapFormat, "
             "bool ascii, "
             "bool verbose ) const" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeToVolumeConnectivityMatrixFunctor< L,
                                            Compare >::getScalarParameterHelp()
{

  try
  {

    std::string help;
    help += ".  N/A\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeToVolumeConnectivityMatrixFunctor< L, "
             "Compare >::getScalarParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeToVolumeConnectivityMatrixFunctor< L,
                                            Compare >::getStringParameterHelp()
{

  try
  {

    std::string help;
    help += ".  N/A\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeToVolumeConnectivityMatrixFunctor< L, "
             "Compare >::getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::VolumeToVolumeConnectivityMatrixFunctor< L,
                                            Compare >::getFunctorHelp()
{

  try
  {

    std::string help;
    help += ".  N/A\n\n";

    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::VolumeToVolumeConnectivityMatrixFunctor< L, "
             "Compare >::getFunctorHelp()" );

}


#endif
