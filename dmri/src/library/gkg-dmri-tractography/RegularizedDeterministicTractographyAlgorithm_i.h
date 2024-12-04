#ifndef _gkg_dmri_tractography_RegularizedDeterministicTractographyAlgorithm_i_h_
#define _gkg_dmri_tractography_RegularizedDeterministicTractographyAlgorithm_i_h_


#include <gkg-dmri-tractography/RegularizedDeterministicTractographyAlgorithm.h>
#include <gkg-dmri-tractography/StreamlineTractographyAlgorithm_i.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


#define CUMULATED_HISTOGRAM_BIN_COUNT 1000
#define CUMULATED_HISTOGRAM_THRESHOLD 0.99


template < class L, class Compare >
inline
gkg::RegularizedDeterministicTractographyAlgorithm< L, Compare >::
                             RegularizedDeterministicTractographyAlgorithm(
                                      const gkg::Vector3d< int32_t >& size,
                                      const gkg::Vector3d< double >& resolution,
                                      float step,
                                      int32_t storingIncrement,
                                      float minimumFiberLength,
                                      float maximumFiberLength,
                                      float apertureAngle,
                                      float gfaLowerBoundary,
                                      float gfaUpperBoundary )
                           : gkg::StreamlineTractographyAlgorithm< L, Compare >(
                                                          size,
                                                          resolution,
                                                          step,
                                                          storingIncrement,
                                                          minimumFiberLength,
                                                          maximumFiberLength,
                                                          apertureAngle ),
                             _gfaLowerBoundary( gfaLowerBoundary ),
                             _gfaUpperBoundary( gfaUpperBoundary )
{
}


template < class L, class Compare >
inline
gkg::RegularizedDeterministicTractographyAlgorithm< L, Compare >::
                             RegularizedDeterministicTractographyAlgorithm(
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
                                     ( float )scalarParameters[ 4 ] ),
                             _gfaLowerBoundary(
                                               ( float )scalarParameters[ 5 ] ),
                             _gfaUpperBoundary(
                                               ( float )scalarParameters[ 6 ] )
{
}


template < class L, class Compare >
inline
gkg::RegularizedDeterministicTractographyAlgorithm< L, Compare >::
                                ~RegularizedDeterministicTractographyAlgorithm()
{
}


template < class L, class Compare >
inline
float 
gkg::RegularizedDeterministicTractographyAlgorithm< L,
                                          Compare >::getGFALowerBoundary() const
{

  return _gfaLowerBoundary;

}


template < class L, class Compare >
inline
float 
gkg::RegularizedDeterministicTractographyAlgorithm< L,
                                          Compare >::getGFAUpperBoundary() const
{

  return _gfaUpperBoundary;

}


template < class L, class Compare >
inline
void gkg::RegularizedDeterministicTractographyAlgorithm< L, Compare >::prepare(
                               gkg::OdfCartesianField& odfCartesianField,
                               const gkg::Volume< int16_t >& computingMask,
                               const gkg::Transform3d< float >&
                                         /*transform3dFromRoisVoxelToOdfsReal*/,
                               const gkg::Transform3d< float >&
                                        /*transform3dFromRoisVoxelToMaskVoxel*/,
                               const gkg::Transform3d< float >&
                                            transform3dFromOdfsRealToMaskVoxel,
                               const gkg::Transform3d< float >&
                                         /*transform3dFromOdfsRealToRoisVoxel*/,
                               bool verbose )
{

  try
  {

    float gfaLowerBoundary = 0.0;
    float gfaUpperBoundary = 1.0; 

    if ( ( _gfaLowerBoundary < 0.0 ) ||
         ( _gfaUpperBoundary < 0.0 ) )
    {

      // getting offset
      gkg::Vector3d< int32_t > 
        offset = odfCartesianField.getOffset();

      // processing GFA map and bounding box
      std::vector< double > scalarParameters;
      std::vector< std::string > stringParameters;
      gkg::Volume< float > gfa;
      gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                              getOdfFunctor( "gfa" ).process( gfa,
                                                              odfCartesianField,
                                                              scalarParameters,
                                                              stringParameters,
                                                              verbose );
      gkg::BoundingBox< int32_t > gfaBoundingBox( 0, gfa.getSizeX() - 1,
                                                  0, gfa.getSizeY() - 1,
                                                  0, gfa.getSizeZ() - 1 );


      // processing GFA boundaries
      if ( verbose )
      {

        std::cout << "processing GFA boundaries : " << std::flush;

      }
      gkg::Vector3d< int32_t > voxelOdfs, site;
      gkg::Vector3d< float > floatVoxelMask;
      gkg::Vector3d< float > pointOdfs;
      float lowerBoundary = 1.0;
      float upperBoundary = 0.0;
      float value = 0.0;

      gkg::Vector3d< int32_t > voxel;
      for ( voxel.z = 0; voxel.z < computingMask.getSizeZ(); voxel.z++ )
      {

        for ( voxel.y = 0; voxel.y < computingMask.getSizeY(); voxel.y++ )
        {

          for ( voxel.x = 0; voxel.x < computingMask.getSizeX(); voxel.x++ )
          {

            if ( computingMask( voxel ) )
            {

              // processing site coordinate in the ODF voxel frame
              floatVoxelMask.x = ( float )voxel.x;
              floatVoxelMask.y = ( float )voxel.y;
              floatVoxelMask.z = ( float )voxel.z;
              transform3dFromOdfsRealToMaskVoxel.getInverse( floatVoxelMask,
                                                             pointOdfs );
              voxelOdfs = this->getVoxelCoordinates( pointOdfs );
              site = voxelOdfs - offset;

              if ( gfaBoundingBox.contains( site ) )
              {

                value = gfa( site );
                if ( value < lowerBoundary )
                {

                  lowerBoundary = value;

                }
                else if ( value > upperBoundary )
                {

                  upperBoundary = value;

                }

              }

            }

          }

        }

      }
      if ( verbose )
      {

        std::cout << lowerBoundary << ", " << upperBoundary << std::endl;

      }

      if ( std::fabs( upperBoundary - lowerBoundary ) > 1e-6 )
      {

        // processing GFA cumulated histogram
        if ( verbose )
        {

          std::cout << "processing GFA cumulated histogram : " << std::flush;

        }
        std::vector< float >
          cumulatedHistogram( CUMULATED_HISTOGRAM_BIN_COUNT, 0.0f );
        int32_t level = 0;

        for ( voxel.z = 0; voxel.z < computingMask.getSizeZ(); voxel.z++ )
        {

          for ( voxel.y = 0; voxel.y < computingMask.getSizeY(); voxel.y++ )
          {

            for ( voxel.x = 0; voxel.x < computingMask.getSizeX(); voxel.x++ )
            {

              if ( computingMask( voxel ) )
              {

                // processing site coordinate in the ODF voxel frame
                floatVoxelMask.x = ( float )voxel.x;
                floatVoxelMask.y = ( float )voxel.y;
                floatVoxelMask.z = ( float )voxel.z;
                transform3dFromOdfsRealToMaskVoxel.getInverse( floatVoxelMask,
                                                               pointOdfs );
                voxelOdfs = this->getVoxelCoordinates( pointOdfs );
                site = voxelOdfs - offset;

                if ( gfaBoundingBox.contains( site ) )
                {

                  value = gfa( site );
                  level = ( int32_t )( ( value - lowerBoundary ) *
                                       ( float )( CUMULATED_HISTOGRAM_BIN_COUNT
                                                  - 1 ) /
                                       ( upperBoundary - lowerBoundary ) );
                  cumulatedHistogram[ level ] += 1.0f;

                }

              }

            }

          }

        }

        // normalizing histogram
        float sum = 0;
        std::vector< float >::iterator h = cumulatedHistogram.begin(),
                                       he = cumulatedHistogram.end();
        while ( h != he )
        {

          sum += *h;
          ++ h;

        }


        if ( sum > 0.0 )
        {

          h = cumulatedHistogram.begin();
          while ( h != he )
          {

            *h /= sum;
            ++ h;

          }

        }

        size_t hh = 0U;

        for ( hh = 1; hh < cumulatedHistogram.size(); hh++ )
        {

          cumulatedHistogram[ hh ] += cumulatedHistogram[ hh - 1 ];

        }
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

        if ( verbose )
        {

          std::cout << "processing GFA rescaler boundaries : " << std::flush;

        }

        // processing GFA rescaler boundaries
        level = CUMULATED_HISTOGRAM_BIN_COUNT - 1;
        while ( ( level >= 0 ) &&
                ( cumulatedHistogram[ level ] >
                  CUMULATED_HISTOGRAM_THRESHOLD ) )
        {

          -- level;

        }
     
        gfaLowerBoundary = lowerBoundary;
        gfaUpperBoundary = lowerBoundary + ( float )level * 
                               ( upperBoundary - lowerBoundary ) /
                               ( float )( CUMULATED_HISTOGRAM_BIN_COUNT - 1 );

      }

      if ( verbose )
      {

        std::cout << gfaLowerBoundary << ", " << gfaUpperBoundary << std::endl;

      }

    }

    if ( verbose )
    {

      std::cout << "used GFA rescaler boundaries : " << std::flush;

    }

    if ( _gfaLowerBoundary >= 0.0 )
    {

      gfaLowerBoundary = _gfaLowerBoundary;

    }
    if ( _gfaUpperBoundary > 0.0 )
    {

      gfaUpperBoundary = _gfaUpperBoundary;

    }
    _gfaRescaler = gkg::Rescaler< float, float >( gfaLowerBoundary,
                                                  gfaUpperBoundary,
                                                  0.0,
                                                  1.0 );

    gkg::Rescaler< gkg::Volume< float >,
                   gkg::Volume< float > > rescaler( gfaLowerBoundary,
                                                    gfaUpperBoundary,
                                                    0.0,
                                                    1.0 );
    if ( verbose )
    {

      std::cout << gfaLowerBoundary << ", " << gfaUpperBoundary << std::endl;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::RegularizedDeterministicTractographyAlgorithm< L, "
             "Compare >::"
             "prepare( gkg::OdfCartesianField& odfCartesianField, "
             "const gkg::Volume< int16_t >& computingMask, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToOdfsReal, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToMaskVoxel, "
             "const gkg::Transform3d< float >& "
             "transform3dFromOdfsRealToMaskVoxel, "
             "const gkg::Transform3d< float >& "
             "transform3dFromOdfsRealToRoisVoxel, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
std::string 
gkg::RegularizedDeterministicTractographyAlgorithm< L, Compare >::getStaticName(
                                                                               )
{

  try
  {

    return "streamline-regularized-deterministic";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::RegularizedDeterministicTractographyAlgorithm< L, "
             "Compare >::getStaticName()" );

}


template < class L, class Compare >
inline
bool gkg::RegularizedDeterministicTractographyAlgorithm< L, Compare >::track(
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
    float alpha = 0.0f;
    int32_t principalOrientationIndex = -1;
    gkg::Vector3d< float > orientation;
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
      principalOrientationIndex = 
                      odf->getPrincipalOrientationIndexFast( orientationIndex );
      
      // processing alpha
      _gfaRescaler.rescale( gfa,
                            alpha );

      // computing the new 'regularized' orientation and orientation index
      orientation =
        orientationSet.getOrientation( orientationIndex ) * ( 1 - alpha ) +
        orientationSet.getOrientation( principalOrientationIndex ) * alpha;
      orientation.normalize();
      orientationIndex = orientationSet.getNearestOrientationIndex(
                                                                 orientation );

      // deleting ODF
      delete odf;

      // computing the new position
      pointOdfs += orientation * this->_step;


    }

    return keepFiber;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool gkg::RegularizedDeterministicTractographyAlgorithm< L, "
             "Compare >:: "
             "track( "
             "const gkg::Vector3d< float >& startingPointOdfs, "
             "const int32_t& startingOrientationIndex, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::RCPointer< gkg::StoppingStrategy >& stoppingStrategy, "
             "std::list< gkg::Vector3d< float > >& trajectory, "
             "float& fiberLength ) const" );

}


#endif
