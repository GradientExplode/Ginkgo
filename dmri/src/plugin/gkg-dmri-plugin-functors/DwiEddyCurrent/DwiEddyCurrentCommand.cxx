#include <gkg-dmri-plugin-functors/DwiEddyCurrent/DwiEddyCurrentCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/Shearing3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-registration/MotionCorrection_i.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


//
// class EddyCurrentProcess
//

class EddyCurrentProcess : public gkg::Process
{

  public:

    EddyCurrentProcess(
     const int32_t& theReferenceRank,
     const std::string& theReferenceName,
     const std::string& theOutputName,
     const std::string& theTransform3dName,
     const std::string& theSimilarityMeasureName,
     const std::string& theOptimizerName,
     const double& theLowerThreshold,
     const int32_t& theRegistrationResamplingOrder,
     const std::list< int32_t >& theSubSamplingMaximumSizes,
     const std::vector< double >& theSimilarityMeasureParameters,
     const std::vector< double >& theOptimizerParameters,
     gkg::Vector& theVectorOfInitialParameters,
     const int32_t& theOutputResamplingOrder,
     const double& theBackground,
     const std::string& theFurtherMotionName,
     const gkg::Vector3d< int32_t >& theFurtherSliceCount,
     const bool& theCorrectQSpaceSampling,
     const std::string& theEddyCurrentProfileName,
     const bool& theVerboseRegistration,
     const bool& theVerbose );

     const int32_t& referenceRank;
     const std::string& referenceName;
     const std::string& outputName;
     const std::string& transform3dName;
     const std::string& similarityMeasureName;
     const std::string& optimizerName;
     const double& lowerThreshold;
     const int32_t& registrationResamplingOrder;
     const std::list< int32_t >& subSamplingMaximumSizes;
     const std::vector< double >& similarityMeasureParameters;
     const std::vector< double >& optimizerParameters;
     gkg::Vector& vectorOfInitialParameters;
     const int32_t& outputResamplingOrder;
     const double& background;
     const std::string& furtherMotionName;
     const gkg::Vector3d< int32_t >& furtherSliceCount;
     const bool& correctQSpaceSampling;
     const std::string& eddyCurrentProfileName;
     const bool& verboseRegistration;
     const bool& verbose;


  private:

    template < class T >
    static void correct( gkg::Process& process,
                         const std::string& inputName,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


EddyCurrentProcess::EddyCurrentProcess(
     const int32_t& theReferenceRank,
     const std::string& theReferenceName,
     const std::string& theOutputName,
     const std::string& theTransform3dName,
     const std::string& theSimilarityMeasureName,
     const std::string& theOptimizerName,
     const double& theLowerThreshold,
     const int32_t& theRegistrationResamplingOrder,
     const std::list< int32_t >& theSubSamplingMaximumSizes,
     const std::vector< double >& theSimilarityMeasureParameters,
     const std::vector< double >& theOptimizerParameters,
     gkg::Vector& theVectorOfInitialParameters,
     const int32_t& theOutputResamplingOrder,
     const double& theBackground,
     const std::string& theFurtherMotionName,
     const gkg::Vector3d< int32_t >& theFurtherSliceCount,
     const bool& theCorrectQSpaceSampling,
     const std::string& theEddyCurrentProfileName,
     const bool& theVerboseRegistration,
     const bool& theVerbose )
                   : gkg::Process( "Volume" ),
                     referenceRank( theReferenceRank ),
                     referenceName( theReferenceName ),
                     outputName( theOutputName ),
                     transform3dName( theTransform3dName ),
                     similarityMeasureName( theSimilarityMeasureName ),
                     optimizerName( theOptimizerName ),
                     lowerThreshold( theLowerThreshold ),
                     registrationResamplingOrder( 
                       theRegistrationResamplingOrder ),
                     subSamplingMaximumSizes( theSubSamplingMaximumSizes ),
                     similarityMeasureParameters(
                                               theSimilarityMeasureParameters ),
                     optimizerParameters( theOptimizerParameters ),
                     vectorOfInitialParameters(
                       theVectorOfInitialParameters ),
                     outputResamplingOrder( theOutputResamplingOrder ),
                     background( theBackground ),
                     furtherMotionName( theFurtherMotionName ),
                     furtherSliceCount( theFurtherSliceCount ),
                     correctQSpaceSampling( theCorrectQSpaceSampling ),
                     eddyCurrentProfileName( theEddyCurrentProfileName ),
                     verboseRegistration( theVerboseRegistration ),
                     verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &correct< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &correct< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &correct< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &correct< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &correct< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &correct< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &correct< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &correct< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &correct< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &correct< double > );

  }
  GKG_CATCH( "EddyCurrentProcess::EddyCurrentProcess( "
             "const int32_t& theReferenceRank, "
             "const std::string& theReferenceName, "
             "const std::string& theOutputName, "
             "const std::string& theTransform3dName, "
             "const std::string& theSimilarityMeasureName, "
             "const std::string& theOptimizerName, "
             "const double& theLowerThreshold, "
             "const int32_t& theRegistrationResamplingOrder, "
             "const std::list< int32_t >& theSubSamplingMaximumSizes, "
             "const std::vector< double >& theSimilarityMeasureParameters, "
             "const std::vector< double >& theOptimizerParameters, "
             "gkg::Vector& theVectorOfInitialParameters, "
             "const int32_t& theOutputResamplingOrder, "
             "const double& theBackground, "
             "const std::string& theFurtherMotionName, "
             "const gkg::Vector3d< int32_t >& theFurtherSliceCount, "
             "const bool& theCorrectQSpaceSampling, "
             "const std::string& theEddyCurrentProfileName, "
             "const bool& theVerboseRegistration, "
             "const bool& theVerbose )" );

}


template < class T >
void 
EddyCurrentProcess::correct( gkg::Process& process,
                             const std::string& inputName,
                             const gkg::AnalyzedObject&,
                             const std::string& )
{

  try
  {

    EddyCurrentProcess&
      eddyCurrentProcess = static_cast< EddyCurrentProcess& >( process );

    const int32_t& referenceRank =
      eddyCurrentProcess.referenceRank;
    const std::string& referenceName =
      eddyCurrentProcess.referenceName;
    const std::string& outputName =
      eddyCurrentProcess.outputName;
    const std::string& transform3dName =
      eddyCurrentProcess.transform3dName;
    const std::string& similarityMeasureName =
      eddyCurrentProcess.similarityMeasureName;
    const std::string& optimizerName =
      eddyCurrentProcess.optimizerName;
    const double& lowerThreshold =
      eddyCurrentProcess.lowerThreshold;
    const int32_t& registrationResamplingOrder =
      eddyCurrentProcess.registrationResamplingOrder;
    const std::list< int32_t >& subSamplingMaximumSizes =
      eddyCurrentProcess.subSamplingMaximumSizes;
    const std::vector< double >& similarityMeasureParameters =
      eddyCurrentProcess.similarityMeasureParameters;
    const std::vector< double >& optimizerParameters =
      eddyCurrentProcess.optimizerParameters;
    gkg::Vector& vectorOfInitialParameters =
      eddyCurrentProcess.vectorOfInitialParameters;
    const int32_t & outputResamplingOrder =
      eddyCurrentProcess.outputResamplingOrder;
    const double& background =
      eddyCurrentProcess.background;
    const std::string& furtherMotionName =
      eddyCurrentProcess.furtherMotionName;
    const gkg::Vector3d< int32_t >& furtherSliceCount =
      eddyCurrentProcess.furtherSliceCount;
    const bool& correctQSpaceSampling =
      eddyCurrentProcess.correctQSpaceSampling;
    const std::string& eddyCurrentProfileName =
      eddyCurrentProcess.eddyCurrentProfileName;
    const bool& verboseRegistration =
      eddyCurrentProcess.verboseRegistration;
    const bool& verbose =
      eddyCurrentProcess.verbose;

    ////////////////////////////////////////////////////////////////////////////
    // reading input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading input volume '"
                << inputName << "' : "
                << std::flush;

    }
    gkg::RCPointer< gkg::Volume< T > > 
      volume( new gkg::Volume< T > );
    std::string format = gkg::Reader::getInstance().read( inputName, *volume );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading reference volume if provided
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Volume< T > > reference;
    if ( !referenceName.empty() )
    {

      std::cout << "reading reference volume '"
                << referenceName << "' : "
                << std::flush;

      reference.reset( new gkg::Volume< T > );
      gkg::Reader::getInstance().read( referenceName,
                                       *reference );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating registration algorithm
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating registration algorithm : " << std::flush;

    }
    gkg::RCPointer< gkg::RegistrationAlgorithm< T, T > >
      registrationAlgorithm( new gkg::RegistrationAlgorithm< T, T >(
                            similarityMeasureName,
                            optimizerName,
                            registrationResamplingOrder,
                            subSamplingMaximumSizes,
                            similarityMeasureParameters,
                            optimizerParameters,
                            verboseRegistration ) );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating vector of 3D transform(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating affine 3D transform(s) : " << std::flush;

    }

    int32_t transform3dCount =
                          ( referenceName.empty() ?
                            volume->getSizeT() - 1 :
                            volume->getSizeT() ); 
    std::vector< gkg::RCPointer< gkg::NonLinearTransform3d< float > > >
      nonLinearTransform3ds( transform3dCount );

    std::vector< gkg::RCPointer<
                                gkg::ParameterizedAffineTransform3d< float > > >
      affineTransform3ds( transform3dCount );

    int32_t t = 0;
    for ( t = 0; t < transform3dCount; t++ )
    {

      affineTransform3ds[ t ].reset(
                             new gkg::ParameterizedAffineTransform3d< float > );

    }

    std::vector< gkg::RCPointer< gkg::VectorFieldTransform3d< float > > >
      vectorFieldTransform3ds( transform3dCount );

    for ( t = 0; t < transform3dCount; t++ )
    {

      nonLinearTransform3ds[ t ].reset(
        new gkg::NonLinearTransform3d< float >(
                                     affineTransform3ds[ t ],
                                     vectorFieldTransform3ds[ t ] ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating a further 3D transform 
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > > furtherMotion;

    if ( !furtherMotionName.empty() )
    {

      try
      {

        gkg::RigidTransform3d< float >* 
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream is( furtherMotionName.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        furtherMotion.reset( rigidTransform3d );

        if ( verbose )
        {

          std::cout << "further motion was a rigid 3D transform"
                    << std::endl;

        }

      }
      catch ( std::exception&  )
      {

        try
        {

          gkg::AffineTransform3d< float >* 
            affineTransform3d = new gkg::AffineTransform3d< float >;
          std::ifstream is( furtherMotionName.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          furtherMotion.reset( affineTransform3d );

          if ( verbose )
          {

            std::cout << "further motion was an affine 3D transform"
                      << std::endl;

          }

        }
        catch( std::exception& )
        {

           throw std::runtime_error( "failed to read the 3D transform" );

        } 

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // performing eddy current correction
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "correcting eddy currents and motion : " << std::flush;

    }
    gkg::MotionCorrection< T >
      motionCorrection( registrationAlgorithm,
                        outputResamplingOrder,
                        ( T )background,
                        verbose );

    if ( referenceName.empty() )
    {

      motionCorrection.correct(
                        volume,
                        referenceRank,
                        vectorOfInitialParameters,
                        ( T )lowerThreshold,
                        nonLinearTransform3ds,
                        furtherMotion,
                        furtherSliceCount );

    }
    else
    {

      motionCorrection.correct(
                        volume,
                        reference,
                        vectorOfInitialParameters,
                        ( T )lowerThreshold,
                        nonLinearTransform3ds,
                        furtherMotion,
                        furtherSliceCount );

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // adding motion 3D rotations to Q-space sampling if existing
    ////////////////////////////////////////////////////////////////////////////

    if ( volume->getHeader().hasAttribute( "diffusion_sensitization_type" ) &&
         correctQSpaceSampling )
    {

      if ( verbose )
      {

        std::cout << "adding 3D rotation(s) to Q-space sampling : "
                  << std::flush;

      }

      gkg::QSpaceSampling*
        qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create(
                                                                     *volume );

      for ( t = 0; t < transform3dCount; t++ )
      {


        if ( !furtherMotion.isNull() )
        {

          gkg::CompositeTransform3d< float > compositeTransform3d;

            compositeTransform3d.compose( 
             *nonLinearTransform3ds[ t ]->getParameterizedAffineTransform3d() );
          compositeTransform3d.compose( *furtherMotion );


          const gkg::HomogeneousTransform3d< float >& 
            composition = compositeTransform3d.getComposition();

          gkg::Matrix rotationAndScalingAndShearings( 3, 3 );
          rotationAndScalingAndShearings( 0, 0 ) =
            composition.getDirectCoefficient( 0, 0 ); 
          rotationAndScalingAndShearings( 0, 1 ) =
            composition.getDirectCoefficient( 0, 1 ); 
          rotationAndScalingAndShearings( 0, 2 ) =
            composition.getDirectCoefficient( 0, 2 ); 

          rotationAndScalingAndShearings( 1, 0 ) =
            composition.getDirectCoefficient( 1, 0 ); 
          rotationAndScalingAndShearings( 1, 1 ) =
            composition.getDirectCoefficient( 1, 1 ); 
          rotationAndScalingAndShearings( 1, 2 ) =
            composition.getDirectCoefficient( 1, 2 ); 

          rotationAndScalingAndShearings( 2, 0 ) =
            composition.getDirectCoefficient( 2, 0 ); 
          rotationAndScalingAndShearings( 2, 1 ) =
            composition.getDirectCoefficient( 2, 1 ); 
          rotationAndScalingAndShearings( 2, 2 ) =
            composition.getDirectCoefficient( 2, 2 ); 
            
          gkg::Vector3d< float > scalings;
          gkg::Vector3d< float > shearings;
          gkg::Vector3d< float > u;
          gkg::Vector3d< float > v;
          gkg::Vector3d< float > w;

          gkg::getXYZScalingsAndShearingsAndTrieder(
            rotationAndScalingAndShearings, scalings, shearings, u, v, w );

          gkg::Vector3d< float > 
            angles = gkg::getXYZAnglesFromTrieder< float >( u, v, w, false );

          qSpaceSampling->setMotionRotation3d( t, angles );

        }
        else
        {

          gkg::ParameterizedAffineTransform3d< float >*
            parameterizedAffineTransform3d =
            static_cast< gkg::ParameterizedAffineTransform3d< float >* >(
              nonLinearTransform3ds[ t ]->
                                    getParameterizedAffineTransform3d().get() );
          gkg::Vector3d< float > angles;
          parameterizedAffineTransform3d->getAffineTransform3d().
                                            getRotation3d().
                                              getDirectAngles( angles );
          qSpaceSampling->setMotionRotation3d( t, angles );

        }

      }
      qSpaceSampling->addInformationToHeader( *volume );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving eddy current profile of rotation/translation/scaling/shearing
    ////////////////////////////////////////////////////////////////////////////

    if ( !eddyCurrentProfileName.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving eddy current profile(s) : "
                  << std::flush;

      }


      std::vector< gkg::Vector3d< float > > rotations( transform3dCount );
      std::vector< gkg::Vector3d< float > > translations( transform3dCount );
      std::vector< gkg::Vector3d< float > > scalings( transform3dCount );
      std::vector< gkg::Vector3d< float > > shearings( transform3dCount );

      int32_t t = 0;
      for ( t = 0; t < transform3dCount; t++ )
      {

        gkg::ParameterizedAffineTransform3d< float >*
          parameterizedAffineTransform3d =
            static_cast< gkg::ParameterizedAffineTransform3d< float >* >(
              nonLinearTransform3ds[ t ]->
                                    getParameterizedAffineTransform3d().get() );

        parameterizedAffineTransform3d->getAffineTransform3d().getRotation3d().
                                              getDirectAngles( rotations[ t ] );

        parameterizedAffineTransform3d->getAffineTransform3d().
                   getTranslation3d().getDirectTranslation( translations[ t ] );

        parameterizedAffineTransform3d->getAffineTransform3d().
                               getScaling3d().getDirectScaling( scalings[ t ] );

        parameterizedAffineTransform3d->getAffineTransform3d().
                           getShearing3d().getDirectShearings( shearings[ t ] );

      }

      std::ofstream
        os( eddyCurrentProfileName.c_str() );

      // saving translations ///////////////////////////////////////////////////
      os << "translationX = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << translations[ t ].x << ", " << std::flush;

      }
      os << translations[ t ].x << " ]" << std::endl;

      os << "translationY = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << translations[ t ].y << ", " << std::flush;

      }
      os << translations[ t ].y << " ]" << std::endl;

      os << "translationZ = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << translations[ t ].z << ", " << std::flush;

      }
      os << translations[ t ].z << " ]" << std::endl;

      // saving rotations //////////////////////////////////////////////////////
      os << "rotationX = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << rotations[ t ].x << ", " << std::flush;

      }
      os << rotations[ t ].x << " ]" << std::endl;

      os << "rotationY = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << rotations[ t ].y << ", " << std::flush;

      }
      os << rotations[ t ].y << " ]" << std::endl;

      os << "rotationZ = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << rotations[ t ].z << ", " << std::flush;

      }
      os << rotations[ t ].z << " ]" << std::endl;

      // saving scalings ///////////////////////////////////////////////////////
      os << "scalingX = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << scalings[ t ].x << ", " << std::flush;

      }
      os << scalings[ t ].x << " ]" << std::endl;

      os << "scalingY = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << scalings[ t ].y << ", " << std::flush;

      }
      os << scalings[ t ].y << " ]" << std::endl;

      os << "scalingZ = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << scalings[ t ].z << ", " << std::flush;

      }
      os << scalings[ t ].z << " ]" << std::endl;

      // saving shearings///////////////////////////////////////////////////////
      os << "shearingXY = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << shearings[ t ].x << ", " << std::flush;

      }
      os << shearings[ t ].x << " ]" << std::endl;

      os << "shearingXZ = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << shearings[ t ].y << ", " << std::flush;

      }
      os << shearings[ t ].y << " ]" << std::endl;

      os << "shearingYZ = " << "[ " << std::flush;
      for ( t = 0; t < transform3dCount - 1; t++ )
      {

        os << shearings[ t ].z << ", " << std::flush;

      }
      os << shearings[ t ].z << " ]" << std::endl;


      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

 
    ////////////////////////////////////////////////////////////////////////////
    // saving eddy current corrected volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {

      std::cout << "saving eddy current corrected volume : " << std::flush;

    }
    gkg::Writer::getInstance().write( outputName,
                                      *volume,
                                      false,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
   

    ////////////////////////////////////////////////////////////////////////////
    // saving affine 3D direct transform
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving affine 3D direct transform(s) : " << std::flush;

    }

    if ( referenceName.empty() )
    {

      if ( !transform3dName.empty() )
      {

        int32_t count = 0;
        for ( t = 0; t < volume->getSizeT(); t++ )
        {

          if ( t != referenceRank )
          {

            std::string linearName = transform3dName;
            linearName += "_";
            linearName += gkg::StringConverter::toString( t );
            linearName += "_to_";
            linearName += gkg::StringConverter::toString( referenceRank );
            linearName += ".trm";

            nonLinearTransform3ds[ count ]->writeTrm( linearName, "", "" );
            ++ count;

          }

        }

      }

    }
    else
    {

      if ( !transform3dName.empty() )
      {

        for ( t = 0; t < volume->getSizeT(); t++ )
        {

          std::string linearName = transform3dName;
          linearName += "_";
          linearName += gkg::StringConverter::toString( t );
          linearName += "_to_";
          linearName += referenceName;
          linearName += ".trm";

          nonLinearTransform3ds[ t ]->writeTrm( linearName, "", "" );

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void EddyCurrentProcess::match( gkg::Process& process, "
             "const std::string& inputName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiEddyCurrentCommand
//

gkg::DwiEddyCurrentCommand::DwiEddyCurrentCommand( int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin,
                                                   bool removeFirst )
                           : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiEddyCurrentCommand::DwiEddyCurrentCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiEddyCurrentCommand::DwiEddyCurrentCommand(
                       const std::string& inputName,
                       int32_t referenceRank,
                       const std::string& referenceName,
                       const std::string& outputName,
                       const std::string& transform3dName,
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       double lowerThreshold,
                       int32_t registrationResamplingOrder,
                       const std::list< int32_t >& subSamplingMaximumSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       const std::vector< double >& optimizerParameters,
                       const std::vector< float >& initialParameters,
                       int32_t outputResamplingOrder,
                       double background,
                       const std::string& furtherMotionName,
                       const gkg::Vector3d< int32_t >& furtherSliceCount,
                       bool correctQSpaceSampling,
                       const std::string& eddyCurrentProfileName,
                       bool verboseRegistration,
                       bool verbose )
                           : gkg::Command()
{

  try
  {

    execute( inputName,
             referenceRank,
             referenceName,
             outputName,
             transform3dName,
             similarityMeasureName,
             optimizerName,
             lowerThreshold,
             registrationResamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             initialParameters,
             outputResamplingOrder,
             background,
             furtherMotionName,
             furtherSliceCount,
             correctQSpaceSampling,
             eddyCurrentProfileName,
             verboseRegistration,
             verbose );

  }
  GKG_CATCH( "gkg::DwiEddyCurrentCommand::DwiEddyCurrentCommand( "
             "const std::string& inputName, "
             "int32_t referenceRank, "
             "const std::string& referenceName, "
             "const std::string& outputName, "
             "const std::string& transform3dName, "
             "const std::string& similarityMeasureName, "
             "const std::string& optimizerName, "
             "double lowerThreshold, "
             "int32_t registrationResamplingOrder, "
             "const std::list< int32_t >& subSamplingMaximumSizes, "
             "const std::vector< double >& similarityMeasureParameters, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< float >& initialParameters, "
             "int32_t outputResamplingOrder, "
             "double background, "
             "const std::string& furtherMotionName, "
             "const gkg::Vector3d< int32_t >& furtherSliceCount, "
             "bool correctQSpaceSampling, "
             "const std::string& eddyCurrentProfileName, "
             "bool verboseRegistration, "
             "bool verbose )" );

}


gkg::DwiEddyCurrentCommand::DwiEddyCurrentCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, inputName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, referenceRank );
    DECLARE_STRING_PARAMETER( parameters, std::string, referenceName );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputName );
    DECLARE_STRING_PARAMETER( parameters, std::string, transform3dName );
    DECLARE_STRING_PARAMETER( parameters, std::string, similarityMeasureName );
    DECLARE_STRING_PARAMETER( parameters, std::string, optimizerName );
    DECLARE_FLOATING_PARAMETER( parameters, double, lowerThreshold );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t,
                               registrationResamplingOrder );
    DECLARE_LIST_OF_INTEGERS_PARAMETER( parameters, std::list< int32_t >,
                                        subSamplingMaximumSizes );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           similarityMeasureParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           optimizerParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           initialParameters );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputResamplingOrder );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_STRING_PARAMETER( parameters, std::string, furtherMotionName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, furtherSliceCountAlongX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, furtherSliceCountAlongY );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, furtherSliceCountAlongZ );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, correctQSpaceSampling );
    DECLARE_STRING_PARAMETER( parameters, std::string, eddyCurrentProfileName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verboseRegistration );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    gkg::Vector3d< int32_t > furtherSliceCount( furtherSliceCountAlongX,
                                                furtherSliceCountAlongY,
                                                furtherSliceCountAlongZ );
    
    execute( inputName,
             referenceRank,
             referenceName,
             outputName,
             transform3dName,
             similarityMeasureName,
             optimizerName,
             lowerThreshold,
             registrationResamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             initialParameters,
             outputResamplingOrder,
             background,
             furtherMotionName,
             furtherSliceCount,
             correctQSpaceSampling,
             eddyCurrentProfileName,
             verboseRegistration,
             verbose );

  }
  GKG_CATCH( "gkg::DwiEddyCurrentCommand::DwiEddyCurrentCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiEddyCurrentCommand::~DwiEddyCurrentCommand()
{
}


std::string gkg::DwiEddyCurrentCommand::getStaticName()
{

  try
  {

    return "DwiEddyCurrent";

  }
  GKG_CATCH( "std::string gkg::DwiEddyCurrentCommand::getStaticName()" );

}


void gkg::DwiEddyCurrentCommand::parse()
{

  try
  {

    std::string inputName;
    int32_t referenceRank = 0;
    std::string referenceName;
    std::string outputName;
    std::string transform3dName;
    std::string similarityMeasureName = "normalized-mutual-information";
    std::string optimizerName = "nelder-mead";
    double lowerThreshold = 0;
    int32_t registrationResamplingOrder = 1;
    std::list< int32_t > subSamplingMaximumSizes;
    std::vector< double > similarityMeasureParameters;
    std::vector< double > optimizerParameters;
    std::vector< float > initialParameters;
    int32_t outputResamplingOrder = 3;
    double background = 0.0;
    std::string furtherMotionName;
    gkg::Vector3d< int32_t > furtherSliceCount;
    bool correctQSpaceSampling = true;
    std::string eddyCurrentProfileName;
    bool verboseRegistration = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D motion and eddy current correction tool"
                                  " dedicated to DWI",
                                  _loadPlugin );
    application.addSingleOption( "-dw",
                                 "Input DW 4D volume",
                                 inputName );
    application.addSingleOption( "-r",
                                 "Reference rank (default=0)",
                                 referenceRank,
                                 true );
    application.addSingleOption( "-t2",
                                 "Reference T2 volume file name",
                                 referenceName,
                                 true );
    application.addSingleOption( "-o",
                                 "Output motion and eddy current corrected "
                                 "4D volume",
                                 outputName,
                                 true );
    application.addSingleOption( "-ot",
                                 "Output 3D transform name",
                                 transform3dName,
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
    application.addSingleOption( "-lowerThreshold",
                                 "Lower threshold (default=0.0)",
                                 lowerThreshold,
                                 true );
    application.addSingleOption( "-registrationResamplingOrder",
                                 "Registration resampling order (0 to 7) "
                                 "(default=1)",
                                 registrationResamplingOrder,
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
      ".  <P3> - variation of scaling along X (default=0.05),\n"
      ".  <P4> - variation of scaling along Y (default=0.05),\n"
      ".  <P5> - variation of scaling along Z (default=0.05),\n"
      ".  <P6> - variation of shearing along XY (default=0.05),\n"
      ".  <P7> - variation of shearing along XZ (default=0.05),\n"
      ".  <P8> - variation of shearing along YZ (default=0.05),\n"
      ".  <P9> - variation of rotation around X (in degrees, default=10),\n"
      ".  <P10> - variation of rotation around Y (in degrees, default=10),\n"
      ".  <P11> - variation of rotation around Z (in degrees, default=10),\n"
      ".  <P12> - variation of translation along X (in mm, default=10),\n"
      ".  <P13> - variation of translation along Y (in mm, default=10),\n"
      ".  <P14> - variation of translation along Z (in mm, default=10),\n\n"
      "-> in case of fletcher-reeves,\n\n"
      ".  <P1>: maximum iteration count per sub-sampling count (default=1000)\n"
      ".  <P2>: step size (default=0.1)\n"
      ".  <P3>: maximum test gradient (default=1000)\n"
      ".  <P4>: maximum tolerance (default=0.01)\n",
      optimizerParameters );
    application.addSeriesOption(
      "-initialParameters",
      "Initial transform coefficients\n\n"
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
      ". - translation along Z (in mm, default=0)\n",
       initialParameters,
      0, 12 );
    application.addSingleOption( "-outputResamplingOrder",
                                 "Output resampling order (0 to 7) (default=3)",
                                 outputResamplingOrder,
                                 true );
    application.addSingleOption( "-b",
                                 "Background resampling level (default=0)",
                                 background,
                                 true );
    application.addSingleOption( "-m",
                                 "Further 3D rigid or affine motion transform "
                                 "file name",
                                 furtherMotionName,
                                 true );
    application.addSingleOption( "-furtherSliceCountAlongX",
                                 "Further slice count along x axis (default=0)",
                                 furtherSliceCount.x,
                                 true );
    application.addSingleOption( "-furtherSliceCountAlongY",
                                 "Further slice count along y axis (default=0)",
                                 furtherSliceCount.y,
                                 true );
    application.addSingleOption( "-furtherSliceCountAlongZ",
                                 "Further slice count along z axis (default=0)",
                                 furtherSliceCount.z,
                                 true );
    application.addSingleOption( "-correctQSpaceSampling",
                                 "Correct Q-space sampling with motion 3D "
                                 "rotations when available (default=true)",
                                 correctQSpaceSampling,
                                 true );
    application.addSingleOption( "-motionProfile",
                                 "Motion profile python filename giving "
                                 "profiles of rotation/translation/scaling/"
                                 "shearing",
                                 eddyCurrentProfileName,
                                 true );
    application.addSingleOption( "-verboseRegistration",
                                 "Show registration running information",
                                 verboseRegistration,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( inputName,
             referenceRank,
             referenceName,
             outputName,
             transform3dName,
             similarityMeasureName,
             optimizerName,
             lowerThreshold,
             registrationResamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             initialParameters,
             outputResamplingOrder,
             background,
             furtherMotionName,
             furtherSliceCount,
             correctQSpaceSampling,
             eddyCurrentProfileName,
             verboseRegistration,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiEddyCurrentCommand::parse()" );

}


void gkg::DwiEddyCurrentCommand::execute(
                       const std::string& inputName,
                       int32_t referenceRank,
                       const std::string& referenceName,
                       const std::string& outputName,
                       const std::string& transform3dName,
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       double lowerThreshold,
                       int32_t registrationResamplingOrder,
                       const std::list< int32_t >& subSamplingMaximumSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       const std::vector< double >& optimizerParameters,
                       const std::vector< float >& initialParameters,
                       int32_t outputResamplingOrder,
                       double background,
                       const std::string& furtherMotionName,
                       const gkg::Vector3d< int32_t >& furtherSliceCount,
                       bool correctQSpaceSampling,
                       const std::string& eddyCurrentProfileName,
                       bool verboseRegistration,
                       bool verbose )
 {

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    std::string theOutputName = outputName;
    if ( theOutputName.empty() )
    {

      theOutputName = inputName;

    }
    std::list< int32_t > theSubSamplingMaximumSizes = subSamplingMaximumSizes;
    if ( theSubSamplingMaximumSizes.empty() )
    {

      theSubSamplingMaximumSizes.push_back( 64 );

    }

    if ( !initialParameters.empty() && ( initialParameters.size() != 12U ) )
    {

      throw std::runtime_error(
                         "option '-initialParameters': must have 12 values" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // setting vectors of initial parameters and delta parameters
    ////////////////////////////////////////////////////////////////////////////

    // initial parameter(s)
    gkg::Vector vectorOfInitialParameters( 12 );
    if ( initialParameters.empty() )
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
    std::vector< double > theOptimizerParameters = optimizerParameters;
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



    ////////////////////////////////////////////////////////////////////////////
    // launching eddy current correction process
    ////////////////////////////////////////////////////////////////////////////
    EddyCurrentProcess
      eddyCurrentProcess( referenceRank,
                          referenceName,
                          theOutputName,
                          transform3dName,
                          similarityMeasureName,
                          optimizerName,
                          lowerThreshold,
                          registrationResamplingOrder,
                          theSubSamplingMaximumSizes,
                          similarityMeasureParameters,
                          theOptimizerParameters,
                          vectorOfInitialParameters,
                          outputResamplingOrder,
                          background,
                          furtherMotionName,
                          furtherSliceCount,
                          correctQSpaceSampling,
                          eddyCurrentProfileName,
                          verboseRegistration,
                          verbose );
    eddyCurrentProcess.execute( inputName );

  }
  GKG_CATCH( "void gkg::DwiEddyCurrentCommand::execute( "
             "const std::string& inputName, "
             "int32_t referenceRank, "
             "const std::string& referenceName, "
             "const std::string& outputName, "
             "const std::string& transform3dName, "
             "const std::string& similarityMeasureName, "
             "const std::string& optimizerName, "
             "double lowerThreshold, "
             "int32_t registrationResamplingOrder, "
             "const std::list< int32_t >& subSamplingMaximumSizes, "
             "const std::vector< double >& similarityMeasureParameters, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< float >& initialParameters, "
             "int32_t outputResamplingOrder, "
             "double background, "
             "const std::string& furtherMotionName, "
             "const gkg::Vector3d< int32_t >& furtherSliceCount, "
             "bool correctQSpaceSampling, "
             "const std::string& eddyCurrentProfileName, "
             "bool verboseRegistration, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiEddyCurrentCommand,
    DECLARE_STRING_PARAMETER_HELP( inputName ) +
    DECLARE_INTEGER_PARAMETER_HELP( referenceRank ) +
    DECLARE_STRING_PARAMETER_HELP( referenceName ) +
    DECLARE_STRING_PARAMETER_HELP( outputName ) +
    DECLARE_STRING_PARAMETER_HELP( transform3dName ) +
    DECLARE_STRING_PARAMETER_HELP( similarityMeasureName ) +
    DECLARE_STRING_PARAMETER_HELP( optimizerName ) +
    DECLARE_FLOATING_PARAMETER_HELP( lowerThreshold ) +
    DECLARE_INTEGER_PARAMETER_HELP( registrationResamplingOrder ) +
    DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( subSamplingMaximumSizes ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( similarityMeasureParameters ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( initialParameters ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputResamplingOrder ) +
    DECLARE_FLOATING_PARAMETER_HELP( background ) +
    DECLARE_STRING_PARAMETER_HELP( furtherMotionName ) +
    DECLARE_INTEGER_PARAMETER_HELP( furtherSliceCountAlongX ) +
    DECLARE_INTEGER_PARAMETER_HELP( furtherSliceCountAlongY ) +
    DECLARE_INTEGER_PARAMETER_HELP( furtherSliceCountAlongZ ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( correctQSpaceSampling ) +
    DECLARE_STRING_PARAMETER_HELP( eddyCurrentProfileName ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verboseRegistration ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
