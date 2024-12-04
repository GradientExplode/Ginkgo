#include <gkg-communication-getopt/Application.h>

#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/SiteMapFactory_i.h>
#include <gkg-processing-coordinates/CartesianVoxelSampler.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSetCache.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-transform/Referential.h>
#include <gkg-processing-transform/Transform3dManager.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>

#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-container/OdfContinuousField.h>
#include <gkg-dmri-container/OdfCartesianToContinuousField.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-tractography/TractographyAlgorithm_i.h>
#include <gkg-dmri-tractography/TractographyAlgorithmFactory.h>
#include <gkg-dmri-sh-basis/SymmetricalSphericalHarmonicsCache.h>

#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-exception/Exception.h>

#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>


#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// main function
////////////////////////////////////////////////////////////////////////////////

int main( int32_t argc, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    std::string fileNameSiteMap;
    std::string fileNameOdfs;

    std::string fileNameDirectionsMap;
    std::string fileNameCoefficientsMap;

    std::string algorithmType = "streamline-probabilistic";

    std::vector< double > algorithmParameters;

    int32_t stepCount = 1000;
    int32_t outputOrientationCount = 0;

    std::string volumeFormat = "gis";

    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
    gkg::Application application( argc, argv, 
                                  "Tests the probabilistic random sampling "
                                  "based on a given ODF distribution" );
    application.addSingleOption( "-s",
                                 "Input site map",
                                 fileNameSiteMap );
    application.addSingleOption( "-o",
                                 "Input odf texture map file name",
                                 fileNameOdfs );
    application.addSingleOption( "-d",
                                 "Output directions file name",
                                 fileNameDirectionsMap );
    application.addSingleOption( "-sh",
                                 "Output SH coefficient file name",
                                 fileNameCoefficientsMap );
    application.addSeriesOption( "-parameters",
                                 "Tractography algorithm parameters as a vector"
                                 " of double <P1> <P2> :\n\n"
                                 "- in case of streamline-probabilistic,\n"
                                 "   <P1>: aperture angle,ie half cone inside"
                                         " which to look for neighbors given"
                                         " degrees\n"
                                 "   <P2>: temperature of the Gibb's sampler\n"
				 "   <P3>: normalize or not the probabilities\n"
                                 "   (default= 90 / 1 / 0)\n",
                                 algorithmParameters );
    application.addSingleOption( "-stepCount",
                                 "Number of random picks (default=1000)",
                                 stepCount,
                                 true );
    application.addSingleOption( "-outputOrientationCount",
                                 "Ouput orientation count (default=0)",
                                 outputOrientationCount,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();


    if ( outputOrientationCount < 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // algorithm parameter(s) check or initialization
    ////////////////////////////////////////////////////////////////////////////

    if ( algorithmType == "streamline-probabilistic"  )
    {

      if ( !algorithmParameters.empty() )
      {

        // aperture angle
        if ( algorithmParameters[ 0 ] <= 0.0 )
        {

          throw std::runtime_error(
                                  "aperture angle must be strictly positive" );

        }
        // Gibb's temperature
        if ( algorithmParameters[ 1 ] <= 0.0 )
        {

          throw std::runtime_error(
                               "Gibb's temperature must be strictly positive" );

        }

        // converting from degree to radian
        algorithmParameters[ 0 ] *= M_PI / 180.0;

      }
      else if ( algorithmParameters.empty() )
      {

        algorithmParameters.resize( 3 );
        algorithmParameters[ 0 ] = M_PI / 2;
	algorithmParameters[ 1 ] = 1;
	algorithmParameters[ 2 ] = 0;

      }
      else
      {

        throw std::runtime_error( "bad algorithm parameter count" );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading site(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameSiteMap << "' : " << std::flush;

    }
    gkg::SiteMap< int32_t, int32_t > siteMap;
    gkg::Reader::getInstance().read( fileNameSiteMap, siteMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading ODF texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameOdfs << "' : " << std::flush;

    }
    gkg::TextureMap< gkg::OrientationDistributionFunction > odfs;
    gkg::Reader::getInstance().read( fileNameOdfs, odfs );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building output orientation count
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building output orientation set : " << std::flush;

    }

    gkg::TextureMap< gkg::OrientationDistributionFunction >::const_iterator
      o = odfs.begin();
    if ( o == odfs.end() )
    {

      throw std::runtime_error( "ODF field is empty!" );

    }
    gkg::OrientationDistributionFunction::BasisType
      basisType = o->second.getBasisType();

    if ( basisType == gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      if ( outputOrientationCount <= 0 )
      {

        throw std::runtime_error( "missing or bad output orientation count" );

      }

    }
    else if ( basisType == gkg::OrientationDistributionFunction::Standard )
    {

      if ( outputOrientationCount > 0 )
      {

        throw std::runtime_error(
         "'outputOrientationCount' parameter will be ignored as ODF "
         "texture map provided has got a standard basis, and therefore a "
         "fixed output orientation count" );

      }
      outputOrientationCount = o->second.getValueCount();

    }
    gkg::OrientationSet outputOrientationSet( gkg::ElectrostaticOrientationSet(
                    outputOrientationCount / 2 ).getSymmetricalOrientations() );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building cartesian ODF field from ODF texture map / site map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building ODF cartesian field : " << std::flush;

    }

    gkg::OdfCartesianField* 
      odfCartesianField = new gkg::OdfCartesianField( siteMap,
                                                      odfs,
                                                      outputOrientationSet );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    // getting site(s)
    const std::list< gkg::Vector3d< int32_t > >&
      sites = siteMap.getSites( 0 );

    // preparing ouput volume
    int32_t sizeX = odfCartesianField->getSizeX();
    int32_t sizeY = odfCartesianField->getSizeY();
    int32_t sizeZ = odfCartesianField->getSizeZ();
    int sizeT = (odfs.getTexture(0)).getValues().size();
    
    if( verbose )
    {
	
      std::cout << sizeX << "x" << sizeY << "x" 
                << sizeZ << "x" << sizeT << "\n";
      
    }
    

    if ( sizeX*sizeY*sizeZ != 1 )
    {

      throw std::runtime_error(
         "Single voxel probabilistic random sampling: please give a single \
voxel site and texture spherical function\n" );

    }

    FILE *directions;
    directions = fopen(fileNameDirectionsMap.c_str(),"w");
    fprintf(directions,"%d\n",stepCount);
    

    FILE *shCoeffs;
    shCoeffs = fopen(fileNameCoefficientsMap.c_str(),"w");
    fprintf(shCoeffs,"%d\n",sizeT);
    
    gkg::OrientationSetCache::getInstance().update( outputOrientationSet,
                                                    algorithmParameters[ 0 ] );

    gkg::OrientationDistributionFunction* myOdf = 
      new gkg::OrientationDistributionFunction( 
			 gkg::OrientationDistributionFunction::SphericalHarmonics,
			 &outputOrientationSet );

    int32_t count = 0;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();

    while ( s != se && count < 1 )
    {
      
      myOdf->setValues( odfCartesianField->getItem( *s )->getValues() );
      if( verbose )
	std::cout << "odf : " << *s << "\n";

      for( int i =0; i < sizeT; i++)
	fprintf(shCoeffs,"%f\n",myOdf->getValues()[i]);

      float apertureAngle = algorithmParameters[ 0 ];
      float temperature   = algorithmParameters[ 1 ];
      bool  normalize     = algorithmParameters[ 2 ];
      if( verbose ) 
      {
	
	std::cout << "Aperture angle : " << apertureAngle << "\n";
	std::cout << "Gibb's temperature : " << temperature 
		  << "\n";
	std::cout << "Normalize probabilities : " << normalize
		  << "\n";	

      }

      int32_t orientationIndex = myOdf->getPrincipalOrientationIndex(
				    myOdf->getPrincipalOrientationIndex(),
				    apertureAngle );

      if(verbose)
	std::cout << "Done\n";

      fprintf( directions,"%f %f %f\n",
	       outputOrientationSet.getOrientation( orientationIndex ).x,
	       outputOrientationSet.getOrientation( orientationIndex ).y,
	       outputOrientationSet.getOrientation( orientationIndex ).z );

      if(verbose)
	printf( "%f %f %f\n",
		outputOrientationSet.getOrientation( orientationIndex ).x,
		outputOrientationSet.getOrientation( orientationIndex ).y,
		outputOrientationSet.getOrientation( orientationIndex ).z );
	      
      // thresholder to remove negative values
      gkg::GreaterOrEqualToFunction< float > greaterOrEqualToFunction( 0.0 );
      gkg::Thresholder< std::vector< float > > probabilityThresholder(
						  greaterOrEqualToFunction, 0.0 );

      // getting numerical analysis implementation factory
      gkg::NumericalAnalysisImplementationFactory*
	factory = gkg::NumericalAnalysisSelector::getInstance().
	                                                getImplementationFactory();
      gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    
      int32_t step;
      bool debug = false;
      for ( step = 1; step < stepCount; step++ )
      {
      		
	// computing the probabilities corresponding to the orientations contained
	// in the solid angle around the current orientation and of aperture angle
	// _apertureAngle, and collecting the orientation indices
	float minimumProbability = +1e38;
	float maximumProbability = -1e38;
	std::vector< float > probabilities;

	if( !normalize )
	{
	
	  probabilities = myOdf->getUnnormalizedOrientationProbabilities(
							orientationIndex,
                                                        apertureAngle,
                                                        &minimumProbability,
                                                        &maximumProbability );
	}
	else 
	{
	
	  probabilities = myOdf->getOrientationProbabilities();
	  minimumProbability = myOdf->getMinimumOrientationProbability();
	  maximumProbability = myOdf->getMaximumOrientationProbability();

	}

	probabilityThresholder.threshold( probabilities, probabilities );
	
	if ( minimumProbability < 0.0 )
	{
	    
	  minimumProbability = 0.0;
	    
	}

	float probabilityRange = maximumProbability - minimumProbability;
	float temperatureTimesProbabilityRange = temperature * probabilityRange;

	if(debug)
	{
	
	  std::cout << "min,max proba : " << minimumProbability 
		    << "," << maximumProbability << "\n";
	  std::cout << "probability range : " <<  "\n";

	}

	const std::vector< int32_t >& solidAngleOrientationIndices =
	  gkg::OrientationSetCache::getInstance().getSolidAngleOrientationIndices(
							       orientationIndex );

	// processing Gibb's likelyhoods
	std::list< float > likelyhoods;
	std::vector< float >::const_iterator
	  p = probabilities.begin(),
	  pe = probabilities.end();
	float likelyhood = 0.0;
	while ( p != pe )
	{
	    
	  if( temperature == 1 )
	  {
	  
	    likelyhood += ( float )(*p - minimumProbability );

	  }
	  else
	  {
	      
	    likelyhood += ( float )std::exp( ( (*p - minimumProbability )) /
					     temperatureTimesProbabilityRange );

	  }

	  likelyhoods.push_back( likelyhood );

	  if(debug)
	  {
	    
	    std::cout << "proba : " << *p << "\t"
		      <<  ( float )std::exp( ( (*p - minimumProbability )) /
					     temperatureTimesProbabilityRange )
		      << "\n";
	  }

	  ++ p;
	    
	}
	
	// choosing randomly the orientation
	float maximumLikelyhood = likelyhoods.back();
	float randomLikelyhood = ( float )factory->getUniformRandomNumber(
						     randomGenerator,
						     0.0,
						     ( double )maximumLikelyhood );
	if(debug)
	  std::cout << "max and random likelyhood : " 
		    << maximumLikelyhood << ", " << randomLikelyhood << "\n";

	
	std::vector< int32_t >::const_iterator
	  i = solidAngleOrientationIndices.begin(),
	  ie = solidAngleOrientationIndices.end();
	std::list< float >::const_iterator
	  l = likelyhoods.begin();
	int32_t randomOrientationIndex = 0;
	gkg::Vector3d< float > randomOrientation;
	while ( i != ie )
	{
	    
	  if ( *l >= randomLikelyhood )
	  {
		
	    randomOrientationIndex = *i;
	    break;
	    
	  }
	    
	  ++ i;
	  ++ l;
	  
	}

	fprintf( directions,"%f %f %f\n",
		 outputOrientationSet.getOrientation( randomOrientationIndex ).x,
		 outputOrientationSet.getOrientation( randomOrientationIndex ).y,
		 outputOrientationSet.getOrientation( randomOrientationIndex ).z );
	
	if(verbose)
	  printf( "%f %f %f\n",
		outputOrientationSet.getOrientation( randomOrientationIndex ).x,
		outputOrientationSet.getOrientation( randomOrientationIndex ).y,
		outputOrientationSet.getOrientation( randomOrientationIndex ).z );
	      
      }

      ++ s;
      ++ count;

    }

    if ( verbose )
    {
	  
      std::cout << "done" << std::flush;
      
    }
      

    
    //////////////
    //  Delete  //
    //////////////
    fclose(directions);
    fclose(shCoeffs);

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

