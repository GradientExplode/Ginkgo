#include <gkg-dmri-container/EapCartesianField.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


RegisterBaseType( gkg::OrientationDistributionFunction*,
                  gkg_OrientationDistributionFunction_pointer );


gkg::EapCartesianField::EapCartesianField(
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ,
                               const gkg::OrientationSet& outputOrientationSet )
  : gkg::DiffusionCartesianField< gkg::OrientationDistributionFunction >(
                                                  sizeX, sizeY, sizeZ,
                                                  resolution,
                                                  siteMap,
                                                  offsetX,
                                                  offsetY,
                                                  offsetZ ),
    _outputOrientationSet( outputOrientationSet )
{
}


gkg::EapCartesianField::EapCartesianField( 
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               const gkg::TextureMap<
                               gkg::OrientationDistributionFunction >& items,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ,
                               const gkg::OrientationSet& outputOrientationSet )
  : gkg::DiffusionCartesianField< gkg::OrientationDistributionFunction >(
                                                  sizeX, sizeY, sizeZ,
                                                  resolution,
                                                  siteMap,
                                                  items,
                                                  offsetX,
                                                  offsetY,
                                                  offsetZ ),
    _outputOrientationSet( outputOrientationSet )
{
}


gkg::EapCartesianField::EapCartesianField(
           const gkg::SiteMap< int32_t, int32_t >& siteMap,
           const gkg::TextureMap< gkg::OrientationDistributionFunction >& items,
           const gkg::OrientationSet& outputOrientationSet )
  : gkg::DiffusionCartesianField< gkg::OrientationDistributionFunction >(
                                                                         siteMap,
                                                                         items ),
    _outputOrientationSet( outputOrientationSet )
{
}


gkg::EapCartesianField::EapCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::OrientationSet* outputOrientationSet )
                       : gkg::DiffusionCartesianField<
                                        gkg::OrientationDistributionFunction >(
                                                                  t2, dw, mask )
{

  try
  {

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::EapCartesianField::EapCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet )" );


}


gkg::EapCartesianField::EapCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< float >& boundingBox,
                               const gkg::Vector3d< float >& superResolution,
                               const gkg::OrientationSet* outputOrientationSet )
                       : gkg::DiffusionCartesianField<
                                    gkg::OrientationDistributionFunction >(
                                    t2, dw, mask, boundingBox, superResolution )
{

  try
  {

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::EapCartesianField::EapCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet )" );

}


gkg::EapCartesianField::EapCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< int32_t >& boundingBox,
                               const gkg::OrientationSet* outputOrientationSet )
                       : gkg::DiffusionCartesianField<
                                    gkg::OrientationDistributionFunction >(
                                                     t2, dw, mask, boundingBox )
{

  try
  {

    // setting default output orientation set
    setOutputOrientationSet( outputOrientationSet );

  }
  GKG_CATCH( "gkg::EapCartesianField::EapCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet )" );

}


gkg::EapCartesianField::~EapCartesianField()
{
}


const gkg::OrientationSet& 
gkg::EapCartesianField::getOutputOrientationSet() const
{

  return _outputOrientationSet;

}


gkg::OrientationDistributionFunction::BasisType 
gkg::EapCartesianField::getBasisType() const
{

  try
  {

    // we assume that the whole field is filled with the same ODF type
    gkg::TextureMap< gkg::OrientationDistributionFunction >::const_iterator
      o = this->_items.begin();
    if ( o == this->_items.end() )
    {

      throw std::runtime_error( "ODF field is empty !" );

    }
    return o->second.getBasisType();

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction::BasisType "
             "gkg::EapCartesianField::getBasisType() const" );

}


void gkg::EapCartesianField::setStandardBasis( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "transforming to standard basis : " << std::flush;

    }

    int32_t count = 0;
    int32_t siteCount = this->getSiteMap().getSiteCount( 0 );
    gkg::EapCartesianField::iterator odf = this->_items.begin(),
                                     odfe = this->_items.end();
    while ( odf != odfe )
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
      odf->second.setStandardBasisType();
      ++ odf;
      ++ count;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 );
      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapCartesianField::setStandardBasis( bool verbose )" );

}


void gkg::EapCartesianField::setOutputOrientationSet(
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
  GKG_CATCH( "void gkg::EapCartesianField::setOutputOrientationSet( "
             "const gkg::OrientationSet* outputOrientationSet )" );

}


