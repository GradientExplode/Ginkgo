#include <gkg-processing-plugin-functors/Registration3d/Registration3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineWoShearingTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-registration/RegistrationAlgorithm_i.h>
#include <gkg-processing-algobase/CenterOfGravity_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>


//
// class RegistrationSubProcess< T1 >
//

template < class T1 >
class RegistrationSubProcess : public gkg::Process
{

  public:

    RegistrationSubProcess(
      const std::string& theReferenceName,
      const std::string& theOutputName,
      const std::string& theOutputInverseName,
      const std::string& theSimilarityMeasureName,
      const std::string& theOptimizerName,
      const int32_t& theResamplingOrder,
      const std::list< int32_t >& theSubSamplingMaximumSizes,
      const std::vector< double >& theSimilarityMeasureParameters,
      const std::vector< double >& theOptimizerParameters,
      gkg::Vector& theVectorOfInitialParameters,
      const double& theReferenceLowerThreshold,
      const double& theFloatingLowerThreshold,
      gkg::RCPointer< gkg::NonLinearTransform3d< float > >&
                                                        theNonLinearTransform3d,
      const std::string& theIntermediateFileName,
      const bool& theIgnoreHeaderTransformations,
      const bool& theInitializedUsingCentreOfGravity,
      const bool& theVerbose );

    const std::string& referenceName;
    const std::string& outputName;
    const std::string& outputInverseName;
    const std::string& similarityMeasureName;
    const std::string& optimizerName;
    const int32_t& resamplingOrder;
    const std::list< int32_t >& subSamplingMaximumSizes;
    const std::vector< double >& similarityMeasureParameters;
    const std::vector< double >& optimizerParameters;
    gkg::Vector& vectorOfInitialParameters;
    const double& referenceLowerThreshold;
    const double& floatingLowerThreshold;
    gkg::RCPointer< gkg::NonLinearTransform3d< float > >& nonLinearTransform3d;
    const std::string& intermediateFileName;
    const bool& ignoreHeaderTransformations;
    const bool& initializedUsingCentreOfGravity;
    const bool& verbose;

  private:

    template < class T2 >
    static void match( gkg::Process& process,
                       const std::string& referenceName,
                       const gkg::AnalyzedObject&,
                       const std::string& );


};


template < class T1 >
RegistrationSubProcess< T1 >::RegistrationSubProcess(
     const std::string& theReferenceName,
     const std::string& theOutputName,
     const std::string& theOutputInverseName,
     const std::string& theSimilarityMeasureName,
     const std::string& theOptimizerName,
     const int32_t& theResamplingOrder,
     const std::list< int32_t >& theSubSamplingMaximumSizes,
     const std::vector< double >& theSimilarityMeasureParameters,
     const std::vector< double >& theOptimizerParameters,
     gkg::Vector& theVectorOfInitialParameters,
     const double& theReferenceLowerThreshold,
     const double& theFloatingLowerThreshold,
     gkg::RCPointer< gkg::NonLinearTransform3d< float > >&
                                                        theNonLinearTransform3d,
     const std::string& theIntermediateFileName,
     const bool& theIgnoreHeaderTransformations,
     const bool& theInitializedUsingCentreOfGravity,
     const bool& theVerbose )
                             : gkg::Process( "Volume" ),
                               referenceName( 
                                 theReferenceName ),
                               outputName( 
                                 theOutputName ),
                               outputInverseName( 
                                 theOutputInverseName ),
                               similarityMeasureName( 
                                 theSimilarityMeasureName ),
                               optimizerName( 
                                 theOptimizerName ),
                               resamplingOrder( 
                                 theResamplingOrder ),
                               subSamplingMaximumSizes( 
                                 theSubSamplingMaximumSizes ),
                               similarityMeasureParameters( 
                                 theSimilarityMeasureParameters ),
                               optimizerParameters( 
                                 theOptimizerParameters ),
                               vectorOfInitialParameters( 
                                 theVectorOfInitialParameters ),
                               referenceLowerThreshold( 
                                 theReferenceLowerThreshold ),
                               floatingLowerThreshold( 
                                 theFloatingLowerThreshold ),
                               nonLinearTransform3d( 
                                 theNonLinearTransform3d ),
                               intermediateFileName( 
                                 theIntermediateFileName ),
                               ignoreHeaderTransformations( 
                                 theIgnoreHeaderTransformations ),
                               initializedUsingCentreOfGravity( 
                                 theInitializedUsingCentreOfGravity ),
                               verbose( 
                                 theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &match< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &match< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &match< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &match< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &match< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &match< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &match< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &match< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &match< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &match< double > );

  }
  GKG_CATCH( "template < class T1 > "
             "RegistrationSubProcess< T1 >::RegistrationSubProcess( "
             "const std::string& theReferenceName, "
             "const std::string& theOutputName, "
             "const std::string& theOutputInverseName, "
             "const std::string& theSimilarityMeasureName, "
             "const std::string& theOptimizerName, "
             "const int32_t& theResamplingOrder, "
             "const std::list< int32_t >& theSubSamplingMaximumSizes, "
             "const std::vector< double >& theSimilarityMeasureParameters, "
             "const std::vector< double >& theOptimizerParameters, "
             "gkg::Vector& theVectorOfInitialParameters, "
             "const double& theReferenceLowerThreshold, "
             "const double& theFloatingLowerThreshold, "
             "gkg::RCPointer< gkg::NonLinearTransform3d< float > >& "
             "theNonLinearTransform3d, "
             "const std::string& theIntermediateFileName, "
             "const bool& theIgnoreHeaderTransformations, "
             "const bool& theInitializedUsingCentreOfGravity, "
             "const bool& theVerbose )" );

}


template < class T1 > template < class T2 >
void 
RegistrationSubProcess< T1 >::match( gkg::Process& process,
                                     const std::string& floatingName,
                                     const gkg::AnalyzedObject&,
                                     const std::string& )
{

  try
  {

    RegistrationSubProcess< T1 >&
      registrationSubProcess = static_cast< RegistrationSubProcess< T1 >& >(
                                                                      process );

    const std::string& referenceName
      = registrationSubProcess.referenceName;
    const std::string& outputName
      = registrationSubProcess.outputName;
    const std::string& outputInverseName
      = registrationSubProcess.outputInverseName;
    const std::string& similarityMeasureName
      = registrationSubProcess.similarityMeasureName;
    const std::string& optimizerName
      = registrationSubProcess.optimizerName;
    const int32_t& resamplingOrder
      = registrationSubProcess.resamplingOrder;
    const std::list< int32_t >& subSamplingMaximumSizes
      = registrationSubProcess.subSamplingMaximumSizes;
    const std::vector< double >& similarityMeasureParameters
      = registrationSubProcess.similarityMeasureParameters;
    const std::vector< double >& optimizerParameters
      = registrationSubProcess.optimizerParameters;
    gkg::Vector& vectorOfInitialParameters
      = registrationSubProcess.vectorOfInitialParameters;
    const double& referenceLowerThreshold
      = registrationSubProcess.referenceLowerThreshold;
    const double& floatingLowerThreshold
      = registrationSubProcess.floatingLowerThreshold;
    gkg::RCPointer< gkg::NonLinearTransform3d< float > >& nonLinearTransform3d
      = registrationSubProcess.nonLinearTransform3d;
    const std::string& intermediateFileName
      = registrationSubProcess.intermediateFileName;
    const bool& ignoreHeaderTransformations
      = registrationSubProcess.ignoreHeaderTransformations;
    const bool& initializedUsingCentreOfGravity
      = registrationSubProcess.initializedUsingCentreOfGravity;
    const bool& verbose
      = registrationSubProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading reference object
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading reference object '"
                << referenceName << "' : "
                << std::flush;

    }
    gkg::RCPointer< gkg::Volume< T1 > > 
      reference( new gkg::Volume< T1 > );
    gkg::Reader::getInstance().read( referenceName, *reference );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading floating object
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading floating object '"
                << floatingName << "' : "
                << std::flush;

    }
    gkg::RCPointer< gkg::Volume< T2 > > floating( new gkg::Volume< T2 > );
    gkg::Reader::getInstance().read( floatingName, *floating );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // Sanity check : volumes size T should be 1 : resize otherwise
    if ( reference->getSizeT() != 1 )
    {

      gkg::RCPointer< gkg::Volume< T1 > > copyRef( new gkg::Volume< T1 > );
      *copyRef = *reference;
      gkg::Vector3d< double > savedResolution( 1.0, 1.0, 1.0 );
      reference->getResolution( savedResolution );

      int32_t sizeX = reference->getSizeX();
      int32_t sizeY = reference->getSizeY();
      int32_t sizeZ = reference->getSizeZ();

      reference->reallocate( sizeX, sizeY, sizeZ, 1 );
      reference->setResolution( savedResolution );
      if ( copyRef->getHeader().hasAttribute( "transformations" ) )
      {

        reference->getHeader()[ "transformations" ] = \
                                      copyRef->getHeader()[ "transformations" ];

      }

      // Copy volume
      typename gkg::Volume< T1 >::iterator i1  = reference->begin(),
                                           i1e = reference->begin() +
                                                 sizeX * sizeY * sizeZ;
      typename gkg::Volume< T1 >::iterator i2  = copyRef->begin();

      while ( i1 != i1e )
      {

        *i2 = *i1;
        ++ i1;
        ++ i2;

      }

    }

    // Sanity check : volumes size T should be 1 : resize otherwise
    if ( floating->getSizeT() != 1 )
    {

      gkg::RCPointer< gkg::Volume< T2 > > copyFloat( new gkg::Volume< T2 > );
      *copyFloat = *floating;
      gkg::Vector3d< double > savedResolution( 1.0, 1.0, 1.0 );
      floating->getResolution( savedResolution );

      int32_t sizeX = floating->getSizeX();
      int32_t sizeY = floating->getSizeY();
      int32_t sizeZ = floating->getSizeZ();

      floating->reallocate( sizeX, sizeY, sizeZ, 1 );
      floating->setResolution( savedResolution );
      if ( copyFloat->getHeader().hasAttribute( "transformations" ) )
      {

        floating->getHeader()[ "transformations" ] = \
                                    copyFloat->getHeader()[ "transformations" ];

      }

      // Copy volume
      typename gkg::Volume< T2 >::iterator i1  = floating->begin(),
                                           i1e = floating->begin() +
                                                 sizeX * sizeY * sizeZ;
      typename gkg::Volume< T2 >::iterator i2  = copyFloat->begin();

      while ( i1 != i1e )
      {

        *i2 = *i1;
        ++ i1;
        ++ i2;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // try to retrieve useful position information
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector initialParameters = vectorOfInitialParameters;

    // If no initial parameters were specified by the user and if we found
    // transformations attribute in header files : we can compute a
    // good approximation of the initial parameters
    if ( ( initializedUsingCentreOfGravity == false ) &&
         ( ignoreHeaderTransformations == false ) &&
         reference->getHeader().hasAttribute( "transformations" ) &&
         floating->getHeader().hasAttribute( "transformations" ) )
    {

      if ( verbose )
      {

        std::cout << "transformations information found in header"
                  << std::endl;

      }

      int32_t referenceIndex = 0;

      // If several referentials exist, take information from the
      // shared referential
      if ( reference->getHeader().hasAttribute( "referentials" ) &&
           floating->getHeader().hasAttribute( "referentials" ) )
      {

        std::vector< std::string > referenceNames;
        std::vector< std::string > floatingNames;

        reference->getHeader().getAttribute( "referentials", referenceNames );
        floating->getHeader().getAttribute( "referentials", floatingNames );

        bool found = false;
        int32_t rIndex = 0;
        std::vector< std::string >::iterator
          r = referenceNames.begin(),
          re = referenceNames.end();

        while ( !found && ( r != re ) )
        {

          //int32_t fIndex = 0;
          std::vector< std::string >::iterator
            f = floatingNames.begin(),
            fe = floatingNames.end();

          while ( !found && ( f != fe ) )
          {

            if ( *f == *r )
            {

              found = true;
              referenceIndex = rIndex;

            }

            ++f;

          }

          rIndex++;
          ++r;

        }

      }

      // Get homogeneous transform matrix of reference volume
      gkg::GenericObjectList transformations;
      std::vector< float > referenceDirectCoefficients( 16 );

      try
      {

        if ( ignoreHeaderTransformations )
        {

          std::cerr << "Ignoring header transformations" << std::endl;
          referenceDirectCoefficients[ 0 ] = 1.0;
          referenceDirectCoefficients[ 5 ] = 1.0;
          referenceDirectCoefficients[ 10 ] = 1.0;
          referenceDirectCoefficients[ 15 ] = 1.0;

        }
        else
        {

          reference->getHeader().getAttribute( "transformations",
                                               transformations );
          gkg::GenericObjectList transformation =
            transformations[ referenceIndex
                                        ]->getValue< gkg::GenericObjectList >();

          for ( int32_t i = 0; i < 16; ++ i )
          {

            referenceDirectCoefficients[ i ] = transformation[ i ]->getScalar();

          }

        }

      }
      catch( std::exception& )
      {

        if ( verbose )
        {

          std::cerr << "Failed to read attribute transformations" << std::endl;

        }

        referenceDirectCoefficients[ 0 ] = 1.0;
        referenceDirectCoefficients[ 5 ] = 1.0;
        referenceDirectCoefficients[ 10 ] = 1.0;
        referenceDirectCoefficients[ 15 ] = 1.0;

      }

      if ( verbose )
      {

        std::cout << "Reference matrix found:" << std::endl;
        for ( int32_t i = 0; i < 16; ++ i )
        {

          if ( i % 4 == 0 )
          {

            std::cout << std::endl;

          }

          std::cout << referenceDirectCoefficients[ i ] << " " << std::flush;

        }

        std::cout << std::endl;

      }


      // Get homogeneous transform matrix of floating volume
      std::vector< float > floatingDirectCoefficients( 16 );

      try
      {

        if ( ignoreHeaderTransformations )
        {

          floatingDirectCoefficients[ 0 ] = 1.0;
          floatingDirectCoefficients[ 5 ] = 1.0;
          floatingDirectCoefficients[ 10 ] = 1.0;
          floatingDirectCoefficients[ 15 ] = 1.0;

        }
        else
        {

          floating->getHeader().getAttribute( "transformations",
                                              transformations );
          gkg::GenericObjectList transformation =
            transformations[ referenceIndex
                                        ]->getValue< gkg::GenericObjectList >();

          for ( int32_t i = 0; i < 16; ++i )
          {

            floatingDirectCoefficients[ i ] = transformation[ i ]->getScalar();

          }

        }

      }
      catch( std::exception& )
      {

        if ( verbose )
        {

          std::cerr << "Failed to read attribute transformations" << std::endl;

        }

        floatingDirectCoefficients[ 0 ] = 1.0;
        floatingDirectCoefficients[ 5 ] = 1.0;
        floatingDirectCoefficients[ 10 ] = 1.0;
        floatingDirectCoefficients[ 15 ] = 1.0;

      }

      if ( verbose )
      {

        std::cout << "Floating matrix found:" << std::endl;
        for ( int32_t i = 0; i < 16; ++i )
        {

          if ( i % 4 == 0 )
          {

            std::cout << std::endl;

          }

          std::cout << floatingDirectCoefficients[ i ] << " " << std::flush;

        }

        std::cout << std::endl;

      }

      bool direct = true;
      bool indirect = false;

      gkg::HomogeneousTransform3d< float > transform3dFloatToMriReferential(
                                         floatingDirectCoefficients, indirect );

      gkg::HomogeneousTransform3d< float > transform3dMriReferentialToRef(
                                          referenceDirectCoefficients, direct );

      gkg::CompositeTransform3d< float > compositeTransform3d;

      compositeTransform3d.compose( transform3dFloatToMriReferential );
      compositeTransform3d.compose( transform3dMriReferentialToRef );


      gkg::Matrix initMatrix( 4, 4 );
      gkg::HomogeneousTransform3d< float > hTransform(
                                        compositeTransform3d.getComposition() );

      for ( int32_t i = 0; i < 4; ++i )
      {

        for ( int32_t j = 0; j < 4; ++j )
        {

          initMatrix( i, j ) = hTransform.getDirectCoefficient( i, j );

        }

      }

      gkg::Vector3d< float > scalings;
      gkg::Vector3d< float > shearings;
      gkg::Vector3d< float > u;
      gkg::Vector3d< float > v;
      gkg::Vector3d< float > w;

      // Find rotation for floating volume
      gkg::getXYZScalingsAndShearingsAndTrieder( initMatrix,
                                                 scalings,
                                                 shearings,
                                                 u, v, w );

      gkg::Vector3d< float > initAngles =
                        gkg::getXYZAnglesFromTrieder< float >( u, v, w, false );

      gkg::Vector3d< double > translationFloatingToReference(
                                                           initMatrix( 0, 3 ),
                                                           initMatrix( 1, 3 ),
                                                           initMatrix( 2, 3 ) );
      if ( verbose )
      {

        std::cout << "translation between floating to reference: "
                  << translationFloatingToReference << std::endl;

      }

      gkg::Vector3d< double > rotationFloatingToReference( initAngles.x,
                                                           initAngles.y,
                                                           initAngles.z );

      if ( verbose )
      {

        std::cout << "angles between floating and reference: " << std::endl
         << "X: " << 180.0 / M_PI * rotationFloatingToReference.x  << std::endl
         << "Y: " << 180.0 / M_PI * rotationFloatingToReference.y  << std::endl
         << "Z: " << 180.0 / M_PI * rotationFloatingToReference.z  << std::endl;

      }

      // Retrieve default parameters
      if ( !nonLinearTransform3d->getParameterizedLinearTransform3d().isNull() )
      {

        int32_t parameterCount =
                     nonLinearTransform3d->getParameterizedLinearTransform3d()->
                                                            getParameterCount();
        initialParameters.reallocate( parameterCount );
        nonLinearTransform3d->getParameterizedLinearTransform3d()->
                                       getDirectParameters( initialParameters );

        // Copy translation and rotation found
        if ( parameterCount >= 6 )
        {

          initialParameters( parameterCount - 6 ) =
                                               rotationFloatingToReference.x;
          initialParameters( parameterCount - 5 ) =
                                               rotationFloatingToReference.y;
          initialParameters( parameterCount - 4 ) =
                                               rotationFloatingToReference.z;
          initialParameters( parameterCount - 3 ) =
                                               translationFloatingToReference.x;
          initialParameters( parameterCount - 2 ) =
                                               translationFloatingToReference.y;
          initialParameters( parameterCount - 1 ) =
                                               translationFloatingToReference.z;

        }
        else
        {

          std::cerr << "bad parameters count, keep default parameters"
                    << std::endl;

        }

      }

    }
    else if ( initializedUsingCentreOfGravity == true )
    {

      if ( verbose )
      {

        std::cout << "No initial parameter specified" << std::endl;
        std::cout << "Init matrix using center of gravity of volumes"
                  << std::endl;

      }

      gkg::CenterOfGravity< T1 > referenceCenterOfGravity;
      gkg::CenterOfGravity< T2 > floatCenterOfGravity;
      gkg::Vector3d< float > referenceCoG =
                                 referenceCenterOfGravity.compute( *reference );
      gkg::Vector3d< float > floatingCoG =
                                      floatCenterOfGravity.compute( *floating );

      gkg::Vector3d< double > translationFloatingToReference(
                                               referenceCoG.x - floatingCoG.x,
                                               referenceCoG.y - floatingCoG.y,
                                               referenceCoG.z - floatingCoG.z );

      if ( !nonLinearTransform3d->getParameterizedLinearTransform3d().isNull() )
      {

        int32_t parameterCount =
                     nonLinearTransform3d->getParameterizedLinearTransform3d()->
                                                            getParameterCount();
        initialParameters.reallocate( parameterCount );
        nonLinearTransform3d->getParameterizedLinearTransform3d()->
                                       getDirectParameters( initialParameters );

        // Copy translations
        if ( parameterCount >= 6 )
        {

          initialParameters( parameterCount - 3 ) =
                                               translationFloatingToReference.x;
          initialParameters( parameterCount - 2 ) =
                                               translationFloatingToReference.y;
          initialParameters( parameterCount - 1 ) =
                                               translationFloatingToReference.z;

        }
        else
        {

          std::cerr << "bad parameters count, keep default parameters"
                    << std::endl;

        }

      }

    }
    else
    {

      // Nothing to do : use parameters already supplied

    }

    ////////////////////////////////////////////////////////////////////////////
    // performing registration
    ////////////////////////////////////////////////////////////////////////////

    gkg::RegistrationAlgorithm< T1, T2 >
      registrationAlgorithm( similarityMeasureName,
                             optimizerName,
                             resamplingOrder,
                             subSamplingMaximumSizes,
                             similarityMeasureParameters,
                             optimizerParameters,
                             verbose );

    registrationAlgorithm.match(
                      reference,
                      floating,
                      initialParameters,
                      ( T1 )referenceLowerThreshold,
                      ( T2 )floatingLowerThreshold,
                      nonLinearTransform3d,
                      intermediateFileName );

    ////////////////////////////////////////////////////////////////////////////
    // saving 3D direct transform
    ////////////////////////////////////////////////////////////////////////////

    if ( !outputName.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving 3D direct transform : " << std::flush;

      }

      std::string outputNameLinear;
      std::string outputNameDiffeomorphicDirect;
      std::string outputNameDiffeomorphicInverse;
      if ( ( outputName.length() > 4U ) &&
           ( outputName.substr( outputName.length() - 4U, 4U ) == ".trm" ) )
      {

        std::string baseName = outputName.substr( 0U,
                                                  outputName.length() - 4U );
        outputNameLinear = baseName + ".trm";
        outputNameDiffeomorphicDirect = baseName +
                                        ".diffeomorphic.direct.trm";
        outputNameDiffeomorphicInverse = baseName + 
                                         ".diffeomorphic.inverse.trm";

      }
      else
      {

        outputNameLinear = outputName + ".trm";
        outputNameDiffeomorphicDirect = outputName +
                                        ".diffeomorphic.direct.trm";
        outputNameDiffeomorphicInverse = outputName + 
                                         ".diffeomorphic.inverse.trm";


      }

      nonLinearTransform3d->writeTrm( outputNameLinear,
                                      outputNameDiffeomorphicDirect,
                                      outputNameDiffeomorphicInverse );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving 3D inverse transform when possible
    ////////////////////////////////////////////////////////////////////////////

    if ( !outputInverseName.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving 3D inverse transform : " << std::flush;

      }


      std::string outputInverseNameLinear;
      std::string outputInverseNameDiffeomorphicDirect;
      std::string outputInverseNameDiffeomorphicInverse;
      if ( ( outputInverseName.length() > 4U ) &&
           ( outputInverseName.substr( outputInverseName.length() - 4U, 4U ) ==
                                                                      ".trm" ) )
      {

        std::string baseName = outputInverseName.substr(
                                              0U,
                                              outputInverseName.length() - 4U );
        outputInverseNameLinear = baseName + ".trm";
        outputInverseNameDiffeomorphicDirect = baseName +
                                               ".diffeomorphic.direct.trm";
        outputInverseNameDiffeomorphicInverse = baseName +
                                                ".diffeomorphic.inverse.trm";

      }
      else
      {

        outputInverseNameLinear = outputInverseName + ".trm";
        outputInverseNameDiffeomorphicDirect = outputInverseName +
                                               ".diffeomorphic.direct.trm";
        outputInverseNameDiffeomorphicInverse = outputInverseName +
                                                ".diffeomorphic.inverse.trm";


      }

      nonLinearTransform3d->writeInverseTrm(
                                        outputInverseNameLinear,
                                        outputInverseNameDiffeomorphicDirect,
                                        outputInverseNameDiffeomorphicInverse );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T1 > template < class T2 > "
             "void  "
             "RegistrationSubProcess< T1 >::match( "
             "gkg::Process& process, "
             "const std::string& referenceName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class RegistrationProcess
//

class RegistrationProcess : public gkg::Process
{

  public:

    RegistrationProcess(
      const std::string& theFloatingName,
      const std::string& theOutputName,
      const std::string& theOutputInverseName,
      const std::string& theSimilarityMeasureName,
      const std::string& theOptimizerName,
      const int32_t& theResamplingOrder,
      const std::list< int32_t >& theSubSamplingMaximumSizes,
      const std::vector< double >& theSimilarityMeasureParameters,
      const std::vector< double >& theOptimizerParameters,
      gkg::Vector& theVectorOfInitialParameters,
      const double& theReferenceLowerThreshold,
      const double& theFloatingLowerThreshold,
      gkg::RCPointer< gkg::NonLinearTransform3d< float > >&
                                                        theNonLinearTransform3d,
      const std::string& theIntermediateFileName,
      const bool& theIgnoreHeaderTransformations,
      const bool& theInitializedUsingCentreOfGravity,
      const bool& theVerbose );

    const std::string& floatingName;
    const std::string& outputName;
    const std::string& outputInverseName;
    const std::string& similarityMeasureName;
    const std::string& optimizerName;
    const int32_t& resamplingOrder;
    const std::list< int32_t >& subSamplingMaximumSizes;
    const std::vector< double >& similarityMeasureParameters;
    const std::vector< double >& optimizerParameters;
    gkg::Vector& vectorOfInitialParameters;
    const double& referenceLowerThreshold;
    const double& floatingLowerThreshold;
    gkg::RCPointer< gkg::NonLinearTransform3d< float > >& nonLinearTransform3d;
    const std::string& intermediateFileName;
    const bool& ignoreHeaderTransformations;
    const bool& initializedUsingCentreOfGravity;
    const bool& verbose;

  private:

    template < class T1 >
    static void match( gkg::Process& process,
                       const std::string& referenceName,
                       const gkg::AnalyzedObject&,
                       const std::string& );


};


RegistrationProcess::RegistrationProcess( 
      const std::string& theFloatingName,
      const std::string& theOutputName,
      const std::string& theOutputInverseName,
      const std::string& theSimilarityMeasureName,
      const std::string& theOptimizerName,
      const int32_t& theResamplingOrder,
      const std::list< int32_t >& theSubSamplingMaximumSizes,
      const std::vector< double >& theSimilarityMeasureParameters,
      const std::vector< double >& theOptimizerParameters,
      gkg::Vector& theVectorOfInitialParameters,
      const double& theReferenceLowerThreshold,
      const double& theFloatingLowerThreshold,
      gkg::RCPointer< gkg::NonLinearTransform3d< float > >&
                                                        theNonLinearTransform3d,
      const std::string& theIntermediateFileName,
      const bool& theIgnoreHeaderTransformations,
      const bool& theInitializedUsingCentreOfGravity,
      const bool& theVerbose )
                    : gkg::Process( "Volume" ),
                      floatingName( theFloatingName ),
                      outputName( theOutputName ),
                      outputInverseName( theOutputInverseName ),
                      similarityMeasureName( theSimilarityMeasureName ),
                      optimizerName( theOptimizerName ),
                      resamplingOrder( theResamplingOrder ),
                      subSamplingMaximumSizes( theSubSamplingMaximumSizes ),
                      similarityMeasureParameters(
                                               theSimilarityMeasureParameters ),
                      optimizerParameters( theOptimizerParameters ),
                      vectorOfInitialParameters( theVectorOfInitialParameters ),
                      referenceLowerThreshold( theReferenceLowerThreshold ),
                      floatingLowerThreshold( theFloatingLowerThreshold ),
                      nonLinearTransform3d( theNonLinearTransform3d ),
                      intermediateFileName( theIntermediateFileName ),
                      ignoreHeaderTransformations(
                                               theIgnoreHeaderTransformations ),
                      initializedUsingCentreOfGravity(
                                           theInitializedUsingCentreOfGravity ),
                      verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &RegistrationProcess::match< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &RegistrationProcess::match< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &RegistrationProcess::match< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &RegistrationProcess::match< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &RegistrationProcess::match< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &RegistrationProcess::match< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &RegistrationProcess::match< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &RegistrationProcess::match< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &RegistrationProcess::match< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &RegistrationProcess::match< double > );

  }
  GKG_CATCH( "RegistrationProcess::RegistrationProcess( "
             "const std::string& theFloatingName, "
             "const std::string& theOutputName, "
             "const std::string& theOutputInverseName, "
             "const std::string& theSimilarityMeasureName, "
             "const std::string& theOptimizerName, "
             "const int32_t& theResamplingOrder, "
             "const std::list< int32_t >& theSubSamplingMaximumSizes, "
             "const std::vector< double >& theSimilarityMeasureParameters, "
             "const std::vector< double >& theOptimizerParameters, "
             "gkg::Vector& theVectorOfInitialParameters, "
             "const double& theReferenceLowerThreshold, "
             "const double& theFloatingLowerThreshold, "
             "gkg::RCPointer< gkg::NonLinearTransform3d< float > >& "
             "theNonLinearTransform3d, "
             "const std::string& theIntermediateFileName, "
             "const bool& theIgnoreHeaderTransformations, "
             "const bool& theInitializedUsingCentreOfGravity, "
             "const bool& theVerbose )" );

}


template < class T1 >
void RegistrationProcess::match( gkg::Process& process,
                                 const std::string& referenceName,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    RegistrationProcess&
      registrationProcess = static_cast< RegistrationProcess& >( process );

    RegistrationSubProcess< T1 > registrationSubProcess(
                          referenceName,
                          registrationProcess.outputName,
                          registrationProcess.outputInverseName,
                          registrationProcess.similarityMeasureName,
                          registrationProcess.optimizerName,
                          registrationProcess.resamplingOrder,
                          registrationProcess.subSamplingMaximumSizes,
                          registrationProcess.similarityMeasureParameters,
                          registrationProcess.optimizerParameters,
                          registrationProcess.vectorOfInitialParameters,
                          registrationProcess.referenceLowerThreshold,
                          registrationProcess.floatingLowerThreshold,
                          registrationProcess.nonLinearTransform3d,
                          registrationProcess.intermediateFileName,
                          registrationProcess.ignoreHeaderTransformations,
                          registrationProcess.initializedUsingCentreOfGravity,
                          registrationProcess.verbose );

    registrationSubProcess.execute( registrationProcess.floatingName );

  }
  GKG_CATCH( "template < class T > "
             "void RegistrationProcess::match( gkg::Process& process, "
             "const std::string& referenceName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   Registration3dCommand
//

gkg::Registration3dCommand::Registration3dCommand( int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin,
                                                   bool removeFirst )
                           : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Registration3dCommand::Registration3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Registration3dCommand::Registration3dCommand(
                       const std::string& referenceName,
                       const std::string& floatingName,
                       const std::string& outputName,
                       const std::string& outputInverseName,
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       double referenceLowerThreshold,
                       double floatingLowerThreshold,
                       int32_t resamplingOrder,
                       const std::list< int32_t >& subSamplingMaximumSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       const std::vector< double >& optimizerParameters,
                       const std::string& transformType,
                       const std::vector< float >& initialParameters,
                       const std::string& initialTrmName,
                       const std::string& intermediateFileName,
                       bool ignoreHeaderTransformations,
                       bool initializedUsingCentreOfGravity,
                       bool verbose )
                           : gkg::Command()
{

  try
  {

    execute( referenceName,
             floatingName,
             outputName,
             outputInverseName,
             similarityMeasureName,
             optimizerName,
             referenceLowerThreshold,
             floatingLowerThreshold,
             resamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             transformType,
             initialParameters,
             initialTrmName,
             intermediateFileName,
             ignoreHeaderTransformations,
             initializedUsingCentreOfGravity,
             verbose );

  }
  GKG_CATCH( "gkg::Registration3dCommand::Registration3dCommand( "
             "const std::string& referenceName, "
             "const std::string& floatingName, "
             "const std::string& outputName, "
             "const std::string& outputInverseName, "
             "const std::string& similarityMeasureName, "
             "const std::string& optimizerName, "
             "double referenceLowerThreshold, "
             "double floatingLowerThreshold, "
             "int32_t resamplingOrder, "
             "const std::list< int32_t >& subSamplingMaximumSizes, "
             "const std::vector< double >& similarityMeasureParameters, "
             "const std::vector< double >& optimizerParameters, "
             "const std::string& transformType, "
             "const std::vector< float >& initialParameters, "
             "const std::string& initialTrmName, "
             "const std::string& intermediateFileName, "
             "bool ignoreHeaderTransformations, "
             "bool initializedUsingCentreOfGravity, "
             "bool verbose )" );

}


gkg::Registration3dCommand::Registration3dCommand(
                                             const gkg::Dictionary& parameters )
                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, referenceName );
    DECLARE_STRING_PARAMETER( parameters, std::string, floatingName );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputName );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputInverseName );
    DECLARE_STRING_PARAMETER( parameters, std::string, similarityMeasureName );
    DECLARE_STRING_PARAMETER( parameters, std::string, optimizerName );
    DECLARE_FLOATING_PARAMETER( parameters, double, referenceLowerThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, double, floatingLowerThreshold );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, resamplingOrder );
    DECLARE_LIST_OF_INTEGERS_PARAMETER( parameters,
                                        std::list< int32_t >,
                                        subSamplingMaximumSizes );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           similarityMeasureParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           optimizerParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, transformType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< float >,
                                           initialParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, initialTrmName );
    DECLARE_STRING_PARAMETER( parameters, std::string, intermediateFileName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ignoreHeaderTransformations );
    DECLARE_BOOLEAN_PARAMETER( parameters,
                               bool,
                               initializedUsingCentreOfGravity );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( referenceName,
             floatingName,
             outputName,
             outputInverseName,
             similarityMeasureName,
             optimizerName,
             referenceLowerThreshold,
             floatingLowerThreshold,
             resamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             transformType,
             initialParameters,
             initialTrmName,
             intermediateFileName,
             ignoreHeaderTransformations,
             initializedUsingCentreOfGravity,
             verbose );

  }
  GKG_CATCH( "gkg::Registration3dCommand::Registration3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Registration3dCommand::~Registration3dCommand()
{
}


std::string gkg::Registration3dCommand::getStaticName()
{

  try
  {

    return "Registration3d";

  }
  GKG_CATCH( "std::string gkg::Registration3dCommand::getStaticName()" );

}


void gkg::Registration3dCommand::parse()
{

  try
  {

    std::string referenceName;
    std::string floatingName;
    std::string outputName;
    std::string outputInverseName;
    std::string initialTrmName;
    std::string similarityMeasureName = "normalized-mutual-information";
    std::string optimizerName = "nelder-mead";
    double referenceLowerThreshold = 0;
    double floatingLowerThreshold = 0;
    int32_t resamplingOrder = 1;
    std::list< int32_t > subSamplingMaximumSizes;
    std::vector< double > similarityMeasureParameters;
    std::vector< double > optimizerParameters;
    std::string transformType = "rigid";
    std::vector< float > initialParameters;
    std::string intermediateFileName = "";
    bool ignoreHeaderTransformations = false;
    bool initializedUsingCentreOfGravity = true;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D registration tool",
                                  _loadPlugin );
    application.addSingleOption( "-reference",
                                 "Reference object",
                                 referenceName );
    application.addSingleOption( "-floating",
                                 "Floating object",
                                 floatingName );
    application.addSingleOption( "-o",
                                 "3D transform result with both reference"
                                 " and floating origins at the center"
                                 " of objects "
                                 "(default='<floating>_to_<reference>.trm')",
                                 outputName,
                                 true );
    application.addSingleOption( "-oi",
                                 "3D inverse transform result with both "
                                 "reference"
                                 " and floating origins at the center"
                                 " of objects "
                                 "(default='<reference>_to_<floating>.trm')",
                                 outputInverseName,
                                 true );
    application.addSingleOption( "-similarityMeasureName",
                                 "Similarity measure name: \n"
                                 "- correlation-coefficient \n"
                                 "- mutual-information \n"
                                 "- normalized-mutual-information \n"
                                 "- correlation-ratio \n"
                                 "(default=normalized-mutual-information)",
                                 similarityMeasureName,
                                 true );
    application.addSingleOption( "-optimizerName",
                                 "Optimizer name: \n"
                                 "- nelder-mead \n"
                                 "- powell-newuoa \n"
                                 "(default=nelder-mead)",
                                 optimizerName,
                                 true );
    application.addSingleOption( "-referenceLowerThreshold",
                                 "Reference lower threshold (default=0.0)",
                                 referenceLowerThreshold,
                                 true );
    application.addSingleOption( "-floatingLowerThreshold",
                                 "Floating lower threshold (default=0.0)",
                                 floatingLowerThreshold,
                                 true );
    application.addSingleOption( "-resamplingOrder",
                                 "Resampling order (0 to 7) (default=1)",
                                 resamplingOrder,
                                 true );
    application.addSeriesOption( "-subSamplingMaximumSizes",
                                 "Sub-sampling maximum sizes during "
                                 "optimization (default=64)",
                                 subSamplingMaximumSizes );
    application.addSeriesOption( "-similarityMeasureParameters",
                                 "Scalar parameters specific to a similarity "
                                 "measure as a vector of double "
                                 "<P1> <P2> <P3> ... <Pn>:\n\n"
                                 "- in case of mutual-information,\n"
                                 ".  <P1>: level count\n"
                                 ".  <P2>: apply smoothing\n"
                                 ".  (default= 32 / 1)\n\n"
                                 "- in case of normalized-mutual-information,\n"
                                 ".  <P1>: level count\n"
                                 ".  <P2>: apply smoothing\n"
                                 ".  (default= 32 / 1)\n\n"
                                 "- in case of correlation-ratio,\n"
                                 ".  <P1>: level count\n"
                                 ".  <P2>: apply smoothing\n"
                                 ".  (default= 32 / 1)\n",
                                 similarityMeasureParameters );
    application.addSeriesOption(
      "-optimizerParameters",
      "Scalar parameters specific to an optimizer "
      "as a vector of double "
      "<P1> <P2> <P3> ... <Pn>:\n\n"
      "-> in case of nelder-mead,\n\n"
      ".  <P1>: maximum iteration count per sub-sampling count (default=1000)\n"
      ".  <P2>: stopping criterion error (default=0.01)\n"
      ".  <P3..n>: variations of parameters\n\n "
      ".     - in case of rigid or rigid_and_diffeomorphic transform:\n "
      ".       <P3> - rotation around X (in degrees, default=10),\n"
      ".       <P4> - rotation around Y (in degrees, default=10),\n"
      ".       <P5> - rotation around Z (in degrees, default=10),\n"
      ".       <P6> - translation along X (in mm, default=10),\n"
      ".       <P7> - translation along Y (in mm, default=10),\n"
      ".       <P8> - translation along Z (in mm, default=10),\n\n"
      ".     - in case of affine_wo_shearing or "
      "affine_wo_shearing_and_diffeomorphic transform:\n\n "
      ".       <P3> - scaling along X (default=0.05),\n"
      ".       <P4> - scaling along Y (default=0.05),\n"
      ".       <P5> - scaling along Z (default=0.05),\n"
      ".       <P6> - rotation around X (in degrees, default=10),\n"
      ".       <P7> - rotation around Y (in degrees, default=10),\n"
      ".       <P8> - rotation around Z (in degrees, default=10),\n"
      ".       <P9> - translation along X (in mm, default=10),\n"
      ".       <P10> - translation along Y (in mm, default=10),\n"
      ".       <P11> - translation along Z (in mm, default=10),\n\n"
      ".     - in case of affine or affine_and_diffeomorphic transform:\n "
      ".       <P3> - scaling along X (default=0.05),\n"
      ".       <P4> - scaling along Y (default=0.05),\n"
      ".       <P5> - scaling along Z (default=0.05),\n"
      ".       <P6> - shearing along XY (default=0.05),\n"
      ".       <P7> - shearing along XZ (default=0.05),\n"
      ".       <P8> - shearing along YZ (default=0.05),\n"
      ".       <P9> - rotation around X (in degrees, default=10),\n"
      ".       <P10> - rotation around Y (in degrees, default=10),\n"
      ".       <P11> - rotation around Z (in degrees, default=10),\n"
      ".       <P12> - translation along X (in mm, default=10),\n"
      ".       <P13> - translation along Y (in mm, default=10),\n"
      ".       <P14> - translation along Z (in mm, default=10),\n\n"
      "-> in case of newuoa,\n\n"
      ".  <P1>: maximum iteration count per sub-sampling count (default=1000)\n"
      ".  <P2>: step size (default=0.1)\n"
      ".  <P3>: maximum test gradient (default=1000)\n"
      ".  <P4>: maximum tolerance (default=0.01)\n",
      optimizerParameters );
    application.addSingleOption( "-t",
                                 "3D transform type : \n"
                                 "- rigid\n"
                                 "- affine_wo_shearing\n"
                                 "- affine\n"
                                 "- rigid_and_diffeomorphic\n"
                                 "- affine_wo_shearing_and_diffeomorphic\n"
                                 "- affine_and_diffeomorphic\n"
                                 "- diffeomorphic\n"
                                 "(default=rigid)\n",
                                 transformType,
                                 true );
    application.addSeriesOption(
      "-initialParameters",
      "Initial transform coefficients "
      "(-initialTrm MUST not be set if this option is used)\n\n"
      "-> in case of rigid or rigid_and_diffeomorphic transform:\n"
      ". - rotation around X (in degrees, default=0 degree),\n"
      ". - rotation around Y (in degrees, default=0 degree),\n"
      ". - rotation around Z (in degrees, default=0 degree),\n"
      ". - translation along X (in mm, default=0),\n"
      ". - translation along Y (in mm, default=0),\n"
      ". - translation along Z (in mm, default=0),\n\n"
      "-> in case of affine_wo_shearing or "
      "affine_wo_shearing_and_diffeomorphic transform:\n"
      ". - scaling along X (default=1.0),\n"
      ". - scaling along Y (default=1.0),\n"
      ". - scaling along Z (default=1.0),\n"
      ". - rotation around X (in degrees, default=0 degree),\n"
      ". - rotation around Y (in degrees, default=0 degree),\n"
      ". - rotation around Z (in degrees, default=0 degree),\n"
      ". - translation along X (in mm, default=0),\n"
      ". - translation along Y (in mm, default=0),\n"
      ". - translation along Z (in mm, default=0),\n\n"
      "-> in case of affine or affine_and_diffeomorphic transform:\n"
      ". - scaling along X (default=1.0),\n"
      ". - scaling along Y (default=1.0),\n"
      ". - scaling along Z (default=1.0),\n"
      ". - shearing along XY (default=0.0),\n"
      ". - shearing along XZ (default=0.0),\n"
      ". - shearing along YZ (default=0.0),\n"
      ". - rotation around X (in degrees, default=0 degree),\n"
      ". - rotation around Y (in degrees, default=0 degree),\n"
      ". - rotation around Z (in degrees, default=0 degree),\n"
      ". - translation along X (in mm, default=0),\n"
      ". - translation along Y (in mm, default=0),\n"
      ". - translation along Z (in mm, default=0)\n\n"
      "-> in case of diffeomorphic transform:\n"
      ". N/A\n",
       initialParameters,
      0, 12 );
    application.addSingleOption( "-initialTrm",
                                 "Initial transformation .trm file "
                                 "(-initialParameters MUST not be set if this "
                                 "option is used)",
                                 initialTrmName,
                                 true );
    application.addSingleOption( "-intermediateFileName",
                                 "Intermediate result file name (default=\"\")",
                                 intermediateFileName,
                                 true );
    application.addSingleOption( "-ignoreHeaderTransformations",
                                 "Ignoring header transformations "
                                 "(default=false)",
                                 ignoreHeaderTransformations,
                                 true );
    application.addSingleOption( "-initializedUsingCentreOfGravity",
                                 "Initialize the 3D transform using a matching "
                                 "of the centre of gravity of the reference and"
                                 "floating volume(s) (default=true)",
                                 initializedUsingCentreOfGravity,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( referenceName,
             floatingName,
             outputName,
             outputInverseName,
             similarityMeasureName,
             optimizerName,
             referenceLowerThreshold,
             floatingLowerThreshold,
             resamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             transformType,
             initialParameters,
             initialTrmName,
             intermediateFileName,
             ignoreHeaderTransformations,
             initializedUsingCentreOfGravity,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Registration3dCommand::parse()" );

}


void gkg::Registration3dCommand::execute(
                  const std::string& referenceName,
                  const std::string& floatingName,
                  const std::string& outputName,
                  const std::string& outputInverseName,
                  const std::string& similarityMeasureName,
                  const std::string& optimizerName,
                  double referenceLowerThreshold ,
                  double floatingLowerThreshold,
                  int32_t resamplingOrder,
                  const std::list< int32_t >& subSamplingMaximumSizes,
                  const std::vector< double >& similarityMeasureParameters,
                  const std::vector< double >& optimizerParameters,
                  const std::string& transformType,
                  const std::vector< float >& initialParameters,
                  const std::string& initialTrmName,
                  const std::string& intermediateFileName,
                  bool ignoreHeaderTransformations,
                  bool initializedUsingCentreOfGravity,
                  bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( !initialTrmName.empty() && !initialParameters.empty() )
    {

      throw std::runtime_error( 
         "-initialParameters and -initialTrm options cannot be used together" );

    }

    std::string theOutputName = outputName;
    if ( outputName.empty() )
    {

      theOutputName = floatingName + "_to_" + referenceName + ".trm";

    }
    std::string theOutputInverseName = outputInverseName;
    if ( outputInverseName.empty() )
    {

      theOutputInverseName = referenceName + "_to_" + floatingName + ".trm";

    }
    std::list< int32_t > theSubSamplingMaximumSizes = subSamplingMaximumSizes;
    if ( subSamplingMaximumSizes.empty() )
    {

      theSubSamplingMaximumSizes.push_back( 64 );

    }

    if ( ( transformType != "rigid" ) &&
         ( transformType != "affine_wo_shearing" ) &&
         ( transformType != "affine" ) &&
         ( transformType != "rigid_and_diffeomorphic" ) &&
         ( transformType != "affine_wo_shearing_and_diffeomorphic" ) &&
         ( transformType != "affine_and_diffeomorphic" ) &&
         ( transformType != "diffeomorphic" ) )
    {

      throw std::runtime_error( "bad transform type" );

    }

    if ( ( transformType == "rigid" ) ||
         ( transformType == "rigid_and_diffeomorphic" ) )
    {

      if ( !initialParameters.empty() && ( initialParameters.size() != 6U ) )
      {

        throw std::runtime_error(
                            "option '-initialParameters': must have 6 values" );

      }

    }
    else if ( ( transformType == "affine_wo_shearing" ) ||
              ( transformType == "affine_wo_shearing_and_diffeomorphic" ) )
    {

      if ( !initialParameters.empty() && ( initialParameters.size() != 9U ) )
      {

        throw std::runtime_error(
                            "option '-initialParameters': must have 9 values" );

      }

    }
    else if ( ( transformType == "affine" ) ||
              ( transformType == "affine_and_diffeomorphic" ) )
    {

      if ( !initialParameters.empty() && ( initialParameters.size() != 12U ) )
      {

        throw std::runtime_error(
                           "option '-initialParameters': must have 12 values" );

      }

    }
    else if ( transformType == "diffeomorphic" )
    {

      if ( !initialParameters.empty() )
      {

        throw std::runtime_error(
                              "option '-initialParameters': must not be used" );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // read initial transformation file
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > initialTrmParameters;
    if ( initialTrmName.size() )
    {

      gkg::AffineTransform3d< float > initialTrm;
      std::ifstream ifs( initialTrmName.c_str() );

      if ( ifs.good() )
      {

        initialTrm.readTrm( ifs );
        ifs.close();
        initialTrm.getDirectParameters( initialTrmParameters );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // setting vectors of initial parameters and delta parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector vectorOfInitialParameters;
    std::vector< double > theOptimizerParameters = optimizerParameters;
    if ( transformType == "rigid" )
    {

      // initial parameter(s)
      vectorOfInitialParameters.reallocate( 6 );
      if ( initialParameters.empty() )
      {

        if ( initialTrmParameters.size() )
        {

          vectorOfInitialParameters( 0 ) = initialTrmParameters[ 6 ];
          vectorOfInitialParameters( 1 ) = initialTrmParameters[ 7 ];
          vectorOfInitialParameters( 2 ) = initialTrmParameters[ 8 ];
          vectorOfInitialParameters( 3 ) = initialTrmParameters[ 9 ];
          vectorOfInitialParameters( 4 ) = initialTrmParameters[ 10 ];
          vectorOfInitialParameters( 5 ) = initialTrmParameters[ 11 ];

        }
        else
        {

          vectorOfInitialParameters( 0 ) = 0.0;
          vectorOfInitialParameters( 1 ) = 0.0;
          vectorOfInitialParameters( 2 ) = 0.0;
          vectorOfInitialParameters( 3 ) = 0.0;
          vectorOfInitialParameters( 4 ) = 0.0;
          vectorOfInitialParameters( 5 ) = 0.0;

        }

      }
      else
      {

        vectorOfInitialParameters( 0 ) = initialParameters[ 0 ] * M_PI / 180.0;
        vectorOfInitialParameters( 1 ) = initialParameters[ 1 ] * M_PI / 180.0;
        vectorOfInitialParameters( 2 ) = initialParameters[ 2 ] * M_PI / 180.0;
        vectorOfInitialParameters( 3 ) = initialParameters[ 3 ];
        vectorOfInitialParameters( 4 ) = initialParameters[ 4 ];
        vectorOfInitialParameters( 5 ) = initialParameters[ 5 ];

      }

      // managing variation(s) of parameter(s)
      if ( optimizerName == "nelder-mead" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 2U + 6U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.01;

          theOptimizerParameters[ 2 + 0 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 1 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 2 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 3 ] = 10.0;
          theOptimizerParameters[ 2 + 4 ] = 10.0;
          theOptimizerParameters[ 2 + 5 ] = 10.0;

        }
        else
        {

          if ( theOptimizerParameters.size() != 8U )
          {

            throw std::runtime_error(
                             "nelder-mead optimizer must have 8 parameters for "
                             "rigid transform" );

          }

          theOptimizerParameters[ 2 + 0 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 1 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 2 ] *= M_PI / 180.0;

        }

      }
      else if ( optimizerName == "powell-newuoa" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 4U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.1;
          theOptimizerParameters[ 2 ] = 1000;
          theOptimizerParameters[ 3 ] = 0.01;

        }
        else
        {

          if ( theOptimizerParameters.size() != 4U )
          {

            throw std::runtime_error(
                        "powell-newuoa optimizer must have 4 parameters for "
                         "rigid transform" );

          }

        }

      }

    }
    else if ( transformType == "affine_wo_shearing" )
    {

      // initial parameter(s)
      vectorOfInitialParameters.reallocate( 9 );
      if ( initialParameters.empty() )
      {

        if ( initialTrmParameters.size() )
        {

          vectorOfInitialParameters( 0 ) = initialTrmParameters[ 0 ];
          vectorOfInitialParameters( 1 ) = initialTrmParameters[ 1 ];
          vectorOfInitialParameters( 2 ) = initialTrmParameters[ 2 ];
          vectorOfInitialParameters( 3 ) = initialTrmParameters[ 6 ];
          vectorOfInitialParameters( 4 ) = initialTrmParameters[ 7 ];
          vectorOfInitialParameters( 5 ) = initialTrmParameters[ 8 ];
          vectorOfInitialParameters( 6 ) = initialTrmParameters[ 9 ];
          vectorOfInitialParameters( 7 ) = initialTrmParameters[ 10 ];
          vectorOfInitialParameters( 8 ) = initialTrmParameters[ 11 ];

        }
        else
        {

          vectorOfInitialParameters( 0 ) = 1.0;
          vectorOfInitialParameters( 1 ) = 1.0;
          vectorOfInitialParameters( 2 ) = 1.0;
          vectorOfInitialParameters( 3 ) = 0.0;
          vectorOfInitialParameters( 4 ) = 0.0;
          vectorOfInitialParameters( 5 ) = 0.0;
          vectorOfInitialParameters( 6 ) = 0.0;
          vectorOfInitialParameters( 7 ) = 0.0;
          vectorOfInitialParameters( 8 ) = 0.0;

        }

      }
      else
      {

        vectorOfInitialParameters( 0 ) = initialParameters[ 0 ];
        vectorOfInitialParameters( 1 ) = initialParameters[ 1 ];
        vectorOfInitialParameters( 2 ) = initialParameters[ 2 ];
        vectorOfInitialParameters( 3 ) = initialParameters[ 3 ] * M_PI / 180.0;
        vectorOfInitialParameters( 4 ) = initialParameters[ 4 ] * M_PI / 180.0;
        vectorOfInitialParameters( 5 ) = initialParameters[ 5 ] * M_PI / 180.0;
        vectorOfInitialParameters( 6 ) = initialParameters[ 6 ];
        vectorOfInitialParameters( 7 ) = initialParameters[ 7 ];
        vectorOfInitialParameters( 8 ) = initialParameters[ 8 ];

      }

      // managing variation(s) of parameter(s)
      if ( optimizerName == "nelder-mead" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 2U + 9U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.01;

          theOptimizerParameters[ 2 + 0 ] = 0.05;
          theOptimizerParameters[ 2 + 1 ] = 0.05;
          theOptimizerParameters[ 2 + 2 ] = 0.05;
          theOptimizerParameters[ 2 + 3 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 4 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 5 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 6 ] = 10.0;
          theOptimizerParameters[ 2 + 7 ] = 10.0;
          theOptimizerParameters[ 2 + 8 ] = 10.0;

        }
        else
        {

          if ( theOptimizerParameters.size() != 11U )
          {

            throw std::runtime_error(
                            "nelder-mead optimizer must have 11 parameters for "
                            "affine_wo_shearing transform" );

          }

          theOptimizerParameters[ 2 + 3 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 4 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 5 ] *= M_PI / 180.0;

        }

      }
      else if ( optimizerName == "powell-newuoa" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 4U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.1;
          theOptimizerParameters[ 2 ] = 1000;
          theOptimizerParameters[ 3 ] = 0.01;

        }
        else
        {

          if ( theOptimizerParameters.size() != 4U )
          {

            throw std::runtime_error(
                         "powell-newuoa optimizer must have 4 parameters for "
                         "affine_wo_shearing transform" );

          }

        }

      }

    }
    else if ( transformType == "affine" )
    {

      // initial parameter(s)
      vectorOfInitialParameters.reallocate( 12 );
      if ( initialParameters.empty() )
      {

        if ( initialTrmParameters.size() )
        {

          vectorOfInitialParameters( 0 ) = initialTrmParameters[ 0 ];
          vectorOfInitialParameters( 1 ) = initialTrmParameters[ 1 ];
          vectorOfInitialParameters( 2 ) = initialTrmParameters[ 2 ];
          vectorOfInitialParameters( 3 ) = initialTrmParameters[ 3 ];
          vectorOfInitialParameters( 4 ) = initialTrmParameters[ 4 ];
          vectorOfInitialParameters( 5 ) = initialTrmParameters[ 5 ];
          vectorOfInitialParameters( 6 ) = initialTrmParameters[ 6 ];
          vectorOfInitialParameters( 7 ) = initialTrmParameters[ 7 ];
          vectorOfInitialParameters( 8 ) = initialTrmParameters[ 8 ];
          vectorOfInitialParameters( 9 ) = initialTrmParameters[ 9 ];
          vectorOfInitialParameters( 10 ) = initialTrmParameters[ 10 ];
          vectorOfInitialParameters( 11 ) = initialTrmParameters[ 11 ];

        }
        else
        {

          vectorOfInitialParameters( 0 ) = 1.0;
          vectorOfInitialParameters( 1 ) = 1.0;
          vectorOfInitialParameters( 2 ) = 1.0;
          vectorOfInitialParameters( 3 ) = 0.0;
          vectorOfInitialParameters( 4 ) = 0.0;
          vectorOfInitialParameters( 5 ) = 0.0;
          vectorOfInitialParameters( 6 ) = 0.0;
          vectorOfInitialParameters( 7 ) = 0.0;
          vectorOfInitialParameters( 8 ) = 0.0;
          vectorOfInitialParameters( 9 ) = 0.0;
          vectorOfInitialParameters( 10 ) = 0.0;
          vectorOfInitialParameters( 11 ) = 0.0;

        }

      }
      else
      {

        vectorOfInitialParameters( 0 ) = initialParameters[ 0 ];
        vectorOfInitialParameters( 1 ) = initialParameters[ 1 ];
        vectorOfInitialParameters( 2 ) = initialParameters[ 2 ];
        vectorOfInitialParameters( 3 ) = initialParameters[ 3 ];
        vectorOfInitialParameters( 4 ) = initialParameters[ 4 ];
        vectorOfInitialParameters( 5 ) = initialParameters[ 5 ];
        vectorOfInitialParameters( 6 ) = initialParameters[ 6 ] * M_PI / 180.0;
        vectorOfInitialParameters( 7 ) = initialParameters[ 7 ] * M_PI / 180.0;
        vectorOfInitialParameters( 8 ) = initialParameters[ 8 ] * M_PI / 180.0;
        vectorOfInitialParameters( 9 ) = initialParameters[ 9 ];
        vectorOfInitialParameters( 10 ) = initialParameters[ 10 ];
        vectorOfInitialParameters( 11 ) = initialParameters[ 11 ];

      }

      // managing variation(s) of parameter(s)
      if ( optimizerName == "nelder-mead" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 2U + 12U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.01;

          theOptimizerParameters[ 2 + 0 ] = 0.05;
          theOptimizerParameters[ 2 + 1 ] = 0.05;
          theOptimizerParameters[ 2 + 2 ] = 0.05;
          theOptimizerParameters[ 2 + 3 ] = 0.05;
          theOptimizerParameters[ 2 + 4 ] = 0.05;
          theOptimizerParameters[ 2 + 5 ] = 0.05;
          theOptimizerParameters[ 2 + 6 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 7 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 8 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 9 ] = 10.0;
          theOptimizerParameters[ 2 + 10 ] = 10.0;
          theOptimizerParameters[ 2 + 11 ] = 10.0;

        }
        else
        {

          if ( theOptimizerParameters.size() != 14U )
          {

            throw std::runtime_error(
                            "nelder-mead optimizer must have 14 parameters for "
                            "affine transform" );

          }

          theOptimizerParameters[ 2 + 6 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 7 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 8 ] *= M_PI / 180.0;

        }

      }
      else if ( optimizerName == "powell-newuoa" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 4U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.1;
          theOptimizerParameters[ 2 ] = 1000;
          theOptimizerParameters[ 3 ] = 0.01;

        }
        else
        {

          if ( theOptimizerParameters.size() != 4U )
          {

            throw std::runtime_error(
                         "powell-newuoa optimizer must have 4 parameters for "
                         "affine transform" );

          }

        }

      }

    }
    else if ( transformType == "rigid_and_diffeomorphic" )
    {

      // initial parameter(s)
      vectorOfInitialParameters.reallocate( 6 );
      if ( initialParameters.empty() )
      {

        if ( initialTrmParameters.size() )
        {

          vectorOfInitialParameters( 0 ) = initialTrmParameters[ 6 ];
          vectorOfInitialParameters( 1 ) = initialTrmParameters[ 7 ];
          vectorOfInitialParameters( 2 ) = initialTrmParameters[ 8 ];
          vectorOfInitialParameters( 3 ) = initialTrmParameters[ 9 ];
          vectorOfInitialParameters( 4 ) = initialTrmParameters[ 10 ];
          vectorOfInitialParameters( 5 ) = initialTrmParameters[ 11 ];

        }
        else
        {

          vectorOfInitialParameters( 0 ) = 0.0;
          vectorOfInitialParameters( 1 ) = 0.0;
          vectorOfInitialParameters( 2 ) = 0.0;
          vectorOfInitialParameters( 3 ) = 0.0;
          vectorOfInitialParameters( 4 ) = 0.0;
          vectorOfInitialParameters( 5 ) = 0.0;

        }

      }
      else
      {

        vectorOfInitialParameters( 0 ) = initialParameters[ 0 ] * M_PI / 180.0;
        vectorOfInitialParameters( 1 ) = initialParameters[ 1 ] * M_PI / 180.0;
        vectorOfInitialParameters( 2 ) = initialParameters[ 2 ] * M_PI / 180.0;
        vectorOfInitialParameters( 3 ) = initialParameters[ 3 ];
        vectorOfInitialParameters( 4 ) = initialParameters[ 4 ];
        vectorOfInitialParameters( 5 ) = initialParameters[ 5 ];

      }

      // managing variation(s) of parameter(s)
      if ( optimizerName == "nelder-mead" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 2U + 6U + 5U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.01;

          theOptimizerParameters[ 2 + 0 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 1 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 2 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 3 ] = 10.0;
          theOptimizerParameters[ 2 + 4 ] = 10.0;
          theOptimizerParameters[ 2 + 5 ] = 10.0;

          theOptimizerParameters[ 8 + 0 ] = 1.0; // diffeo opt config count
          theOptimizerParameters[ 8 + 1 ] = 1.0; // half width search spaces
          theOptimizerParameters[ 8 + 2 ] = 1.0; // control point spacings
          theOptimizerParameters[ 8 + 3 ] = 1.0; // search space quantisations
          theOptimizerParameters[ 8 + 4 ] = 5.0; // regularization weight

        }
        else
        {

          if ( theOptimizerParameters.size() < 13U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have at least 13 parameters for "
                   "rigid_and_diffeomorphic transform" );

          }

          if ( ( theOptimizerParameters.size() - 13U ) % 4U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have a number of parameters "
                   "equal to 9+k*4 where k is the number of diffeomorphic "
                   "optimization configurations for the "
                   "rigid_and_diffeomorphic transform" );

          }

          theOptimizerParameters[ 2 + 0 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 1 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 2 ] *= M_PI / 180.0;

        }

      }
      else if ( optimizerName == "powell-newuoa" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 4U + 5U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.1;
          theOptimizerParameters[ 2 ] = 1000;
          theOptimizerParameters[ 3 ] = 0.01;

          theOptimizerParameters[ 4 + 0 ] = 1.0; // diffeo opt config count
          theOptimizerParameters[ 4 + 1 ] = 1.0; // half width search spaces
          theOptimizerParameters[ 4 + 2 ] = 1.0; // control point spacings
          theOptimizerParameters[ 4 + 3 ] = 1.0; // search space quantisations
          theOptimizerParameters[ 4 + 4 ] = 5.0; // regularization weight

        }
        else
        {

          if ( theOptimizerParameters.size() < 9U )
          {

            throw std::runtime_error(
                  "powell-newuoa optimizer must have at least 9 parameters for "
                  "rigid_and_diffeomorphic transform" );

          }

          if ( ( theOptimizerParameters.size() - 9U ) % 4U )
          {

            throw std::runtime_error(
                   "powell-newuoad optimizer must have a number of parameters "
                   "equal to 5+k*4 where k is the number of diffeomorphic "
                   "optimization configurations for the "
                   "rigid_and_diffeomorphic transform" );

          }

        }

      }

    }
    else if ( transformType == "affine_wo_shearing_and_diffeomorphic" )
    {

      // initial parameter(s)
      vectorOfInitialParameters.reallocate( 9 );
      if ( initialParameters.empty() )
      {

        if ( initialTrmParameters.size() )
        {

          vectorOfInitialParameters( 0 ) = initialTrmParameters[ 0 ];
          vectorOfInitialParameters( 1 ) = initialTrmParameters[ 1 ];
          vectorOfInitialParameters( 2 ) = initialTrmParameters[ 2 ];
          vectorOfInitialParameters( 3 ) = initialTrmParameters[ 6 ];
          vectorOfInitialParameters( 4 ) = initialTrmParameters[ 7 ];
          vectorOfInitialParameters( 5 ) = initialTrmParameters[ 8 ];
          vectorOfInitialParameters( 6 ) = initialTrmParameters[ 9 ];
          vectorOfInitialParameters( 7 ) = initialTrmParameters[ 10 ];
          vectorOfInitialParameters( 8 ) = initialTrmParameters[ 11 ];

        }
        else
        {

          vectorOfInitialParameters( 0 ) = 1.0;
          vectorOfInitialParameters( 1 ) = 1.0;
          vectorOfInitialParameters( 2 ) = 1.0;
          vectorOfInitialParameters( 3 ) = 0.0;
          vectorOfInitialParameters( 4 ) = 0.0;
          vectorOfInitialParameters( 5 ) = 0.0;
          vectorOfInitialParameters( 6 ) = 0.0;
          vectorOfInitialParameters( 7 ) = 0.0;
          vectorOfInitialParameters( 8 ) = 0.0;

        }

      }
      else
      {

        vectorOfInitialParameters( 0 ) = initialParameters[ 0 ];
        vectorOfInitialParameters( 1 ) = initialParameters[ 1 ];
        vectorOfInitialParameters( 2 ) = initialParameters[ 2 ];
        vectorOfInitialParameters( 3 ) = initialParameters[ 3 ] * M_PI / 180.0;
        vectorOfInitialParameters( 4 ) = initialParameters[ 4 ] * M_PI / 180.0;
        vectorOfInitialParameters( 5 ) = initialParameters[ 5 ] * M_PI / 180.0;
        vectorOfInitialParameters( 6 ) = initialParameters[ 6 ];
        vectorOfInitialParameters( 7 ) = initialParameters[ 7 ];
        vectorOfInitialParameters( 8 ) = initialParameters[ 8 ];

      }

      // managing variation(s) of parameter(s)
      if ( optimizerName == "nelder-mead" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 2U + 9U + 5U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.01;

          theOptimizerParameters[ 2 + 0 ] = 0.05;
          theOptimizerParameters[ 2 + 1 ] = 0.05;
          theOptimizerParameters[ 2 + 2 ] = 0.05;
          theOptimizerParameters[ 2 + 3 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 4 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 5 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 6 ] = 10.0;
          theOptimizerParameters[ 2 + 7 ] = 10.0;
          theOptimizerParameters[ 2 + 8 ] = 10.0;

          theOptimizerParameters[ 11 + 0 ] = 1.0; // diffeo opt config count
          theOptimizerParameters[ 11 + 1 ] = 1.0; // half width search spaces
          theOptimizerParameters[ 11 + 2 ] = 1.0; // control point spacings
          theOptimizerParameters[ 11 + 3 ] = 1.0; // search space quantisations
          theOptimizerParameters[ 11 + 4 ] = 5.0; // regularization weight

        }
        else
        {

          if ( theOptimizerParameters.size() < 16U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have at least 16 parameters for "
                   "affine_wo_shearing_and_diffeomorphic transform" );

          }

          if ( ( theOptimizerParameters.size() - 16U ) % 4U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have a number of parameters "
                   "equal to 12+k*4 where k is the number of diffeomorphic "
                   "optimization configurations for the "
                   "affine_wo_shearing_and_diffeomorphic transform" );

          }

          theOptimizerParameters[ 2 + 3 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 4 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 5 ] *= M_PI / 180.0;

        }

      }
      else if ( optimizerName == "powell-newuoa" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 4U + 5U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.1;
          theOptimizerParameters[ 2 ] = 1000;
          theOptimizerParameters[ 3 ] = 0.01;

          theOptimizerParameters[ 4 + 0 ] = 1.0; // diffeo opt config count
          theOptimizerParameters[ 4 + 1 ] = 1.0; // half width search spaces
          theOptimizerParameters[ 4 + 2 ] = 1.0; // control point spacings
          theOptimizerParameters[ 4 + 3 ] = 1.0; // search space quantisations
          theOptimizerParameters[ 4 + 4 ] = 5.0; // regularization weight

        }
        else
        {

          if ( theOptimizerParameters.size() < 9U )
          {

            throw std::runtime_error(
                  "powell-newuoa optimizer must have at least 9 parameters for "
                  "affine_wo_shearing_and_diffeomorphic transform" );

          }

        }

      }

    }
    else if ( transformType == "affine_and_diffeomorphic" )
    {

      // initial parameter(s)
      vectorOfInitialParameters.reallocate( 12 );
      if ( initialParameters.empty() )
      {

        if ( initialTrmParameters.size() )
        {

          vectorOfInitialParameters( 0 ) = initialTrmParameters[ 0 ];
          vectorOfInitialParameters( 1 ) = initialTrmParameters[ 1 ];
          vectorOfInitialParameters( 2 ) = initialTrmParameters[ 2 ];
          vectorOfInitialParameters( 3 ) = initialTrmParameters[ 3 ];
          vectorOfInitialParameters( 4 ) = initialTrmParameters[ 4 ];
          vectorOfInitialParameters( 5 ) = initialTrmParameters[ 5 ];
          vectorOfInitialParameters( 6 ) = initialTrmParameters[ 6 ];
          vectorOfInitialParameters( 7 ) = initialTrmParameters[ 7 ];
          vectorOfInitialParameters( 8 ) = initialTrmParameters[ 8 ];
          vectorOfInitialParameters( 9 ) = initialTrmParameters[ 9 ];
          vectorOfInitialParameters( 10 ) = initialTrmParameters[ 10 ];
          vectorOfInitialParameters( 11 ) = initialTrmParameters[ 11 ];

        }
        else
        {

          vectorOfInitialParameters( 0 ) = 1.0;
          vectorOfInitialParameters( 1 ) = 1.0;
          vectorOfInitialParameters( 2 ) = 1.0;
          vectorOfInitialParameters( 3 ) = 0.0;
          vectorOfInitialParameters( 4 ) = 0.0;
          vectorOfInitialParameters( 5 ) = 0.0;
          vectorOfInitialParameters( 6 ) = 0.0;
          vectorOfInitialParameters( 7 ) = 0.0;
          vectorOfInitialParameters( 8 ) = 0.0;
          vectorOfInitialParameters( 9 ) = 0.0;
          vectorOfInitialParameters( 10 ) = 0.0;
          vectorOfInitialParameters( 11 ) = 0.0;

        }

      }
      else
      {

        vectorOfInitialParameters( 0 ) = initialParameters[ 0 ];
        vectorOfInitialParameters( 1 ) = initialParameters[ 1 ];
        vectorOfInitialParameters( 2 ) = initialParameters[ 2 ];
        vectorOfInitialParameters( 3 ) = initialParameters[ 3 ];
        vectorOfInitialParameters( 4 ) = initialParameters[ 4 ];
        vectorOfInitialParameters( 5 ) = initialParameters[ 5 ];
        vectorOfInitialParameters( 6 ) = initialParameters[ 6 ] * M_PI / 180.0;
        vectorOfInitialParameters( 7 ) = initialParameters[ 7 ] * M_PI / 180.0;
        vectorOfInitialParameters( 8 ) = initialParameters[ 8 ] * M_PI / 180.0;
        vectorOfInitialParameters( 9 ) = initialParameters[ 9 ];
        vectorOfInitialParameters( 10 ) = initialParameters[ 10 ];
        vectorOfInitialParameters( 11 ) = initialParameters[ 11 ];

      }

      // managing variation(s) of parameter(s)
      if ( optimizerName == "nelder-mead" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 2U + 12U + 5U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.01;

          theOptimizerParameters[ 2 + 0 ] = 0.05;
          theOptimizerParameters[ 2 + 1 ] = 0.05;
          theOptimizerParameters[ 2 + 2 ] = 0.05;
          theOptimizerParameters[ 2 + 3 ] = 0.05;
          theOptimizerParameters[ 2 + 4 ] = 0.05;
          theOptimizerParameters[ 2 + 5 ] = 0.05;
          theOptimizerParameters[ 2 + 6 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 7 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 8 ] = M_PI / 18;
          theOptimizerParameters[ 2 + 9 ] = 10.0;
          theOptimizerParameters[ 2 + 10 ] = 10.0;
          theOptimizerParameters[ 2 + 11 ] = 10.0;

          theOptimizerParameters[ 14 + 0 ] = 1.0; // diffeo opt config count
          theOptimizerParameters[ 14 + 1 ] = 1.0; // half width search spaces
          theOptimizerParameters[ 14 + 2 ] = 1.0; // control point spacings
          theOptimizerParameters[ 14 + 3 ] = 1.0; // search space quantisations
          theOptimizerParameters[ 14 + 4 ] = 5.0; // regularization weight

        }
        else
        {

          if ( theOptimizerParameters.size() < 19U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have at least 19 parameters for "
                   "affine_and_diffeomorphic transform" );

          }

          if ( ( theOptimizerParameters.size() - 19U ) % 4U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have a number of parameters "
                   "equal to 15+k*4 where k is the number of diffeomorphic "
                   "optimization configurations for the "
                   "affine_and_diffeomorphic transform" );

          }

          theOptimizerParameters[ 2 + 6 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 7 ] *= M_PI / 180.0;
          theOptimizerParameters[ 2 + 8 ] *= M_PI / 180.0;

        }

      }
      else if ( optimizerName == "powell-newuoa" )
      {

        if ( theOptimizerParameters.empty() )
        {

          theOptimizerParameters.resize( 4U + 5U );
          theOptimizerParameters[ 0 ] = 1000;
          theOptimizerParameters[ 1 ] = 0.1;
          theOptimizerParameters[ 2 ] = 1000;
          theOptimizerParameters[ 3 ] = 0.01;

          theOptimizerParameters[ 4 + 0 ] = 1.0; // diffeo opt config count
          theOptimizerParameters[ 4 + 1 ] = 1.0; // half width search spaces
          theOptimizerParameters[ 4 + 2 ] = 1.0; // control point spacings
          theOptimizerParameters[ 4 + 3 ] = 1.0; // search space quantisations
          theOptimizerParameters[ 4 + 4 ] = 5.0; // regularization weight

        }
        else
        {

          if ( theOptimizerParameters.size() < 9U )
          {

            throw std::runtime_error(
                  "powell-newuoa optimizer must have at least 9 parameters for "
                  "affine_and_diffeomorphic transform" );

          }

        }

      }

    }
    else if ( transformType == "diffeomorphic" )
    {

      if ( theOptimizerParameters.empty() )
      {

        theOptimizerParameters.resize( 5U );
        theOptimizerParameters[ 0 ] = 1.0; // diffeo opt config count
        theOptimizerParameters[ 1 ] = 1.0; // half width search spaces
        theOptimizerParameters[ 2 ] = 1.0; // control point spacings
        theOptimizerParameters[ 3 ] = 1.0; // search space quantisations
        theOptimizerParameters[ 4 ] = 5.0; // regularization weight

      }
      else
      {

        if ( theOptimizerParameters.size() < 5U )
        {

          throw std::runtime_error(
                 "dynamic programming optimizer must have at least 5 parameters"
                 " for diffeomorphic transform" );

        }

        if ( ( theOptimizerParameters.size() - 5U ) % 4U )
        {

          throw std::runtime_error(
                 "dynamic programming optimizer must have a number of "
                 "parameters equal to 1+k*4 where k is the number of "
                 "diffeomorphic optimization configurations for the "
                 "diffeomorphic transform" );

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // initializing linear 3D transform
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

      std::cout << "initializing linear parameterized 3D transform : "
                << std::flush;

    }

    gkg::RCPointer< gkg::ParameterizedRigidTransform3d< float > >
      rigidTransform3d;
    gkg::RCPointer< gkg::ParameterizedAffineWoShearingTransform3d< float > >
      affineWoShearingTransform3d;
    gkg::RCPointer< gkg::ParameterizedAffineTransform3d< float > >
      affineTransform3d;

    if ( ( transformType == "rigid" ) ||
         ( transformType == "rigid_and_diffeomorphic" ) )
    {

      rigidTransform3d.reset(
                              new gkg::ParameterizedRigidTransform3d< float > );

    }
    else if ( ( transformType == "affine_wo_shearing" ) ||
              ( transformType == "affine_wo_shearing_and_diffeomorphic" ) )
    {

      affineWoShearingTransform3d.reset(
                   new gkg::ParameterizedAffineWoShearingTransform3d< float > );

    }
    else if ( ( transformType == "affine" ) ||
              ( transformType == "affine_and_diffeomorphic" ) )
    {

      affineTransform3d.reset(
                             new gkg::ParameterizedAffineTransform3d< float > );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // initializing vector field 3D transform
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "initializing vector field 3D transform : "
                << std::flush;

    }
    gkg::RCPointer< gkg::VectorFieldTransform3d< float > >
      vectorFieldTransform3d;

    if ( ( transformType == "rigid_and_diffeomorphic" ) ||
         ( transformType == "affine_wo_shearing_and_diffeomorphic" ) ||
         ( transformType == "affine_and_diffeomorphic" ) ||
         ( transformType == "diffeomorphic" ) )
    {

      gkg::Volume< float > directVectorField( 1, 1, 1, 3 );
      gkg::Volume< float > inverseVectorField( 1, 1, 1, 3 );
      vectorFieldTransform3d.reset( new gkg::VectorFieldTransform3d< float >(
                                                         directVectorField,
                                                         inverseVectorField ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // initializing non-linear 3D transform
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "initializing non-linear 3D transform : "
                << std::flush;

    }


    gkg::RCPointer< gkg::NonLinearTransform3d< float > > nonLinearTransform3d;

    if ( ( transformType == "rigid" ) ||
         ( transformType == "rigid_and_diffeomorphic" ) )
    {

      nonLinearTransform3d.reset(
        new gkg::NonLinearTransform3d< float >( rigidTransform3d,
                                                vectorFieldTransform3d ) );

    }
    else if ( ( transformType == "affine_wo_shearing" ) ||
              ( transformType == "affine_wo_shearing_and_diffeomorphic" ) )
    {

      nonLinearTransform3d.reset(
        new gkg::NonLinearTransform3d< float >( affineWoShearingTransform3d,
                                                vectorFieldTransform3d ) );

    }
    else if ( ( transformType == "affine" ) ||
              ( transformType == "affine_and_diffeomorphic" ) )
    {

      nonLinearTransform3d.reset(
        new gkg::NonLinearTransform3d< float >( affineTransform3d,
                                                vectorFieldTransform3d ) );

    }
    else if ( transformType == "diffeomorphic" )
    {

      nonLinearTransform3d.reset(
        new gkg::NonLinearTransform3d< float >( vectorFieldTransform3d ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // launching registration process
    ////////////////////////////////////////////////////////////////////////////
    RegistrationProcess registrationProcess( floatingName,
                                             theOutputName,
                                             theOutputInverseName,
                                             similarityMeasureName,
                                             optimizerName,
                                             resamplingOrder,
                                             theSubSamplingMaximumSizes,
                                             similarityMeasureParameters,
                                             theOptimizerParameters,
                                             vectorOfInitialParameters,
                                             referenceLowerThreshold,
                                             floatingLowerThreshold,
                                             nonLinearTransform3d,
                                             intermediateFileName,
                                             ignoreHeaderTransformations,
                                             initializedUsingCentreOfGravity,
                                             verbose );
    registrationProcess.execute( referenceName );

  }
  GKG_CATCH( "void gkg::Registration3dCommand::execute( "
             "const std::string& referenceName, "
             "const std::string& floatingName, "
             "const std::string& outputName, "
             "const std::string& outputInverseName, "
             "const std::string& similarityMeasureName, "
             "const std::string& optimizerName, "
             "double referenceLowerThreshold , "
             "double floatingLowerThreshold, "
             "int32_t resamplingOrder, "
             "const std::list< int32_t >& subSamplingMaximumSizes, "
             "const std::vector< double >& similarityMeasureParameters, "
             "const std::vector< double >& optimizerParameters, "
             "const std::string& transformType, "
             "const std::vector< float >& initialParameters, "
             "const std::string& initialTrmName, "
             "const std::string& intermediateFileName, "
             "bool ignoreHeaderTransformations, "
             "bool initializedUsingCentreOfGravity, "
             "bool verbose )" );

}


RegisterCommandCreator(
       Registration3dCommand,
       DECLARE_STRING_PARAMETER_HELP( referenceName ) +
       DECLARE_STRING_PARAMETER_HELP( floatingName ) +
       DECLARE_STRING_PARAMETER_HELP( outputName ) +
       DECLARE_STRING_PARAMETER_HELP( outputInverseName ) +
       DECLARE_STRING_PARAMETER_HELP( similarityMeasureName ) +
       DECLARE_STRING_PARAMETER_HELP( optimizerName ) +
       DECLARE_FLOATING_PARAMETER_HELP( referenceLowerThreshold ) +
       DECLARE_FLOATING_PARAMETER_HELP( floatingLowerThreshold ) +
       DECLARE_INTEGER_PARAMETER_HELP( resamplingOrder ) +
       DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( subSamplingMaximumSizes ) +
       DECLARE_LIST_OF_FLOATINGS_PARAMETER_HELP( similarityMeasureParameters ) +
       DECLARE_LIST_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters );
       DECLARE_STRING_PARAMETER_HELP( transformType ) +
       DECLARE_LIST_OF_FLOATINGS_PARAMETER_HELP( initialParameters ) +
       DECLARE_STRING_PARAMETER_HELP( initialTrmName ) +
       DECLARE_STRING_PARAMETER_HELP( intermediateFileName ) +
       DECLARE_BOOLEAN_PARAMETER_HELP( ignoreHeaderTransformations ) +
       DECLARE_BOOLEAN_PARAMETER_HELP( initializedUsingCentreOfGravity ) +
       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
