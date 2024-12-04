#include <gkg-dmri-container/DiffusoidCartesianField.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-coordinates/SO3Interpolator.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/TypeOf.h>


RegisterBaseType( gkg::Diffusoid*,
                  gkg_Diffusoid_pointer );


gkg::DiffusoidCartesianField::DiffusoidCartesianField(
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               const gkg::OrientationSet& outputOrientationSet,
                               const gkg::SO3Interpolator* so3Interpolator )
                             : gkg::DiffusionCartesianField< gkg::Diffusoid >(
                                                            sizeX, sizeY, sizeZ,
                                                            resolution,
                                                            siteMap ),
                               _outputOrientationSet( outputOrientationSet ),
                               _so3Interpolator( so3Interpolator )
{


  try
  {

    if ( _so3Interpolator == 0 )
    {

      throw std::runtime_error( "nul SO3 interpolator pointer" );

    }

  }
  GKG_CATCH( "gkg::DiffusoidCartesianField::DiffusoidCartesianField( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, "
             "const gkg::Vector3d< double >& resolution, "
             "const gkg::SiteMap< int32_t, int32_t >& siteMap, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator )" );

}


gkg::DiffusoidCartesianField::DiffusoidCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::OrientationSet* outputOrientationSet,
                               const gkg::SO3Interpolator* so3Interpolator )
                             : gkg::DiffusionCartesianField< gkg::Diffusoid >(
                                                                 t2, dw, mask ),
                               _so3Interpolator( so3Interpolator )
{

  try
  {

    if ( _so3Interpolator == 0 )
    {

      throw std::runtime_error( "nul SO3 interpolator pointer" );

    }

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );


  }
  GKG_CATCH( "gkg::DiffusoidCartesianField::DiffusoidCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator )" );


}


gkg::DiffusoidCartesianField::DiffusoidCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< float >& boundingBox,
                               const gkg::Vector3d< float >& superResolution,
                               const gkg::OrientationSet* outputOrientationSet,
                               const gkg::SO3Interpolator* so3Interpolator )
                             : gkg::DiffusionCartesianField< gkg::Diffusoid >(
                                   t2, dw, mask, boundingBox, superResolution ),
                               _so3Interpolator( so3Interpolator )
{

  try
  {

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::DiffusoidCartesianField::DiffusoidCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator )" );

}


gkg::DiffusoidCartesianField::DiffusoidCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< int32_t >& boundingBox,
                               const gkg::OrientationSet* outputOrientationSet,
                               const gkg::SO3Interpolator* so3Interpolator )
                             : gkg::DiffusionCartesianField< gkg::Diffusoid >(
                                                    t2, dw, mask, boundingBox ),
                               _so3Interpolator( so3Interpolator )
{

  try
  {

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::DiffusoidCartesianField::DiffusoidCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator )" );

}


gkg::DiffusoidCartesianField::DiffusoidCartesianField(
                                const gkg::SiteMap< int32_t, int32_t >& siteMap,
                                const gkg::TextureMap< gkg::Diffusoid >& items )
                      : gkg::DiffusionCartesianField< gkg::Diffusoid >( siteMap,
                                                                        items )
{
}


gkg::DiffusoidCartesianField::~DiffusoidCartesianField()
{
}


const gkg::OrientationSet& 
gkg::DiffusoidCartesianField::getOutputOrientationSet() const
{

  return _outputOrientationSet;

}


void gkg::DiffusoidCartesianField::setOutputOrientationSet(
                               const gkg::OrientationSet* outputOrientationSet )
{

  try
  {

    if ( !outputOrientationSet )
    {

      // by default, we choose the symmetrical orientation set corresponding to
      // the acquisition orientation set
      _outputOrientationSet = gkg::OrientationSet(
                            _inputOrientationSet.getSymmetricalOrientations() );

    }
    else
    {

      _outputOrientationSet = *outputOrientationSet;

    }

  }
  GKG_CATCH( "void gkg::DiffusoidCartesianField::setOutputOrientationSet( "
             "const gkg::OrientationSet* outputOrientationSet )" );

}


void gkg::DiffusoidCartesianField::getReconstructionMatrix( bool /*verbose*/ )
{

  try
  {

    // the input orientation set needs to be symmetrized before processing
    // diffusoid
    std::vector< gkg::Vector3d< float > >
      orientations( 2 * _inputOrientationSet.getCount() );
    int32_t o;
    for ( o = 0; o < _inputOrientationSet.getCount(); o++ )
    {

      orientations[ o ] = _inputOrientationSet.getOrientation( o );
      orientations[ o + _inputOrientationSet.getCount() ] = -orientations[ o ];

    }
    gkg::OrientationSet inputOrientationSet( orientations );


    // measure count
    int32_t M = inputOrientationSet.getCount();

    // reconstruction point count
    int32_t N = _outputOrientationSet.getCount();

    // allocating final reconstruction matrix
    _reconstructionMatrix.reallocate( N, M );
    _reconstructionMatrix.fill( 0.0 );

    // building reinterpolation matrix
    int32_t m, n;
    for ( n = 0; n < N; n++ )
    {

      std::vector< std::pair< int32_t, float > >
        indicesAndWeights = _so3Interpolator->getNeighborIndicesAndWeights(
                                    inputOrientationSet,
                                    _outputOrientationSet.getOrientation( n ) );
      for ( m = 0; m < ( int32_t )indicesAndWeights.size(); m++ )
      {

        _reconstructionMatrix( n, indicesAndWeights[ m ].first ) =
          indicesAndWeights[ m ].second;

      }

    }

  }
  GKG_CATCH( "void gkg::DiffusoidCartesianField::getReconstructionMatrix( "
             "bool /*verbose*/ )" );

}

