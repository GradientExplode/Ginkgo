#include <gkg-dmri-plugin-functors/DwiMotionCorrection/DwiMotionCorrectionCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineWoShearingTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
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
// class DwiMotionCorrectionProcess
//

class DwiMotionCorrectionProcess : public gkg::Process
{

  public:

    DwiMotionCorrectionProcess(
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
      const std::string& theTransformType,
      gkg::Vector& theVectorOfInitialParameters,
      const int32_t& theOutputResamplingOrder,
      const double& theBackground,
      const std::string& theFurtherMotionName,
      const gkg::Vector3d< int32_t >& theFurtherSliceCount,
      const bool& theCorrectQSpaceSampling,
      const std::string& theMotionProfileName,
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
    const std::string& transformType;
    gkg::Vector& vectorOfInitialParameters;
    const int32_t & outputResamplingOrder;
    const double& background;
    const std::string& furtherMotionName;
    const gkg::Vector3d< int32_t >& furtherSliceCount;
    const bool& correctQSpaceSampling;
    const std::string& motionProfileName;
    const bool& verboseRegistration;
    const bool& verbose;

  private:

    template < class T >
    static void correct( gkg::Process& process,
                         const std::string& inputName,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


DwiMotionCorrectionProcess::DwiMotionCorrectionProcess(
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
      const std::string& theTransformType,
      gkg::Vector& theVectorOfInitialParameters,
      const int32_t& theOutputResamplingOrder,
      const double& theBackground,
      const std::string& theFurtherMotionName,
      const gkg::Vector3d< int32_t >& theFurtherSliceCount,
      const bool& theCorrectQSpaceSampling,
      const std::string& theMotionProfileName,
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
                          transformType( theTransformType ),
                          vectorOfInitialParameters(
                            theVectorOfInitialParameters ),
                          outputResamplingOrder( theOutputResamplingOrder ),
                          background( theBackground ),
                          furtherMotionName( theFurtherMotionName ),
                          furtherSliceCount( theFurtherSliceCount ),
                          correctQSpaceSampling( theCorrectQSpaceSampling ),
                          motionProfileName( theMotionProfileName ),
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
  GKG_CATCH( "DwiMotionCorrectionProcess::DwiMotionCorrectionProcess( "
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
             "const std::string& theTransformType, "
             "gkg::Vector& theVectorOfInitialParameters, "
             "const int32_t& theOutputResamplingOrder, "
             "const double& theBackground, "
             "const std::string& theFurtherMotionName, "
             "const gkg::Vector3d< int32_t >& theFurtherSliceCount, "
             "const bool& theCorrectQSpaceSampling, "
             "const std::string& theMotionProfileName, "
             "const bool& theVerboseRegistration, "
             "const bool& theVerbose )" );

}


template < class T >
void 
DwiMotionCorrectionProcess::correct( gkg::Process& process,
                                     const std::string& inputName,
                                     const gkg::AnalyzedObject&,
                                     const std::string& )
{

  try
  {

    DwiMotionCorrectionProcess&
      dwiMotionCorrectionProcess = static_cast< DwiMotionCorrectionProcess& >(
                                                                      process );

    const int32_t& referenceRank =
      dwiMotionCorrectionProcess.referenceRank;
    const std::string& referenceName =
      dwiMotionCorrectionProcess.referenceName;
    const std::string& outputName =
      dwiMotionCorrectionProcess.outputName;
    const std::string& transform3dName =
      dwiMotionCorrectionProcess.transform3dName;
    const std::string& similarityMeasureName =
      dwiMotionCorrectionProcess.similarityMeasureName;
    const std::string& optimizerName =
      dwiMotionCorrectionProcess.optimizerName;
    const double& lowerThreshold =
      dwiMotionCorrectionProcess.lowerThreshold;
    const int32_t& registrationResamplingOrder =
      dwiMotionCorrectionProcess.registrationResamplingOrder;
    const std::list< int32_t >& subSamplingMaximumSizes =
      dwiMotionCorrectionProcess.subSamplingMaximumSizes;
    const std::vector< double >& similarityMeasureParameters =
      dwiMotionCorrectionProcess.similarityMeasureParameters;
    const std::vector< double >& optimizerParameters =
      dwiMotionCorrectionProcess.optimizerParameters;
    const std::string& transformType =
      dwiMotionCorrectionProcess.transformType;
    gkg::Vector& vectorOfInitialParameters =
      dwiMotionCorrectionProcess.vectorOfInitialParameters;
    const int32_t & outputResamplingOrder =
      dwiMotionCorrectionProcess.outputResamplingOrder;
    const double& background =
      dwiMotionCorrectionProcess.background;
    const std::string& furtherMotionName =
      dwiMotionCorrectionProcess.furtherMotionName;
    const gkg::Vector3d< int32_t >& furtherSliceCount =
      dwiMotionCorrectionProcess.furtherSliceCount;
    const bool& correctQSpaceSampling =
      dwiMotionCorrectionProcess.correctQSpaceSampling;
    const std::string& motionProfileName =
      dwiMotionCorrectionProcess.motionProfileName;
    const bool& verboseRegistration =
      dwiMotionCorrectionProcess.verboseRegistration;
    const bool& verbose =
      dwiMotionCorrectionProcess.verbose;

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

      std::cout << "allocating 3D transform(s) : " << std::flush;

    }

    int32_t transform3dCount = ( referenceName.empty() ?
                                 volume->getSizeT() - 1 :
                                 volume->getSizeT() ); 
    std::vector< gkg::RCPointer< gkg::NonLinearTransform3d< float > > >
      nonLinearTransform3ds( transform3dCount);

    std::vector< gkg::RCPointer< gkg::ParameterizedRigidTransform3d< float > > >
      rigidTransform3ds;
    std::vector< gkg::RCPointer< 
                      gkg::ParameterizedAffineWoShearingTransform3d< float > > >
      affineWoShearingTransform3ds;
    std::vector< gkg::RCPointer< 
                                gkg::ParameterizedAffineTransform3d< float > > >
      affineTransform3ds;

    int32_t t = 0;
    if ( ( transformType == "rigid" ) ||
         ( transformType == "rigid_and_diffeomorphic" ) )
    {

      rigidTransform3ds.resize( transform3dCount );
      for ( t = 0; t < transform3dCount; t++ )
      {

        rigidTransform3ds[ t ].reset(
                              new gkg::ParameterizedRigidTransform3d< float > );

      }

    }
    else if ( ( transformType == "affine_wo_shearing" ) ||
              ( transformType == "affine_wo_shearing_and_diffeomorphic" ) )
    {

      affineWoShearingTransform3ds.resize( transform3dCount );
      for ( t = 0; t < transform3dCount; t++ )
      {

        affineWoShearingTransform3ds[ t ].reset(
                   new gkg::ParameterizedAffineWoShearingTransform3d< float > );

      }

    }
    else if ( ( transformType == "affine" ) ||
              ( transformType == "affine_and_diffeomorphic" ) )
    {

      affineTransform3ds.resize( transform3dCount );
      for ( t = 0; t < transform3dCount; t++ )
      {

        affineTransform3ds[ t ].reset(
                             new gkg::ParameterizedAffineTransform3d< float > );

      }

    }

    std::vector< gkg::RCPointer< gkg::VectorFieldTransform3d< float > > >
      vectorFieldTransform3ds( transform3dCount );

    if ( ( transformType == "rigid" ) ||
         ( transformType == "rigid_and_diffeomorphic" ) )
    {

      for ( t = 0; t < transform3dCount; t++ )
      {

        nonLinearTransform3ds[ t ].reset(
          new gkg::NonLinearTransform3d< float >(
                                     rigidTransform3ds[ t ],
                                     vectorFieldTransform3ds[ t ] ) );

      }

    }
    else if ( ( transformType == "affine_wo_shearing" ) ||
              ( transformType == "affine_wo_shearing_and_diffeomorphic" ) )
    {

      for ( t = 0; t < transform3dCount; t++ )
      {

        nonLinearTransform3ds[ t ].reset(
          new gkg::NonLinearTransform3d< float >(
                                     affineWoShearingTransform3ds[ t ],
                                     vectorFieldTransform3ds[ t ] ) );

      }

    }
    else if ( ( transformType == "affine" ) ||
              ( transformType == "affine_and_diffeomorphic" ) )
    {

      for ( t = 0; t < transform3dCount; t++ )
      {

        nonLinearTransform3ds[ t ].reset(
          new gkg::NonLinearTransform3d< float >(
                                     affineTransform3ds[ t ],
                                     vectorFieldTransform3ds[ t ] ) );

      }

    }
    else if ( transformType == "diffeomorphic" )
    {

      for ( t = 0; t < transform3dCount; t++ )
      {

        nonLinearTransform3ds[ t ].reset(
          new gkg::NonLinearTransform3d< float >(
                                     vectorFieldTransform3ds[ t ] ) );

      }

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
        std::ifstream is( furtherMotionName );
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
          std::ifstream is( furtherMotionName );
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
    // performing motion correction
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "correcting motion : " << std::flush;

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

      if ( ( transformType == "rigid" ) || 
           ( transformType == "rigid_and_diffeomorphic" ) )
      {

        for ( t = 0; t < transform3dCount; t++ )
        {
 
          if ( !furtherMotion.isNull() )
          {

            gkg::CompositeTransform3d< float > compositeTransform3d;

            compositeTransform3d.compose(
              *nonLinearTransform3ds[ t ]->getParameterizedRigidTransform3d() );
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

            gkg::ParameterizedRigidTransform3d< float >*
              parameterizedRigidTransform3d =
              static_cast< gkg::ParameterizedRigidTransform3d< float >* >(
                nonLinearTransform3ds[ t ]->
                                     getParameterizedRigidTransform3d().get() );
            gkg::Vector3d< float > angles;
            parameterizedRigidTransform3d->getRigidTransform3d().
                                             getRotation3d().
                                               getDirectAngles( angles );
            qSpaceSampling->setMotionRotation3d( t, angles );

          }

        }

      }
      else if ( ( transformType == "affine_wo_shearing" ) ||
                ( transformType == "affine_wo_shearing_and_diffeomorphic" ) )
      {

        for ( t = 0; t < transform3dCount; t++ )
        {
 
          if ( !furtherMotion.isNull() )
          {

            gkg::CompositeTransform3d< float > compositeTransform3d;

            compositeTransform3d.compose(
            *nonLinearTransform3ds[ t ]->
                                getParameterizedAffineWoShearingTransform3d() );
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

            gkg::ParameterizedAffineWoShearingTransform3d< float >*
              parameterizedAffineWoShearingTransform3d =
              static_cast<
                      gkg::ParameterizedAffineWoShearingTransform3d< float >* >(
                nonLinearTransform3ds[ t ]->
                          getParameterizedAffineWoShearingTransform3d().get() );
            gkg::Vector3d< float > angles;
            parameterizedAffineWoShearingTransform3d->
                                             getAffineWoShearingTransform3d().
                                               getRotation3d().
                                                 getDirectAngles( angles );
            qSpaceSampling->setMotionRotation3d( t, angles );

          }

        }

      }
      else if ( ( transformType == "affine" ) ||
                ( transformType == "affine_and_diffeomorphic" ) )
      {

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

      }
      qSpaceSampling->addInformationToHeader( *volume );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

 
    ////////////////////////////////////////////////////////////////////////////
    // saving motion profile of rotation/translation/scaling/shearing
    ////////////////////////////////////////////////////////////////////////////

    if ( !motionProfileName.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving motion profile(s) : "
                  << std::flush;

      }


      std::vector< gkg::Vector3d< float > > rotations( transform3dCount );
      std::vector< gkg::Vector3d< float > > translations( transform3dCount );
      std::vector< gkg::Vector3d< float > > scalings( transform3dCount );
      std::vector< gkg::Vector3d< float > > shearings( transform3dCount );

      int32_t t = 0;
      for ( t = 0; t < transform3dCount; t++ )
      {

        if ( ( transformType == "affine" ) ||
             ( transformType == "affine_and_diffeomorphic" ) )
        {

          gkg::ParameterizedAffineTransform3d< float >*
            parameterizedAffineTransform3d =
              static_cast< gkg::ParameterizedAffineTransform3d< float >* >(
                nonLinearTransform3ds[ t ]->
                                    getParameterizedAffineTransform3d().get() );

          parameterizedAffineTransform3d->getAffineTransform3d().
                   getRotation3d().getDirectAngles( rotations[ t ] );

          parameterizedAffineTransform3d->getAffineTransform3d().
                   getTranslation3d().getDirectTranslation( translations[ t ] );

          parameterizedAffineTransform3d->getAffineTransform3d().
                   getScaling3d().getDirectScaling( scalings[ t ] );

          parameterizedAffineTransform3d->getAffineTransform3d().
                   getShearing3d().getDirectShearings( shearings[ t ] );

        }
        else if ( ( transformType == "affine_wo_shearing" ) ||
                  ( transformType == "affine_wo_shearing_and_diffeomorphic" ) )
        {

          gkg::ParameterizedAffineWoShearingTransform3d< float >*
            parameterizedAffineWoShearingTransform3d =
              static_cast< gkg::ParameterizedAffineWoShearingTransform3d<
                                                                     float >* >(
                nonLinearTransform3ds[ t ]->
                          getParameterizedAffineWoShearingTransform3d().get() );

          parameterizedAffineWoShearingTransform3d->
                   getAffineWoShearingTransform3d().
                   getRotation3d().getDirectAngles( rotations[ t ] );

          parameterizedAffineWoShearingTransform3d->
                   getAffineWoShearingTransform3d().
                   getTranslation3d().getDirectTranslation( translations[ t ] );

          parameterizedAffineWoShearingTransform3d->
                   getAffineWoShearingTransform3d().
                   getScaling3d().getDirectScaling( scalings[ t ] );

          shearings[ t ].x = 0.0;
          shearings[ t ].y = 0.0;
          shearings[ t ].z = 0.0;

        }
        else if ( ( transformType == "rigid" ) ||
                  ( transformType == "rigid_and_diffeomorphic" ) )
        {

          gkg::ParameterizedRigidTransform3d< float >*
            parameterizedRigidTransform3d =
              static_cast< gkg::ParameterizedRigidTransform3d< float >* >(
                nonLinearTransform3ds[ t ]->
                                     getParameterizedRigidTransform3d().get() );

          parameterizedRigidTransform3d->
                   getRigidTransform3d().
                   getRotation3d().getDirectAngles( rotations[ t ] );

          parameterizedRigidTransform3d->
                   getRigidTransform3d().
                   getTranslation3d().getDirectTranslation( translations[ t ] );

          scalings[ t ].x = 1.0;
          scalings[ t ].y = 1.0;
          scalings[ t ].z = 1.0;

          shearings[ t ].x = 0.0;
          shearings[ t ].y = 0.0;
          shearings[ t ].z = 0.0;

        }

      }

      std::ofstream os( motionProfileName );

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
    // saving motion corrected volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {

      std::cout << "saving motion corrected volume : " << std::flush;

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
    // saving 3D direct transform
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving 3D direct transform(s) : " << std::flush;

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
            linearName += "_linear.trm";

            std::string diffeomorphicDirectName = transform3dName;
            diffeomorphicDirectName += "_";
            diffeomorphicDirectName += gkg::StringConverter::toString( t );
            diffeomorphicDirectName += "_to_";
            diffeomorphicDirectName += gkg::StringConverter::toString(
                                                                referenceRank );
            diffeomorphicDirectName += "_diffeomorphic.direct.trm";

            std::string diffeomorphicInverseName = transform3dName;
            diffeomorphicInverseName += "_";
            diffeomorphicInverseName += gkg::StringConverter::toString( t );
            diffeomorphicInverseName += "_to_";
            diffeomorphicInverseName += gkg::StringConverter::toString(
                                                                referenceRank );
            diffeomorphicInverseName += "_diffeomorphic.inverse.trm";

            nonLinearTransform3ds[ count ]->writeTrm(
                                                     linearName,
                                                     diffeomorphicDirectName,
                                                     diffeomorphicInverseName );

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
          linearName += "_linear.trm";

          std::string diffeomorphicDirectName = transform3dName;
          diffeomorphicDirectName += "_";
          diffeomorphicDirectName += gkg::StringConverter::toString( t );
          diffeomorphicDirectName += "_to_";
          diffeomorphicDirectName += referenceName;
          diffeomorphicDirectName += "_diffeomorphic.direct.trm";

          std::string diffeomorphicInverseName = transform3dName;
          diffeomorphicInverseName += "_";
          diffeomorphicInverseName += gkg::StringConverter::toString( t );
          diffeomorphicInverseName += "_to_";
          diffeomorphicInverseName += referenceName;
          diffeomorphicInverseName += "_diffeomorphic.inverse.trm";

          nonLinearTransform3ds[ t ]->writeTrm( linearName,
                                                diffeomorphicDirectName,
                                                diffeomorphicInverseName );

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void DwiMotionCorrectionProcess::match( gkg::Process& process, "
             "const std::string& inputName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiMotionCorrectionCommand
//

gkg::DwiMotionCorrectionCommand::DwiMotionCorrectionCommand( int32_t argc,
                                                             char* argv[],
                                                             bool loadPlugin,
                                                             bool removeFirst )
                                : gkg::Command( argc, argv, loadPlugin,
                                                removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiMotionCorrectionCommand::DwiMotionCorrectionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiMotionCorrectionCommand::DwiMotionCorrectionCommand(
                       const std::string inputName,
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
                       const std::string& transformType,
                       const std::vector< float >& initialParameters,
                       int32_t outputResamplingOrder,
                       double background,
                       const std::string& furtherMotionName,
                       const gkg::Vector3d< int32_t >& furtherSliceCount,
                       bool correctQSpaceSampling,
                       const std::string& motionProfileName,
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
             transformType,
             initialParameters,
             outputResamplingOrder,
             background,
             furtherMotionName,
             furtherSliceCount,
             correctQSpaceSampling,
             motionProfileName,
             verboseRegistration,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMotionCorrectionCommand::DwiMotionCorrectionCommand( "
             "const std::string inputName, "
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
             "const std::string& transformType, "
             "const std::vector< float >& initialParameters, "
             "int32_t outputResamplingOrder, "
             "double background, "
             "const std::string& furtherMotionName, "
             "const gkg::Vector3d< int32_t >& furtherSliceCount, "
             "bool correctQSpaceSampling, "
             "const std::string& motionProfileName, "
             "bool verboseRegistration, "
             "bool verbose )" );

}


gkg::DwiMotionCorrectionCommand::DwiMotionCorrectionCommand(
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
    DECLARE_STRING_PARAMETER( parameters, std::string, transformType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           initialParameters );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputResamplingOrder );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_STRING_PARAMETER( parameters, std::string, furtherMotionName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, furtherSliceCountAlongX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, furtherSliceCountAlongY );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, furtherSliceCountAlongZ );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, correctQSpaceSampling );
    DECLARE_STRING_PARAMETER( parameters, std::string, motionProfileName );
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
             transformType,
             initialParameters,
             outputResamplingOrder,
             background,
             furtherMotionName,
             furtherSliceCount,
             correctQSpaceSampling,
             motionProfileName,
             verboseRegistration,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMotionCorrectionCommand::DwiMotionCorrectionCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiMotionCorrectionCommand::~DwiMotionCorrectionCommand()
{
}


std::string gkg::DwiMotionCorrectionCommand::getStaticName()
{

  try
  {

    return "DwiMotionCorrection";

  }
  GKG_CATCH( "std::string gkg::DwiMotionCorrectionCommand::getStaticName()" );

}


void gkg::DwiMotionCorrectionCommand::parse()
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
    std::string transformType = "rigid";
    std::vector< float > initialParameters;
    int32_t outputResamplingOrder = 3;
    double background = 0.0;
    std::string furtherMotionName;
    gkg::Vector3d< int32_t > furtherSliceCount;
    bool correctQSpaceSampling = false;
    std::string motionProfileName;
    bool verboseRegistration = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "DWI 3D motion correction tool",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input 4D DWI volume",
                                 inputName );
    application.addSingleOption( "-r",
                                 "Reference rank (default=0)",
                                 referenceRank,
                                 true );
    application.addSingleOption( "-rv",
                                 "Reference volume file name",
                                 referenceName,
                                 true );
    application.addSingleOption( "-o",
                                 "Output motion corrected 4D DWI volume",
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
      "Initial transform coefficients\n\n"
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
                                 "rotations when available (default=false)",
                                 correctQSpaceSampling,
                                 true );
    application.addSingleOption( "-motionProfile",
                                 "Motion profile python filename giving "
                                 "profiles of rotation/translation/scaling/"
                                 "shearing",
                                 motionProfileName,
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
             transformType,
             initialParameters,
             outputResamplingOrder,
             background,
             furtherMotionName,
             furtherSliceCount,
             correctQSpaceSampling,
             motionProfileName,
             verboseRegistration,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiMotionCorrectionCommand::parse()" );

}


void gkg::DwiMotionCorrectionCommand::execute(
                       const std::string inputName,
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
                       const std::string& transformType,
                       const std::vector< float >& initialParameters,
                       int32_t outputResamplingOrder,
                       double background,
                       const std::string& furtherMotionName,
                       const gkg::Vector3d< int32_t >& furtherSliceCount,
                       bool correctQSpaceSampling,
                       const std::string& motionProfileName,
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

        vectorOfInitialParameters( 0 ) = 0.0;
        vectorOfInitialParameters( 1 ) = 0.0;
        vectorOfInitialParameters( 2 ) = 0.0;
        vectorOfInitialParameters( 3 ) = 0.0;
        vectorOfInitialParameters( 4 ) = 0.0;
        vectorOfInitialParameters( 5 ) = 0.0;

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

        vectorOfInitialParameters( 0 ) = 0.0;
        vectorOfInitialParameters( 1 ) = 0.0;
        vectorOfInitialParameters( 2 ) = 0.0;
        vectorOfInitialParameters( 3 ) = 0.0;
        vectorOfInitialParameters( 4 ) = 0.0;
        vectorOfInitialParameters( 5 ) = 0.0;

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

          if ( ( theOptimizerParameters.size() - 13U ) % 3U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have a number of parameters "
                   "equal to 10+k*3 where k is the number of diffeomorphic "
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

          if ( ( theOptimizerParameters.size() - 9U ) % 3U )
          {

            throw std::runtime_error(
                   "powell-newuoad optimizer must have a number of parameters "
                   "equal to 6+k*3 where k is the number of diffeomorphic "
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

          if ( ( theOptimizerParameters.size() - 16U ) % 3U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have a number of parameters "
                   "equal to 13+k*3 where k is the number of diffeomorphic "
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
                   "nelder-mead optimizer must have at least 14 parameters for "
                   "affine_and_diffeomorphic transform" );

          }

          if ( ( theOptimizerParameters.size() - 19U ) % 3U )
          {

            throw std::runtime_error(
                   "nelder-mead optimizer must have a number of parameters "
                   "equal to 16+k*3 where k is the number of diffeomorphic "
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

        if ( ( theOptimizerParameters.size() - 5U ) % 3U )
        {

          throw std::runtime_error(
                 "dynamic programming optimizer must have a number of "
                 "parameters equal to 2+k*3 where k is the number of "
                 "diffeomorphic optimization configurations for the "
                 "diffeomorphic transform" );

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // launching motion correction process
    ////////////////////////////////////////////////////////////////////////////
    DwiMotionCorrectionProcess
      dwiMotionCorrectionProcess( referenceRank,
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
                                  transformType,
                                  vectorOfInitialParameters,
                                  outputResamplingOrder,
                                  background,
                                  furtherMotionName,
                                  furtherSliceCount,
                                  correctQSpaceSampling,
                                  motionProfileName,
                                  verboseRegistration,
                                  verbose );
    dwiMotionCorrectionProcess.execute( inputName );

  }
  GKG_CATCH( "void gkg::DwiMotionCorrectionCommand::execute( "
             "const std::string inputName, "
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
             "const std::string& transformType, "
             "const std::vector< float >& initialParameters, "
             "int32_t outputResamplingOrder, "
             "double background, "
             "const std::string& furtherMotionName, "
             "const gkg::Vector3d< int32_t >& furtherSliceCount, "
             "bool correctQSpaceSampling, "
             "const std::string& motionProfileName, "
             "bool verboseRegistration, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiMotionCorrectionCommand,
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
    DECLARE_STRING_PARAMETER_HELP(  transformType ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( initialParameters ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputResamplingOrder ) +
    DECLARE_FLOATING_PARAMETER_HELP(  background ) +
    DECLARE_STRING_PARAMETER_HELP( furtherMotionName ) +
    DECLARE_INTEGER_PARAMETER_HELP( furtherSliceCountAlongX ) +
    DECLARE_INTEGER_PARAMETER_HELP( furtherSliceCountAlongY ) +
    DECLARE_INTEGER_PARAMETER_HELP( furtherSliceCountAlongZ ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( correctQSpaceSampling ) +
    DECLARE_STRING_PARAMETER_HELP( motionProfileName ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verboseRegistration ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
