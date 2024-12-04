#include <gkg-dmri-io/EapFunctorWriter.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfEap_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfEap_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfEap_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfEap_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfEap_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfEap_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <utility>


////////////////////////////////////////////////////////////////////////////////
// creating disk format for TextureMap< OrientationDistributionFunction >
////////////////////////////////////////////////////////////////////////////////

// already declared in OdfFunctorWriter.cxx
// template class gkg::TextureMapAnalyzer<
//                                       gkg::OrientationDistributionFunction >;
// template class gkg::DiskFormatFactory<
//                    gkg::TextureMap< gkg::OrientationDistributionFunction > >;
// template class gkg::TextureMapDiskFormat<
//                                       gkg::OrientationDistributionFunction >;

//RegisterTextureAnalyzer( gkg::OrientationDistributionFunction, gkg_eap );
//RegisterTextureDiskFormat( gkg::OrientationDistributionFunction, gkg_eap );

gkg::EapFunctorWriter::EapFunctorWriter( float rgbScale,
					 float meshScale,
					 std::vector< float > meshRadii,
					 bool meshOpacity,
					 bool marginalOdf,
					 std::vector< float > odfIntegralLowerUpperLimits,
                                         const std::string& volumeFormat,
                                         const std::string& meshMapFormat,
                                         const std::string& textureMapFormat,
                                         bool ascii,
                                         bool verbose )
                        : _scalarParameters( 7 + meshRadii.size() ),
			  _stringParameters( 4 ),
			  _volumeFormat( volumeFormat ),
			  _meshMapFormat( meshMapFormat ),
			  _textureMapFormat( textureMapFormat ),
			  _ascii( ascii ),
			  _verbose( verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////
    if ( rgbScale <= 0 )
    {

      throw std::runtime_error( "RGB scale must be positive" );

    }
    if ( meshScale <= 0 )
    {

      throw std::runtime_error( "mesh scale must be positive" );

    }

    
    // filling scalar and string parameter(s)
    _scalarParameters[ 0 ] = rgbScale;
    _scalarParameters[ 1 ] = meshScale;
    _scalarParameters[ 2 ] = ( ascii ? 1.0 : 0.0 );
    _scalarParameters[ 3 ] = marginalOdf;
    _scalarParameters[ 4 ] = odfIntegralLowerUpperLimits[ 0 ];
    _scalarParameters[ 5 ] = odfIntegralLowerUpperLimits[ 1 ];
    _scalarParameters[ 6 ] = ( meshOpacity ? 1.0 : 0.0 );
    
    int32_t radiiCount = meshRadii.size();
    int32_t r;
    if ( _verbose )
    {

      std::cout << "Building output meshes of radius : ";

    }
    for( r = 0; r < radiiCount; r++ ) 
    {
      
      if ( meshRadii[ r ] < 0 )
      {
	
	throw std::runtime_error( "meshes radius must be positive" );

      }
      else 
      {
	
	_scalarParameters[ 7 + r ] = meshRadii[ r ];
	if ( _verbose )
	{
    
	  std::cout << _scalarParameters[ 7 + r ] << " ";

	}

      }

    }
    
    if ( _verbose )
    {
    
      std::cout << "\n";

    }

    _stringParameters[ 0 ] = _volumeFormat;
    _stringParameters[ 1 ] = _meshMapFormat;
    _stringParameters[ 2 ] = _textureMapFormat;

  }
  GKG_CATCH( "gkg::EapFunctorWriter::EapFunctorWriter( float rgbScale, "
             "float meshScale, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::EapFunctorWriter::~EapFunctorWriter()
{
}


void gkg::EapFunctorWriter::write(
                             const std::list< std::string >& eapFunctorNames,
                             const std::list< std::string >& fileNames,
                             gkg::EapCartesianField& eapCartesianField )
{

  try
  {


    // splitting EAP functor name(s) between those not requiring to be in the
    // standard basis and those requiring to be in the standard basis
    std::list< std::pair< std::string, std::string > > 
      namesNotRequiringStandardBasis;
    std::list< std::pair< std::string, std::string > > 
      namesRequiringStandardBasis;

    splitNames( eapFunctorNames, fileNames,
                namesNotRequiringStandardBasis, namesRequiringStandardBasis );

    // first, processing map(s) that do not require converting the EAP
    // map to the standard basis
    write( namesNotRequiringStandardBasis, eapCartesianField );
    
    if ( !namesRequiringStandardBasis.empty() )
    {

      // transforming EAP texture map to standard basis
      if ( eapCartesianField.getBasisType() != 
           gkg::OrientationDistributionFunction::Standard )
      {
	
        eapCartesianField.setStandardBasis( _verbose );

      }

      // second, processing map(s) that require converting the EAP
      // map to the standard basis
      write( namesRequiringStandardBasis, eapCartesianField );

    }

  }
  GKG_CATCH( "void gkg::EapFunctorWriter::write( "
             "const std::list< std::string >& eapFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "gkg::EapCartesianField& eapCartesianField ) const" );

}


std::list< std::string > gkg::EapFunctorWriter::getNames()
{

  try
  {

    std::list< std::string > functorNames;

    // C = gkg::Volume< float >, I = float
    std::list< std::string > otherFunctorNames =
      gkg::EapFunctorFactory< gkg::Volume< float >,
                                        float >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
    otherFunctorNames =
      gkg::EapFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                       gkg::Vector3d< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< float >, I = std::vector< float >
    otherFunctorNames =
      gkg::EapFunctorFactory< gkg::Volume< float >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
    otherFunctorNames =
      gkg::EapFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                            gkg::RGBComponent >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
    otherFunctorNames =
      gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                         std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = std::vector< gkg::MeshMap< int32_t, float, 3U > >, 
    // I = std::vector< float >
    otherFunctorNames =
      gkg::EapFunctorFactory< std::vector < gkg::MeshMap< int32_t, float, 3U > >,
                              std::vector< float > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::MeshMap< int32_t, float, 3U >,
    // I = std::vector< std::vector< float > >
    otherFunctorNames =
      gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
          std::vector< std::vector< float > > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
    // I = gkg::OrientationDistributionFunction
    otherFunctorNames =
      gkg::EapFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
         gkg::OrientationDistributionFunction >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
    otherFunctorNames =
      gkg::EapFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                     gkg::Vector3d< int32_t > >::getInstance().getNames();
    functorNames.merge( otherFunctorNames );

    return functorNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::EapFunctorWriter::getNames()" );

}


void gkg::EapFunctorWriter::write( 
                      const std::list< std::pair< std::string, std::string > >&
                                                        eapFunctorAndFileNames,
                      gkg::EapCartesianField& eapCartesianField )
{

  try
  {

    std::list< std::pair< std::string,
                          std::string > >::const_iterator
      
      n = eapFunctorAndFileNames.begin(),
      ne = eapFunctorAndFileNames.end();

    while ( n != ne )
    {
      
      // C = gkg::Volume< float >, I = float
      if ( gkg::EapFunctorFactory< gkg::Volume< float >,
                             float >::getInstance().hasEapFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< float > output;
        gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
          getEapFunctor( n->first ).process( output,
                                             eapCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
      else if ( gkg::EapFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
            gkg::Vector3d< float > >::getInstance().hasEapFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< gkg::Vector3d< float > > output;
        gkg::EapFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                                       gkg::Vector3d< float > >::getInstance().
          getEapFunctor( n->first ).process( output,
                                             eapCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::Volume< float >, I = std::vector< float >
      else if ( gkg::EapFunctorFactory< gkg::Volume< float >,
              std::vector< float > >::getInstance().hasEapFunctor( n->first ) )
      {

	_stringParameters[ 3 ] = n->second;
	gkg::Volume< float > output;
	gkg::EapFunctorFactory< gkg::Volume< float >,
	  std::vector< float > >::getInstance().
	  getEapFunctor( n->first ).process( output,
					     eapCartesianField,
					     _scalarParameters,
					     _stringParameters,
					     _verbose );
	if ( _verbose )
	{

	  std::cout << "writing " << n->first << " '" << n->second << "' : "
		    << std::flush;
	  
	}
	gkg::Writer::getInstance().write( n->second,
					  output,
					  _ascii,
					  _volumeFormat );
	if ( _verbose )
	{

	  std::cout << "done" << std::endl;
	  
	}

      }
	// C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
      else if ( gkg::EapFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                 gkg::RGBComponent >::getInstance().hasEapFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::Volume< gkg::RGBComponent > output;
        gkg::EapFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                                            gkg::RGBComponent >::getInstance().
          getEapFunctor( n->first ).process( output,
                                             eapCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = std::vector< gkg::MeshMap< int32_t, float, 3U > >, 
      // I = std::vector< float >
      else if ( gkg::EapFunctorFactory< std::vector< 
		gkg::MeshMap< int32_t, float, 3U > >,
		std::vector< float > >::getInstance().hasEapFunctor( n->first ) )
      {

	_stringParameters[ 3 ] = (n->second).c_str();
	
	std::vector < gkg::MeshMap< int32_t, float, 3U > > output;
	int32_t radiiCount = _scalarParameters.size() - 7;
	int32_t r, rinverse = radiiCount;

	for ( r = 0; r < radiiCount; r++ )
	{

	  gkg::MeshMap< int32_t, float, 3U >  eapAtR0;
	  output.push_back( eapAtR0 );	  

	}

	gkg::EapFunctorFactory< std::vector< 	
	                          gkg::MeshMap< int32_t, float, 3U > >,
	                               std::vector< float > >::getInstance().
	                                getEapFunctor( n->first ).process( 
							       output,
							       eapCartesianField,
							       _scalarParameters,
							       _stringParameters,
							       _verbose );
	
	

	float meshOpacityIncr = (1.0 - 0.1) / radiiCount;
	// writing output
	for ( r = 0; r < radiiCount; r++ )
	{
	
	  char filename[300];
	  sprintf( filename, "%s0%d", (n->second).c_str(), r );

	  _stringParameters[ 3 ] = filename;
	  _scalarParameters[ 7 ] = _scalarParameters[ 7 + r ];
	  
	  if ( _verbose )
	  {

	    std::cout << "writing " << n->first << " '" 
		      << _stringParameters[ 3 ] << "' : "
		      << std::flush;
	    
	  }

	  if ( _scalarParameters[ 6 ] ) //setting opacity
	  {
	    
	    output[r].addSemantic( "__materials_details__", 
				   "material", 
				   gkg::Semantic( 
				   gkg::TypeOf< gkg::Dictionary >::getName() ) );

	    // default ambient in anatomist
	    // 	    std::vector< float > ambient;
	    // 	    ambient.push_back(0.1);
	    // 	    ambient.push_back(0.1);
	    // 	    ambient.push_back(0.1);
	    // 	    ambient.push_back(1);
	    
	    std::vector< float > diffuse;
	    diffuse.push_back(0.8);  //red [0,1]
	    diffuse.push_back(0.8);  //green [0,1]
	    diffuse.push_back(0.8);  //blue [0,1]
	    diffuse.push_back( rinverse*meshOpacityIncr );    //opacity [0,1] 
	    
	    gkg::Dictionary dictionary;
	    dictionary[ "diffuse" ] = diffuse;

 	    output[r].getHeader().addAttribute( "material", dictionary );
	    
	  }

	  gkg::Writer::getInstance().write( _stringParameters[ 3 ],
					    output[r],
					    _ascii,
					    _meshMapFormat );

	  
	  if ( _verbose )
	  {
	    
	    std::cout << "done" << std::endl;
	    
	  }
	  
	  rinverse--;
	  
	}

      }
      // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
      else if ( gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
              std::vector< float > >::getInstance().hasEapFunctor( n->first ) )
      {

	_stringParameters[ 3 ] = (n->second).c_str();

	gkg::MeshMap< int32_t, float, 3U > output;
	gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
	  std::vector< float > >::getInstance().
	  getEapFunctor( n->first ).process( output,
					     eapCartesianField,
					     _scalarParameters,
					     _stringParameters,
					     _verbose );
	  if ( _verbose )
	  {

	    std::cout << "writing " << n->first << " '" 
		      << _stringParameters[ 3 ] << "' : "
		      << std::flush;
	    
	  }
	  
	  gkg::Writer::getInstance().write( n->second,
					    output,
					    _ascii,
					    _meshMapFormat );

	  if ( _verbose )
	  {

	    std::cout << "done" << std::endl;
	    
	  }
	  
      }
      // C = gkg::MeshMap< int32_t, float, 3U >,
      // I = std::vector< std::vector< float > >
      else if ( gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
                                                hasEapFunctor( n->first ) )
      {
	
        _stringParameters[ 3 ] = n->second;
        gkg::MeshMap< int32_t, float, 3U > output;
        gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
          getEapFunctor( n->first ).process( output,
                                             eapCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _meshMapFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
      // I = gkg::OrientationDistributionFunction
      else if ( gkg::EapFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
                         gkg::OrientationDistributionFunction >::getInstance().
                                                    hasEapFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::TextureMap< gkg::OrientationDistributionFunction > output;
        gkg::EapFunctorFactory< gkg::TextureMap<
                                gkg::OrientationDistributionFunction >,
                         gkg::OrientationDistributionFunction >::getInstance().
          getEapFunctor( n->first ).process( output,
                                             eapCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
      else if ( gkg::EapFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::getInstance().
                                                    hasEapFunctor( n->first ) )
      {

        _stringParameters[ 3 ] = n->second;
        gkg::SiteMap< int32_t, int32_t > output;
        gkg::EapFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                      gkg::Vector3d< int32_t > >::getInstance().
          getEapFunctor( n->first ).process( output,
                                             eapCartesianField,
                                             _scalarParameters,
                                             _stringParameters,
                                             _verbose );
        if ( _verbose )
        {

          std::cout << "writing " << n->first << " '" << n->second << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( n->second,
                                          output,
                                          _ascii,
                                          _volumeFormat );
        if ( _verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

      ++ n;

    }

  }
  GKG_CATCH( "void gkg::EapFunctorWriter::write( "
             "const std::list< std::pair< std::string, std::string > >& "
             "eapFunctorAndFileNames ) const" );

}


void gkg::EapFunctorWriter::splitNames(
    const std::list< std::string >& eapFunctorNames,
    const std::list< std::string >& fileNames,
    std::list< std::pair< std::string,
                          std::string > >& namesNotRequiringStandardBasis,
    std::list< std::pair< std::string,
                          std::string > >& namesRequiringStandardBasis )
{

  try
  {

    std::list< std::string >::const_iterator
      n = eapFunctorNames.begin(),
      ne = eapFunctorNames.end();
    std::list< std::string >::const_iterator
      f = fileNames.begin();
    while ( n != ne )
    {

      // C = gkg::Volume< float >, I = float
      if ( gkg::EapFunctorFactory< gkg::Volume< float >,
                                   float >::getInstance().hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::Volume< float >,
                                   float >::getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::Volume< gkg::Vector3d< float > >, I = gkg::Vector3d< float >
      else if ( gkg::EapFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                  gkg::Vector3d< float > >::getInstance().hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::Volume< gkg::Vector3d< float > >,
                  gkg::Vector3d< float > >::getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::Volume< float >, I = std::vector< float >
      else if ( gkg::EapFunctorFactory< gkg::Volume< float >,
                    std::vector< float > >::getInstance().hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::Volume< float >,
                    std::vector< float > >::getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::Volume< gkg::RGBComponent >, I = gkg::RGBComponent
      else if ( gkg::EapFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                       gkg::RGBComponent >::getInstance().hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                       gkg::RGBComponent >::getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::MeshMap< int32_t, float, 3U >, I = std::vector< float >
      else if ( gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< float > >::getInstance().hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< float > >::getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {
	  
          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = std::vector< gkg::MeshMap< int32_t, float, 3U > >
      // I = std::vector< float >
      else if ( gkg::EapFunctorFactory< std::vector< 
		gkg::MeshMap< int32_t, float, 3U > >,
		std::vector< float > >::getInstance().hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< std::vector<
	     gkg::MeshMap< int32_t, float, 3U > >,
	     std::vector< float > >::getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::MeshMap< int32_t, float, 3U >,
      // I = std::vector< std::vector< float > >
      else if ( gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                      std::vector< std::vector< float > > >::getInstance().
                                                      hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                    std::vector< std::vector< float > > >::getInstance().
                                                     getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = gkg::TextureMap< gkg::OrientationDistributionFunction >,
      // I = gkg::OrientationDistributionFunction
      else if ( gkg::EapFunctorFactory<
                  gkg::TextureMap< gkg::OrientationDistributionFunction >,
                  gkg::OrientationDistributionFunction >::getInstance().
                                                          hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::TextureMap<
                                       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >::
                                          getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }
      // C = SiteMap< int32_t, int32_t >, I = gkg::Vector3d< int32_t >
      else if ( gkg::EapFunctorFactory<
                  gkg::SiteMap< int32_t, int32_t >,
                  gkg::Vector3d< int32_t > >::getInstance().
                                                          hasEapFunctor( *n ) )
      {

        if ( gkg::EapFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                                     gkg::Vector3d< int32_t > >::
                                          getInstance().getEapFunctor( *n ).
             isStandardBasisRequired() )
        {

          namesRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }
        else
        {

          namesNotRequiringStandardBasis.push_back(
                             std::pair< std::string, std::string >( *n, *f ) );

        }

      }

      ++ n;
      ++ f;

    }

  }
  GKG_CATCH( "void gkg::EapFunctorWriter::splitNames( "
             "const std::list< std::string >& eapFunctorNames, "
             "const std::list< std::string >& fileNames, "
             "std::list< std::pair< std::string, "
             "std::string > >& namesNotRequiringStandardBasis, "
             "std::list< std::pair< std::string, "
             "std::string > >& namesRequiringStandardBasis )" );

}


