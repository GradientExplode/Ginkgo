#ifndef _gkg_dmri_tractography_DeterministicTractographyAlgorithm_i_h_
#define _gkg_dmri_tractography_DeterministicTractographyAlgorithm_i_h_


#include <gkg-dmri-tractography/DeterministicTractographyAlgorithm.h>
#include <gkg-dmri-tractography/StreamlineTractographyAlgorithm_i.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::DeterministicTractographyAlgorithm< L, Compare >::
                             DeterministicTractographyAlgorithm(
                                      const gkg::Vector3d< int32_t >& size,
                                      const gkg::Vector3d< double >& resolution,
                                      float step,
                                      int32_t storingIncrement,
                                      float minimumFiberLength,
                                      float maximumFiberLength,
                                      float apertureAngle )
                           : gkg::StreamlineTractographyAlgorithm< L, Compare >(
                                                          size,
                                                          resolution,
                                                          step,
                                                          storingIncrement,
                                                          minimumFiberLength,
                                                          maximumFiberLength,
                                                          apertureAngle )
{
}


template < class L, class Compare >
inline
gkg::DeterministicTractographyAlgorithm< L, Compare >::
                             DeterministicTractographyAlgorithm(
                      const gkg::Vector3d< int32_t >& size,
                      const gkg::Vector3d< double >& resolution,
                      const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& /* stringParameters */ )
                           : gkg::StreamlineTractographyAlgorithm< L, Compare >(
                                     size,
                                     resolution,
                                     ( float )scalarParameters[ 0 ],
                                     ( int32_t )( scalarParameters[ 1 ] + 0.5 ),
                                     ( float )scalarParameters[ 2 ],
                                     ( float )scalarParameters[ 3 ],
                                     ( float )scalarParameters[ 4 ] )
{
}


template < class L, class Compare >
inline
gkg::DeterministicTractographyAlgorithm< L, Compare >::
                                           ~DeterministicTractographyAlgorithm()
{
}


template < class L, class Compare >
inline
std::string 
gkg::DeterministicTractographyAlgorithm< L, Compare >::getStaticName()
{

  return "streamline-deterministic";

}


template < class L, class Compare >
inline
bool gkg::DeterministicTractographyAlgorithm< L, Compare >::track(
              const gkg::Vector3d< float >& startingPointOdfs,
              const int32_t& startingOrientationIndex,
              gkg::OdfContinuousField& odfContinuousField,
              const gkg::RCPointer< gkg::StoppingStrategy >& stoppingStrategy,
              std::list< gkg::Vector3d< float > >& trajectory,
              float& fiberLength ) const
{

  try
  {

    // getting reference to the output orientation set of the ODF field
    const gkg::OrientationSet&
      orientationSet = odfContinuousField.getOutputOrientationSet();

    // initializing current point & orientation in ODF real frame
    gkg::Vector3d< float > pointOdfs = startingPointOdfs;
    int32_t orientationIndex = startingOrientationIndex;

    // performing the tracking
    gkg::OrientationDistributionFunction* odf = 0;
    float gfa = 0.0f;
    bool keepFiber = true;

    while ( !stoppingStrategy->stop( pointOdfs,
                                      odf,
                                      gfa,
                                      fiberLength,
                                      trajectory,
                                      keepFiber ) )
    {


      // computing the index of the principal orientation corresponding to
      // the maximum probability in the solid angle around the current 
      // orientation and of aperture angle _apertureAngle, and collecting the 
      // orientation indices
      orientationIndex = 
                      odf->getPrincipalOrientationIndexFast( orientationIndex );
      
      // deleting ODF
      delete odf;

      // computing the new position
      pointOdfs += orientationSet.getOrientation( orientationIndex ) *
                   this->_step;

    }

    return keepFiber;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool gkg::DeterministicTractographyAlgorithm< L, Compare >:: "
             "track( "
             "const gkg::Vector3d< float >& startingPointOdfs, "
             "const int32_t& startingOrientationIndex, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::RCPointer< gkg::StoppingStrategy >& stoppingStrategy, "
             "std::list< gkg::Vector3d< float > >& trajectory, "
             "float& fiberLength ) const" );

}


#endif

