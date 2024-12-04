#include <gkg-dmri-odf-dsi/DsiOdfCartesianField.h>
#include <gkg-dmri-odf-dsi/DsiOdf.h>
#include <gkg-dmri-odf-dsi/DsiOdfCartesianFieldFunctor.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-odf-dsi/HanningWindow.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::DsiOdfCartesianField::DsiOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
		 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        outputOrientationSet )
{

  try
  {

    _filterOrNot = ( bool )( specificScalarParameters[ 0 ] );
    _R0min = ( float ) ( specificScalarParameters[ 1 ] );
    _R0max = ( float ) ( specificScalarParameters[ 2 ] );
    _marginalODF = ( bool ) ( specificScalarParameters[ 3 ] );

    int32_t totalNumberOfbValues = this->getBValues().size();
    if ( totalNumberOfbValues !=  _inputOrientationSet.getCount() )
    {

      throw std::runtime_error(
                "Error in dw minf attributes : number of bValues "
                "and number of diffusion gradient orientations not the same" );

    }
    
    if ( totalNumberOfbValues !=  515 )
    {

      throw std::runtime_error( 
	 "This DSI code was designed for a fixed Cartesian grid with 515 points");
      
    }
	
    _gridRadius = 8;

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::DsiOdf >::getName() );

    //gkg::Matrix m;
    //this->_functor = new gkg::DsiOdfCartesianFieldFunctor( *this, m );
    this->_functor = new gkg::DsiOdfCartesianFieldFunctor( *this );

    prepareForDSI( verbose );
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );
    
  }
  GKG_CATCH( "gkg::DsiOdfCartesianField::DsiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
	     "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::DsiOdfCartesianField::DsiOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< float >& boundingBox,
                 const gkg::Vector3d< float >& superResolution,
		 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        superResolution,
                                                        outputOrientationSet )
{

  try
  {

    _filterOrNot = ( bool )( specificScalarParameters[ 0 ] );
    _R0min = ( float ) ( specificScalarParameters[ 1 ] );
    _R0max = ( float ) ( specificScalarParameters[ 2 ] );
    _marginalODF = ( bool ) ( specificScalarParameters[ 3 ] );
        
    int32_t totalNumberOfbValues = this->getBValues().size();
    if ( totalNumberOfbValues !=  _inputOrientationSet.getCount() )
    {

      throw std::runtime_error(
                "Error in dw minf attributes : number of bValues "
                "and number of diffusion gradient orientations not the same" );

    }

    if ( totalNumberOfbValues !=  515 )
    {

      throw std::runtime_error( 
	 "This DSI code was designed for a fixed Cartesian grid with 515 points");
      
    }
	
    _gridRadius = 8;
    
    _items.getHeader().addAttribute( "odf_type",
                                    gkg::TypeOf< gkg::DsiOdf >::getName() );

    //gkg::Matrix m;
    //this->_functor = new gkg::DsiOdfCartesianFieldFunctor( *this, m );
    this->_functor = new gkg::DsiOdfCartesianFieldFunctor( *this );

    prepareForDSI( verbose );
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::DsiOdfCartesianField::DsiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
	     "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::DsiOdfCartesianField::DsiOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& /*specificStringParameters*/,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        outputOrientationSet )
{

  try
  {

    _filterOrNot = ( bool )( specificScalarParameters[ 0 ] );
    _R0min = ( float ) ( specificScalarParameters[ 1 ] );
    _R0max = ( float ) ( specificScalarParameters[ 2 ] );
    _marginalODF = ( bool ) ( specificScalarParameters[ 3 ] );

    int32_t totalNumberOfbValues = this->getBValues().size();
    if ( totalNumberOfbValues !=  _inputOrientationSet.getCount() )
    {

      throw std::runtime_error(
                "Error in dw minf attributes : number of bValues "
                "and number of diffusion gradient orientations not the same" );

    }

    if ( totalNumberOfbValues !=  515 )
    {

      throw std::runtime_error( 
	 "This DSI code was designed for a fixed Cartesian grid with 515 points");
      
    }
	
    _gridRadius = 8;

    _items.getHeader().addAttribute( "odf_type",
                                    gkg::TypeOf< gkg::DsiOdf >::getName() );

    // gkg::Matrix m;
//     this->_functor = new gkg::DsiOdfCartesianFieldFunctor( *this, m );
    this->_functor = new gkg::DsiOdfCartesianFieldFunctor( *this );

    prepareForDSI( verbose );
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::DsiOdfCartesianField::DsiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::DsiOdfCartesianField::~DsiOdfCartesianField()
{
}


bool gkg::DsiOdfCartesianField::getMarginalODF() const
{

  return _marginalODF;

}

bool gkg::DsiOdfCartesianField::getFilterOrNot() const
{

  return _filterOrNot;

}

int32_t gkg::DsiOdfCartesianField::getGridRadius() const
{

  return _gridRadius;

}


float gkg::DsiOdfCartesianField::getMinimumR0() const
{

  return _R0min;

}

float gkg::DsiOdfCartesianField::getMaximumR0() const
{

  return _R0max;

}

void gkg::DsiOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificScalarParameters.empty() )
    {
      
      specificScalarParameters.resize( 4 );
      specificScalarParameters[ 0 ] = 1;   //true
      specificScalarParameters[ 1 ] = 2;
      specificScalarParameters[ 2 ] = 6;
      specificScalarParameters[ 3 ] = 1;  //true

    }
    else
    {

      if ( specificScalarParameters.size() != 4U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }

            if( specificScalarParameters [ 1 ] > specificScalarParameters [ 2 ] ) 
      {
	
	throw std::runtime_error("invalid scalar parameter 'Minimum/Maximum R0'");

      }


      if( specificScalarParameters [ 1 ] < 0 || 
	  specificScalarParameters [ 1 ] > 8 ) 
      {
	
	throw std::runtime_error( "invalid scalar parameter 'Minimum R0'" );

      }

      if( specificScalarParameters [ 2 ] > 8 ) 
      {
	
	throw std::runtime_error( "invalid scalar parameter 'Maximum R0'. \
Must be smaller or equal to 8" );

      }



    }

    if ( !specificStringParameters.empty() )
    {

      throw std::runtime_error( "no specific string parameter" );

    }

  }
  GKG_CATCH( "void gkg::DsiOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::DsiOdfCartesianField::prepareForDSI( bool verbose )
{

  try
  {

    int32_t gridDiameter = _gridRadius*2 + 1;
    
    if ( verbose )
    {

      std::cout << "Preparing for DSI" << std::flush;

    }

    _filter.reallocate( gridDiameter, gridDiameter, gridDiameter );
    
    if( _filterOrNot ) // get Hanning window filter
    {

      for( int i = 0; i < _filter.getSizeX(); i++ )
      {
	
	for( int j = 0; j < _filter.getSizeY(); j++ )
	{
	
	  for( int k = 0; k < _filter.getSizeZ(); k++ )
	  {

	    _filter( i, j, k ) = hanningWindow[i][j][k];

	  }

	}      

      }
      
    }
    else
    {

      _filter.fill( 1.0 );  // Identity 
      
    }


    if ( verbose )
    {

      std::cout << gkg::Eraser( 30 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::DsiOdfCartesianField::prepareForDSI( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::DsiOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DsiOdf dsiOdf( &_inputOrientationSet,
			&_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_filter,
			_R0min, _R0max, _marginalODF );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dsiOdf );

    }
    else
    {

      return dsiOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DsiOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::DsiOdfCartesianField::newItem(
                                       const gkg::Vector3d< float >& originSite,
                                       const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::DsiOdf dsiOdf( &_inputOrientationSet,
		        &_outputOrientationSet,
			*_t2, *_dw,
			originSite,
			_filter,
			_R0min, _R0max, _marginalODF );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              dsiOdf );

    }
    else
    {

      return dsiOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::DsiOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}

