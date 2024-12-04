#ifndef _gkg_processing_registration_RegistrationAlgorithm_i_h_
#define _gkg_processing_registration_RegistrationAlgorithm_i_h_


#include <gkg-processing-registration/RegistrationAlgorithm.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-registration/SimilarityMeasureFactory_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-registration/NelderMeadSimilarityMeasureFunction_i.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-graph/MinimumSpanningTree_i.h>
#include <gkg-processing-algobase/MinimumIndexFilter_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/SumOfSquareFilter_i.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-cppext/Limits.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>

#include <fstream>
#include <iostream>

#define FAST_INVERSE_TRANSFORM_MAXIMUM_ITERATION_COUNT 10

#define DIFFEO_MUTUAL_INFO_LEVEL_COUNT 16



template < class T1, class T2 >
inline
gkg::RegistrationAlgorithm< T1, T2 >::RegistrationAlgorithm(
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       int32_t resamplingOrder,
                       const std::list< int32_t >& subSamplingSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       const std::vector< double >& optimizerParameters,
                       bool verbose )
                                     : _similarityMeasureName(
                                                       similarityMeasureName ),
                                       _optimizerName( optimizerName ),
                                       _resamplingOrder( resamplingOrder ),
                                       _subSamplingSizes( subSamplingSizes ),
                                       _similarityMeasureParameters(
                                                  similarityMeasureParameters ),
                                       _optimizerParameters(
                                                          optimizerParameters ),
                                       _verbose( verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    gkg::SimilarityMeasureFactory< T1, T2 >::getInstance().
      checkSimilarityMeasureName( _similarityMeasureName );
    gkg::SimilarityMeasureFactory< T1, T2 >::getInstance().
      checkScalarParameters( _similarityMeasureName,
                             _similarityMeasureParameters );

    std::list< int32_t >::const_iterator s = _subSamplingSizes.begin(),
                                         se = _subSamplingSizes.end();
    while ( s != se )
    {

      if ( ( *s < 32 ) || ( *s > 4096 ) )
      {

        throw std::runtime_error(
                      "sub-sampling maximum size must be between 32 and 4096" );

      }
      ++ s;

    }

    if ( optimizerName == "nelder-mead" )
    {

      int32_t maximumIterationCount = ( int32_t )( _optimizerParameters[ 0 ] +
                                                                          0.5 );
      if ( maximumIterationCount < 1 )
      {

        throw std::runtime_error(
                         "maximum iteration count must be strictly positive" );

      }

      double maximumTestSize = _optimizerParameters[ 1 ];
      if ( maximumTestSize <= 0.0 )
      {

        throw std::runtime_error(
                                "maximum test size must be strictly positive" );

      }

    }
    else
    {

      throw std::runtime_error( "not a valid optimizer name" );

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "gkg::RegistrationAlgorithm< T1, T2 >::RegistrationAlgorithm( "
             "const std::string& similarityMeasureName, "
             "const std::string& optimizerName, "
             "int32_t resamplingOrder, "
             "const std::list< int32_t >& subSamplingSizes, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< double >& optimizerParameters, "
             "bool verbose )" );

}


template < class T1, class T2 >
inline
gkg::RegistrationAlgorithm< T1, T2 >::~RegistrationAlgorithm()
{
}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::match(
          gkg::RCPointer< gkg::Volume< T1 > > reference,
          gkg::RCPointer< gkg::Volume< T2 > > floating,
          const gkg::Vector& initialParameters,
          const T1& referenceLowerThreshold,
          const T2& floatingLowerThreshold,
          gkg::RCPointer< gkg::NonLinearTransform3d< float > > transform3d,
          const std::string& intermediateFileName,
          typename gkg::RegistrationAlgorithm< T1, T2 >::TunedParameters 
                                                               tunedParameters )
{

  try
  {

    /////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    /////////////////////////////////////////////////////////////////////////
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                  getImplementationFactory();



    /////////////////////////////////////////////////////////////////////////
    // getting RCPointers to linear and vector field 3D transformations
    /////////////////////////////////////////////////////////////////////////

     gkg::RCPointer< gkg::ParameterizedTransform3d< float > >
       linearTransform3d =
         transform3d->getParameterizedLinearTransform3d();
     gkg::RCPointer< gkg::VectorFieldTransform3d< float > >
       vectorFieldTransform3d =
         transform3d->getVectorFieldTransform3d();


    /////////////////////////////////////////////////////////////////////////
    // initializing parameterized 3D transform
    /////////////////////////////////////////////////////////////////////////

    if ( !linearTransform3d.isNull() )
    {

      if ( tunedParameters ==
           gkg::RegistrationAlgorithm< T1, T2 >::Direct )
      {

        linearTransform3d->setDirectParameters( initialParameters );

      }
      else
      {

        linearTransform3d->setInverseParameters( initialParameters );

      }

    }


    /////////////////////////////////////////////////////////////////////////
    // looping over sub-sampling size(s) to compute the linear part of the
    // transformation
    /////////////////////////////////////////////////////////////////////////

    if ( !linearTransform3d.isNull() )
    {


      gkg::Vector parameters;
      int32_t count = 1;
      std::list< int32_t >::const_iterator s = _subSamplingSizes.begin(),
                                           se = _subSamplingSizes.end();
      while ( s != se )
      {

        ///////////////////////////////////////////////////////////////////////
        // creating similarity measure
        ///////////////////////////////////////////////////////////////////////

        if ( tunedParameters ==
             gkg::RegistrationAlgorithm< T1, T2 >::Direct )
        {

          linearTransform3d->getDirectParameters( parameters );

        }
        else
        {

          linearTransform3d->getInverseParameters( parameters );

        }


        ///////////////////////////////////////////////////////////////////////
        // creating similarity measure
        ///////////////////////////////////////////////////////////////////////

        gkg::RCPointer< gkg::SimilarityMeasure< T1, T2 > >
        similarityMeasure =
          gkg::SimilarityMeasureFactory< T1, T2 >::getInstance().create(
            _similarityMeasureName,
            reference,
            floating,
            referenceLowerThreshold,
            floatingLowerThreshold,
            _resamplingOrder,
            *s,
            _similarityMeasureParameters,
            _verbose );

        if ( !intermediateFileName.empty() )
        {

          //
          // saving resampled reference U8
          //

          std::string referenceU8Name =
                                      intermediateFileName + "_" +
                                      gkg::StringConverter::toString( count ) +
                                      "-" +
                                      gkg::StringConverter::toString( *s ) +
                                      + "_referenceU8" ;
          if ( _verbose )
          {

            std::cout << "saving '" << referenceU8Name << "' : "
                      << std::flush;

          }
          gkg::Writer::getInstance().write(
                                referenceU8Name,
                                *similarityMeasure->getResampledReferenceU8() );
          if ( _verbose )
          {

            std::cout << std::endl;

          }

          //
          // saving resampled floating U8
          //

          std::string floatingU8Name = intermediateFileName + "_" +
                                       gkg::StringConverter::toString( count ) +
                                       "-" +
                                       gkg::StringConverter::toString( *s ) +
                                       + "_floatingU8" ;
          if ( _verbose )
          {

            std::cout << "saving '" << floatingU8Name << "' : "
                      << std::flush;

          }
          gkg::Writer::getInstance().write(
                                floatingU8Name,
                                *similarityMeasure->getResampledFloatingU8() );
          if ( _verbose )
          {

            std::cout << std::endl;

          }

          //
          // saving resampled reference mask
          //

          std::string referenceMaskName =
                                      intermediateFileName + "_" +
                                      gkg::StringConverter::toString( count ) +
                                      "-" +
                                      gkg::StringConverter::toString( *s ) +
                                      + "_referenceMask" ;
          if ( _verbose )
          {

            std::cout << "saving '" << referenceMaskName << "' : "
                      << std::flush;

          }
          gkg::Writer::getInstance().write(
                              referenceMaskName,
                              *similarityMeasure->getResampledReferenceMask() );
          if ( _verbose )
          {

            std::cout << "done" << std::endl;

          }

          //
          // saving resampled floating mask
          //

          std::string floatingMaskName =
                                      intermediateFileName + "_" +
                                      gkg::StringConverter::toString( count ) +
                                      "-" +
                                      gkg::StringConverter::toString( *s ) +
                                      + "_floatingMask" ;
          if ( _verbose )
          {

            std::cout << "saving '" << floatingMaskName << "' : "
                      << std::flush;

          }
          gkg::Writer::getInstance().write(
                              floatingMaskName,
                              *similarityMeasure->getResampledFloatingMask() );
          if ( _verbose )
          {

            std::cout << "done" << std::endl;

          }

        }

        ///////////////////////////////////////////////////////////////////////
        // getting parameters that optimize my function
        ///////////////////////////////////////////////////////////////////////

        if ( _verbose )
        {

          std::cout << "optimizing (sub-sampling " << *s << ") : "
                    << std::endl;

        }

        if ( _optimizerName == "nelder-mead" )
        {

          double testSize = 0.0; 
          int32_t maximumIterationCount = 
                                 ( int32_t )( _optimizerParameters[ 0 ] + 0.5 );
          double maximumTestSize = _optimizerParameters[ 1 ];

          gkg::Vector dParameters( parameters.getSize() );

          if ( ( ( int32_t )_optimizerParameters.size() ) >=
               ( parameters.getSize() + 2 ) )
          {

            // each individual parameters has a specific variation
            int32_t p = 0;
            for ( p = 0; p < parameters.getSize(); p++ )
            {

              dParameters( p ) = _optimizerParameters[ p + 2 ];
              if ( dParameters( p ) < 0.0 )
              {

                throw std::runtime_error(
                                    "variation of parameter must be positive" );

              }

            }

          }
          else
          {

            throw std::runtime_error(
              std::string( "optimizer parameters must have at least " ) +
              gkg::StringConverter::toString( parameters.getSize() + 2 ) +
              " parameters" );

          }

          gkg::NelderMeadSimilarityMeasureFunction< T1, T2 >
            nelderMeadSimilarityMeasureFunction( similarityMeasure,
                                                 linearTransform3d,
                                                 tunedParameters );

          factory->getNelderMeadSimplexOptimumParameters(
                                            parameters,
                                            dParameters,
                                            testSize,
                                            nelderMeadSimilarityMeasureFunction,
                                            maximumIterationCount,
                                            maximumTestSize,
                                            _verbose );


          if ( tunedParameters ==
               gkg::RegistrationAlgorithm< T1, T2 >::Direct )
          {

            linearTransform3d->setDirectParameters( parameters );

          }
          else
          {

            linearTransform3d->setInverseParameters( parameters );

          }

        }
        else if ( _optimizerName == "newuoa" )
        {

          //

        }

        ++ s;

      }

    }


    /////////////////////////////////////////////////////////////////////////
    // looping over configurations to compute the non-linear part of the
    // transformation if a diffeomorphic part is required
    /////////////////////////////////////////////////////////////////////////

    if ( !vectorFieldTransform3d.isNull() )
    {

      // collecting the reference volume size
      gkg::Vector3d< int32_t > referenceSize;
      reference->getSize( referenceSize );

      gkg::Vector3d< double > referenceResolution;
      reference->getResolution( referenceResolution );

      // first tuning the direct and inverse vector fields of the diffeomorphic
      // transformation according to the floating size(s) and resolution(s)

      gkg::Vector3d< int32_t > floatingSize;
      floating->getSize( floatingSize );

      gkg::Vector3d< double > floatingResolution;
      floating->getResolution( floatingResolution );


      // computing the resampling resolution and size required to compute
      // the diffeomorphism
      gkg::Vector3d< double > 
        diffeomorphismResolution( std::min( referenceResolution.x,
                                            floatingResolution.x ),
                                  std::min( referenceResolution.y,
                                            floatingResolution.y ),
                                  std::min( referenceResolution.z,
                                            floatingResolution.z ) );

      gkg::Vector3d< int32_t >
        diffeomorphismSize( std::max( ( int32_t )( referenceSize.x *
                                                   referenceResolution.x /
                                                   diffeomorphismResolution.x +
                                                   0.5 ),
                                      ( int32_t )( floatingSize.x *
                                                   floatingResolution.x /
                                                   diffeomorphismResolution.x +
                                                   0.5 ) ),
                            std::max( ( int32_t )( referenceSize.y *
                                                   referenceResolution.y /
                                                   diffeomorphismResolution.y +
                                                   0.5 ),
                                      ( int32_t )( floatingSize.y *
                                                   floatingResolution.y /
                                                   diffeomorphismResolution.y +
                                                   0.5 ) ),
                            std::max( ( int32_t )( referenceSize.z *
                                                   referenceResolution.z /
                                                   diffeomorphismResolution.z +
                                                   0.5 ),
                                      ( int32_t )( floatingSize.z *
                                                   floatingResolution.z /
                                                   diffeomorphismResolution.z +
                                                   0.5 ) ) );

      gkg::RCPointer< gkg::Volume< T1 > >
        diffeomorphismReference( new gkg::Volume< T1 >( diffeomorphismSize ) );
      diffeomorphismReference->setResolution( diffeomorphismResolution );
                            
      gkg::RCPointer< gkg::Volume< T2 > >
        diffeomorphismFloating( new gkg::Volume< T2 >( diffeomorphismSize ) );
      diffeomorphismFloating->setResolution( diffeomorphismResolution );
                                      
      gkg::Resampler< T1 >* referenceResampler = 
        gkg::ResamplerFactory< T1 >::getInstance().getResampler( 1 );
                                            
      gkg::Resampler< T2 >* floatingResampler = 
        gkg::ResamplerFactory< T2 >::getInstance().getResampler( 1 );

      gkg::IdentityTransform3d< float > identityTransform3d;
      referenceResampler->resample( *reference,
                                    identityTransform3d,
                                    0.0f,
                                    *diffeomorphismReference );

      /*if ( !linearTransform3d.isNull() )
      {

        floatingResampler->resample( *floating,
                                     *linearTransform3d,
                                     0.0f,
                                     *diffeomorphismFloating );
         gkg::Writer::getInstance().write( "002_linearResampled.ima",
                                           *diffeomorphismFloating );

      }
      else*/
      {

        floatingResampler->resample( *floating,
                                     identityTransform3d,
                                     0.0f,
                                     *diffeomorphismFloating );

      }

      gkg::Writer::getInstance().write( "diffeomorphismReference.ima", *diffeomorphismReference );
      gkg::Writer::getInstance().write( "diffeomorphismFloating.ima", *diffeomorphismFloating );

      int32_t subSamplingSize = std::max( diffeomorphismSize.x,
                                  std::max( diffeomorphismSize.y,
                                            diffeomorphismSize.z ) );

      gkg::Volume< float > directVectorField( diffeomorphismSize, 3 );
      directVectorField.setResolution( diffeomorphismResolution );
      directVectorField.fill( 0.0f );

      gkg::Volume< float > inverseVectorField( diffeomorphismSize, 3 );
      inverseVectorField.setResolution( diffeomorphismResolution );
      inverseVectorField.fill( 0.0f );

      gkg::Volume< float > directFlowFieldAlongX( diffeomorphismSize );
      directFlowFieldAlongX.setResolution( diffeomorphismResolution );
      gkg::Volume< float > directFlowFieldAlongY( diffeomorphismSize );
      directFlowFieldAlongY.setResolution( diffeomorphismResolution );
      gkg::Volume< float > directFlowFieldAlongZ( diffeomorphismSize );
      directFlowFieldAlongZ.setResolution( diffeomorphismResolution );

      gkg::Volume< float > inverseFlowFieldAlongX( diffeomorphismSize );
      inverseFlowFieldAlongX.setResolution( diffeomorphismResolution );
      gkg::Volume< float > inverseFlowFieldAlongY( diffeomorphismSize );
      inverseFlowFieldAlongY.setResolution( diffeomorphismResolution );
      gkg::Volume< float > inverseFlowFieldAlongZ( diffeomorphismSize );
      inverseFlowFieldAlongZ.setResolution( diffeomorphismResolution );

      vectorFieldTransform3d->setDirectAndInverseVectorFields( 
                                                           directVectorField,
                                                           inverseVectorField );


      int32_t c = 0;
      int32_t optimizerConfigurationCount = 0;
      std::vector< int32_t > halfWidthSearchSpaces;
      std::vector< int32_t > controlPointSpacings;
      std::vector< int32_t > searchSpaceQuantisations;
      std::vector< float > regularizationWeights;
      if ( _optimizerName == "nelder-mead" )
      {

        int32_t optimizerParameterOffset = 2;
        if ( !linearTransform3d.isNull() )
        {

          optimizerParameterOffset += linearTransform3d->getParameterCount();

        }

        optimizerConfigurationCount =
                                ( int32_t )( _optimizerParameters[
                                             optimizerParameterOffset ] + 0.5 );
        
        halfWidthSearchSpaces.resize( optimizerConfigurationCount );
        controlPointSpacings.resize( optimizerConfigurationCount );
        searchSpaceQuantisations.resize( optimizerConfigurationCount );
        regularizationWeights.resize( optimizerConfigurationCount );

        for ( c = 0; c < optimizerConfigurationCount; c++ )
        {

          halfWidthSearchSpaces[ c ] = ( int32_t )( _optimizerParameters[
                               optimizerParameterOffset + 4 * c + 1 ] + 0.5 );
          controlPointSpacings[ c ] = ( int32_t )( _optimizerParameters[
                               optimizerParameterOffset + 4 * c + 2 ] + 0.5 );
          searchSpaceQuantisations[ c ] = ( int32_t )( _optimizerParameters[
                               optimizerParameterOffset + 4 * c + 3 ] + 0.5 );
          regularizationWeights[ c ] = ( float )_optimizerParameters[
                               optimizerParameterOffset + 4 * c + 4 ];

        }        

      }
      else if ( _optimizerName == "powell-newuoa" )
      {

        //

      }

      // allocating the similarity measure
      gkg::RCPointer< gkg::SimilarityMeasure< T1, T2 > >
        directSimilarityMeasure =
          gkg::SimilarityMeasureFactory< T1, T2 >::getInstance().create(
            _similarityMeasureName,
            diffeomorphismReference,
            diffeomorphismFloating,
            referenceLowerThreshold,
            floatingLowerThreshold,
            _resamplingOrder,
            subSamplingSize,
            _similarityMeasureParameters,
            _verbose );

      gkg::RCPointer< gkg::SimilarityMeasure< T2, T1 > >
        inverseSimilarityMeasure =
          gkg::SimilarityMeasureFactory< T2, T1 >::getInstance().create(
            _similarityMeasureName,
            diffeomorphismFloating,
            diffeomorphismReference,
            floatingLowerThreshold,
            referenceLowerThreshold,
            _resamplingOrder,
            subSamplingSize,
            _similarityMeasureParameters,
            _verbose );

      Resampler< float >* deformationResampler = 
      gkg::ResamplerFactory< float >::getInstance().getResampler( 1 );

      gkg::RCPointer< gkg::InverseTransform3d< float > >
        inverseTransform3d( new gkg::InverseTransform3d< float >(
                                                                transform3d ) );

      gkg::Vector3d< double > 
        controlPointVolumeResolution( diffeomorphismResolution );

      gkg::Vector3d< int32_t > 
        coarserControlPointVolumeSize = diffeomorphismSize /
                                        controlPointSpacings[ 0 ];
      gkg::Vector3d< double > 
      coarserControlPointVolumeResolution = controlPointVolumeResolution *
                                            ( double )controlPointSpacings[ 0 ];

      gkg::Volume< float > 
        coarserDirectFlowFieldAlongX( coarserControlPointVolumeSize );
      coarserDirectFlowFieldAlongX.setResolution( 
                                          coarserControlPointVolumeResolution );
      gkg::Volume< float > 
        coarserDirectFlowFieldAlongY( coarserControlPointVolumeSize );
      coarserDirectFlowFieldAlongY.setResolution( 
                                          coarserControlPointVolumeResolution );
      gkg::Volume< float > 
        coarserDirectFlowFieldAlongZ( coarserControlPointVolumeSize );
      coarserDirectFlowFieldAlongZ.setResolution( 
                                          coarserControlPointVolumeResolution );

      gkg::Volume< float > 
        coarserInverseFlowFieldAlongX( coarserControlPointVolumeSize );
      coarserInverseFlowFieldAlongX.setResolution( 
                                          coarserControlPointVolumeResolution );
      gkg::Volume< float > 
        coarserInverseFlowFieldAlongY( coarserControlPointVolumeSize );
      coarserInverseFlowFieldAlongY.setResolution( 
                                          coarserControlPointVolumeResolution );
      gkg::Volume< float > 
        coarserInverseFlowFieldAlongZ( coarserControlPointVolumeSize );
      coarserInverseFlowFieldAlongZ.setResolution( 
                                          coarserControlPointVolumeResolution );

      coarserDirectFlowFieldAlongX.fill( 0.0f );
      coarserDirectFlowFieldAlongY.fill( 0.0f );
      coarserDirectFlowFieldAlongZ.fill( 0.0f );

      coarserInverseFlowFieldAlongX.fill( 0.0f );
      coarserInverseFlowFieldAlongY.fill( 0.0f );
      coarserInverseFlowFieldAlongZ.fill( 0.0f );
      
      gkg::Volume< float > currentDirectFlowFieldAlongX;
      gkg::Volume< float > currentDirectFlowFieldAlongY;
      gkg::Volume< float > currentDirectFlowFieldAlongZ;

      gkg::Volume< float > currentInverseFlowFieldAlongX;
      gkg::Volume< float > currentInverseFlowFieldAlongY;
      gkg::Volume< float > currentInverseFlowFieldAlongZ;

      gkg::Vector3d< int32_t > currentControlPointVolumeSize;
      gkg::Vector3d< double > currentControlPointVolumeResolution;

      double directSimilarityValueBefore = 0.0;
      double directSimilarityValueAfter = 0.0;
      int32_t currentSearchSpaceSize = 0;
      int32_t currentSearchSpaceVoxelCount = 0;
      int32_t currentControlPointCount = 0;

      gkg::Volume< uint8_t > 
        directTransformedFloatingU8( diffeomorphismSize );
      directTransformedFloatingU8.setResolution( diffeomorphismResolution );
      gkg::Volume< uint8_t > 
        inverseTransformedReferenceU8( diffeomorphismSize );
      inverseTransformedReferenceU8.setResolution( diffeomorphismResolution );

      ///////////////////////////// looping over configuration(s) //////////////
      for ( c = 0; c < optimizerConfigurationCount; c++ )
      {

        // compute direct similarity measure before deformation
        directSimilarityValueBefore = directSimilarityMeasure->getValueAt(
                                                                 *transform3d );

        // computing resampled floating volume with direct transformation
        directSimilarityMeasure->computeTransformedFloatingU8(
                                                *transform3d,
                                                directTransformedFloatingU8 );

        // computing resampled reference volume with inverse transformation
        inverseSimilarityMeasure->computeTransformedFloatingU8(
                                                *inverseTransform3d,
                                                inverseTransformedReferenceU8 );

        gkg::Writer::getInstance().write( "directTransformedFloatingU8.ima",
                                          directTransformedFloatingU8 );

        gkg::Writer::getInstance().write( "inverseTransformedReferenceU8.ima",
                                          inverseTransformedReferenceU8 );


        // computing the current 
        const int32_t& currentHalfWidthSearchSpace = halfWidthSearchSpaces[ c ];
        const int32_t& currentControlPointSpacing = controlPointSpacings[ c ];
        const float& currentSearchSpaceQuantisation = 
                                                  searchSpaceQuantisations[ c ];
        const float& currentRegularizationWeight = regularizationWeights[ c ];

        currentSearchSpaceSize = 2 * currentHalfWidthSearchSpace + 1;
        currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                       currentSearchSpaceSize *
                                       currentSearchSpaceSize;
        currentControlPointVolumeSize = diffeomorphismSize /
                                        currentControlPointSpacing;
        currentControlPointCount = currentControlPointVolumeSize.x *
                                   currentControlPointVolumeSize.y *
                                   currentControlPointVolumeSize.z;
        currentControlPointVolumeResolution =
                                           diffeomorphismResolution *
                                           ( double )currentControlPointSpacing;

        if ( _verbose )
        {

          std::cout << "========================="
                    << " configuration " << c + 1
                    << " ========================="
                    << std::endl;
          std::cout << "- grid size / resolution : "
                    << currentControlPointVolumeSize
                    << "  / "
                    << currentControlPointVolumeResolution
                    << std::endl
                    << "- half width search space : "
                    << currentHalfWidthSearchSpace
                    << std::endl;

        }

        currentDirectFlowFieldAlongX.reallocate( currentControlPointVolumeSize,
                                                 1,
                                                 false );
        currentDirectFlowFieldAlongX.setResolution( 
                                          currentControlPointVolumeResolution );
        currentDirectFlowFieldAlongY.reallocate( currentControlPointVolumeSize,
                                                 1,
                                                 false );
        currentDirectFlowFieldAlongY.setResolution( 
                                          currentControlPointVolumeResolution );
        currentDirectFlowFieldAlongZ.reallocate( currentControlPointVolumeSize,
                                                 1,
                                                 false );
        currentDirectFlowFieldAlongZ.setResolution( 
                                          currentControlPointVolumeResolution );

        currentInverseFlowFieldAlongX.reallocate( currentControlPointVolumeSize,
                                                  1,
                                                  false );
        currentInverseFlowFieldAlongX.setResolution( 
                                          currentControlPointVolumeResolution );
        currentInverseFlowFieldAlongY.reallocate( currentControlPointVolumeSize,
                                                  1,
                                                  false );
        currentInverseFlowFieldAlongY.setResolution( 
                                          currentControlPointVolumeResolution );
        currentInverseFlowFieldAlongZ.reallocate( currentControlPointVolumeSize,
                                                  1,
                                                  false );
        currentInverseFlowFieldAlongZ.setResolution( 
                                          currentControlPointVolumeResolution );

        deformationResampler->resample( coarserDirectFlowFieldAlongX,
                                        identityTransform3d,
                                        0.0f,
                                        currentDirectFlowFieldAlongX );
        deformationResampler->resample( coarserDirectFlowFieldAlongY,
                                        identityTransform3d,
                                        0.0f,
                                        currentDirectFlowFieldAlongY );
        deformationResampler->resample( coarserDirectFlowFieldAlongZ,
                                        identityTransform3d,
                                        0.0f,
                                        currentDirectFlowFieldAlongZ );

        deformationResampler->resample( coarserInverseFlowFieldAlongX,
                                        identityTransform3d,
                                        0.0f,
                                        currentInverseFlowFieldAlongX );
        deformationResampler->resample( coarserInverseFlowFieldAlongY,
                                        identityTransform3d,
                                        0.0f,
                                        currentInverseFlowFieldAlongY );
        deformationResampler->resample( coarserInverseFlowFieldAlongZ,
                                        identityTransform3d,
                                        0.0f,
                                        currentInverseFlowFieldAlongZ );

        // computing minimum spanning trees of floating and reference volumes
        gkg::MinimumSpanningTree< uint8_t >
          referenceMinimumSpanningTree(
            *directSimilarityMeasure->getResampledReferenceU8(),
            gkg::Neighborhood3d::Neighborhood3d_6,
            currentControlPointSpacing );

        gkg::MinimumSpanningTree< uint8_t >
          floatingMinimumSpanningTree(
            *directSimilarityMeasure->getResampledFloatingU8(),
            gkg::Neighborhood3d::Neighborhood3d_6,
            currentControlPointSpacing );

        // computing all the direct displacement cost(s)
        std::vector< float > 
          directDisplacementCosts( currentControlPointCount * 
                                   currentSearchSpaceVoxelCount, 0.0f );
        this->computeDisplacementCosts(
                        *( directSimilarityMeasure->getResampledReferenceU8() ),
                        directTransformedFloatingU8,
                        currentHalfWidthSearchSpace,
                        currentControlPointSpacing,
                        currentSearchSpaceQuantisation,
                        currentRegularizationWeight,
                        directDisplacementCosts );

        // computing all the inverse displacement cost(s)
        std::vector< float > 
          inverseDisplacementCosts( currentControlPointCount * 
                                    currentSearchSpaceVoxelCount, 0.0f );
        this->computeDisplacementCosts(
                        *( directSimilarityMeasure->getResampledFloatingU8() ),
                        inverseTransformedReferenceU8,
                        currentHalfWidthSearchSpace,
                        currentControlPointSpacing,
                        currentSearchSpaceQuantisation,
                        currentRegularizationWeight,
                        inverseDisplacementCosts );

        // now, the coarser becomes the current
        coarserControlPointVolumeSize = currentControlPointVolumeSize;
        coarserControlPointVolumeResolution =
                                          currentControlPointVolumeResolution;

        // reallocating coarser direct and inverse flow fields
        coarserDirectFlowFieldAlongX.reallocate( coarserControlPointVolumeSize,
                                                 1,
                                                 false );
        coarserDirectFlowFieldAlongX.setResolution(
                                          coarserControlPointVolumeResolution );
        coarserDirectFlowFieldAlongX.fill( 0.0f );
        coarserDirectFlowFieldAlongY.reallocate( coarserControlPointVolumeSize,
                                                 1,
                                                 false );
        coarserDirectFlowFieldAlongY.setResolution(
                                          coarserControlPointVolumeResolution );
        coarserDirectFlowFieldAlongY.fill( 0.0f );
        coarserDirectFlowFieldAlongZ.reallocate( coarserControlPointVolumeSize,
                                                 1,
                                                 false );
        coarserDirectFlowFieldAlongZ.setResolution(
                                          coarserControlPointVolumeResolution );
        coarserDirectFlowFieldAlongZ.fill( 0.0f );
 
        coarserInverseFlowFieldAlongX.reallocate( coarserControlPointVolumeSize,
                                                  1,
                                                  false );
        coarserInverseFlowFieldAlongX.setResolution(
                                          coarserControlPointVolumeResolution );
        coarserInverseFlowFieldAlongX.fill( 0.0f );
        coarserInverseFlowFieldAlongY.reallocate( coarserControlPointVolumeSize,
                                                  1,
                                                  false );
        coarserInverseFlowFieldAlongY.setResolution(
                                          coarserControlPointVolumeResolution );
        coarserInverseFlowFieldAlongY.fill( 0.0f );
        coarserInverseFlowFieldAlongZ.reallocate( coarserControlPointVolumeSize,
                                                  1,
                                                  false );
        coarserInverseFlowFieldAlongZ.setResolution(
                                          coarserControlPointVolumeResolution );
        coarserInverseFlowFieldAlongZ.fill( 0.0f );

        // attributing optimal displacement(s) to each node of the direct MST
        this->attributeOptimalDisplacements( currentDirectFlowFieldAlongX,
                                             currentDirectFlowFieldAlongY,
                                             currentDirectFlowFieldAlongZ,
                                             directDisplacementCosts,
                                             currentHalfWidthSearchSpace,
                                             currentSearchSpaceQuantisation,
                                             referenceMinimumSpanningTree,
                                             coarserDirectFlowFieldAlongX,
                                             coarserDirectFlowFieldAlongY,
                                             coarserDirectFlowFieldAlongZ );

        // attributing optimal displacement(s) to each node of the inverse MST
        this->attributeOptimalDisplacements( currentInverseFlowFieldAlongX,
                                             currentInverseFlowFieldAlongY,
                                             currentInverseFlowFieldAlongZ,
                                             inverseDisplacementCosts,
                                             currentHalfWidthSearchSpace,
                                             currentSearchSpaceQuantisation,
                                             floatingMinimumSpanningTree,
                                             coarserInverseFlowFieldAlongX,
                                             coarserInverseFlowFieldAlongY,
                                             coarserInverseFlowFieldAlongZ );

        // scaling and squaring the direct transformation
        this->scaleAndSquareTransformation( coarserDirectFlowFieldAlongX,
                                            coarserDirectFlowFieldAlongY,
                                            coarserDirectFlowFieldAlongZ,
                                            currentControlPointSpacing,
                                            4 );

        // scaling and squaring the inverse transformation
        this->scaleAndSquareTransformation( coarserInverseFlowFieldAlongX,
                                            coarserInverseFlowFieldAlongY,
                                            coarserInverseFlowFieldAlongZ,
                                            currentControlPointSpacing,
                                            4 );

        // making direct and inverse transformations symmetric
        this->symmetrizeDirectAndInverseTransformations(
                                                 coarserInverseFlowFieldAlongX,
                                                 coarserInverseFlowFieldAlongY,
                                                 coarserInverseFlowFieldAlongZ,
                                                 coarserDirectFlowFieldAlongX,
                                                 coarserDirectFlowFieldAlongY,
                                                 coarserDirectFlowFieldAlongZ,
                                                 currentControlPointSpacing );

        // computing Jacobian determinant
        float jacobianMean = 0.0f;
        float jacobianStandardDeviation = 0.0f;
        float jacobianMinimum = 0.0f;
        float jacobianMaximum = 0.0f;
        float jacobianNegativeRatio = 0.0f;
        this->computeJacobianDeterminant( coarserDirectFlowFieldAlongX,
                                          coarserDirectFlowFieldAlongY,
                                          coarserDirectFlowFieldAlongZ,
                                          currentControlPointSpacing,
                                          jacobianMean,
                                          jacobianStandardDeviation,
                                          jacobianMinimum,
                                          jacobianMaximum,
                                          jacobianNegativeRatio );
        if ( _verbose )
        {

           std::cout << "- Jacobian of deformations : "
                     << " mean=" << jacobianMean
                     << " std=" << jacobianStandardDeviation
                     << " min=" << jacobianMinimum
                     << " max=" << jacobianMaximum
                     << " %neg=" << jacobianNegativeRatio * 100.0f
                     << std::endl;
 
        }
        
        std::ofstream os( "jacobien.csv" );
        if ( os )
        {

           os << " Jacobian Mean : " << jacobianMean << std::endl;
           os << " Jacobian Standard Deviation : " 
              << jacobianStandardDeviation 
              << std::endl;
           os << " Jacobian Minimum : " << jacobianMinimum << std::endl;
           os << " Jacobian Maximum : " << jacobianMaximum << std::endl;
           os << " Jacobian Negative Ratio : " 
              << jacobianNegativeRatio * 100.0f
              << std::endl;

           os.close();

        }
        else
        {

           std::cerr << "Impossible d'ouvrir le fichier" << std::endl;

        }

        // upsampling direct transformation to full-resolution grid
        deformationResampler->resample( coarserDirectFlowFieldAlongX,
                                        identityTransform3d,
                                        0.0f,
                                        directFlowFieldAlongX );
        deformationResampler->resample( coarserDirectFlowFieldAlongY,
                                        identityTransform3d,
                                        0.0f,
                                        directFlowFieldAlongY );
        deformationResampler->resample( coarserDirectFlowFieldAlongZ,
                                        identityTransform3d,
                                        0.0f,
                                        directFlowFieldAlongZ );

        // upsampling inverse transformation to full-resolution grid
        deformationResampler->resample( coarserInverseFlowFieldAlongX,
                                        identityTransform3d,
                                        0.0f,
                                        inverseFlowFieldAlongX );
        deformationResampler->resample( coarserInverseFlowFieldAlongY,
                                        identityTransform3d,
                                        0.0f,
                                        inverseFlowFieldAlongY );
        deformationResampler->resample( coarserInverseFlowFieldAlongZ,
                                        identityTransform3d,
                                        0.0f,
                                        inverseFlowFieldAlongZ );

        // computing the harmonic energy of the direct transformation to check
        // whether the transformation can be inverted
        float energy = this->computeHarmonicEnergy( directFlowFieldAlongX,
                                                    directFlowFieldAlongY,
                                                    directFlowFieldAlongZ );

        if ( _verbose )
        {

          std::cout << "- Harmonic energy of deformation field : "
                    << energy
                    << std::endl;

        }

        // now copying back the full-resolution direct and inverse flow fields 
        // along X/Y/Z to the vector field
        vectorFieldTransform3d->setDirectAndInverseVectorFields( 
                                                       directFlowFieldAlongX,
                                                       directFlowFieldAlongY,
                                                       directFlowFieldAlongZ,
                                                       inverseFlowFieldAlongX,
                                                       inverseFlowFieldAlongY,
                                                       inverseFlowFieldAlongZ );
                                     
        // compute direct similarity measure after deformation
        directSimilarityValueAfter = directSimilarityMeasure->getValueAt(
                                                                 *transform3d );

        // gkg::Volume< uint8_t > 
        // intermediateDirectTransformedFloatingU8( diffeomorphismSize );
        // intermediateDirectTransformedFloatingU8.setResolution(
        //                                           diffeomorphismResolution );
        // directSimilarityMeasure->computeTransformedFloatingU8(
        //                            *transform3d,
        //                            intermediateDirectTransformedFloatingU8 );

        // gkg::Writer::getInstance().write(
        //                            "/tmp/floating_deformed.ima",
        //                            intermediateDirectTransformedFloatingU8 );

        if ( _verbose )
        {

          std::cout << "- similarity energy before registration : "
                    << directSimilarityValueBefore
                    << " and after : "
                    << directSimilarityValueAfter
                    << std::endl;

        }

      }

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::match( "
             "gkg::RCPointer< gkg::Volume< T1 > > reference, "
             "gkg::RCPointer< gkg::Volume< T2 > > floating, "
             "const gkg::Vector& initialParameters, "
             "const T1& referenceLowerThreshold, "
             "const T2& floatingLowerThreshold, "
             "gkg::RCPointer< gkg::NonLinearTransform3d< float > > "
             "transform3d, "
             "const std::string& intermediateFileName, "
             "typename gkg::RegistrationAlgorithm< T1, T2 >::TunedParameters "
             "tunedParameters )" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::computeDisplacementCosts(
              const gkg::Volume< uint8_t >& sourceVolume,
              const gkg::Volume< uint8_t >& transformedVolume,
              int32_t currentHalfWidthSearchSpace,
              int32_t currentControlPointSpacing,
              int32_t currentSearchSpaceQuantisation,
              float currentRegularizationWeight,
              std::vector< float >& displacementCosts ) const
{

  try
  {

    gkg::Vector3d< int32_t > sourceVolumeSize;
    sourceVolume.getSize( sourceVolumeSize );

    gkg::Vector3d< double > sourceVolumeResolution;
    sourceVolume.getResolution( sourceVolumeResolution );

    gkg::BoundingBox< int32_t > sourceBoundingBox( sourceVolume );

    gkg::Vector3d< int32_t > 
      currentControlPointVolumeSize = sourceVolumeSize /
                                      currentControlPointSpacing;

    int32_t currentControlPointCount = currentControlPointVolumeSize.x *
                                       currentControlPointVolumeSize.y *
                                       currentControlPointVolumeSize.z;

    int32_t currentSearchSpaceSize = 2 * currentHalfWidthSearchSpace + 1;
    int32_t currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                           currentSearchSpaceSize *
                                           currentSearchSpaceSize;

    std::vector< gkg::Vector3d< float > > displacements;
    this->getDisplacements( currentSearchSpaceSize,
                            currentSearchSpaceVoxelCount,
                            currentHalfWidthSearchSpace,
                            currentSearchSpaceQuantisation,
                            displacements );


    int32_t currentHalfWidthSearchSpaceBis = currentHalfWidthSearchSpace *
                                             currentSearchSpaceQuantisation;

    bool useRandomMode = false;
    int32_t maximumSampleCount = currentControlPointSpacing *
                                 currentControlPointSpacing *
                                 currentControlPointSpacing;
    if ( maximumSampleCount > 64 )
    {

      useRandomMode = true;
      maximumSampleCount = 64;

    } 

    float totalRegularizationWeight = 
                               ( ( float )currentControlPointSpacing /
                                 ( currentRegularizationWeight *
                                   ( float )currentSearchSpaceQuantisation ) ) /
                               ( float )maximumSampleCount;
    std::vector< float > unregularizedDisplacementCosts(
                                                   currentSearchSpaceVoxelCount,
                                                   0.0f );



    int32_t cx = 0;
    int32_t cy = 0;
    int32_t cz = 0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    bool isWithinBoundaries = true;
    int32_t s = 0;

    int32_t spx = 0;
    int32_t spy = 0;
    int32_t spz = 0;

    int32_t nx = 0;
    int32_t ny = 0;
    int32_t nz = 0;

    int32_t tx = 0;
    int32_t ty = 0;
    int32_t tz = 0;

    int32_t d = 0;

    int32_t currentControlPointIndex = 0;


    std::vector< gkg::Matrix >
      jointProbabilityMatrices( currentSearchSpaceVoxelCount );
    for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
    {

      jointProbabilityMatrices[ d ].reallocate(
                                               DIFFEO_MUTUAL_INFO_LEVEL_COUNT,
                                               DIFFEO_MUTUAL_INFO_LEVEL_COUNT );
      jointProbabilityMatrices[ d ].setZero();

    }
    gkg::DericheGaussianCoefficients< double >
      dericheGaussianCoefficients(
                        1.0,
                        gkg::DericheGaussianCoefficients< double >::Smoothing );
    int32_t referenceLevel = 0;
    int32_t floatingLevel = 0;
    gkg::Matrix smoothedJointProbabilities( DIFFEO_MUTUAL_INFO_LEVEL_COUNT,
                                            DIFFEO_MUTUAL_INFO_LEVEL_COUNT );

    double scale = ( double )( DIFFEO_MUTUAL_INFO_LEVEL_COUNT - 1 ) / 255.0;

    if ( _verbose )
    {

      std::cout << "- computing displacement costs : " << std::flush;

    }

    for ( cz = 0; cz < currentControlPointVolumeSize.z; cz++ )
    {


      if ( _verbose )
      {

        if ( cz != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "slice[ " << std::setw( 4 ) << cz + 1 
                  << " / " << std::setw( 4 ) << currentControlPointVolumeSize.z
                  << " ]" << std::flush;



      }

      for ( cy = 0; cy < currentControlPointVolumeSize.y; cy++ )
      {

        for ( cx = 0; cx < currentControlPointVolumeSize.x; cx++ )
        {

          // computing coordinates of point in the source image
          x = cx * currentControlPointSpacing;
          y = cy * currentControlPointSpacing;
          z = cz * currentControlPointSpacing;

          // checking whether the control point plus its search space lie
          // within the source bounding box
          isWithinBoundaries = true;
          if ( !sourceBoundingBox.contains( x + currentControlPointSpacing - 1 +
                                            currentHalfWidthSearchSpaceBis,
                                            y + currentControlPointSpacing - 1 +
                                            currentHalfWidthSearchSpaceBis,
                                            z + currentControlPointSpacing - 1 +
                                            currentHalfWidthSearchSpaceBis ) )
          {

            isWithinBoundaries = false;

          }
          if ( !sourceBoundingBox.contains( x - currentHalfWidthSearchSpaceBis,
                                            y - currentHalfWidthSearchSpaceBis,
                                            z - currentHalfWidthSearchSpaceBis ) )
          {

            isWithinBoundaries = false;

          }


          // resetting the displacement costs to nul
          for ( s = 0; s < currentSearchSpaceVoxelCount; s++ )
          {

            unregularizedDisplacementCosts[ s ] = 0.0f;

          }

          // looping over samples
          // in case of active random mode, randomly picking up samples 
          if ( useRandomMode )
          {

            for ( s = 0; s < maximumSampleCount; s++ )
            {

              // randomly selecting and building the neighbor voxel
              spx = ( int32_t )( ( float )std::rand() *
                                ( float )currentControlPointSpacing /
                                ( float )RAND_MAX );
              spy = ( int32_t )( ( float )std::rand() *
                                ( float )currentControlPointSpacing /
                                ( float )RAND_MAX );
              spz = ( int32_t )( ( float )std::rand() *
                                ( float )currentControlPointSpacing /
                                ( float )RAND_MAX );

              nx = x + spx;
              ny = y + spy;
              nz = z + spz;


              // computing the unregularized energy cost for all the possible
              // displacements 
              for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
              {

                tx = nx + ( int32_t )displacements[ d ].x;
                ty = ny + ( int32_t )displacements[ d ].y;
                tz = nz + ( int32_t )displacements[ d ].z;


                if ( !isWithinBoundaries )
                {

                  tx = std::max( std::min( tx, sourceVolumeSize.x - 1 ), 0 );
                  ty = std::max( std::min( ty, sourceVolumeSize.y - 1 ), 0 );
                  tz = std::max( std::min( tz, sourceVolumeSize.z - 1 ), 0 );

                }

                unregularizedDisplacementCosts[ d ] += 
                           std::abs( ( float )sourceVolume( nx, ny, nz ) -
                                     ( float )transformedVolume( tx, ty, tz ) );

              }

            }

          }
          // else going through all samples
          else
          {

            for ( spz = 0; spz < currentControlPointSpacing; spz++ )
            {

              for ( spy = 0; spy < currentControlPointSpacing; spy++ )
              {

                for ( spx = 0; spx < currentControlPointSpacing; spx++ )
                {

                  nx = x + spx;
                  ny = y + spy;
                  nz = z + spz;

                  // computing the unregularized energy cost for all the 
                  // possible displacements 
                  for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
                  {

                    tx = nx + ( int32_t )displacements[ d ].x;
                    ty = ny + ( int32_t )displacements[ d ].y;
                    tz = nz + ( int32_t )displacements[ d ].z;

                    if ( !isWithinBoundaries )
                    {

                      tx = std::max( std::min( tx, sourceVolumeSize.x - 1 ),
                                     0 );
                      ty = std::max( std::min( ty, sourceVolumeSize.y - 1 ),
                                     0 );
                      tz = std::max( std::min( tz, sourceVolumeSize.z - 1 ),
                                     0 );

                    }

//                     unregularizedDisplacementCosts[ d ] += 
//                            std::abs( ( float )sourceVolume( nx, ny, nz ) -
//                                      ( float )transformedVolume( tx, ty, tz ) );

                    referenceLevel = 
                      ( int32_t )( ( double )sourceVolume( nx, ny, nz ) *
                                   scale );
                    floatingLevel =
                      ( int32_t )( ( double )transformedVolume( tx, ty, tz ) *
                                   scale );

                    ++ jointProbabilityMatrices[ d ]( referenceLevel,
                                                      floatingLevel );

                  }

                }

              }

            }

          }

          // regularizing and storing the displacement costs in the output 
          // container for the current control point
          for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
          {

//             displacementCosts[ currentControlPointIndex + 
//                                currentControlPointCount * d ] =
//               totalRegularizationWeight * unregularizedDisplacementCosts[ d ];

            gkg::Matrix& jointProbabilities = jointProbabilityMatrices[ d ];

//             gkg::DericheGaussianFilter3d< double >::getInstance().filter(
//               jointProbabilities,
//               dericheGaussianCoefficients,
//               gkg::DericheGaussianFilter3d< double >::X_AXIS,
//               smoothedJointProbabilities );
//             gkg::DericheGaussianFilter3d< double >::getInstance().filter(
//               smoothedJointProbabilities,
//               dericheGaussianCoefficients,
//               gkg::DericheGaussianFilter3d< double >::Y_AXIS,
//               jointProbabilities );


            // processing the normalization factor
            double normalizationFactor = 0.0;
            int32_t referenceLevel = 0;
            int32_t floatingLevel = 0;
            for ( referenceLevel = 0;
                  referenceLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
                  referenceLevel++ )
            {

              for ( floatingLevel = 0;
                    floatingLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
                    floatingLevel++ )
              {

                normalizationFactor += jointProbabilities( referenceLevel,
                                                           floatingLevel );

              }

            }

            // normalizing the join probabilities
            if ( normalizationFactor > 0.0 )
            {

              for ( referenceLevel = 0;
                    referenceLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
                    referenceLevel++ )
              {

                for ( floatingLevel = 0;
                      floatingLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
                      floatingLevel++ )
                {

                  jointProbabilities( referenceLevel,
                                      floatingLevel ) /= normalizationFactor;

                }

              }

            }

            displacementCosts[ currentControlPointIndex + 
                               currentControlPointCount * d ] =
              totalRegularizationWeight * 
              this->computeMutualInformation( jointProbabilities );

          }

          ++ currentControlPointIndex;

        }

      }

    }

    if ( _verbose )
    {

      std::cout << std::endl;

    }


    std::cout << "end of computeDisplacementCosts" << std::endl;

//     gkg::Volume< float >
//       displacementCostsVolume( currentControlPointVolumeSize,
//                                currentSearchSpaceVoxelCount );
//     gkg::Volume< float >::iterator dout = displacementCostsVolume.begin();
//     std::vector< float >::const_iterator din = displacementCosts.begin(),
//                                          dine = displacementCosts.end();
//     while ( din != dine )
//     {
// 
//       *dout = *din;
//       ++ dout;
//       ++ din;
// 
//     }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::"
             "computeDisplacementCosts( "
             "const gkg::Volume< uint8_t >& sourceVolume, "
             "const gkg::Volume< uint8_t >& transformedVolume, "
             "int32_t currentHalfWidthSearchSpace, "
             "int32_t currentControlPointSpacing, "
             "int32_t currentSearchSpaceQuantisation, "
             "float currentRegularizationWeight, "
             "std::vector< float >& displacementCosts )" );


}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::attributeOptimalDisplacements(
                 const gkg::Volume< float >& currentFlowFieldAlongX,
                 const gkg::Volume< float >& currentFlowFieldAlongY,
                 const gkg::Volume< float >& currentFlowFieldAlongZ,
                 std::vector< float >& displacementCosts,
                 int32_t currentHalfWidthSearchSpace,
                 int32_t currentSearchSpaceQuantisation,
                 const gkg::MinimumSpanningTree< uint8_t >& minimumSpanningTree,
                 gkg::Volume< float >& coarserFlowFieldAlongX,
                 gkg::Volume< float >& coarserFlowFieldAlongY,
                 gkg::Volume< float >& coarserFlowFieldAlongZ ) const
{

  try
  {


    std::cout << "attributeOptimalDisplacements : 1" << std::endl;

    gkg::Vector3d< int32_t > currentFlowFieldSize;
    currentFlowFieldAlongX.getSize( currentFlowFieldSize );

    int32_t currentFlowFieldPointCount = currentFlowFieldSize.x *
                                         currentFlowFieldSize.y *
                                         currentFlowFieldSize.z;

    int32_t currentSearchSpaceSize = 2 * currentHalfWidthSearchSpace + 1;
    int32_t currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                           currentSearchSpaceSize *
                                           currentSearchSpaceSize;

    std::vector< gkg::Vector3d< float > > displacements;
    this->getDisplacements( currentSearchSpaceSize,
                            currentSearchSpaceVoxelCount,
                            currentHalfWidthSearchSpace,
                            currentSearchSpaceQuantisation,
                            displacements );

    std::vector< float > costs( currentSearchSpaceVoxelCount, 0.0f );
    std::vector< float >::iterator
      cc = costs.begin(),
      cce = costs.end();
    while ( cc != cce )
    {

      *cc = 0.0f;
      ++cc;

    }

    std::cout << "attributeOptimalDisplacements : 2" << std::endl;

    std::vector< int32_t > displacementIndices( currentSearchSpaceVoxelCount,
                                                0 );
    std::vector< int32_t >::iterator
      dd = displacementIndices.begin(),
      dde = displacementIndices.end();
    while ( dd != dde )
    {

      *dd = 0;
      ++dd;

    }
    std::vector< int32_t > allDisplacementIndices( currentFlowFieldPointCount *
                                                   currentSearchSpaceVoxelCount,
                                                   0 );
    std::vector< int32_t >::iterator
      ad = allDisplacementIndices.begin(),
      ade = allDisplacementIndices.end();
    while ( ad != ade )
    {

      *ad = 0;
      ++ad;

    }

    std::cout << "attributeOptimalDisplacements : 3" << std::endl;

    int32_t d = 0;
    int32_t index = currentFlowFieldPointCount - 1;
    int32_t childNodeIndex = 0;
    int32_t parentNodeIndex = 0;
    gkg::Vector3d< float > deltaDisplacement;
    while ( index )
    {

      // collecting the child node index
      childNodeIndex = minimumSpanningTree.getNodeIndex( index );

      // collecting the voxel corresponding to the current node index
      const gkg::Vector3d< int32_t >& 
        childVoxel = minimumSpanningTree.getVoxelFromIndex( childNodeIndex );

      // collecting the parent node index of the child
      parentNodeIndex = minimumSpanningTree.getParentNodeIndex( childNodeIndex );

      // collecting the voxel corresponding to the current node index
      const gkg::Vector3d< int32_t >& 
        parentVoxel = minimumSpanningTree.getVoxelFromIndex( parentNodeIndex );

      // initializing the cost from the previous cost
      for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
      {

        costs[ d ] = displacementCosts[ childNodeIndex +
                                        d * currentFlowFieldPointCount ];

      }

      // computing the displacement residual beetwen parent and child voxel(s)
      deltaDisplacement.x = ( currentFlowFieldAlongX( parentVoxel ) -
                              currentFlowFieldAlongX( childVoxel ) ) /
                            ( float )currentSearchSpaceQuantisation;
      deltaDisplacement.y = ( currentFlowFieldAlongY( parentVoxel ) -
                              currentFlowFieldAlongY( childVoxel ) ) /
                            ( float )currentSearchSpaceQuantisation;
      deltaDisplacement.z = ( currentFlowFieldAlongZ( parentVoxel ) -
                              currentFlowFieldAlongZ( childVoxel ) ) /
                            ( float )currentSearchSpaceQuantisation;

      // fast distance transform
      fastDistanceTransform( currentSearchSpaceSize,
                             deltaDisplacement,
                             costs,
                             displacementIndices );

      // computing the argmin for each displacement (=label)
      for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
      {

        allDisplacementIndices[ childNodeIndex +
                                d * currentFlowFieldPointCount ] =
          displacementIndices[ d ];

      }

      for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
      {

        // adding mincost to the parent node
        displacementCosts[ parentNodeIndex +
                           d * currentFlowFieldPointCount ] +=
          costs[ d ];

      }

      -- index;

    }

    std::cout << "attributeOptimalDisplacements : 4" << std::endl;

    // computing the MST cost and selecting the displacement for the root node
    int32_t rootNodeIndex = minimumSpanningTree.getNodeIndex( 0 );
    const gkg::Vector3d< int32_t >& 
      rootVoxel = minimumSpanningTree.getVoxelFromIndex( rootNodeIndex );

    for ( d = 0; d < currentSearchSpaceVoxelCount; d++ )
    {

      costs[ d ] = displacementCosts[ rootNodeIndex +
                                      d * currentFlowFieldPointCount ];
      allDisplacementIndices[ rootNodeIndex +
                              d * currentFlowFieldPointCount ] = d;

    }

    std::cout << "attributeOptimalDisplacements : 5" << std::endl;

    // looking for the index corresponding to the minimum value
    int32_t minimumDisplacementIndex = 0;
    gkg::MinimumIndexFilter< std::vector< float >, int32_t >
      minimumIndexFilter;
    minimumIndexFilter.filter( costs, minimumDisplacementIndex );

    std::vector< int32_t > 
                        selectedDisplacementIndices( 
                                             currentFlowFieldPointCount, 0 );
    std::vector< int32_t >::iterator
      sd = selectedDisplacementIndices.begin(),
      sde = selectedDisplacementIndices.end();
    while ( sd != sde )
    {

      *sd = 0;
      ++sd;

    }

    std::cout << "attributeOptimalDisplacements : 6" << std::endl;

    selectedDisplacementIndices[ rootNodeIndex ] = minimumDisplacementIndex;

    coarserFlowFieldAlongX( rootVoxel ) =
                                    currentFlowFieldAlongX( rootVoxel ) +
                                    displacements[ minimumDisplacementIndex ].x;
    coarserFlowFieldAlongY( rootVoxel ) =
                                    currentFlowFieldAlongY( rootVoxel ) +
                                    displacements[ minimumDisplacementIndex ].y;
    coarserFlowFieldAlongZ( rootVoxel ) =
                                    currentFlowFieldAlongZ( rootVoxel ) +
                                    displacements[ minimumDisplacementIndex ].z;

    for ( index = 1; index < currentFlowFieldPointCount; index++ )
    {

      // collecting the child node index
      childNodeIndex = minimumSpanningTree.getNodeIndex( index );

      // collecting the parent node index of the child
      parentNodeIndex = minimumSpanningTree.getParentNodeIndex( childNodeIndex );

      // computing the displacement index corresponding to the minimum cost
      minimumDisplacementIndex =
       allDisplacementIndices[ childNodeIndex +
                               selectedDisplacementIndices[ parentNodeIndex ] * 
                               currentFlowFieldPointCount ];

      // storing the displacement index corresponding to the minimum cost
      selectedDisplacementIndices[ childNodeIndex ] = minimumDisplacementIndex;

      // collecting the voxel corresponding to the current node index
      const gkg::Vector3d< int32_t >& 
        childVoxel = minimumSpanningTree.getVoxelFromIndex( childNodeIndex );

      // updating the coarser flow field
      coarserFlowFieldAlongX( childVoxel ) =
                                    currentFlowFieldAlongX( childVoxel ) +
                                    displacements[ minimumDisplacementIndex ].x;
      coarserFlowFieldAlongY( childVoxel ) =
                                    currentFlowFieldAlongY( childVoxel ) +
                                    displacements[ minimumDisplacementIndex ].y;
      coarserFlowFieldAlongZ( childVoxel ) =
                                    currentFlowFieldAlongZ( childVoxel ) +
                                    displacements[ minimumDisplacementIndex ].z;

    }
    std::cout << "attributeOptimalDisplacements : 7" << std::endl;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::"
             "attributeOptimalDisplacements( "
             "const gkg::Volume< float >& currentFlowFieldAlongX, "
             "const gkg::Volume< float >& currentFlowFieldAlongY, "
             "const gkg::Volume< float >& currentFlowFieldAlongZ, "
             "const std::vector< float >& displacementCosts, "
             "int32_t currentHalfWidthSearchSpace, "
             "int32_t currentSearchSpaceQuantisation, "
             "const gkg::MinimumSpanningTree< uint8_t >& minimumSpanningTree, "
             "gkg::Volume< float >& coarserFlowFieldAlongX, "
             "gkg::Volume< float >& coarserFlowFieldAlongY, "
             "gkg::Volume< float >& coarserFlowFieldAlongZ ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::scaleAndSquareTransformation(
                                         gkg::Volume< float >& flowFieldAlongX,
                                         gkg::Volume< float >& flowFieldAlongY,
                                         gkg::Volume< float >& flowFieldAlongZ,
                                         int32_t currentControlPointSpacing,
                                         int32_t exponentiationStepCount ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowFieldAlongX.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    flowFieldAlongX.getResolution( flowFieldResolution );


    // allocating temporary volumes
    gkg::Volume< float > flowFieldAlongX1( flowFieldAlongX );
    gkg::Volume< float > flowFieldAlongY1( flowFieldAlongY );
    gkg::Volume< float > flowFieldAlongZ1( flowFieldAlongZ );

    gkg::Volume< float > flowFieldAlongX2( flowFieldSize );
    flowFieldAlongX2.setResolution( flowFieldResolution );
    gkg::Volume< float > flowFieldAlongY2( flowFieldSize );
    flowFieldAlongY2.setResolution( flowFieldResolution );
    gkg::Volume< float > flowFieldAlongZ2( flowFieldSize );
    flowFieldAlongZ2.setResolution( flowFieldResolution );

    // scaling the flow field(s) according to the exponentiation step count
    // and the current control point spacing
    gkg::Scaler< gkg::Volume< float > > 
      scaler1( 1.0f / ( ( float )currentControlPointSpacing * 
                        ( float )std::pow(2.0, exponentiationStepCount ) ) );

    scaler1.scale( flowFieldAlongX1 );
    scaler1.scale( flowFieldAlongY1 );
    scaler1.scale( flowFieldAlongZ1 );


    int32_t exponentiationStepIndex = 0;
    for ( exponentiationStepIndex = 0;
          exponentiationStepIndex < exponentiationStepCount;
          exponentiationStepIndex++ )
    {

      // combining deformations
      combineDeformations3d( flowFieldAlongX1,
                             flowFieldAlongY1,
                             flowFieldAlongZ1,
                             flowFieldAlongX1,
                             flowFieldAlongY1,
                             flowFieldAlongZ1,
                             flowFieldAlongX2,
                             flowFieldAlongY2,
                             flowFieldAlongZ2 );

      // copying back result to input
      flowFieldAlongX1 = flowFieldAlongX2;
      flowFieldAlongY1 = flowFieldAlongY2;
      flowFieldAlongZ1 = flowFieldAlongZ2;

    }


    flowFieldAlongX = flowFieldAlongX2;
    flowFieldAlongY = flowFieldAlongY2;
    flowFieldAlongZ = flowFieldAlongZ2;

    gkg::Scaler< gkg::Volume< float > >
      scaler2( ( float )currentControlPointSpacing ); 

    scaler2.scale( flowFieldAlongX );
    scaler2.scale( flowFieldAlongY );
    scaler2.scale( flowFieldAlongZ );

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::"
             "scaleAndSquareTransformation( "
             "gkg::Volume< float >& flowFieldAlongX, "
             "gkg::Volume< float >& flowFieldAlongY, "
             "gkg::Volume< float >& flowFieldAlongZ, "
             "int32_t currentControlPointSpacing, "
             "int32_t exponentiationStepCount ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::
                                      symmetrizeDirectAndInverseTransformations(
                                   gkg::Volume< float >& directFlowFieldAlongX,
                                   gkg::Volume< float >& directFlowFieldAlongY,
                                   gkg::Volume< float >& directFlowFieldAlongZ,
                                   gkg::Volume< float >& inverseFlowFieldAlongX,
                                   gkg::Volume< float >& inverseFlowFieldAlongY,
                                   gkg::Volume< float >& inverseFlowFieldAlongZ,
                                   int32_t currentControlPointSpacing ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    directFlowFieldAlongX.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    directFlowFieldAlongX.getResolution( flowFieldResolution );


    // allocating temporary volume(s)
    gkg::Volume< float > symDirectFlowFieldAlongX( directFlowFieldAlongX );
    gkg::Volume< float > symDirectFlowFieldAlongY( directFlowFieldAlongY );
    gkg::Volume< float > symDirectFlowFieldAlongZ( directFlowFieldAlongZ );
    gkg::Volume< float > symInverseFlowFieldAlongX( inverseFlowFieldAlongX );
    gkg::Volume< float > symInverseFlowFieldAlongY( inverseFlowFieldAlongY );
    gkg::Volume< float > symInverseFlowFieldAlongZ( inverseFlowFieldAlongZ );

    // scaling the flow field(s) according to the current control point spacing
    gkg::Scaler< gkg::Volume< float > > 
      scaler1( 0.5f / ( ( float )currentControlPointSpacing ) );

    scaler1.scale( symDirectFlowFieldAlongX );
    scaler1.scale( symDirectFlowFieldAlongY );
    scaler1.scale( symDirectFlowFieldAlongZ );

    scaler1.scale( symInverseFlowFieldAlongX );
    scaler1.scale( symInverseFlowFieldAlongY );
    scaler1.scale( symInverseFlowFieldAlongZ );

    // computing the symmetric direct and inverse flow field(s)
    gkg::Volume< float > tmpInverseFlowFieldAlongX1( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongY1( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongZ1( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongX2( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongY2( flowFieldSize );
    gkg::Volume< float > tmpInverseFlowFieldAlongZ2( flowFieldSize );

    tmpInverseFlowFieldAlongX1.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongY1.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongZ1.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongX2.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongY2.setResolution( flowFieldResolution );
    tmpInverseFlowFieldAlongZ2.setResolution( flowFieldResolution );

    this->fastInverseTransform( symDirectFlowFieldAlongX,
                                symDirectFlowFieldAlongY,
                                symDirectFlowFieldAlongZ,
                                tmpInverseFlowFieldAlongX1,
                                tmpInverseFlowFieldAlongY1,
                                tmpInverseFlowFieldAlongZ1 );
                          

    this->fastInverseTransform( symInverseFlowFieldAlongX,
                                symInverseFlowFieldAlongY,
                                symInverseFlowFieldAlongZ,
                                tmpInverseFlowFieldAlongX2,
                                tmpInverseFlowFieldAlongY2,
                                tmpInverseFlowFieldAlongZ2 );

    // combining deformations to symmetrize them
    this->combineDeformations3d( tmpInverseFlowFieldAlongX2,
                                 tmpInverseFlowFieldAlongY2,
                                 tmpInverseFlowFieldAlongZ2,
                                 symDirectFlowFieldAlongX,
                                 symDirectFlowFieldAlongY,
                                 symDirectFlowFieldAlongZ,
                                 directFlowFieldAlongX,
                                 directFlowFieldAlongY,
                                 directFlowFieldAlongZ );

    this->combineDeformations3d( tmpInverseFlowFieldAlongX1,
                                 tmpInverseFlowFieldAlongY1,
                                 tmpInverseFlowFieldAlongZ1,
                                 symInverseFlowFieldAlongX,
                                 symInverseFlowFieldAlongY,
                                 symInverseFlowFieldAlongZ,
                                 inverseFlowFieldAlongX,
                                 inverseFlowFieldAlongY,
                                 inverseFlowFieldAlongZ );

    // scaling the flow field(s) according to the current control point spacing
    gkg::Scaler< gkg::Volume< float > > 
      scaler2( ( float )currentControlPointSpacing );

    scaler2.scale( directFlowFieldAlongX );
    scaler2.scale( directFlowFieldAlongY );
    scaler2.scale( directFlowFieldAlongZ );

    scaler2.scale( inverseFlowFieldAlongX );
    scaler2.scale( inverseFlowFieldAlongY );
    scaler2.scale( inverseFlowFieldAlongZ );

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >:: "
             "symmetrizeDirectAndInverseTransformations( "
             "gkg::Volume< float >& directFlowFieldAlongX, "
             "gkg::Volume< float >& directFlowFieldAlongY, "
             "gkg::Volume< float >& directFlowFieldAlongZ, "
             "gkg::Volume< float >& inverseFlowFieldAlongX, "
             "gkg::Volume< float >& inverseFlowFieldAlongY, "
             "gkg::Volume< float >& inverseFlowFieldAlongZ, "
             "int32_t currentControlPointSpacing ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::computeJacobianDeterminant(
                                  const gkg::Volume< float >& flowFieldAlongX,
                                  const gkg::Volume< float >& flowFieldAlongY,
                                  const gkg::Volume< float >& flowFieldAlongZ,
                                  int32_t currentControlPointSpacing,
                                  float& jacobianMean,
                                  float& jacobianStandardDeviation,
                                  float& jacobianMinimum,
                                  float& jacobianMaximum,
                                  float& jacobianNegativeRatio ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowFieldAlongX.getSize( flowFieldSize );
    float sizeXYZ = ( float )flowFieldSize.x *
                    ( float )flowFieldSize.y *
                    ( float )flowFieldSize.z;

    // computing the gradient volumes along X, Y and Z
    gkg::Volume< float > J11( flowFieldSize );
    this->computeGradientAlongX( flowFieldAlongX, J11 );
    gkg::Volume< float > J12( flowFieldSize );
    this->computeGradientAlongY( flowFieldAlongX, J12 );
    gkg::Volume< float > J13( flowFieldSize );
    this->computeGradientAlongZ( flowFieldAlongX, J13 );

    gkg::Volume< float > J21( flowFieldSize );
    this->computeGradientAlongX( flowFieldAlongY, J21 );
    gkg::Volume< float > J22( flowFieldSize );
    this->computeGradientAlongY( flowFieldAlongY, J22 );
    gkg::Volume< float > J23( flowFieldSize );
    this->computeGradientAlongZ( flowFieldAlongY, J23 );

    gkg::Volume< float > J31( flowFieldSize );
    this->computeGradientAlongX( flowFieldAlongZ, J31 );
    gkg::Volume< float > J32( flowFieldSize );
    this->computeGradientAlongY( flowFieldAlongZ, J32 );
    gkg::Volume< float > J33( flowFieldSize );
    this->computeGradientAlongZ( flowFieldAlongZ, J33 );


    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          J11( x, y, z ) /= ( float )currentControlPointSpacing;
          J11( x, y, z ) += 1.0f;
          J12( x, y, z ) /= ( float )currentControlPointSpacing;
          J13( x, y, z ) /= ( float )currentControlPointSpacing;
          J21( x, y, z ) /= ( float )currentControlPointSpacing;
          J22( x, y, z ) /= ( float )currentControlPointSpacing;
          J22( x, y, z ) += 1.0f;
          J23( x, y, z ) /= ( float )currentControlPointSpacing;
          J31( x, y, z ) /= ( float )currentControlPointSpacing;
          J32( x, y, z ) /= ( float )currentControlPointSpacing;
          J33( x, y, z ) /= ( float )currentControlPointSpacing;
          J33( x, y, z ) += 1.0f;

        }

      }

    }


    // creating an output volume for the Jacobian and filling it with zeros
    gkg::Volume< float > Jacobian( flowFieldSize );
    Jacobian.fill( 0.0f );

    // computing the Jacobian from the flow field gradient(s)
    float J = 0.0f;
    jacobianMean = 0.0f;
    jacobianMinimum = 1.0f;
    jacobianMaximum = 1.0f;
    jacobianNegativeRatio = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          J = J11( x, y, z ) * ( J22( x, y, z ) * J33( x, y, z ) - 
                                 J23( x, y, z ) * J32( x, y, z ) ) -
              J21( x, y, z ) * ( J12( x, y, z ) * J33( x, y, z ) - 
                                 J13( x, y, z ) * J32( x, y, z ) ) +
              J31( x, y, z ) * ( J12( x, y, z ) * J23( x, y, z ) - 
                                 J13( x, y, z ) * J22( x, y, z ) );
          jacobianMean += J;
          if ( J > jacobianMaximum )
          {

            jacobianMaximum = J;

          }
          else if ( J < jacobianMinimum )
          {

            jacobianMinimum = J;

          }
          if ( J < 0.0f )
          {

            ++ jacobianNegativeRatio;

          }
          Jacobian( x, y, z ) = J;

        }

      }

    }

    jacobianMean /= sizeXYZ;

    float jacobianDifference = 0.0f;
    float jacobianVariance = 0.0f;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

           jacobianDifference = Jacobian( x, y, z ) - jacobianMean;
           jacobianVariance += jacobianDifference * jacobianDifference;
          

        }

      }

    }
    jacobianStandardDeviation = std::sqrt( jacobianVariance / 
                                           ( sizeXYZ - 1.0f ) );
    
    jacobianNegativeRatio /= sizeXYZ;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::"
             "computeJacobianDeterminant( "
             "const gkg::Volume< float >& flowFieldAlongX, "
             "const gkg::Volume< float >& flowFieldAlongY, "
             "const gkg::Volume< float >& flowFieldAlongZ, "
             "int32_t currentControlPointSpacing, "
             "float& jacobianMean, "
             "float& jacobianStandardDeviation, "
             "float& jacobianMinimum, "
             "float& jacobianMaximum, "
             "float& jacobianNegativeRatio ) const" );

}


template < class T1, class T2 >
inline
float gkg::RegistrationAlgorithm< T1, T2 >::computeHarmonicEnergy(
                             const gkg::Volume< float >& flowFieldAlongX,
                             const gkg::Volume< float >& flowFieldAlongY,
                             const gkg::Volume< float >& flowFieldAlongZ ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowFieldAlongX.getSize( flowFieldSize );
    float sizeXYZ = ( float )flowFieldSize.x *
                    ( float )flowFieldSize.y *
                    ( float )flowFieldSize.z;

    float harmonicEnergy = 0.0f;
    float sumOfSquare = 0.0f;
    gkg::Volume< float > flowFieldGradient( flowFieldSize );
    gkg::SumOfSquareFilter< gkg::Volume< float >, float > sumOfSquareFilter;

    ////////////////////////////////////////////////////////////////////////////
    // gradients of flowFieldAlongX
    ////////////////////////////////////////////////////////////////////////////

    // flow field gradient of flowFieldAlongX along X
    this->computeGradientAlongX( flowFieldAlongX, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongX along Y
    this->computeGradientAlongY( flowFieldAlongX, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongX along Z
    this->computeGradientAlongZ( flowFieldAlongX, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    ////////////////////////////////////////////////////////////////////////////
    // gradients of flowFieldAlongY
    ////////////////////////////////////////////////////////////////////////////

    // flow field gradient of flowFieldAlongY along X
    this->computeGradientAlongX( flowFieldAlongY, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongY along Y
    this->computeGradientAlongY( flowFieldAlongY, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongY along Z
    this->computeGradientAlongZ( flowFieldAlongY, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    ////////////////////////////////////////////////////////////////////////////
    // gradients of flowFieldAlongZ
    ////////////////////////////////////////////////////////////////////////////

    // flow field gradient of flowFieldAlongZ along X
    this->computeGradientAlongX( flowFieldAlongZ, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongZ along Y
    this->computeGradientAlongY( flowFieldAlongZ, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    // flow field gradient of flowFieldAlongZ along Z
    this->computeGradientAlongZ( flowFieldAlongZ, flowFieldGradient );
    sumOfSquareFilter.filter( flowFieldGradient, sumOfSquare );
    harmonicEnergy += sumOfSquare;

    return harmonicEnergy / sizeXYZ;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "float gkg::RegistrationAlgorithm< T1, T2 >::"
             "computeHarmonicEnergy( "
             "const gkg::Volume< float >& flowFieldAlongX, "
             "const gkg::Volume< float >& flowFieldAlongY, "
             "const gkg::Volume< float >& flowFieldAlongZ ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::getDisplacements(  
                    int32_t currentSearchSpaceSize,
                    int32_t currentSearchSpaceVoxelCount,
                    int32_t currentHalfWidthSearchSpace,
                    int32_t currentSearchSpaceQuantisation,
                    std::vector< gkg::Vector3d< float > >& displacements ) const
{

  try
  {

    displacements.resize( currentSearchSpaceVoxelCount );

    int32_t sx = 0;
    int32_t sy = 0;
    int32_t sz = 0;
    int32_t displacementIndex = 0;
    for ( sz = 0; sz < currentSearchSpaceSize; sz++ )
    {

      for ( sy = 0; sy < currentSearchSpaceSize; sy++ )
      {

        for ( sx = 0; sx < currentSearchSpaceSize; sx++ )
        {

          displacements[ displacementIndex ].x =
                                 ( float )( sx - currentHalfWidthSearchSpace ) *
                                 currentSearchSpaceQuantisation;
          displacements[ displacementIndex ].y =
                                 ( float )( sy - currentHalfWidthSearchSpace ) *
                                 currentSearchSpaceQuantisation;
          displacements[ displacementIndex ].z =
                                 ( float )( sz - currentHalfWidthSearchSpace ) *
                                 currentSearchSpaceQuantisation;

          ++ displacementIndex;

        }

      }

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::getDisplacements( "
             "int32_t currentSearchSpaceSize, "
             "int32_t currentSearchSpaceVoxelCount, "
             "int32_t currentHalfWidthSearchSpace, "
             "int32_t currentSearchSpaceQuantisation, "
             "std::vector< gkg::Vector3d< float > >& displacements ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::fastDistanceTransform(
                             int32_t currentSearchSpaceSize,
                             const gkg::Vector3d< float >& deltaDisplacement,
                             std::vector< float >& costs,
                             std::vector< int32_t >& displacementIndices ) const
{

  try
  {

    int32_t currentSearchSpaceVoxelCount = currentSearchSpaceSize *
                                           currentSearchSpaceSize *
                                           currentSearchSpaceSize;
    int32_t currentSearchSpacePlaneVoxelCount = currentSearchSpaceSize *
                                                currentSearchSpaceSize;

    int32_t spi = 0;
    for( spi = 0; spi < currentSearchSpaceVoxelCount; spi++ )
    {
	
      displacementIndices[ spi ] = spi;
	
    }

    std::vector< int32_t > v( currentSearchSpaceSize, 0 );
    std::vector< int32_t >::iterator
     iv = v.begin(),
     ive = v.end();
    while ( iv != ive )
    {

      *iv = 0;
      ++ iv;

    }

    std::vector< float > z( currentSearchSpaceSize + 1, 0.0f );
    std::vector< float >::iterator
     iz = z.begin(),
     ize = z.end();
    while ( iz != ize )
    {

      *iz = 0.0f;
      ++ iz;

    }

    std::vector< float > f( currentSearchSpaceSize, 0.0f );
    std::vector< float >::iterator
     itf = f.begin(),
     itfe = f.end();
    while ( itf != itfe )
    {

      *itf = 0.0f;
      ++ itf;

    }

    std::vector< int32_t > i1( currentSearchSpaceSize, 0 ); 
    std::vector< int32_t >::iterator
     ii1 = i1.begin(),
     ii1e = i1.end();
    while ( ii1 != ii1e )
    {

      *ii1 = 0;
      ++ ii1;

    }

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    for( k = 0; k < currentSearchSpaceSize; k++ )
    {

      for( i = 0; i < currentSearchSpaceSize; i++ )
      {
	
        this->fastDistanceTransformSquare(
                                     i + k * currentSearchSpacePlaneVoxelCount,
                                     currentSearchSpaceSize,
                                     -deltaDisplacement.x,
                                     currentSearchSpaceSize,
                                     v,
                                     z,
                                     f,
                                     i1,
                                     costs,
                                     displacementIndices );

      }

    }

    for( k = 0; k < currentSearchSpaceSize; k++ )
    {

      for( j = 0; j < currentSearchSpaceSize; j++ )
      {
	
        this->fastDistanceTransformSquare(
                                     j * currentSearchSpaceSize +
                                     k * currentSearchSpacePlaneVoxelCount,
                                     currentSearchSpaceSize,
                                     -deltaDisplacement.y,
                                     1,
                                     v,
                                     z,
                                     f,
                                     i1,
                                     costs,
                                     displacementIndices );

      }

    }

    for( j = 0; j < currentSearchSpaceSize; j++ )
    {

      for( i = 0; i < currentSearchSpaceSize; i++ )
      {
	
        this->fastDistanceTransformSquare(
                                     i + j * currentSearchSpaceSize,
                                     currentSearchSpaceSize,
                                     -deltaDisplacement.z,
                                     currentSearchSpacePlaneVoxelCount,
                                     v,
                                     z,
                                     f,
                                     i1,
                                     costs,
                                     displacementIndices );

      }

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::"
             "fastDistanceTransform( "
             "int32_t currentSearchSpaceSize, "
             "const gkg::Vector3d< float >& deltaDisplacement, "
             "std::vector< float >& costs, "
             "std::vector< int32_t >& displacementIndices ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::fastDistanceTransformSquare(
                             int32_t index,
                             int32_t length,
                             float displacement,
                             int32_t offsetToNextElement,
                             std::vector< int32_t >& v,
                             std::vector< float >& z,
                             std::vector< float >& f,
                             std::vector< int32_t >& ind1,
                             std::vector< float >& costs,
                             std::vector< int32_t >& displacementIndices ) const
{

  try
  {

    float infinity = std::numeric_limits< float >::max();

    int32_t j = 0;
    z[ 0 ] = -infinity;
    z[ 1 ] = infinity;
    v[ 0 ] = 0;

    int32_t q = 0;
    for ( q = 1; q < length; q++ )
    {
	
      float s = ( ( costs[ index + q * offsetToNextElement ] + 
                    ( ( float )q + displacement ) *
                    ( ( float )q + displacement ) ) -
                    ( costs[ index + v[ j ] * offsetToNextElement ] + 
                    ( ( float )v[ j ] + displacement ) *
                    ( ( float )v[ j ] + displacement ) ) ) / 
                ( 2.0 * ( float )( q - v[ j ] ) );
		
      while ( s <= z[ j ] )
      {
	
        --j;
		
	s = ( ( costs[ index + q * offsetToNextElement ] + 
                ( ( float )q + displacement ) *
                ( ( float )q + displacement ) ) -
              ( costs[ index + v[ j ] * offsetToNextElement ] + 
                ( ( float )v[ j ] + displacement ) *
                ( ( float )v[ j ] + displacement ) ) ) / 
            ( 2.0 * ( float )( q - v[ j ] ) );

      }
		
      ++j;
      v[ j ] = q;
      z[ j ] = s;
      z[ j + 1 ] = infinity;
	
    }

    j = 0;
    for ( q = 0; q < length; q++ )
    {
	
      f[ q ] = costs[ index + q * offsetToNextElement ];
      ind1[ q ] = displacementIndices[ index + q * offsetToNextElement ];
	
    } 

    for ( q = 0; q < length; q++ )
    {
	
      while ( z[ j + 1 ] < q )
      {
		
	++j;

      }
		
      displacementIndices[ index + q * offsetToNextElement ] = ind1[ v[ j ] ];
      costs[ index + q * offsetToNextElement ] =
        ( ( float )q - ( ( float )v[ j ] + displacement ) ) *
        ( ( float )q - ( ( float )v[ j ] + displacement ) ) + f[ v[ j ] ];
	
    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::"
             "fastDistanceTransformSquare( "
             "int32_t index, "
             "int32_t length, "
             "float displacement, "
             "int32_t offsetToNextElement, "
             "std::vector< int32_t >& v, "
             "std::vector< float >& z, "
             "std::vector< float >& f, "
             "std::vector< int32_t >& ind1, "
             "std::vector< float >& costs, "
             "std::vector< int32_t >& displacementIndices ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::getInterpolation3d(
                                    const gkg::Volume< float >& flowField,
                                    const gkg::Volume< float >& flowFieldAlongX,
                                    const gkg::Volume< float >& flowFieldAlongY,
                                    const gkg::Volume< float >& flowFieldAlongZ,
                                    gkg::Volume< float >& flowFieldPrime ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    flowField.getResolution( flowFieldResolution );

    float flowFieldValueAlongX = 0.0f;
    float flowFieldValueAlongY = 0.0f; 
    float flowFieldValueAlongZ = 0.0f; 

    int32_t floorFlowFieldValueAlongX = 0.0f; 
    int32_t floorFlowFieldValueAlongY = 0.0f; 
    int32_t floorFlowFieldValueAlongZ = 0.0f;

    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    for( k = 0; k < flowFieldSize.z; ++k )
    {
		
      for( j = 0; j < flowFieldSize.y; ++j )
      {
			
        for( i = 0; i < flowFieldSize.x; ++i )
        {

          flowFieldValueAlongX = flowFieldAlongX( i, j, k );
          flowFieldValueAlongY = flowFieldAlongY( i, j, k );
          flowFieldValueAlongZ = flowFieldAlongZ( i, j, k );

          floorFlowFieldValueAlongX = std::floor( flowFieldValueAlongX );
          floorFlowFieldValueAlongY = std::floor( flowFieldValueAlongY );
          floorFlowFieldValueAlongZ = std::floor( flowFieldValueAlongZ );
  
          dx = flowFieldValueAlongX - floorFlowFieldValueAlongX;
          dy = flowFieldValueAlongY - floorFlowFieldValueAlongY;
          dz = flowFieldValueAlongZ - floorFlowFieldValueAlongZ;

          flowFieldPrime( i, j, k ) =
            ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +
 
            ( 1.0f - dx ) * dy * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +

            ( 1.0f - dx ) * ( 1.0f - dy ) * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * dy * ( 1.0f - dz ) * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ, 0 ),
                        flowFieldSize.z - 1 ) ) +

            ( 1.0f - dx ) * dy * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * ( 1.0f - dy ) * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) ) +

            dx * dy * dz * 
            flowField(
              std::min( std::max( i + floorFlowFieldValueAlongX + 1, 0 ),
                        flowFieldSize.x - 1 ),
              std::min( std::max( j + floorFlowFieldValueAlongY + 1, 0 ),
                        flowFieldSize.y - 1 ),
              std::min( std::max( k + floorFlowFieldValueAlongZ + 1, 0 ),
                        flowFieldSize.z - 1 ) );

        }

      }

    }
    
  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::getInterpolation3d( "
             "const gkg::Volume< float >& flowField, "
             "const gkg::Volume< float >& flowFieldAlongX, "
             "const gkg::Volume< float >& flowFieldAlongY, "
             "const gkg::Volume< float >& flowFieldAlongZ, "
             "const gkg::Volume< float >& flowFieldPrime ) const" );


}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::combineDeformations3d(
                           const gkg::Volume< float >& flowFieldAlongX1,
                           const gkg::Volume< float >& flowFieldAlongY1,
                           const gkg::Volume< float >& flowFieldAlongZ1,
                           const gkg::Volume< float >& flowFieldAlongX2,
                           const gkg::Volume< float >& flowFieldAlongY2,
                           const gkg::Volume< float >& flowFieldAlongZ2,
                           gkg::Volume< float >& combinedFlowFieldAlongX,
                           gkg::Volume< float >& combinedFlowFieldAlongY,
                           gkg::Volume< float >& combinedFlowFieldAlongZ ) const
{

  try
  {

    // combining deformations
    this->getInterpolation3d( flowFieldAlongX1,
                              flowFieldAlongX2,
                              flowFieldAlongY2,
                              flowFieldAlongZ2,
                              combinedFlowFieldAlongX );

    this->getInterpolation3d( flowFieldAlongY1,
                              flowFieldAlongX2,
                              flowFieldAlongY2,
                              flowFieldAlongZ2,
                              combinedFlowFieldAlongY );

    this->getInterpolation3d( flowFieldAlongZ1,
                              flowFieldAlongX2,
                              flowFieldAlongY2,
                              flowFieldAlongZ2,
                              combinedFlowFieldAlongZ );

    gkg::Volume< float >::const_iterator 
      fX2 = flowFieldAlongX2.begin(),
      fX2e = flowFieldAlongX2.end(),
      fY2 = flowFieldAlongY2.begin(),
      fZ2 = flowFieldAlongZ2.begin();
    gkg::Volume< float >::iterator 
      fXp = combinedFlowFieldAlongX.begin(),
      fYp = combinedFlowFieldAlongY.begin(),
      fZp = combinedFlowFieldAlongZ.begin();
    while ( fX2 != fX2e )
    {

      *fXp += *fX2;
      *fYp += *fY2;
      *fZp += *fZ2;

      ++ fX2;
      ++ fY2;
      ++ fZ2;
      ++ fXp;
      ++ fYp;
      ++ fZp;

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::"
             "combineDeformations3d( "
             "const gkg::Volume< float >& flowFieldAlongX1, "
             "const gkg::Volume< float >& flowFieldAlongY1, "
             "const gkg::Volume< float >& flowFieldAlongZ1, "
             "const gkg::Volume< float >& flowFieldAlongX2, "
             "const gkg::Volume< float >& flowFieldAlongY2, "
             "const gkg::Volume< float >& flowFieldAlongZ2, "
             "gkg::Volume< float >& combinedFlowFieldAlongX, "
             "gkg::Volume< float >& combinedFlowFieldAlongY, "
             "gkg::Volume< float >& combinedFlowFieldAlongZ ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::fastInverseTransform(
                            const gkg::Volume< float >& directFlowFieldAlongX,
                            const gkg::Volume< float >& directFlowFieldAlongY,
                            const gkg::Volume< float >& directFlowFieldAlongZ,
                            gkg::Volume< float >& inverseFlowFieldAlongX,
                            gkg::Volume< float >& inverseFlowFieldAlongY,
                            gkg::Volume< float >& inverseFlowFieldAlongZ ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    directFlowFieldAlongX.getSize( flowFieldSize );

    // collecting flow field resolution
    gkg::Vector3d< double > flowFieldResolution;
    directFlowFieldAlongX.getResolution( flowFieldResolution );

    // resetting inverse flow field(s) to zero
    inverseFlowFieldAlongX.fill( 0.0f );
    inverseFlowFieldAlongY.fill( 0.0f );
    inverseFlowFieldAlongZ.fill( 0.0f );

    // allocating temporary volume(s)
    gkg::Volume< float > newDirectFlowFieldAlongX( flowFieldSize );
    gkg::Volume< float > newDirectFlowFieldAlongY( flowFieldSize );
    gkg::Volume< float > newDirectFlowFieldAlongZ( flowFieldSize );
    newDirectFlowFieldAlongX.setResolution( flowFieldResolution );
    newDirectFlowFieldAlongY.setResolution( flowFieldResolution );
    newDirectFlowFieldAlongZ.setResolution( flowFieldResolution );

    gkg::Volume< float > newInverseFlowFieldAlongX( flowFieldSize );
    gkg::Volume< float > newInverseFlowFieldAlongY( flowFieldSize );
    gkg::Volume< float > newInverseFlowFieldAlongZ( flowFieldSize );
    newInverseFlowFieldAlongX.setResolution( flowFieldResolution );
    newInverseFlowFieldAlongY.setResolution( flowFieldResolution );
    newInverseFlowFieldAlongZ.setResolution( flowFieldResolution );


    // initializing the temporary inverse flow field by taking the opposit of
    // the direct flow field
    gkg::Volume< float >::const_iterator 
      dfX = directFlowFieldAlongX.begin(),
      dfXe = directFlowFieldAlongX.end(),
      dfY = directFlowFieldAlongY.begin(),
      dfZ = directFlowFieldAlongZ.begin();
    gkg::Volume< float >::iterator 
      ifX = newInverseFlowFieldAlongX.begin(),
      ifY = newInverseFlowFieldAlongY.begin(),
      ifZ = newInverseFlowFieldAlongZ.begin();
    while ( dfX != dfXe )
    {

      *ifX = -*dfX;
      *ifY = -*dfY;
      *ifZ = -*dfZ;

      ++ dfX;
      ++ dfY;
      ++ dfZ;
      ++ ifX;
      ++ ifY;
      ++ ifZ;

    }

    int32_t iteration = 0;
    for ( iteration = 0;
          iteration < FAST_INVERSE_TRANSFORM_MAXIMUM_ITERATION_COUNT;
          iteration++ )
    {
 
      this->getInterpolation3d( newInverseFlowFieldAlongX,
                                inverseFlowFieldAlongX,
                                inverseFlowFieldAlongY,
                                inverseFlowFieldAlongZ,
                                newDirectFlowFieldAlongX );
      this->getInterpolation3d( newInverseFlowFieldAlongY,
                                inverseFlowFieldAlongX,
                                inverseFlowFieldAlongY,
                                inverseFlowFieldAlongZ,
                                newDirectFlowFieldAlongY );
      this->getInterpolation3d( newInverseFlowFieldAlongZ,
                                inverseFlowFieldAlongX,
                                inverseFlowFieldAlongY,
                                inverseFlowFieldAlongZ,
                                newDirectFlowFieldAlongZ );

      inverseFlowFieldAlongX = newDirectFlowFieldAlongX;
      inverseFlowFieldAlongY = newDirectFlowFieldAlongY;
      inverseFlowFieldAlongZ = newDirectFlowFieldAlongZ;

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void gkg::RegistrationAlgorithm< T1, T2 >::fastInverseTransform( "
             "const gkg::Volume< float >& directFlowFieldAlongX, "
             "const gkg::Volume< float >& directFlowFieldAlongY, "
             "const gkg::Volume< float >& directFlowFieldAlongZ, "
             "gkg::Volume< float >& inverseFlowFieldAlongX, "
             "gkg::Volume< float >& inverseFlowFieldAlongY, "
             "gkg::Volume< float >& inverseFlowFieldAlongZ ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::computeGradientAlongX(
                           const gkg::Volume< float >& flowField,
                           gkg::Volume< float >& flowFieldGradientAlongX ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    flowFieldGradientAlongX.fill( 0.0f);

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          flowFieldGradientAlongX( x, y, z ) =
          ( flowField( std::max( std::min( x + 1, flowFieldSize.x - 1  ), 0 ),
                       y,
                       z ) -
            flowField( std::max( std::min( x - 1, flowFieldSize.x - 1  ), 0 ),
                       y,
                       z ) ) / 2.0f;

        }

      }

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void "
             "gkg::RegistrationAlgorithm< T1, T2 >::computeGradientAlongX( "
             "const gkg::Volume< float >& flowField, "
             "gkg::Volume< float >& flowFieldGradientAlongX ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::computeGradientAlongY(
                           const gkg::Volume< float >& flowField,
                           gkg::Volume< float >& flowFieldGradientAlongY ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    flowFieldGradientAlongY.fill( 0.0f);

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          flowFieldGradientAlongY( x, y, z ) =
          ( flowField( x,
                       std::max( std::min( y + 1, flowFieldSize.y - 1  ), 0 ),
                       z ) -
            flowField( x,
                       std::max( std::min( y - 1, flowFieldSize.y - 1  ), 0 ),
                       z ) ) / 2.0f;

        }

      }

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void "
             "gkg::RegistrationAlgorithm< T1, T2 >::computeGradientAlongY( "
             "const gkg::Volume< float >& flowField, "
             "gkg::Volume< float >& flowFieldGradientAlongY ) const" );

}


template < class T1, class T2 >
inline
void gkg::RegistrationAlgorithm< T1, T2 >::computeGradientAlongZ(
                           const gkg::Volume< float >& flowField,
                           gkg::Volume< float >& flowFieldGradientAlongZ ) const
{

  try
  {

    // collecting flow field size
    gkg::Vector3d< int32_t > flowFieldSize;
    flowField.getSize( flowFieldSize );

    flowFieldGradientAlongZ.fill( 0.0f);

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < flowFieldSize.z; z++ )
    {

      for ( y = 0; y < flowFieldSize.y; y++ )
      {

        for ( x = 0; x < flowFieldSize.x; x++ )
        {

          flowFieldGradientAlongZ( x, y, z ) =
          ( flowField( x,
                       y,
                       std::max( std::min( z + 1, flowFieldSize.z - 1  ), 0 ) )-
            flowField( x,
                       y,
                       std::max( std::min( z - 1, flowFieldSize.z - 1  ), 0 ) )
          ) / 2.0f;

        }

      }

    }

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void "
             "gkg::RegistrationAlgorithm< T1, T2 >::computeGradientAlongZ( "
             "const gkg::Volume< float >& flowField, "
             "gkg::Volume< float >& flowFieldGradientAlongZ ) const" );

}


template < class T1, class T2 >
inline
float gkg::RegistrationAlgorithm< T1, T2 >::computeMutualInformation(
                                   const gkg::Matrix& jointProbabilities ) const
{

  try
  {

    // processing the reference marginal probabilities
    static gkg::Vector
      referenceMarginalProbabilities( DIFFEO_MUTUAL_INFO_LEVEL_COUNT );
    static gkg::Vector
      floatingMarginalProbabilities( DIFFEO_MUTUAL_INFO_LEVEL_COUNT );

    referenceMarginalProbabilities.setZero();
    floatingMarginalProbabilities.setZero();

    int32_t referenceLevel = 0;
    int32_t floatingLevel = 0;
    double probability = 0.0;
    for ( referenceLevel = 0;
          referenceLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
          referenceLevel++ )
    {

      for ( floatingLevel = 0;
            floatingLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
            floatingLevel++ )
      {

        probability = jointProbabilities( referenceLevel, floatingLevel );
        referenceMarginalProbabilities( referenceLevel ) += probability;
        floatingMarginalProbabilities( floatingLevel ) += probability;

      }

    }

    // processing the mutual information
    double referenceMarginalProbability = 0.0;
    double floatingMarginalProbability = 0.0;
    double jointProbability = 0.0;
    double mutualInformation = 0.0;
    for ( referenceLevel = 0;
          referenceLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
          referenceLevel++ )
    {

      referenceMarginalProbability = referenceMarginalProbabilities(
                                                               referenceLevel );
      if ( referenceMarginalProbability > 0.0 )
      {

        for ( floatingLevel = 0;
              floatingLevel < DIFFEO_MUTUAL_INFO_LEVEL_COUNT;
              floatingLevel++ )
        {

          floatingMarginalProbability = floatingMarginalProbabilities(
                                                                floatingLevel );
          jointProbability = jointProbabilities( referenceLevel,
                                                 floatingLevel );
           if ( ( floatingMarginalProbability > 0.0 ) &&
                ( jointProbability > 0.0 ) )
           {

             mutualInformation += jointProbability *
                                  std::log( jointProbability /
                                            ( referenceMarginalProbability *
                                              floatingMarginalProbability ) );

           }

        }

      }

    }

    return ( float )-mutualInformation;

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "float gkg::RegistrationAlgorithm< T1, T2 >::"
             "computeMutualInformation( "
             "const gkg::Matrix& jointProbabilities ) const" );

}


#undef FAST_INVERSE_TRANSFORM_MAXIMUM_ITERATION_COUNT

#endif
