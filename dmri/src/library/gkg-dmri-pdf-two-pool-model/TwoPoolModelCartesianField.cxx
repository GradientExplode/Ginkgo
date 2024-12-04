#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelCartesianField.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/TypeOf.h>


RegisterBaseType( gkg::TwoPoolModel*,
                  gkg_TwoPoolModel_pointer );



gkg::TwoPoolModelCartesianField::TwoPoolModelCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& /*specificScalarParameters*/,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                                : gkg::DiffusionCartesianField<
                                            gkg::TwoPoolModel >( t2, dw, mask ),
                                  _fitMethod( getFitMethod(
                                               specificStringParameters[ 0 ] ) )
                
{

  try
  {

    getQSpaceSamples();
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::TwoPoolModelCartesianField::TwoPoolModelCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );


}


gkg::TwoPoolModelCartesianField::TwoPoolModelCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::BoundingBox< float >& boundingBox,
                     const gkg::Vector3d< float >& superResolution,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& /*specificScalarParameters*/,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                                : gkg::DiffusionCartesianField<
                                         gkg::TwoPoolModel >( t2, dw, mask,
                                                              boundingBox,
                                                              superResolution ),
                                  _fitMethod( getFitMethod(
                                               specificStringParameters[ 0 ] ) )
{

  try
  {

    getQSpaceSamples();
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::TwoPoolModelCartesianField::TwoPoolModelCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::TwoPoolModelCartesianField::TwoPoolModelCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::BoundingBox< int32_t >& boundingBox,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& /*specificScalarParameters*/,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                                : gkg::DiffusionCartesianField<
                                             gkg::TwoPoolModel >( t2, dw, mask,
                                                                  boundingBox ),
                                  _fitMethod( getFitMethod(
                                               specificStringParameters[ 0 ] ) )
{

  try
  {

    getQSpaceSamples();
    calculatingOnGrid( boundingBox, verbose );

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::TwoPoolModelCartesianField::TwoPoolModelCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose = false )" );

}


gkg::TwoPoolModelCartesianField::TwoPoolModelCartesianField(
           const gkg::SiteMap< int32_t, int32_t >& siteMap,
           const gkg::TextureMap< gkg::TwoPoolModel >& items,
           const gkg::OrientationSet& outputOrientationSet )
                                : gkg::DiffusionCartesianField<
                                                   gkg::TwoPoolModel >( siteMap,
                                                                        items ),
                                 _outputOrientationSet( outputOrientationSet )
{
}


gkg::TwoPoolModelCartesianField::~TwoPoolModelCartesianField()
{
}


const gkg::OrientationSet& 
gkg::TwoPoolModelCartesianField::getOutputOrientationSet() const
{

  return _outputOrientationSet;

}


void gkg::TwoPoolModelCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( !specificScalarParameters.empty() )
    {

      throw std::runtime_error( "no specific scalar parameter" );

    }
    if ( specificStringParameters.empty() )
    {

      specificStringParameters.resize( 1 );
      specificStringParameters[ 0 ] = "nelder_mead_simplex";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }

      if ( ( specificStringParameters[ 0 ] != "levenberg_marquardt" ) && 
           ( specificStringParameters[ 0 ] != "nelder_mead_simplex" ) )
      {

        throw std::runtime_error( "invalid string parameter" );

      }

    }

  }
  GKG_CATCH( "void gkg::TwoPoolModelCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


gkg::TwoPoolModel::FitMethod 
gkg::TwoPoolModelCartesianField::getFitMethod(
                                    const std::string& specificStringParameter )
{

  try
  {

    if ( specificStringParameter == "levenberg_marquardt" )
    {

      return gkg::TwoPoolModel::LevenbergMarquardt;

    }
    else if ( specificStringParameter == "nelder_mead_simplex" )
    {

      return gkg::TwoPoolModel::NelderMeadSimplex;

    }
    else
    {

      throw std::runtime_error( "not a valid fit method" );

    }

  }
  GKG_CATCH( "static gkg::TwoPoolModel::FitMethod "
             "gkg::TwoPoolModelCartesianField::getFitMethod( "
             "const std::string& specificStringParameter )" );

}


void gkg::TwoPoolModelCartesianField::getQSpaceSamples()
{

  try
  {

    // reading Q-space sampling information from headered object
    gkg::QSpaceSampling*
     qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create(
                                                                   *this->_dw );

    // getting the pointer to the concrete Q-space sampling instance
    gkg::MultipleShellQSpaceSampling*
      multipleShellQSpaceSampling =
       dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );

    if ( !multipleShellQSpaceSampling )
    {

      throw std::runtime_error( "unable to read the Q-space sampling as a "
                                "multiple same shell Q-space sampling" );

    }

    // generating samples from this two-pool model on two shells
    int32_t shellCount = multipleShellQSpaceSampling->getShellCount();
    int32_t s = 0;
    std::vector< int32_t > orientationCounts( shellCount );
    int32_t qSpaceSampleCount = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      orientationCounts[ s ] = multipleShellQSpaceSampling->getOrientationSet(
                                                                s ). getCount();
      qSpaceSampleCount += orientationCounts[ s ];

    }

    _qSpaceSamples.reallocate( qSpaceSampleCount, 4 );
    int32_t o = 0;

    int32_t q = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      double bValue = multipleShellQSpaceSampling->getBValue( s );
      for ( o = 0; o < orientationCounts[ s ]; o++ )
      {

        const gkg::Vector3d< float >&
          orientation = multipleShellQSpaceSampling->getOrientation( q );

        _qSpaceSamples( q, 0 ) = ( double )orientation.x;
        _qSpaceSamples( q, 1 ) = ( double )orientation.y;
        _qSpaceSamples( q, 2 ) = ( double )orientation.z;
        _qSpaceSamples( q, 3 ) = bValue;
        ++ q;

      }

    }


  }
  GKG_CATCH( "void gkg::TwoPoolModelCartesianField::getQSpaceSamples()" );


}


gkg::TwoPoolModel  gkg::TwoPoolModelCartesianField::newItem(
                                        const Vector3d< int32_t >& originSite,
                                        const Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::TwoPoolModel( &_outputOrientationSet,
                              *_t2,
                              *_dw,
                              _qSpaceSamples,
                              originSite,
                              _fitMethod );

  }
  GKG_CATCH( "gkg::TwoPoolModel  gkg::TwoPoolModelCartesianField::newItem( "
             "const Vector3d< int32_t >& originSite, "
             "const Vector3d< int32_t >& lutSite )" );

}


gkg::TwoPoolModel  gkg::TwoPoolModelCartesianField::newItem(
                                        const Vector3d< float >& originSite,
                                        const Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::TwoPoolModel( &_outputOrientationSet,
                              *_t2,
                              *_dw,
                              _qSpaceSamples,
                              originSite,
                              _fitMethod );

  }
  GKG_CATCH( "gkg::TwoPoolModel  gkg::TwoPoolModelCartesianField::newItem( "
             "const Vector3d< float >& originSite, "
             "const Vector3d< int32_t >& lutSite )" );

}


void gkg::TwoPoolModelCartesianField::setOutputOrientationSet(
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
  GKG_CATCH( "void gkg::OdfCartesianField::setOutputOrientationSet( "
             "const gkg::OrientationSet* outputOrientationSet )" );

}


