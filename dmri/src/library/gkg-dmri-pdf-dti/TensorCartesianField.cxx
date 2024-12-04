#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/TypeOf.h>


RegisterBaseType( gkg::Tensor*,
                  gkg_Tensor_pointer );



gkg::TensorCartesianField::TensorCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const std::vector< double >& /*specificScalarParameters*/,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                          : gkg::DiffusionCartesianField< gkg::Tensor >(
                              t2, dw, mask ),
                            _fitMethod( 
                                 getFitMethod( specificStringParameters[ 0 ] ) )
                
{

  try
  {

    if ( _fitMethod == gkg::Tensor::RobustPositiveDefinite )
    {

      getBMatrix();

    }
    getOrientationSVDDecomposition();
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );
  }
  GKG_CATCH( "gkg::TensorCartesianField::TensorCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );


}


gkg::TensorCartesianField::TensorCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::BoundingBox< float >& boundingBox,
                     const gkg::Vector3d< float >& superResolution,
                     const std::vector< double >& /*specificScalarParameters*/,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                          : gkg::DiffusionCartesianField< gkg::Tensor >(
                              t2, dw, mask,
                              boundingBox,
                              superResolution ),
                            _fitMethod( 
                                 getFitMethod( specificStringParameters[ 0 ] ) )
{

  try
  {

    if ( _fitMethod == gkg::Tensor::RobustPositiveDefinite )
    {

      getBMatrix();

    }
    getOrientationSVDDecomposition();
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::TensorCartesianField::TensorCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::TensorCartesianField::TensorCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::BoundingBox< int32_t >& boundingBox,
                     const std::vector< double >& /*specificScalarParameters*/,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                          : gkg::DiffusionCartesianField< gkg::Tensor >(
                              t2, dw, mask,
                              boundingBox ),
                            _fitMethod( 
                                 getFitMethod( specificStringParameters[ 0 ] ) )
{

  try
  {

    if ( _fitMethod == gkg::Tensor::RobustPositiveDefinite )
    {

      getBMatrix();

    }
    getOrientationSVDDecomposition();
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::TensorCartesianField::TensorCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose = false )" );

}


gkg::TensorCartesianField::~TensorCartesianField()
{
}


void gkg::TensorCartesianField::getBMatrix()
{

  try
  {

    int32_t orientationCount = _inputOrientationSet.getCount();
    
    _matrixB.reallocate( orientationCount + 1, 6 );

    // first trial is T2-weighted only
    _matrixB( 0, 0 ) = 0;
    _matrixB( 0, 1 ) = 0;
    _matrixB( 0, 2 ) = 0;
    _matrixB( 0, 3 ) = 0;
    _matrixB( 0, 4 ) = 0;
    _matrixB( 0, 5 ) = 0;

    // filling other diffusion-weighted b-matrices
    int32_t o;
    float* inPtr = (float*)&_inputOrientationSet.getOrientations()[ 0 ];
    float* bPtr = (float*)&_bValues[ 0 ];

    for ( o = 0; o < orientationCount; o++ )
    {                                          

#if 0
      _matrixB( o + 1, 0 ) = _bValues[ o ] *
                             _inputOrientationSet.getOrientation( o ).x *
                             _inputOrientationSet.getOrientation( o ).x;
      _matrixB( o + 1, 1 ) = _bValues[ o ] *
                             _inputOrientationSet.getOrientation( o ).x *
                             _inputOrientationSet.getOrientation( o ).y;
      _matrixB( o + 1, 2 ) = _bValues[ o ] *
                             _inputOrientationSet.getOrientation( o ).x *
                             _inputOrientationSet.getOrientation( o ).z;
      _matrixB( o + 1, 3 ) = _bValues[ o ] *
                             _inputOrientationSet.getOrientation( o ).y *
                             _inputOrientationSet.getOrientation( o ).y;
      _matrixB( o + 1, 4 ) = _bValues[ o ] *
                             _inputOrientationSet.getOrientation( o ).y *
                             _inputOrientationSet.getOrientation( o ).z;
      _matrixB( o + 1, 5 ) = _bValues[ o ] *
                             _inputOrientationSet.getOrientation( o ).z *
                             _inputOrientationSet.getOrientation( o ).z;
#endif

      // x * y
      _matrixB( o + 1, 1 ) = *bPtr * 1e6 * *inPtr * inPtr[ 1 ];
      // x * z
      _matrixB( o + 1, 2 ) = *bPtr * 1e6 *  *inPtr * inPtr[ 2 ];
      // y * z
      _matrixB( o + 1, 4 ) = *bPtr * 1e6 *  inPtr[ 1 ] * inPtr[ 2 ];
      // x * x
      _matrixB( o + 1, 0 ) = *bPtr * 1e6 *  *inPtr * *inPtr;
      inPtr++;
      // y * y
      _matrixB( o + 1, 3 ) = *bPtr * 1e6 *  *inPtr * *inPtr;
      inPtr++;
      // z * z
      _matrixB( o + 1, 5 ) = *bPtr++ * 1e6 *  *inPtr * *inPtr;
      inPtr++;

    }

  }
  GKG_CATCH( "void gkg::TensorCartesianField::getBMatrix()" );


}


void gkg::TensorCartesianField::getOrientationSVDDecomposition()
{

  try
  {

    int32_t orientationCount = _inputOrientationSet.getCount();

    _matrixU.reallocate( orientationCount, 6 );
    _vectorS.reallocate( 6 );
    _matrixV.reallocate( 6, 6 );

    int32_t o;
    float* inPtr = (float*)&_inputOrientationSet.getOrientations()[ 0 ];
    float* bPtr = (float*)&_bValues[ 0 ];

    for ( o = 0; o < orientationCount; o++ )
    {                                          

#if 0
      _matrixU( o, 0 ) = _bValues[ o ] *
                         _inputOrientationSet.getOrientation( o ).x *
                         _inputOrientationSet.getOrientation( o ).x;
      _matrixU( o, 1 ) = 2 * _bValues[ o ] *
                         _inputOrientationSet.getOrientation( o ).x *
                         _inputOrientationSet.getOrientation( o ).y;
      _matrixU( o, 2 ) = 2 * _bValues[ o ] *
                         _inputOrientationSet.getOrientation( o ).x *
                         _inputOrientationSet.getOrientation( o ).z;
      _matrixU( o, 3 ) = _bValues[ o ] *
                         _inputOrientationSet.getOrientation( o ).y *
                         _inputOrientationSet.getOrientation( o ).y;
      _matrixU( o, 4 ) = 2 * _bValues[ o ] *
                         _inputOrientationSet.getOrientation( o ).y *
                         _inputOrientationSet.getOrientation( o ).z;
      _matrixU( o, 5 ) = _bValues[ o ] *
                         _inputOrientationSet.getOrientation( o ).z *
                         _inputOrientationSet.getOrientation( o ).z;
#endif

      // x * y
      _matrixU( o, 1 ) = 2.0f * *bPtr * *inPtr * inPtr[ 1 ];
      // x * z
      _matrixU( o, 2 ) = 2.0f * *bPtr * *inPtr * inPtr[ 2 ];
      // y * z
      _matrixU( o, 4 ) = 2.0f * *bPtr * inPtr[ 1 ] * inPtr[ 2 ];
      // x * x
      _matrixU( o, 0 ) = *bPtr * *inPtr * *inPtr;
      inPtr++;
      // y * y
      _matrixU( o, 3 ) = *bPtr * *inPtr * *inPtr;
      inPtr++;
      // z * z
      _matrixU( o, 5 ) = *bPtr++ * *inPtr * *inPtr;
      inPtr++;

    }

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // performing SVD decomposition
    factory->getSingularValueDecomposition( _matrixU, _matrixV, _vectorS );

  }
  GKG_CATCH( "void gkg::TensorCartesianField::getOrientationSVDDecomposition()" );

}


void gkg::TensorCartesianField::checkOrInitializeDefaultParameters(
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
      specificStringParameters[ 0 ] = "linear_square";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }

      if ( ( specificStringParameters[ 0 ] != "linear_square" ) && 
           ( specificStringParameters[ 0 ] != "robust_positive_definite" ) )
      {

        throw std::runtime_error( "invalid string parameter" );

      }

    }

  }
  GKG_CATCH( "void gkg::TensorCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


gkg::Tensor::FitMethod 
gkg::TensorCartesianField::getFitMethod(
                                    const std::string& specificStringParameter )
{

  try
  {

    if ( specificStringParameter == "linear_square" )
    {

      return gkg::Tensor::LinearSquare;

    }
    else if ( specificStringParameter == "robust_positive_definite" )
    {

      return gkg::Tensor::RobustPositiveDefinite;

    }
    else
    {

      throw std::runtime_error( "not a valid fit method" );

    }

  }
  GKG_CATCH( "static gkg::Tensor::FitMethod "
             "gkg::TensorCartesianField::getFitMethod( "
             "const std::string& specificStringParameter )" );

}


gkg::Tensor  gkg::TensorCartesianField::newItem(
                                        const Vector3d< int32_t >& originSite,
                                        const Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {
//std::cout << "originSite" << originSite << " " << std::flush;
    return gkg::Tensor( 0,
                        *_t2, *_dw,
                        _matrixB,
                        _matrixU,
                        _vectorS,
                        _matrixV,
                        originSite,
                        _fitMethod );

  }
  GKG_CATCH( "gkg::Tensor  gkg::TensorCartesianField::newItem( "
             "const Vector3d< int32_t >& originSite, "
             "const Vector3d< int32_t >& lutSite )" );

}


gkg::Tensor  gkg::TensorCartesianField::newItem(
                                        const Vector3d< float >& originSite,
                                        const Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::Tensor( 0,
                        *_t2, *_dw,
                        _matrixB,
                        _matrixU,
                        _vectorS,
                        _matrixV,
                        originSite,
                        _fitMethod );

  }
  GKG_CATCH( "gkg::Tensor  gkg::TensorCartesianField::newItem( "
             "const Vector3d< float >& originSite, "
             "const Vector3d< int32_t >& lutSite )" );

}


/*
void gkg::TensorCartesianField::calculatingOnGridTmp(
                                 const gkg::BoundingBox< int32_t >& boundingBox,
                                 bool verbose )

{

  try
  {

    int32_t sizeX = _t2->getSizeX();
    int32_t sizeY = _t2->getSizeY();
    int32_t sizeZ = _t2->getSizeZ();

    int32_t x, y, z;
    
    // counting the number of sites
    int32_t siteCount = 0;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( ( *_mask )( x, y, z ) && boundingBox.contains( x, y, z ) )
          {

            ++ siteCount;

          }

        }

      }

    }

    // filling sites
    std::vector< gkg::Vector3d< int32_t > > sites( siteCount );

    // filling sites
    int32_t siteIndex = 0;
    
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {


          if ( ( *_mask )( x, y, z ) && boundingBox.contains( x, y, z ) )
          {

            gkg::Vector3d< int32_t >& site = sites[ siteIndex ];
            site.x = x;
            site.y = y;
            site.z = z;
            siteIndex ++;

          }

        }

      }

    }

    // adding sites to Cartesian field 
    this->CartesianField< gkg::Tensor >::_siteMap.addSites(
                                                                     0, sites );

    // creating (rank,site)<->index LUT
    this->CartesianField< gkg::Tensor >::_rankSiteLut = 
      gkg::RankSiteLut< int32_t, int32_t >(
                              this->CartesianField< gkg::Tensor >::_siteMap );

    // creating items and filling LUT
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = this->CartesianField<
              gkg::Tensor >::_siteMap.getSites( 0 ).begin(),
      se = this->CartesianField<
              gkg::Tensor >::_siteMap.getSites( 0 ).end();
    int32_t count = 0;
    while ( s != se )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << gkg::Eraser( 28 );

          }
          std::cout << " [ " << std::setw( 10 ) << count + 1
                    << " / " << std::setw( 10 ) << siteCount
                    << " ]" << std::flush;

        }

      }
 

      gkg::Vector3d< int32_t > site( s->x - boundingBox.getLowerX(),
                                     s->y - boundingBox.getLowerY(),
                                     s->z - boundingBox.getLowerZ() );

      int32_t index = this->CartesianField<
              gkg::Tensor >::_rankSiteLut.getIndex( 0,
                                                                        *s );

      gkg::Tensor&
        item = this->CartesianField< 
        gkg::Tensor >::_items.addTexture( index,
                                                              newItem(
                                                                     *s,
                                                                     site ) );
      this->CartesianField< gkg::Tensor >::_lut( site ) =
                                                                          &item;
      ++ count;
      ++ s;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 28 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::TensorCartesianField::calculatingOnGridTmp( "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "bool verbose )" );

}
*/
