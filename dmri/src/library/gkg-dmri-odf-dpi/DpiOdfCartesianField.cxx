#include <gkg-dmri-odf-dpi/DpiOdfCartesianField.h>
#include <gkg-dmri-odf-dpi/DpiOdf.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::DpiOdfCartesianField::DpiOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& specificStringParameters,
		 std::vector< float > meshRadii,
                 bool verbose )
  : gkg::DpiEapCartesianField( t2, dw, mask, outputOrientationSet,
			       specificScalarParameters, 
			       specificStringParameters, verbose )
{

  try
  {

    if( meshRadii.size() == 0 || meshRadii.size() == 1 ) 
    {      
      
      throw std::runtime_error( "invalid radii input.  "
                                "Need at least two radii "
                                "to compute ODF from DPI" );
      
    }
    
    _R0min = meshRadii[ 0 ];
    _R0max = meshRadii[ meshRadii.size() - 1 ];
    
    _items.getHeader().addAttribute( "eap_odf_type",
                                     gkg::TypeOf< gkg::DpiOdf >::getName() );

  }
  GKG_CATCH( "gkg::DpiOdfCartesianField::DpiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
	     "std::vector< float > meshRadii, "
             "bool verbose )" );

}


gkg::DpiOdfCartesianField::DpiOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< float >& boundingBox,
                 const gkg::Vector3d< float >& superResolution,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& specificStringParameters,
		 std::vector< float > meshRadii, 
                 bool verbose )
    : gkg::DpiEapCartesianField( t2, dw, mask, 
				 boundingBox,
				 superResolution,
				 outputOrientationSet,
				 specificScalarParameters, 
				 specificStringParameters, verbose )
{

  try
  {

    if( meshRadii.size() == 0 || meshRadii.size() == 1 ) 
    {      
      
      throw std::runtime_error( "invalid radii input.  "
                                "Need at least two radii "
                                "to compute ODF from DPI" );
      
    }

    _R0min = meshRadii[ 0 ];
    _R0max = meshRadii[ meshRadii.size() - 1 ];
    
    _items.getHeader().addAttribute( "eap_odf_type",
                                     gkg::TypeOf< gkg::DpiOdf >::getName() );

  }
  GKG_CATCH( "gkg::DpiOdfCartesianField::DpiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
	     "std::vector< float > meshRadii, "
             "bool verbose )" );

}


gkg::DpiOdfCartesianField::DpiOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& specificStringParameters,
		 std::vector< float > meshRadii,
                 bool verbose )
  : gkg::DpiEapCartesianField( t2, dw, mask, boundingBox, 
			       outputOrientationSet,
			       specificScalarParameters, 
			       specificStringParameters, verbose ) 

{

  try
  {
    
    if( meshRadii.size() == 0 || meshRadii.size() == 1 ) 
    {      
      
      throw std::runtime_error( "invalid radii input.  "
                                "Need at least two radii "
                                "to compute ODF from DPI" );
      
    }

    _R0min = meshRadii[ 0 ];
    _R0max = meshRadii[ meshRadii.size() - 1 ];
    
    _items.getHeader().addAttribute( "eap_odf_type",
                                     gkg::TypeOf< gkg::DpiOdf >::getName() );


  }
  GKG_CATCH( "gkg::DpiOdfCartesianField::DpiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
	     "std::vector< float > meshRadii, "
             "bool verbose )" );

}


gkg::DpiOdfCartesianField::~DpiOdfCartesianField()
{
}

float gkg::DpiOdfCartesianField::getMaximumR0Value() const
{

  return _R0max;

}

float gkg::DpiOdfCartesianField::getMinimumR0Value() const
{

  return _R0min;

}

void gkg::DpiOdfCartesianField::checkOrInitializeDefaultParameters(
						std::vector< float > meshRadii )
{

  try
  {
    
    if( meshRadii.size() == 0 || meshRadii.size() == 1 ) 
    {      
      
      throw std::runtime_error( "invalid radii input.  Need at least two radii \
to compute ODF from DPI" );
      
    }
    
  }
  GKG_CATCH( "void gkg::DpiOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


gkg::OrientationDistributionFunction 
gkg::DpiOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DpiOdf dpiOdf( &_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_reconstructionMatrix,
			_R0min, _R0max );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dpiOdf );

    }
    else
    {

      return dpiOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DpiOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::DpiOdfCartesianField::newItem(
                                       const gkg::Vector3d< float >& originSite,
                                       const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DpiOdf dpiOdf( &_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_reconstructionMatrix,
			_R0min, _R0max );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dpiOdf );

    }
    else
    {

      return dpiOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DpiOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}

