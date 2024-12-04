#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::PdfSamplingContext::PdfSamplingContext(
                       gkg::TextureMap< gkg::ProbabilityDensityFunction >& pdfs,
                       int32_t* lut,
                       std::map< float, gkg::OrientationSet >& omss,
                       int32_t resamplingNeighborCount,
                       float resamplingGaussianSigma )
                       : gkg::LoopContext< int32_t >(),
                         _pdfs( pdfs ),
                         _lut( lut ),
                         _omss( omss ),
                         _resamplingNeighborCount( resamplingNeighborCount ),
                         _resamplingGaussianSigma( resamplingGaussianSigma )
{
}

			     
void gkg::PdfSamplingContext::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    int32_t* l = &_lut[ startIndex ];
    
    while ( count-- )
    {

      gkg::ProbabilityDensityFunction& pdf = _pdfs.getTexture( *l++ );
      pdf.setOutputSampling( &_omss,
                             _resamplingNeighborCount,
                             _resamplingGaussianSigma );

    }

  }
  GKG_CATCH( "void gkg::PdfSamplingContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}


//
// class PdfCartesianField
//


RegisterBaseType( gkg::ProbabilityDensityFunction*,
                  gkg_ProbabilityDensityFunction_pointer );


gkg::PdfCartesianField::PdfCartesianField(
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ,
                               const std::map< float, gkg::OrientationSet >&
                                                  outputMultipleShellSampling )
                       : gkg::DiffusionCartesianField<
                                              gkg::ProbabilityDensityFunction >(
                                                  sizeX, sizeY, sizeZ,
                                                  resolution,
                                                  siteMap,
                                                  offsetX,
                                                  offsetY,
                                                  offsetZ ),
    _hasMultipleShellOutputSampling( true ),
    _outputMultipleShellSampling( outputMultipleShellSampling )
{
}


gkg::PdfCartesianField::PdfCartesianField( 
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               const gkg::TextureMap<
                               gkg::ProbabilityDensityFunction >& items,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ,
                               const std::map< float, gkg::OrientationSet >&
                                                  outputMultipleShellSampling )
                       : gkg::DiffusionCartesianField<
                                              gkg::ProbabilityDensityFunction >(
                                                  sizeX, sizeY, sizeZ,
                                                  resolution,
                                                  siteMap,
                                                  items,
                                                  offsetX,
                                                  offsetY,
                                                  offsetZ ),
    _hasMultipleShellOutputSampling( true ),
    _outputMultipleShellSampling( outputMultipleShellSampling )
{
}


gkg::PdfCartesianField::PdfCartesianField(
           const gkg::SiteMap< int32_t, int32_t >& siteMap,
           const gkg::TextureMap< gkg::ProbabilityDensityFunction >& items,
           const std::map< float, gkg::OrientationSet >&
                                                  outputMultipleShellSampling,
           int32_t resamplingNeighborCount,
           float resamplingGaussianSigma )
                         : gkg::DiffusionCartesianField<
                                              gkg::ProbabilityDensityFunction >(
                                                                     siteMap,
                                                                     items ),
    _hasMultipleShellOutputSampling( true ),
    _outputMultipleShellSampling( outputMultipleShellSampling )
{

  try
  {

    gkg::TextureMap< gkg::ProbabilityDensityFunction >::iterator
      p = _items.begin(),
      pe = _items.end();
    int32_t N = int32_t( _items.getCount() );
    int32_t lut[ N ];
    int32_t* l = lut;
    
    while ( p != pe )
    {

      *l++ = p->first;
      ++p;

    }

    _items.begin()->second.checkAndModifyOutputSampling(
                                                &_outputMultipleShellSampling );

    gkg::PdfSamplingContext context( _items,
                                     lut,
                                     _outputMultipleShellSampling,
                                     resamplingNeighborCount,
                                     resamplingGaussianSigma );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, N );

    threadedLoop.launch();

  }
  GKG_CATCH( "gkg::PdfCartesianField::PdfCartesianField( "
             "const gkg::SiteMap< int32_t, int32_t >& siteMap, "
             "const gkg::TextureMap< gkg::ProbabilityDensityFunction >& items, "
             "const std::map< float, gkg::OrientationSet >& "
             " outputMultipleShellSampling )" );

}


gkg::PdfCartesianField::PdfCartesianField(
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ,
                               const Vector3d< int32_t >&
                                                 outputCartesianGridSize,
                               const Vector3d< double >& 
                                                 outputCartesianGridResolution )
                       : gkg::DiffusionCartesianField<
                                              gkg::ProbabilityDensityFunction >(
                                                  sizeX, sizeY, sizeZ,
                                                  resolution,
                                                  siteMap,
                                                  offsetX,
                                                  offsetY,
                                                  offsetZ ),
                         _hasMultipleShellOutputSampling( false ),
                         _outputCartesianGridSize( outputCartesianGridSize ),
                         _outputCartesianGridResolution(
                                               outputCartesianGridResolution )
{
}


gkg::PdfCartesianField::PdfCartesianField( 
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               const gkg::TextureMap<
                               gkg::ProbabilityDensityFunction >& items,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ,
                               const Vector3d< int32_t >&
                                                 outputCartesianGridSize,
                               const Vector3d< double >& 
                                                 outputCartesianGridResolution )
                       : gkg::DiffusionCartesianField<
                                              gkg::ProbabilityDensityFunction >(
                                                  sizeX, sizeY, sizeZ,
                                                  resolution,
                                                  siteMap,
                                                  items,
                                                  offsetX,
                                                  offsetY,
                                                  offsetZ ),
                         _hasMultipleShellOutputSampling( false ),
                         _outputCartesianGridSize( outputCartesianGridSize ),
                         _outputCartesianGridResolution(
                                               outputCartesianGridResolution )
{
}


gkg::PdfCartesianField::PdfCartesianField(
           const gkg::SiteMap< int32_t, int32_t >& siteMap,
           const gkg::TextureMap< gkg::ProbabilityDensityFunction >& items,
           const Vector3d< int32_t >& outputCartesianGridSize,
           const Vector3d< double >& outputCartesianGridResolution )
                       : gkg::DiffusionCartesianField<
                                              gkg::ProbabilityDensityFunction >(
                                                                     siteMap,
                                                                     items ),
                         _hasMultipleShellOutputSampling( false ),
                         _outputCartesianGridSize( outputCartesianGridSize ),
                         _outputCartesianGridResolution(
                                               outputCartesianGridResolution )
{
}


gkg::PdfCartesianField::PdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const std::map< float, gkg::OrientationSet >*
                                                  outputMultipleShellSampling )
                       : gkg::DiffusionCartesianField<
                                        gkg::ProbabilityDensityFunction >(
                                                                  t2, dw, mask )
{

  try
  {

    // setting default output multiple shell sampling
    setOutputMultipleShellSampling( outputMultipleShellSampling );

  }
  GKG_CATCH( "gkg::PdfCartesianField::PdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling )" );


}


gkg::PdfCartesianField::PdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< float >& boundingBox,
                               const gkg::Vector3d< float >& superResolution,
                               const std::map< float, gkg::OrientationSet >*
                                                  outputMultipleShellSampling )
                       : gkg::DiffusionCartesianField<
                                    gkg::ProbabilityDensityFunction >(
                                    t2, dw, mask, boundingBox, superResolution )
{

  try
  {

    // setting default output multiple shell sampling
    setOutputMultipleShellSampling( outputMultipleShellSampling );

  }
  GKG_CATCH( "gkg::PdfCartesianField::PdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling )" );

}


gkg::PdfCartesianField::PdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< int32_t >& boundingBox,
                               const std::map< float, gkg::OrientationSet >*
                                                  outputMultipleShellSampling )
                       : gkg::DiffusionCartesianField<
                                    gkg::ProbabilityDensityFunction >(
                                                     t2, dw, mask, boundingBox )
{

  try
  {

    // setting default output multiple shell sampling
    setOutputMultipleShellSampling( outputMultipleShellSampling );

  }
  GKG_CATCH( "gkg::PdfCartesianField::PdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling )" );

}


gkg::PdfCartesianField::PdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::Vector3d< int32_t >*
                                                 outputCartesianGridSize,
                               const gkg::Vector3d< double >*
                                                 outputCartesianGridResolution )
                       : gkg::DiffusionCartesianField<
                                        gkg::ProbabilityDensityFunction >(
                                                                  t2, dw, mask )
{

  try
  {

    // setting default output Carteisan grid sampling
    setOutputCartesianGridSampling( outputCartesianGridSize,
                                    outputCartesianGridResolution );

  }
  GKG_CATCH( "gkg::PdfCartesianField::PdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::Vector3d< int32_t >* "
             "outputCartesianGridSize, "
             "const gkg::Vector3d< double >* "
             "outputCartesianGridResolution )" );


}


gkg::PdfCartesianField::PdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< float >& boundingBox,
                               const gkg::Vector3d< float >& superResolution,
                               const gkg::Vector3d< int32_t >*
                                                 outputCartesianGridSize,
                               const gkg::Vector3d< double >*
                                                 outputCartesianGridResolution )
                       : gkg::DiffusionCartesianField<
                                    gkg::ProbabilityDensityFunction >(
                                    t2, dw, mask, boundingBox, superResolution )
{

  try
  {

    // setting default output Carteisan grid sampling
    setOutputCartesianGridSampling( outputCartesianGridSize,
                                    outputCartesianGridResolution );

  }
  GKG_CATCH( "gkg::PdfCartesianField::PdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::Vector3d< int32_t >* "
             "outputCartesianGridSize, "
             "const gkg::Vector3d< double >* "
             "outputCartesianGridResolution )" );

}


gkg::PdfCartesianField::PdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< int32_t >& boundingBox,
                               const gkg::Vector3d< int32_t >*
                                                 outputCartesianGridSize,
                               const gkg::Vector3d< double >*
                                                 outputCartesianGridResolution )
                       : gkg::DiffusionCartesianField<
                                    gkg::ProbabilityDensityFunction >(
                                                     t2, dw, mask, boundingBox )
{

  try
  {

    // setting default output Carteisan grid sampling
    setOutputCartesianGridSampling( outputCartesianGridSize,
                                    outputCartesianGridResolution );

  }
  GKG_CATCH( "gkg::PdfCartesianField::PdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::Vector3d< int32_t >* "
             "outputCartesianGridSize, "
             "const gkg::Vector3d< double >* "
             "outputCartesianGridResolution )" );

}


gkg::PdfCartesianField::~PdfCartesianField()
{
}


bool gkg::PdfCartesianField::hasMultipleShellOutputSampling() const
{

  try
  {

    return _hasMultipleShellOutputSampling;

  }
  GKG_CATCH( "bool gkg::PdfCartesianField::"
             "hasMultipleShellOutputSampling() const" );

}


const std::map< float, gkg::OrientationSet >&
gkg::PdfCartesianField::getOutputMultipleShellSampling() const
{

  try
  {

    return _outputMultipleShellSampling;

  }
  GKG_CATCH( "const std::map< float, gkg::OrientationSet >& "
             "gkg::PdfCartesianField::getOutputMultipleShellSampling() const" );

}


const gkg::Vector3d< int32_t >&
gkg::PdfCartesianField::getOutputCartesianGridSize() const
{

  try
  {

    return _outputCartesianGridSize;

  }
  GKG_CATCH( "const gkg::Vector3d< int32_t >& "
             "gkg::PdfCartesianField::"
             "getOutputCartesianGridSize() const" );

}


const gkg::Vector3d< double >&
gkg::PdfCartesianField::getOutputCartesianGridResolution() const
{

  try
  {

    return _outputCartesianGridResolution;

  }
  GKG_CATCH( "const gkg::Vector3d< double >& "
             "gkg::PdfCartesianField::"
             "getOutputCartesianGridResolution() const" );

}


gkg::ProbabilityDensityFunction::BasisType 
gkg::PdfCartesianField::getBasisType() const
{

  try
  {

    // we assume that the whole field is filled with the same PDF type
    gkg::TextureMap< gkg::ProbabilityDensityFunction >::const_iterator
      o = this->_items.begin();
    if ( o == this->_items.end() )
    {

      throw std::runtime_error( "PDF field is empty !" );

    }
    return o->second.getBasisType();

  }
  GKG_CATCH( "gkg::ProbabilityDensityFunction::BasisType "
             "gkg::PdfCartesianField::getBasisType() const" );

}


std::string 
gkg::PdfCartesianField::getBasisTypeName() const
{

  try
  {

    std::string basisTypeName = "";

    gkg::ProbabilityDensityFunction::BasisType
      basisType = this->getBasisType();

    if ( basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      basisTypeName = "std";

    }
    else if ( basisType == gkg::ProbabilityDensityFunction::SecondOrderTensor )
    {

      basisTypeName = "te";

    }
    else if ( basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      basisTypeName = "ph";

    }

    return basisTypeName;

  }
  GKG_CATCH( "std::string "
             "gkg::PdfCartesianField::getBasisTypeName() const" );

}




void gkg::PdfCartesianField::setStandardBasis( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "transforming to standard basis : " << std::flush;

    }

    int32_t count = 0;
    int32_t siteCount = this->getSiteMap().getSiteCount( 0 );
    gkg::PdfCartesianField::iterator pdf = this->_items.begin(),
                                     pdfe = this->_items.end();
    while ( pdf != pdfe )
    {

      if ( verbose )
      {

        if ( ( ( count + 1 ) % 100 == 0 ) ||
             ( count == 0 ) ||
             ( count == siteCount - 1 ) )
        {

          if ( count )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << " [ " << std::setw( 7 ) << count + 1
                    << " / " << std::setw( 7 ) << siteCount
                    << " ]" << std::flush;

        }

      }
      pdf->second.setStandardBasisType();
      ++ pdf;
      ++ count;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 );
      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::PdfCartesianField::setStandardBasis( bool verbose )" );

}


void gkg::PdfCartesianField::setOutputMultipleShellSampling(
     const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling )
{

  try
  {

    if ( !outputMultipleShellSampling )
    {

      throw std::runtime_error( "invalid output multiple shell sampling" );

    }
    else
    {

      _hasMultipleShellOutputSampling = true;
      _outputMultipleShellSampling = *outputMultipleShellSampling;

    }

  }
  GKG_CATCH( "void gkg::PdfCartesianField::setOutputMultipleShellSampling( "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling )" );

}


void gkg::PdfCartesianField::setOutputCartesianGridSampling(
                  const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                  const gkg::Vector3d< double >* outputCartesianGridResolution )
{

  try
  {

    if ( !outputCartesianGridSize )
    {

      throw std::runtime_error( "invalid output Cartesian grid size" );

    }

    if ( !outputCartesianGridResolution )
    {

      throw std::runtime_error( "invalid output Cartesian grid resolution" );

    }

    _hasMultipleShellOutputSampling = false;
    _outputCartesianGridSize = *outputCartesianGridSize;
    _outputCartesianGridResolution = *outputCartesianGridResolution;

  }
  GKG_CATCH( "void gkg::PdfCartesianField::setOutputCartesianGridSampling( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution )" );

}


