#include <gkg-dmri-plugin-functors/DwiPostMortemBlockToWholeImageComposer/DwiPostMortemBlockToWholeImageComposerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadGauge.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-qspace-sampling/CustomMultipleDifferentShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/CustomSingleShellQSpaceSampling.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <gkg-processing-transform/Rotation3dFunctions.h>



//
// class DwiComposingGauge
//

class DwiComposingGauge : public gkg::ThreadGauge< int32_t >
{

  public: 
  
    DwiComposingGauge( int32_t maxCount ); 
    
    void add( int32_t value ); 
    void reset(); 
    
  private: 
  
    void display(); 
    
    int32_t _count; 
    int32_t _maxCount; 
    bool _alreadyDisplayed; 

};


DwiComposingGauge::DwiComposingGauge( int32_t maxCount )
                  : _count( 0 ),
                    _maxCount( maxCount ),
                    _alreadyDisplayed( false )
{
}


void DwiComposingGauge::add( int32_t value )
{

  try
  {
   
    _count += value; 
    
    display();
   
  }
  GKG_CATCH( "DwiComposingGauge::add( int32_t value )" );
  
}


void DwiComposingGauge::reset()
{

  try
  {
   
    _count = 0; 
   
  }
  GKG_CATCH( "DwiComposingGauge::reset()" );
  
}


void DwiComposingGauge::display()
{

  try
  {
    
    if ( _alreadyDisplayed )
    {
    
      std::cout << gkg::Eraser( 25 );
    
    }
    
    std::cout << std::setw( 5 ) << _count
              << " / "
              << std::setw( 5 ) << _maxCount
              << " x1000 sites" 
              << std::flush;

    _alreadyDisplayed = true; 
  
  }
  GKG_CATCH( "void DwiComposingGauge::display()" ); 

}


//
// class DwiComposingLoopContext
//

template < class T >
class DwiComposingLoopContext : public gkg::LoopContext< int32_t >
{

  public:

    DwiComposingLoopContext( 
                 DwiComposingGauge& dwiComposingGauge,
                 const std::vector< gkg::Vector3d< int32_t > >& voxelToTreat,
                 const gkg::Vector3d< int32_t >& referenceSizes,
                 const int32_t& dwCount,
                 const gkg::Vector3d< double >& referenceResolutions,
                 const double& currentFovScaling,
                 gkg::Resampler< T >* resampler,
                 const std::list< gkg::RCPointer< gkg::Transform3d< float > > >&
                                                                   transform3ds,
                 gkg::RCPointer< gkg::Volume< T > > t2FovVolume,
                 gkg::RCPointer< gkg::Volume< int16_t > > outputMaskVolume,
                 gkg::RCPointer< gkg::Volume< T > > dwFovVolume,
                 double background,
                 const gkg::BoundingBox< int32_t >& outputBoundingBox,
                 gkg::PartialVolumingQuickResampler< T >& quickResampler,
                 const std::vector< gkg::Matrix >& Ls,
                 const std::vector< gkg::Matrix >& Btargets,
                 const std::vector< double >& laplaceBeltramiRegularizationFactors,
                 const gkg::CustomMultipleDifferentShellQSpaceSampling& originQSpaceSampling,
                 const std::vector< int32_t >& sphericalHarmonicOrders,
                 int32_t globalMatrixSize,
                 const std::vector< float >& rawBValues,
                 const std::vector< gkg::Vector3d< float > >& rawOrientations,
                 float deltaBValue,
                 gkg::Volume< T >* fileNameOutputT2Volume,
                 gkg::Volume< T >* fileNameOutputDWVolume,
                 const std::vector< gkg::OrientationSet >& targetOrientationSets,
                 bool verbose );

    void doIt( int32_t startIndex, int32_t count );

  protected:

    bool composeItem(
                T& newOutputValue,
                const T& currentOutputValue,
                const T& outputValue,
                double currentFovScaling );

    void composeDwiItem(
                T& newOutputValue,
                const T& currentOutputValue,
                const T& outputValue,
                double currentFovScaling );

    const std::vector< gkg::Vector3d< int32_t > >& _voxelToTreat;
    const gkg::Vector3d< int32_t >& _referenceSizes;
    const int32_t& _dwCount;
    const gkg::Vector3d< double >& _referenceResolutions;
    const double& _currentFovScaling;
    gkg::Resampler< T >* _resampler;
    std::list< gkg::RCPointer< gkg::Transform3d< float > > > _transform3ds;
    gkg::RCPointer< gkg::Volume< T > > _t2FovVolume;
    gkg::RCPointer< gkg::Volume< int16_t > > _outputMaskVolume;
    gkg::RCPointer< gkg::Volume< T > > _dwFovVolume;
    double _background;
    const gkg::BoundingBox< int32_t >& _outputBoundingBox;
    gkg::PartialVolumingQuickResampler< T >& _quickResampler;
    const std::vector< gkg::Matrix >& _Ls;
    const std::vector< gkg::Matrix >& _Btargets;
    const std::vector< double >& _laplaceBeltramiRegularizationFactors;
    // gkg::RCPointer< gkg::CustomMultipleDifferentShellQSpaceSampling > _originQspaceSampling;
    const std::vector< int32_t >& _sphericalHarmonicOrders;
    int32_t _globalMatrixSize;
    const std::vector< float >& _rawBValues;
    const std::vector< gkg::Vector3d< float > >& _rawOrientations;
    float _deltaBValue;
    

    gkg::Volume< T >* _outputT2Volume;
    gkg::Volume< T >* _outputDWVolume;
    const std::vector< gkg::OrientationSet >& _targetOrientationSets;
    bool _verbose;

};


template < class T >
inline
DwiComposingLoopContext< T >::DwiComposingLoopContext( 
                 DwiComposingGauge& dwiComposingGauge,
                 const std::vector< gkg::Vector3d< int32_t > >& voxelToTreat,
                 const gkg::Vector3d< int32_t >& referenceSizes,
                 const int32_t& dwCount,
                 const gkg::Vector3d< double >& referenceResolutions,
                 const double& currentFovScaling,
                 gkg::Resampler< T >* resampler,
                 const std::list< gkg::RCPointer< gkg::Transform3d< float > > >&
                                                                   transform3ds,
                 gkg::RCPointer< gkg::Volume< T > > t2FovVolume,
                 gkg::RCPointer< gkg::Volume< int16_t > > outputMaskVolume,
                 gkg::RCPointer< gkg::Volume< T > > dwFovVolume,
                 double background,
                 const gkg::BoundingBox< int32_t >& outputBoundingBox,
                 gkg::PartialVolumingQuickResampler< T >& quickResampler,
                 const std::vector< gkg::Matrix >& Ls,
                 const std::vector< gkg::Matrix >& Btargets,
                 const std::vector< double >& laplaceBeltramiRegularizationFactors,
                 const gkg::CustomMultipleDifferentShellQSpaceSampling& /*originQSpaceSampling*/,
                 const std::vector< int32_t >& sphericalHarmonicOrders,
                 int32_t globalMatrixSize,
                 const std::vector< float >& rawBValues,
                 const std::vector< gkg::Vector3d< float > >& rawOrientations,
                 float deltaBValue,
                 gkg::Volume< T >* outputT2Volume,
                 gkg::Volume< T >* outputDWVolume,
                 const std::vector< gkg::OrientationSet >& targetOrientationSets,
                 bool verbose )
                             : gkg::LoopContext< int32_t >(
                                                           &dwiComposingGauge ),
                               _voxelToTreat( voxelToTreat ),
                               _referenceSizes( referenceSizes ),
                               _dwCount( dwCount ),
                               _referenceResolutions( referenceResolutions ),
                               _currentFovScaling( currentFovScaling ),
                               _resampler( resampler ),
                               _transform3ds( transform3ds ),
                               _t2FovVolume( t2FovVolume ),
                               _outputMaskVolume( outputMaskVolume ),
                               _dwFovVolume( dwFovVolume ),
                               _background( background ),
                               _outputBoundingBox( outputBoundingBox ),
                               _quickResampler( quickResampler ),
                               _Ls( Ls ),
                               _Btargets( Btargets ),
                               _laplaceBeltramiRegularizationFactors( laplaceBeltramiRegularizationFactors ),
                               _sphericalHarmonicOrders( sphericalHarmonicOrders ),
                               _globalMatrixSize( globalMatrixSize ),
                               _rawBValues( rawBValues ),
                               _rawOrientations( rawOrientations ),
                               _deltaBValue( deltaBValue ),
                               _outputT2Volume( outputT2Volume ),
                               _outputDWVolume( outputDWVolume ),
                               _targetOrientationSets( targetOrientationSets ),
                               _verbose( verbose )
{

  // try
  // {

  //   // gkg::Dictionary gradientCharacteristics;
  //   // gradientCharacteristics[ "type" ] = std::string( "unknown" );
  //   // gkg::IdentityTransform3d< float > identityTransform3d;

  //   // _originQspaceSampling.reset(
  //   //       new gkg::CustomMultipleDifferentShellQSpaceSampling(
  //   //                         originQSpaceSampling.getRawBValues(),
  //   //                         originQSpaceSampling.getRawOrientations(),
  //   //                         originQSpaceSampling.getDeltaBValue(),
  //   //                         identityTransform3d,
  //   //                         gradientCharacteristics ) );

  //   // std::vector< float > bval = originQSpaceSampling.getRawBValues();
  //   // for ( uint32_t i = 0; i < bval.size(); ++i )
  //   // {
  //   //   std::cout << i << " " << bval[i] << std::endl;
  //   // }

  // }
  // GKG_CATCH( "" );

}


template < class T >
inline
void DwiComposingLoopContext< T >::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    gkg::Dictionary gradientCharacteristics;
    gradientCharacteristics[ "type" ] = std::string( "unknown" );
    gkg::IdentityTransform3d< float > identityTransform3d;

    gkg::CustomMultipleDifferentShellQSpaceSampling _originQspaceSampling(
                            _rawBValues,
                            _rawOrientations,
                            _deltaBValue,
                            identityTransform3d,
                            gradientCharacteristics );

    std::vector< float > bval = _originQspaceSampling.getRawBValues();

    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    ////////////////////////////////////////////////////////////////////////////
    // collecting the FOV resolution
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > fovSize;
    _t2FovVolume->getSize( fovSize );

    gkg::Vector3d< double > fovResolution;
    _t2FovVolume->getResolution( fovResolution );


    gkg::BoundingBox< float >
      fovBoundingBox( ( float )( -fovResolution.x / 2.0f ), 
                      ( float )( ( ( double )fovSize.x + 0.5 ) *
                                 fovResolution.x ),
                      ( float )( -fovResolution.y / 2.0f ), 
                      ( float )( ( ( double )fovSize.y + 0.5 ) *
                                 fovResolution.y ),
                      ( float )( -fovResolution.z / 2.0f ), 
                      ( float )( ( ( double )fovSize.z + 0.5 ) *
                                 fovResolution.z ) );
                      

    ////////////////////////////////////////////////////////////////////////////
    // allocating memory buffers for output T2 and DW items
    ////////////////////////////////////////////////////////////////////////////

    // std::vector< T > t2Mask( count, false );
    // std::vector< T > t2Outputs( count, ( T )0 );
    // std::vector< std::vector< T > > dwOutputs( _dwCount );

    int32_t dwIndex = 0;
    // for ( dwIndex = 0; dwIndex < _dwCount; dwIndex++ )
    // {

    //   dwOutputs[ dwIndex ] = std::vector< T >( count, ( T )0 );

    // }


    ////////////////////////////////////////////////////////////////////////////
    // preparing partial output T2 & DW volume proxies and sub-volumes
    ////////////////////////////////////////////////////////////////////////////

    // // output T2 proxy and sub-volume
    // gkg::VolumeProxy< T > outputT2VolumeProxy;
    // std::string readingT2Format =
    //   gkg::Reader::getInstance().template preparePartialBinaryRead<
    //                                                gkg::Volume< T > >(
    //                                                _fileNameOutputT2Volume,
    //                                                outputT2VolumeProxy );
    // gkg::BoundingBox< int32_t > outputT2BoundingBox( 0, _referenceSizes.x - 1,
    //                                                  0, _referenceSizes.y - 1,
    //                                                  0, 0,
    //                                                  0, 0 );
    // gkg::Volume< T > outputT2SubVolume;


    // // output DW proxy and sub-volume
    // gkg::VolumeProxy< T > outputDWVolumeProxy;
    // std::string readingDWFormat =
    //   gkg::Reader::getInstance().template preparePartialBinaryRead<
    //                                                gkg::Volume< T > >(
    //                                                _fileNameOutputDWVolume,
    //                                                outputDWVolumeProxy );
    // gkg::BoundingBox< int32_t > outputDWBoundingBox( 0, _referenceSizes.x - 1,
    //                                                  0, _referenceSizes.y - 1,
    //                                                  0, 0,
    //                                                  0, _dwCount - 1 );
    // gkg::Volume< T > outputDWSubVolume;


    ////////////////////////////////////////////////////////////////////////////
    // looping over output volume voxel(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > outputVoxel;
    gkg::Vector3d< float > outputSite;
    gkg::Vector3d< float > fovSite;
    gkg::Vector3d< float > tmpSite;
    T outputT2Value = T( 0 );
    std::vector< T > outputDWValues( _dwCount, T( 0 ) );

    gkg::Vector normalizedDWValues( _dwCount );
    gkg::Vector transformedNormalizedDWValues( _dwCount );

    std::vector< T > qSpaceInterpolatedDWValues( _dwCount, T( 0 ) );

    int32_t indexVoxelToTreat = 0;

    gkg::Vector3d< float > fovSiteX1;
    gkg::Vector3d< float > fovSiteX2;
    gkg::Vector3d< float > dx;
    gkg::Matrix M( 3, 3 );
    gkg::Matrix Mt( 3, 3 );
    gkg::Matrix MMt( 3, 3 );
    gkg::Matrix MMtinv( 3, 3 );
    gkg::Vector D( 3 );
    gkg::Matrix V( 3, 3 );
    gkg::Matrix X( 3, 3 );
    gkg::Matrix Vt( 3, 3 );
    gkg::Matrix R( 3, 3 );

    for ( indexVoxelToTreat = startIndex; indexVoxelToTreat < startIndex + count; indexVoxelToTreat++ )
    {

      outputVoxel = _voxelToTreat[ indexVoxelToTreat ];

      if ( _verbose && indexVoxelToTreat % 1000 == 0 )
      {

        lock();
        gaugeAdd( 1 );
        unlock();

      }

      outputSite.z = ( outputVoxel.z + 0.5 ) * _referenceResolutions.z;
      outputSite.y = ( outputVoxel.y + 0.5 ) * _referenceResolutions.y;
      outputSite.x = ( outputVoxel.x + 0.5 ) * _referenceResolutions.x;
      outputT2Value = ( T )0;


      //_resampler->resample( *_fovVolume,
      //                      *_transform3d,
      //                      _background,
      //                      outputSite,
      //                      outputValue,
      //                      &_fovResolution );

      if ( _outputMaskVolume->getSizeX() <= 1 ||
            ( *_outputMaskVolume )( outputVoxel ) == 1 )
      {

        tmpSite = outputSite;
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >::
          const_reverse_iterator t = _transform3ds.rbegin(),
                                te = _transform3ds.rend();
        while ( t != te )
        {

          ( *t )->getInverse( tmpSite, fovSite );
          tmpSite = fovSite;
          ++ t;

        }

        if ( fovBoundingBox.contains( fovSite ) )
        {

          _quickResampler.resample( *_t2FovVolume,
                                    _background,
                                    fovSite,
                                    outputT2Value,
                                    &fovResolution );
          bool isModified = this->composeItem(
                                          ( *_outputT2Volume )( outputVoxel ),
                                          ( *_outputT2Volume )( outputVoxel ),
                                          outputT2Value,
                                          _currentFovScaling );

          if ( isModified )
          {

            for ( dwIndex = 0; dwIndex < _dwCount; dwIndex++ )
            {

              outputDWValues[ dwIndex ] = ( T )0;
              _quickResampler.resample( *_dwFovVolume,
                                        _background,
                                        fovSite,
                                        outputDWValues[ dwIndex ],
                                        &fovResolution,
                                        dwIndex );

            }


            for ( dwIndex = 0; dwIndex < _dwCount; dwIndex++ )
            {

              if ( outputT2Value > ( T )0 )
              {

                normalizedDWValues( dwIndex ) =
                            ( double )outputDWValues[ dwIndex ] / outputT2Value;

              }
              else
              {

                std::cout << "happening" << std::endl;
                normalizedDWValues( dwIndex ) = 0.0;

              }

            }
      
            // computing the transformation

            // X1 and X2
            tmpSite = outputSite;
            tmpSite.x -= 0.05f * _referenceResolutions.x;
            t = _transform3ds.rbegin();
            while ( t != te )
            {

              ( *t )->getInverse( tmpSite, fovSiteX1 );
              tmpSite = fovSiteX1;
              ++ t;

            }
            tmpSite = outputSite;
            tmpSite.x += 0.05f * _referenceResolutions.x;
            t = _transform3ds.rbegin();
            while ( t != te )
            {

              ( *t )->getInverse( tmpSite, fovSiteX2 );
              tmpSite = fovSiteX2;
              ++ t;

            }

            dx = fovSiteX2 - fovSiteX1;

            M( 0, 0 ) = dx.x / 0.1f *_referenceResolutions.x;
            M( 0, 1 ) = dx.y / 0.1f *_referenceResolutions.x;
            M( 0, 2 ) = dx.z / 0.1f *_referenceResolutions.x;

            // Y1 and Y2
            tmpSite = outputSite;
            tmpSite.y -= 0.05f * _referenceResolutions.y;
            t = _transform3ds.rbegin();
            while ( t != te )
            {

              ( *t )->getInverse( tmpSite, fovSiteX1 );
              tmpSite = fovSiteX1;
              ++ t;

            }
            tmpSite = outputSite;
            tmpSite.y += 0.05f * _referenceResolutions.y;
            t = _transform3ds.rbegin();
            while ( t != te )
            {

              ( *t )->getInverse( tmpSite, fovSiteX2 );
              tmpSite = fovSiteX2;
              ++ t;

            }
            dx = fovSiteX2 - fovSiteX1;

            M( 1, 0 ) = dx.x / 0.1f *_referenceResolutions.y;
            M( 1, 1 ) = dx.y / 0.1f *_referenceResolutions.y;
            M( 1, 2 ) = dx.z / 0.1f *_referenceResolutions.y;

            // Z1 and Z2
            tmpSite = outputSite;
            tmpSite.z -= 0.05f * _referenceResolutions.z;
            t = _transform3ds.rbegin();
            while ( t != te )
            {

              ( *t )->getInverse( tmpSite, fovSiteX1 );
              tmpSite = fovSiteX1;
              ++ t;

            }
            tmpSite = outputSite;
            tmpSite.z += 0.05f * _referenceResolutions.z;
            t = _transform3ds.rbegin();
            while ( t != te )
            {

              ( *t )->getInverse( tmpSite, fovSiteX2 );
              tmpSite = fovSiteX2;
              ++ t;

            }
            dx = fovSiteX2 - fovSiteX1;

            M( 2, 0 ) = dx.x / 0.1f *_referenceResolutions.z;
            M( 2, 1 ) = dx.y / 0.1f *_referenceResolutions.z;
            M( 2, 2 ) = dx.z / 0.1f *_referenceResolutions.z;

            // M is the jacobian

            Mt = M;
            Mt.transpose();

            MMt = Mt.getComposition( M );
            factory->getInverse( MMt, MMtinv );
            factory->getEigenSystem( MMtinv, D, V );

            X.fill( 0.f );

            X( 0, 0 ) = std::sqrt( D( 0 ) );
            X( 1, 1 ) = std::sqrt( D( 1 ) );
            X( 2, 2 ) = std::sqrt( D( 2 ) );

            Vt = V;
            Vt.transpose();


            R = V.getComposition( X ).getComposition( Vt ).getComposition( M );

            gkg::Rotation3d< float > rotation(
                  std::atan2( R( 2, 1 ), R( 2, 2 ) ),
                  std::atan2( -R( 2, 0 ), std::sqrt(
                            R( 2, 1 ) * R( 2, 1 ) + R( 2, 2 ) * R( 2, 2 ) ) ),
                  std::atan2( R( 1, 0 ), R( 0, 0 ) ) );

            std::map< float, std::list< int32_t > > bValueAndIndices;
            _originQspaceSampling.getShells( bValueAndIndices );


            // allTargetOrientations

            gkg::Matrix globalReconstructionMatrix( _globalMatrixSize,
                                                    _globalMatrixSize );
            globalReconstructionMatrix.setZero();

            std::map< float, std::list< int32_t > >::const_iterator
              s = bValueAndIndices.begin(),
              se = bValueAndIndices.end();
            std::vector< int32_t >::const_iterator
              sh = _sphericalHarmonicOrders.begin();
            int32_t shellIndex = 0;

            int32_t offset = 0;
            while ( s != se )
            {

              int32_t M = ( int32_t )s->second.size();
              int32_t N = ( ( *sh + 1 ) * ( *sh + 2 ) ) / 2;

              // allocating origin SH basis matrix
              gkg::Matrix Btarget( M, N );
              gkg::Vector3d< float > orientation;
              double radial = 0.0;
              double theta = 0.0;
              double phi = 0.0;

              for ( int32_t m = 0; m < M; m++ )
              {

                rotation.getInverse( _targetOrientationSets[ shellIndex ].getOrientation( m ), orientation );
                factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                            ( double )orientation.y,
                                                            ( double )orientation.z,
                                                            radial,
                                                            theta,
                                                            phi );
                for ( int32_t n = 0; n < N; n++ )
                {

                  Btarget( m, n ) = factory->getSymmetricalSphericalHarmonic( n,
                                                                              phi,
                                                                              theta );

                }

              }

              globalReconstructionMatrix.fill( offset, offset,
                                        Btarget.getComposition( _Ls[shellIndex] ) );
              offset += M;

              ++ s;
              ++ sh;
              ++ shellIndex;

            }

            // reinterpoling the signal in the rotated Q-space
            transformedNormalizedDWValues = 
                  globalReconstructionMatrix.getComposition( normalizedDWValues );

            for ( dwIndex = 0; dwIndex < _dwCount; dwIndex++ )
            {

              this->composeDwiItem(
                              ( *_outputDWVolume)( outputVoxel,
                                                    dwIndex ),
                              ( *_outputDWVolume)( outputVoxel,
                                                    dwIndex ),
                              ( T )( transformedNormalizedDWValues( dwIndex ) *
                                      outputT2Value ),
                              _currentFovScaling );

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > inline "
             "void ComposingLoopContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );
  

}


template < class T >
inline
bool DwiComposingLoopContext< T >::composeItem( 
                      T& newOutputValue,
                      const T& currentOutputValue,
                      const T& outputValue,
                      double currentFovScaling )
{

  try
  {

    if ( ( double )outputValue * currentFovScaling >
     ( double )currentOutputValue )
    {

      newOutputValue = ( T )( ( double )outputValue * currentFovScaling );
      return true;

    }
    else
    {

      newOutputValue = currentOutputValue;
      return false;

    }

  }
  GKG_CATCH( "template < class T > inline "
             "bool ComposingLoopContext< T >::composeItem( "
             "T& newOutputValue, "
             "const T& currentOutputValue, "
             "const T& outputValue, "
             "double currentFovScaling )" );

}


template < class T >
inline
void DwiComposingLoopContext< T >::composeDwiItem( 
                      T& newOutputValue,
                      const T& /*currentOutputValue*/,
                      const T& outputValue,
                      double currentFovScaling )
{

  try
  {

    newOutputValue = ( T )( ( double )outputValue * currentFovScaling );

  }
  GKG_CATCH( "template < class T > inline "
             "void ComposingLoopContext< T >::composeDwiItem( "
             "T& newOutputValue, "
             "const T& currentOutputValue, "
             "const T& outputValue, "
             "double currentFovScaling )" );

}


//
// class DwiPostMortemBlockToWholeImageComposerProcess
//

class DwiPostMortemBlockToWholeImageComposerProcess : public gkg::Process
{

  public:

    DwiPostMortemBlockToWholeImageComposerProcess(
        const std::vector< std::string >& theFileNameT2Fovs,
        const std::vector< std::string >& fileNameOutputCompositionMask,
        const std::vector< std::string >& theFileNameDWFovs,
        const std::vector< std::string >& theFileNameBvalFovs,
        const std::vector< std::string >& theFileNameBvecFovs,
        const std::vector< std::string >& theFileNameFlippingFovs,
        const std::vector< std::string >& theFileNameFovToReferenceTransform3ds,
        const std::vector< std::string >& theFovToReferenceTransform3dTypes,
        const std::string& theFileNameOutputT2Volume,
        const std::string& theFileNameOutputDWVolume,
        const std::string& theFileNameOutputBVal,
        const std::string& theFileNameOutputBVec,
        const std::vector< double >& theFovScalings,
        const gkg::Vector3d< int32_t >& theReferenceSizes,
        const gkg::Vector3d< double >& theReferenceResolutions,
        double theDeltaBValue,
        const std::vector< int32_t >& theSphericalHarmonicOrders,
        const std::vector< double >& theLaplaceBeltramiRegularizationFactors,
        int32_t theResamplingOrder,
        double theBackground,
        const gkg::BoundingBox< int32_t >& theOutputBoundingBox,
        const std::string& theFormat,
        bool theVerbose );

    const std::vector< std::string >& fileNameT2Fovs;
    const std::vector< std::string >& fileNameOutputCompositionMask;
    const std::vector< std::string >& fileNameDWFovs;
    const std::vector< std::string >& fileNameBvalFovs;
    const std::vector< std::string >& fileNameBvecFovs;
    const std::vector< std::string >& fileNameFlippingFovs;
    const std::vector< std::string >& fileNameFovToReferenceTransform3ds;
    const std::vector< std::string >& fovToReferenceTransform3dTypes;
    const std::string& fileNameOutputT2Volume;
    const std::string& fileNameOutputDWVolume;
    const std::string& fileNameOutputBVal;
    const std::string& fileNameOutputBVec;
    const std::vector< double >& fovScalings;
    const gkg::Vector3d< int32_t >& referenceSizes;
    const gkg::Vector3d< double >& referenceResolutions;
    double deltaBValue;
    const std::vector< int32_t >& sphericalHarmonicOrders;
    const std::vector< double >& laplaceBeltramiRegularizationFactors;
    int32_t resamplingOrder;
    double background;
    const gkg::BoundingBox< int32_t >& outputBoundingBox;
    const std::string& format;
    bool verbose;

  private:

    template < class T >
    static void compose( gkg::Process& process,
                         const std::string& fileNameFov,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    static void getFlipping3d(
                             const std::string& fileName,
                             gkg::HomogeneousTransform3d< float >& flipping3d );

    static void readTransform3ds(
        const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
        int32_t offsetTransform3d,
        const std::vector< int32_t >& transform3dFileNameCounts,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& transform3ds,
        const std::list< bool >& applyLinearTransformFirstList,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& rotation3ds,
        bool verbose );

};


DwiPostMortemBlockToWholeImageComposerProcess::
                                  DwiPostMortemBlockToWholeImageComposerProcess(
        const std::vector< std::string >& theFileNameT2Fovs,
        const std::vector< std::string >& theFileNameOutputCompositionMask,
        const std::vector< std::string >& theFileNameDWFovs,
        const std::vector< std::string >& theFileNameBvalFovs,
        const std::vector< std::string >& theFileNameBvecFovs,
        const std::vector< std::string >& theFileNameFlippingFovs,
        const std::vector< std::string >& theFileNameFovToReferenceTransform3ds,
        const std::vector< std::string >& theFovToReferenceTransform3dTypes,
        const std::string& theFileNameOutputT2Volume,
        const std::string& theFileNameOutputDWVolume,
        const std::string& theFileNameOutputBVal,
        const std::string& theFileNameOutputBVec,
        const std::vector< double >& theFovScalings,
        const gkg::Vector3d< int32_t >& theReferenceSizes,
        const gkg::Vector3d< double >& theReferenceResolutions,
        double theDeltaBValue,
        const std::vector< int32_t >& theSphericalHarmonicOrders,
        const std::vector< double >& theLaplaceBeltramiRegularizationFactors,
        int32_t theResamplingOrder,
        double theBackground,
        const gkg::BoundingBox< int32_t >& theOutputBoundingBox,
        const std::string& theFormat,
        bool theVerbose )
                         : gkg::Process( "Volume" ),
                           fileNameT2Fovs( theFileNameT2Fovs ),
                           fileNameOutputCompositionMask(
                                             theFileNameOutputCompositionMask ),
                           fileNameDWFovs( theFileNameDWFovs ),
                           fileNameBvalFovs( theFileNameBvalFovs ),
                           fileNameBvecFovs( theFileNameBvecFovs ),
                           fileNameFlippingFovs( theFileNameFlippingFovs ),
                           fileNameFovToReferenceTransform3ds(
                                        theFileNameFovToReferenceTransform3ds ),
                           fovToReferenceTransform3dTypes(
                                            theFovToReferenceTransform3dTypes ),
                           fileNameOutputT2Volume( theFileNameOutputT2Volume ),
                           fileNameOutputDWVolume( theFileNameOutputDWVolume ),
                           fileNameOutputBVal( theFileNameOutputBVal ),
                           fileNameOutputBVec( theFileNameOutputBVec ),
                           fovScalings( theFovScalings ),
                           referenceSizes( theReferenceSizes ),
                           referenceResolutions( theReferenceResolutions ),
                           deltaBValue( theDeltaBValue ),
                           sphericalHarmonicOrders(
                                                   theSphericalHarmonicOrders ),
                           laplaceBeltramiRegularizationFactors(
                                      theLaplaceBeltramiRegularizationFactors ),
                           resamplingOrder( theResamplingOrder ),
                           background( theBackground ),
                           outputBoundingBox( theOutputBoundingBox ),
                           format( theFormat ),
                           verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &compose< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &compose< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &compose< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &compose< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &compose< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &compose< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &compose< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &compose< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &compose< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &compose< double > );

  }
  GKG_CATCH( "DwiPostMortemBlockToWholeImageComposerProcess:: "
             "DwiPostMortemBlockToWholeImageComposerProcess( "
             "const std::vector< std::string >& theFileNameT2Fovs, "
             "const std::vector< std::string >& "
             "theFileNameOutputCompositionMask, "
             "const std::vector< std::string >& theFileNameDWFovs, "
             "const std::vector< std::string >& theFileNameBvalFovs, "
             "const std::vector< std::string >& theFileNameBvecFovs, "
             "const std::vector< std::string >& theFileNameFlippingFovs, "
             "const std::vector< std::string >& "
             "theFileNameFovToReferenceTransform3ds, "
             "const std::vector< std::string >& "
             "theFovToReferenceTransform3dTypes, "
             "const std::string& theFileNameOutputT2Volume, "
             "const std::string& theFileNameOutputDWVolume, "
             "const std::string& theFileNameOutputBVal, "
             "const std::string& theFileNameOutputBVec, "
             "const std::vector< double >& theFovScalings, "
             "const gkg::Vector3d< int32_t >& theReferenceSizes, "
             "const gkg::Vector3d< double >& theReferenceResolutions, "
             "double theDeltaBValue, "
             "const std::vector< int32_t >& theSphericalHarmonicOrders, "
             "const std::vector< double >& "
             "theLaplaceBeltramiRegularizationFactors, "
             "int32_t theResamplingOrder, "
             "double theBackground, "
             "const gkg::BoundingBox< int32_t >& theOutputBoundingBox, "
             "const std::string& theFormat, "
             "bool theVerbose )" );

}


template < class T >
void 
DwiPostMortemBlockToWholeImageComposerProcess::compose(
                                                gkg::Process& process,
                                                const std::string&,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    DwiPostMortemBlockToWholeImageComposerProcess&
      dwiPostMortemBlockToWholeImageComposerProcess = static_cast<
                   DwiPostMortemBlockToWholeImageComposerProcess& >( process );


    const std::vector< std::string >& fileNameT2Fovs =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameT2Fovs;
    const std::vector< std::string >& fileNameOutputCompositionMask =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameOutputCompositionMask;
    const std::vector< std::string >& fileNameDWFovs =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameDWFovs;
    const std::vector< std::string >& fileNameBvalFovs =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameBvalFovs;
    const std::vector< std::string >& fileNameBvecFovs =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameBvecFovs;
    const std::vector< std::string >& fileNameFlippingFovs =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameFlippingFovs;
    const std::vector< std::string >& fileNameFovToReferenceTransform3ds =
      dwiPostMortemBlockToWholeImageComposerProcess.
                                            fileNameFovToReferenceTransform3ds;
    const std::vector< std::string >& fovToReferenceTransform3dTypes =
      dwiPostMortemBlockToWholeImageComposerProcess.
                                                 fovToReferenceTransform3dTypes;
    const std::string& fileNameOutputT2Volume =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameOutputT2Volume;
    const std::string& fileNameOutputDWVolume =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameOutputDWVolume;
    const std::string& fileNameOutputBVal =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameOutputBVal;
    const std::string& fileNameOutputBVec =
      dwiPostMortemBlockToWholeImageComposerProcess.fileNameOutputBVec;
    const std::vector< double >& fovScalings =
      dwiPostMortemBlockToWholeImageComposerProcess.fovScalings;
    const gkg::Vector3d< int32_t >& referenceSizes =
      dwiPostMortemBlockToWholeImageComposerProcess.referenceSizes;
    const gkg::Vector3d< double >& referenceResolutions =
      dwiPostMortemBlockToWholeImageComposerProcess.referenceResolutions;
    double deltaBValue =
      dwiPostMortemBlockToWholeImageComposerProcess.deltaBValue;
    const std::vector< int32_t >& sphericalHarmonicOrders =
      dwiPostMortemBlockToWholeImageComposerProcess.sphericalHarmonicOrders;
    const std::vector< double >& laplaceBeltramiRegularizationFactors =
      dwiPostMortemBlockToWholeImageComposerProcess.
                                           laplaceBeltramiRegularizationFactors;
    int32_t resamplingOrder =
      dwiPostMortemBlockToWholeImageComposerProcess.resamplingOrder;
    double background =
      dwiPostMortemBlockToWholeImageComposerProcess.background;
    const gkg::BoundingBox< int32_t >& outputBoundingBox =
      dwiPostMortemBlockToWholeImageComposerProcess.outputBoundingBox;
    // const std::string& format =
    //   dwiPostMortemBlockToWholeImageComposerProcess.format;
    bool verbose =
      dwiPostMortemBlockToWholeImageComposerProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    int32_t fovCount = ( int32_t )fileNameT2Fovs.size();

    if ( ( int32_t )fileNameDWFovs.size() !=
         fovCount )
    {

      throw std::runtime_error( "inconsistent number of DW FOVs and T2 FOVs" );

    }

    int32_t outputMaskCount = ( int32_t )fileNameOutputCompositionMask.size();
    if ( outputMaskCount != 0 && outputMaskCount != fovCount )
    {

      throw std::runtime_error(
                "inconsistent output mask filename count and T2 FOVs" );

    }

    if ( ( int32_t )fileNameBvalFovs.size() !=
         fovCount )
    {

      throw std::runtime_error(
                           "inconsistent number of *.bval files and T2 FOVs" );

    }
    if ( ( int32_t )fileNameBvecFovs.size() !=
         fovCount )
    {

      throw std::runtime_error(
                            "inconsistent number of *.bvec files and T2 FOVs" );

    }


    int32_t transform3dCount = ( int32_t )fovToReferenceTransform3dTypes.size();
    std::vector< int32_t > transform3dFileNameCounts;
    int32_t totalFileNameTransform3dCount = 0;
    std::list< bool > applyLinearTransformFirstList;
    if ( !fovToReferenceTransform3dTypes.empty() )
    {

      transform3dFileNameCounts.resize( transform3dCount );
      int32_t t = 0;
      for ( t = 0; t < transform3dCount; t++ )
      {

        if ( fovToReferenceTransform3dTypes[ t ] == "linear" )
        {

          transform3dFileNameCounts[ t ] = 1;
          totalFileNameTransform3dCount += 1;

        }
        else if ( fovToReferenceTransform3dTypes[ t ] ==
                  "linear_and_diffeomorphic" )
        {

          transform3dFileNameCounts[ t ] = 3;
          totalFileNameTransform3dCount += 3;
          applyLinearTransformFirstList.push_back( false );

        }
        else if ( fovToReferenceTransform3dTypes[ t ] ==
                  "diffeomorphic_and_linear" )
        {

          transform3dFileNameCounts[ t ] = 3;
          totalFileNameTransform3dCount += 3;
          applyLinearTransformFirstList.push_back( true );

        }
        else
        {

          throw std::runtime_error(
                                  std::string( "unknown 3d transform type '" ) +
                                  fovToReferenceTransform3dTypes[ t ] + "'" );

        }

      }

    }

    if ( ( int32_t )fileNameFovToReferenceTransform3ds.size() !=
         fovCount * totalFileNameTransform3dCount )
    {

      throw std::runtime_error(
                              "inconsistent number of 3D transforms and FOVs" );

    }

    if ( ( int32_t )fileNameFlippingFovs.size() != transform3dCount * fovCount )
    {

      throw std::runtime_error(
                          "need as many flipping files as 3D transform count" );

    }


    if ( ( int32_t )fovScalings.size() != fovCount )
    {

      throw std::runtime_error( "inconsistent number of scalings and T2 FOVs" );

    }

    if ( ( referenceSizes.x <= 0 ) ||
         ( referenceSizes.y <= 0 ) ||
         ( referenceSizes.z <= 0 ) )
    {

      throw std::runtime_error( "reference sizes must be strictly positive" );

    }
         
    if ( ( referenceResolutions.x <= 0 ) ||
         ( referenceResolutions.y <= 0 ) ||
         ( referenceResolutions.z <= 0 ) )
    {

      throw std::runtime_error(
                            "reference resolutions must be strictly positive" );

    }

    if ( sphericalHarmonicOrders.size() !=
         laplaceBeltramiRegularizationFactors.size() )
    {

      throw std::runtime_error(
                             "inconsistent SH order count and Laplace-Beltrami "
                             "regularization factor count" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // checking bounding box
    ////////////////////////////////////////////////////////////////////////////

    gkg::BoundingBox< int32_t > localOutputBoundingBox = outputBoundingBox;
    if ( localOutputBoundingBox.getLowerX() < 0 )
    {

      localOutputBoundingBox.setLowerX( 0 );

    }
    if ( localOutputBoundingBox.getLowerY() < 0 )
    {

      localOutputBoundingBox.setLowerY( 0 );

    }
    if ( localOutputBoundingBox.getLowerZ() < 0 )
    {

      localOutputBoundingBox.setLowerZ( 0 );

    }
    if ( ( localOutputBoundingBox.getUpperX() < 0 ) ||
         ( localOutputBoundingBox.getUpperX() >= referenceSizes.x ) )
    {

      localOutputBoundingBox.setUpperX( referenceSizes.x - 1 );

    }
    if ( ( localOutputBoundingBox.getUpperY() < 0 ) ||
         ( localOutputBoundingBox.getUpperY() >= referenceSizes.y ) )
    {

      localOutputBoundingBox.setUpperY( referenceSizes.y - 1 );

    }
    if ( ( localOutputBoundingBox.getUpperZ() < 0 ) ||
         ( localOutputBoundingBox.getUpperZ() >= referenceSizes.z ) )
    {

      localOutputBoundingBox.setUpperZ( referenceSizes.z - 1 );

    }


    if ( verbose )
    {

      std::cout << "output bounding box : " 
                << "( "
                << localOutputBoundingBox.getLowerX() << ", "
                << localOutputBoundingBox.getLowerY() << ", "
                << localOutputBoundingBox.getLowerZ() << " ) -> ( "
                << localOutputBoundingBox.getUpperX() << ", "
                << localOutputBoundingBox.getUpperY() << ", "
                << localOutputBoundingBox.getUpperZ() << " )"
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating output T2 and DW volume(s) : "
                << std::flush;

    }


    // collecting the number of DW volumes from the first DW FOV
    gkg::VolumeProxy< T > inputFirstDWFovVolumeProxy;

    std::string readingFormat =
      gkg::Reader::getInstance().template preparePartialBinaryRead<
                                                   gkg::Volume< T > >(
                                                   fileNameDWFovs[ 0 ],
                                                   inputFirstDWFovVolumeProxy );
    int32_t dwCount = inputFirstDWFovVolumeProxy.getSizeT();


    gkg::Volume< T > outputT2Volume( referenceSizes );
    outputT2Volume.setResolution( referenceResolutions );
    outputT2Volume.fill( T( 0 ) );
    gkg::BoundingBox< int32_t > outputT2BoundingBox( 0, referenceSizes.x - 1,
                                                     0, referenceSizes.y - 1,
                                                     0, referenceSizes.z - 1,
                                                     0, 0 );

    // // allocating the output DW volume
    // gkg::VolumeProxy< T > outputDWVolumeProxy( referenceSizes, dwCount );
    // outputDWVolumeProxy.setResolution( referenceResolutions );

    gkg::Volume< T > outputDWVolume( referenceSizes, dwCount );
    outputDWVolume.setResolution( referenceResolutions );
    outputDWVolume.fill( T( 0 ) );
    gkg::BoundingBox< int32_t > outputDWBoundingBox( 0, referenceSizes.x - 1,
                                                     0, referenceSizes.y - 1,
                                                     0, referenceSizes.z - 1,
                                                     0, 0 );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 14 );
      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating a resampler
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating " << resamplingOrder << "-order resampler : "
                << std::flush;

    }

    gkg::Resampler< T >* 
      resampler = gkg::ResamplerFactory< T >::getInstance().getResampler(
                                                              resamplingOrder );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }



    ////////////////////////////////////////////////////////////////////////////
    // getting a pointer to the numerical analysis factory
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();


    ////////////////////////////////////////////////////////////////////////////
    // looping over FOVs
    ////////////////////////////////////////////////////////////////////////////

    int32_t fovIndex = 0;
    int32_t offsetTransform3d = 0;
    for ( fovIndex = 0; fovIndex < fovCount; fovIndex++ )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading current T2 FOV
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "reading current T2 FOV '" << fileNameT2Fovs[ fovIndex ]
                  << "' : "
                  << std::flush;

      }

      gkg::RCPointer< gkg::Volume< T > > t2FovVolume( new gkg::Volume< T > );
      gkg::Reader::getInstance().read( fileNameT2Fovs[ fovIndex ],
                                       *t2FovVolume );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }


      //////////////////////////////////////////////////////////////////////////
      // reading current output composition mask
      //////////////////////////////////////////////////////////////////////////


      gkg::RCPointer< gkg::Volume< int16_t > > outputMaskVolume(
                                                   new gkg::Volume< int16_t > );

      if ( outputMaskCount > 0 )
      {

        if ( verbose )
        {

          std::cout << "reading current output mask '"
                    << fileNameOutputCompositionMask[ fovIndex ]
                    << "' : "
                    << std::flush;

        }

        gkg::Reader::getInstance().read( fileNameOutputCompositionMask[ fovIndex ],
                                        *outputMaskVolume );

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // reading current DW FOV
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "reading current DW FOV '" << fileNameDWFovs[ fovIndex ]
                  << "' : "
                  << std::flush;

      }

      gkg::RCPointer< gkg::Volume< T > > dwFovVolume( new gkg::Volume< T > );
      gkg::Reader::getInstance().read( fileNameDWFovs[ fovIndex ],
                                       *dwFovVolume );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }


      //////////////////////////////////////////////////////////////////////////
      // reading 3D transform
      //////////////////////////////////////////////////////////////////////////

      std::list< gkg::RCPointer< gkg::Transform3d< float > > > transform3ds;
      std::list< gkg::RCPointer< gkg::Transform3d< float > > > rotation3ds;
      if ( !fovToReferenceTransform3dTypes.empty() )
      {

        if ( verbose )
        {

          std::cout << "reading 3D transform(s) : "
                    << std::endl;

        }


        DwiPostMortemBlockToWholeImageComposerProcess::readTransform3ds(
                                             fileNameFovToReferenceTransform3ds,
                                             offsetTransform3d,
                                             transform3dFileNameCounts,
                                             transform3ds,
                                             applyLinearTransformFirstList,
                                             rotation3ds,
                                             verbose );

      }
      else
      {

        if ( verbose )
        {

          std::cout << "setting 3D transform(s) to identity : "
                    << std::flush;

        }

        gkg::RCPointer< gkg::Transform3d< float > >
          identityTransform3d( new gkg::IdentityTransform3d< float > );

        transform3ds.push_back( identityTransform3d );
        rotation3ds.push_back( identityTransform3d );

        if ( verbose )
        {

          std::cout << "done"
                    << std::endl;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // dealing with flipping(s)
      //////////////////////////////////////////////////////////////////////////

      std::list< gkg::HomogeneousTransform3d< float > > flipping3ds;
      if ( !fovToReferenceTransform3dTypes.empty() )
      {      

        int32_t flipping3dIndex = 0;
        for ( flipping3dIndex = 0; flipping3dIndex < transform3dCount;
              flipping3dIndex++ )
        {

          gkg::HomogeneousTransform3d< float > flipping3d;
          DwiPostMortemBlockToWholeImageComposerProcess::getFlipping3d(
                   fileNameFlippingFovs[ fovIndex * transform3dCount +
                                         flipping3dIndex ],
                   flipping3d );

          std::cout << "flipping[ " << flipping3dIndex << " ] = " << std::endl
                    << flipping3d << std::endl;

          flipping3ds.push_back( flipping3d );

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // creating composite transformation from rotation(s) and flipping(s)
      //////////////////////////////////////////////////////////////////////////

      gkg::CompositeTransform3d< float > compositeTransformation;

      std::list< gkg::RCPointer< gkg::Transform3d< float > > >::const_iterator
        r = rotation3ds.begin(),
        re = rotation3ds.end();
      std::list< gkg::HomogeneousTransform3d< float > >::const_iterator
        f = flipping3ds.begin();
      while ( r != re )
      {

        compositeTransformation.compose( *f );
        compositeTransformation.compose( **r );
        ++ f;
        ++ r;

      }


      //////////////////////////////////////////////////////////////////////////
      // creating the transformation matrix for DW signal(s)
      //////////////////////////////////////////////////////////////////////////

      gkg::Dictionary gradientCharacteristics;
      gradientCharacteristics[ "type" ] = std::string( "unknown" );

        gkg::IdentityTransform3d< float > identityTransform3d;
      // creating the origin Q-space sampling
      gkg::CustomMultipleDifferentShellQSpaceSampling
        originQSpaceSampling(
                            fileNameBvalFovs[ fovIndex ],
                            fileNameBvecFovs[ fovIndex ],
                            deltaBValue,
                            identityTransform3d,
                            gradientCharacteristics );
      int32_t shellCount = originQSpaceSampling.getShellCount();

      // sanity checks
      if ( int32_t( sphericalHarmonicOrders.size() ) != shellCount )
      {

        throw std::runtime_error( "SH order count or Laplace-Beltrami "
                                  "regularization factor count not equal to "
                                  "number of shell" );

      }

      std::map< float, std::list< int32_t > > bValueAndIndices;
      originQSpaceSampling.getShells( bValueAndIndices );

      // creating the target Q-space sampling
      std::vector< gkg::OrientationSet >
        targetOrientationSets( shellCount );
      std::vector< gkg::Vector3d< float > > allTargetOrientations;
      int32_t shellIndex = 0;
      std::map< float, std::list< int32_t > >::const_iterator
        s = bValueAndIndices.begin(),
        se = bValueAndIndices.end();
      while ( s != se )
      {

        targetOrientationSets[ shellIndex ] =
               gkg::ElectrostaticOrientationSet( ( int32_t )s->second.size() );
        allTargetOrientations.insert(
                  allTargetOrientations.end(),
                  targetOrientationSets[ shellIndex ].getOrientations().begin(),
                  targetOrientationSets[ shellIndex ].getOrientations().end() );

        ++ s;
        ++ shellIndex;

      }
      for ( int i = 0; i < (int32_t)allTargetOrientations.size(); ++ i )
      {

        std::cout << allTargetOrientations[ i ] << std::endl;

      }

      gkg::RCPointer< gkg::QSpaceSampling > targetQSpaceSampling;

      if ( shellCount == 1 )
      {

        gkg::IdentityTransform3d< float > identityTransform3d;
        targetQSpaceSampling.reset( 
          new gkg::CustomSingleShellQSpaceSampling(
                            originQSpaceSampling.getRawBValues(),
                            allTargetOrientations,
                            identityTransform3d,
                            gradientCharacteristics ) );

      }
      else
      {

        gkg::IdentityTransform3d< float > identityTransform3d;
        targetQSpaceSampling.reset( 
          new gkg::CustomMultipleDifferentShellQSpaceSampling(
                            originQSpaceSampling.getRawBValues(),
                            allTargetOrientations,
                            deltaBValue,
                            identityTransform3d,
                            gradientCharacteristics ) );

      }

      int32_t globalMatrixSize = ( int32_t )allTargetOrientations.size();
      gkg::Matrix globalReconstructionMatrix( globalMatrixSize,
                                              globalMatrixSize );
      globalReconstructionMatrix.setZero();

      s = bValueAndIndices.begin();
      std::vector< int32_t >::const_iterator
        sh = sphericalHarmonicOrders.begin();
      std::vector< double >::const_iterator
        lambda = laplaceBeltramiRegularizationFactors.begin();
      shellIndex = 0;

      std::vector< gkg::Matrix > Ls;
      std::vector< gkg::Matrix > Btargets;

      // int32_t offset = 0;
      while ( s != se )
      {

        std::vector< int32_t > orientationIndicesForShell( s->second.begin(),
                                                           s->second.end() );
        int32_t M = ( int32_t )orientationIndicesForShell.size();
        int32_t N = ( ( *sh + 1 ) * ( *sh + 2 ) ) / 2;

        // building Laplace-Beltrami smoothness matrix for regularization
        gkg::Matrix L( N, N );
        L.setZero();
        double sphericalLaplacian = 0;
        int32_t n = 0;
        int32_t l = 0;
        int32_t m = 0;
        for ( n = 0; n < N; n++ )
        {

          factory->getSymmetricalSphericalHarmonicLM( n, l, m );
          sphericalLaplacian = - l * ( l + 1 );
          L( n, n ) = sphericalLaplacian * sphericalLaplacian;

        }
        // Ls.push_back( L );

        // allocating origin SH basis matrix
        gkg::Matrix Borigin( M, N );
        gkg::Vector3d< float > orientation;
        double radial = 0.0;
        double theta = 0.0;
        double phi = 0.0;
        for ( m = 0; m < M; m++ )
        {

          orientation = originQSpaceSampling.getOrientation(
                                              orientationIndicesForShell[ m ] );
          factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                       ( double )orientation.y,
                                                       ( double )orientation.z,
                                                       radial,
                                                       theta,
                                                       phi );
          for ( n = 0; n < N; n++ )
          {

            Borigin( m, n ) = factory->getSymmetricalSphericalHarmonic( n,
                                                                        phi,
                                                                        theta );

          }

        }

        // building Bplus matrix with size N x M
        // Bplus = (B^T B + l L )^(-1) B^T
        gkg::Matrix Bplus( N, M );
        factory->getThikonovPseudoInverse( Borigin, L, *lambda, Bplus );

        Ls.push_back( Bplus );
        // allocating target SH basis matrix
        // gkg::Matrix Btarget( M, N );
        // for ( m = 0; m < M; m++ )
        // {

        //   orientation = targetOrientationSets[ shellIndex ].getOrientation( m );
        //   factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
        //                                                ( double )orientation.y,
        //                                                ( double )orientation.z,
        //                                                radial,
        //                                                theta,
        //                                                phi );
        //   for ( n = 0; n < N; n++ )
        //   {

        //     Btarget( m, n ) = factory->getSymmetricalSphericalHarmonic( n,
        //                                                                 phi,
        //                                                                 theta );

        //   }

        // }
        // Btargets.push_back( Btarget );

        // globalReconstructionMatrix.fill( offset, offset,
        //                                  Btarget.getComposition( Bplus ) );

        // offset += M;

        ++ s;
        ++ sh;
        ++ lambda;
        ++ shellIndex;

      }


      //////////////////////////////////////////////////////////////////////////
      // pointing to the current scaling factor
      //////////////////////////////////////////////////////////////////////////

      const double& currentFovScaling = fovScalings[ fovIndex ];
      if ( verbose )
      {

        std::cout << "applied FOV scaling : " << currentFovScaling
                  << std::endl;

      }


      //////////////////////////////////////////////////////////////////////////
      // resetting resampling if it is a spline resampler
      //////////////////////////////////////////////////////////////////////////

      gkg::SplineResampler< T >* splineResampler =
        dynamic_cast< gkg::SplineResampler< T >* >( resampler );
      if ( splineResampler )
      {

        splineResampler->reset();

      }


      //////////////////////////////////////////////////////////////////////////
      // looping over voxel of the output T2 and DW volume(s)
      //////////////////////////////////////////////////////////////////////////

      gkg::Vector3d< int32_t > fovSize;
      t2FovVolume->getSize( fovSize );

      gkg::Vector3d< double > fovResolution;
      t2FovVolume->getResolution( fovResolution );

      std::vector< gkg::Vector3d< int32_t > > voxelToTreat;
      gkg::BoundingBox< float >
        fovBoundingBox( ( float )( -fovResolution.x / 2.0f ), 
                        ( float )( ( ( double )fovSize.x + 0.5 ) *
                                  fovResolution.x ),
                        ( float )( -fovResolution.y / 2.0f ), 
                        ( float )( ( ( double )fovSize.y + 0.5 ) *
                                  fovResolution.y ),
                        ( float )( -fovResolution.z / 2.0f ), 
                        ( float )( ( ( double )fovSize.z + 0.5 ) *
                                  fovResolution.z ) );

      gkg::Vector3d< int32_t > outputVoxel;
      gkg::Vector3d< float > outputSite;
      gkg::Vector3d< float > fovSite;
      gkg::Vector3d< float > tmpSite;
      for ( outputVoxel.z = localOutputBoundingBox.getLowerZ();
            outputVoxel.z < localOutputBoundingBox.getUpperZ() + 1;
            outputVoxel.z++ )
      {

        outputSite.z = ( outputVoxel.z + 0.5 ) * referenceResolutions.z;
        for ( outputVoxel.y = localOutputBoundingBox.getLowerY();
              outputVoxel.y < localOutputBoundingBox.getUpperY() + 1;
              outputVoxel.y++ )
        {

          outputSite.y = ( outputVoxel.y + 0.5 ) * referenceResolutions.y;
          for ( outputVoxel.x = localOutputBoundingBox.getLowerX();
                outputVoxel.x < localOutputBoundingBox.getUpperX() + 1;
                outputVoxel.x++ )
          {

            outputSite.x = ( outputVoxel.x + 0.5 ) * referenceResolutions.x;


            if ( outputMaskVolume->getSizeX() <= 1 ||
                ( *outputMaskVolume )( outputVoxel ) == 1 )
            {

              tmpSite = outputSite;
              std::list< gkg::RCPointer< gkg::Transform3d< float > > >::
                const_reverse_iterator t = transform3ds.rbegin(),
                                      te = transform3ds.rend();
              while ( t != te )
              {

                ( *t )->getInverse( tmpSite, fovSite );
                tmpSite = fovSite;
                ++ t;

              }

              if ( fovBoundingBox.contains( fovSite ) )
              {

                voxelToTreat.push_back( outputVoxel );

              }

            }

          }

        }

      }

      if ( verbose )
      {

        std::cout << "composing : " << std::flush;

      }

      DwiComposingGauge dwiComposingGauge( voxelToTreat.size() / 1000 );
      DwiComposingLoopContext< T >
        dwiComposingLoopContext(
                         dwiComposingGauge,
                         voxelToTreat,
                         referenceSizes,
                         dwCount,
                         referenceResolutions,
                         currentFovScaling,
                         resampler,
                         transform3ds,
                         t2FovVolume,
                         outputMaskVolume,
                         dwFovVolume,
                         background,
                         localOutputBoundingBox,
                         gkg::PartialVolumingQuickResampler< T >::getInstance(),
                         Ls,
                         Btargets,
                         laplaceBeltramiRegularizationFactors,
                         originQSpaceSampling,
                         sphericalHarmonicOrders,
                         globalMatrixSize,
                         originQSpaceSampling.getRawBValues(),
                         originQSpaceSampling.getRawOrientations(),
                         originQSpaceSampling.getDeltaBValue(),
                         &outputT2Volume,
                         &outputDWVolume,
                         targetOrientationSets,
                         verbose );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &dwiComposingLoopContext,
                      0,
                      voxelToTreat.size() );

      threadedLoop.launch();

      // adding the Q-space sampling information only for the last FOV since
      // it is the same for all FOVs
      if ( fovIndex == fovCount - 1 )
      {

        gkg::Writer::getInstance().write( fileNameOutputT2Volume, outputT2Volume );
        targetQSpaceSampling->addInformationToHeader( outputDWVolume );
        gkg::Writer::getInstance().write( fileNameOutputDWVolume, outputDWVolume );
        targetQSpaceSampling->saveBValAndBVecFiles( fileNameOutputBVal,
                                                    fileNameOutputBVec );

      }

      offsetTransform3d += totalFileNameTransform3dCount;


    }

  }
  GKG_CATCH( "template < class T > "
             "void "
             "DwiPostMortemBlockToWholeImageComposerProcess::compose( "
             "gkg::Process& process, "
             "const std::string&, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}



void DwiPostMortemBlockToWholeImageComposerProcess::getFlipping3d(
                              const std::string& fileName,
                              gkg::HomogeneousTransform3d< float >& flipping3d )
{

  try
  {

    if ( fileName == "no_flipping" )
    {

      flipping3d = gkg::IdentityTransform3d< float >();

    }
    else
    {

      std::ifstream is ( fileName.c_str() );
    
      if ( !is )
      {
    
        throw std::runtime_error( std::string("unable to open '" ) + 
                                  fileName + "'" );

      }
    

      gkg::CompositeTransform3d< float >
        compositeTransform3d;
      std::string flippingType;
      while ( !is.eof() )
      {

        flippingType = "";
        is >> flippingType;

        if ( flippingType == "xy" )
        {

          gkg::HomogeneousTransform3d< float >
            localFlipping3d( 0.0f, 1.0f, 0.0f, 0.0f,
                             1.0f, 0.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 1.0f, 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f );
          compositeTransform3d.compose( localFlipping3d );

        }
        else if ( flippingType == "xz" )
        {

          gkg::HomogeneousTransform3d< float >
            localFlipping3d( 0.0f, 0.0f, 1.0f, 0.0f,
                             0.0f, 1.0f, 0.0f, 0.0f,
                             1.0f, 0.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f );
          compositeTransform3d.compose( localFlipping3d );

        }
        else if ( flippingType == "yz" )
        {

          gkg::HomogeneousTransform3d< float >
            localFlipping3d( 1.0f, 0.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 1.0f, 0.0f,
                             0.0f, 1.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 0.0f, 1.0f );
          compositeTransform3d.compose( localFlipping3d );

        }
        else if ( flippingType == "x" )
        {

          gkg::HomogeneousTransform3d< float >
            localFlipping3d( -1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f );
          compositeTransform3d.compose( localFlipping3d );

        }
        else if ( flippingType == "y" )
        {

          gkg::HomogeneousTransform3d< float >
            localFlipping3d( 1.0f,  0.0f, 0.0f, 0.0f,
                             0.0f, -1.0f, 0.0f, 0.0f,
                             0.0f,  0.0f, 1.0f, 0.0f,
                             0.0f,  0.0f, 0.0f, 1.0f );
          compositeTransform3d.compose( localFlipping3d );

        }
        else if ( flippingType == "z" )
        {

          gkg::HomogeneousTransform3d< float >
            localFlipping3d( 1.0f, 0.0f,  0.0f, 0.0f,
                             0.0f, 1.0f,  0.0f, 0.0f,
                             0.0f, 0.0f, -1.0f, 0.0f,
                             0.0f, 0.0f,  0.0f, 1.0f );
          compositeTransform3d.compose( localFlipping3d );

        }

      }
    
      is.close();

      flipping3d = compositeTransform3d.getComposition();

    }

  }
  GKG_CATCH( "void DwiPostMortemBlockToWholeImageComposerProcess::"
             "getFlipping3d( "
             "const std::string& fileName, "
             "gkg::HomogeneousTransform3d< float >& flipping3d )" );

}


void DwiPostMortemBlockToWholeImageComposerProcess::readTransform3ds(
        const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
        int32_t offsetTransform3d,
        const std::vector< int32_t >& transform3dFileNameCounts,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& transform3ds,
        const std::list< bool >& applyLinearTransformFirstList,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& rotation3ds,
        bool verbose )
{

  try
  {

    int32_t transform3dCount = ( int32_t )transform3dFileNameCounts.size();

    int32_t t = 0;
    int32_t localOffsetTransform3d = offsetTransform3d;
    std::list< bool >::const_iterator
      applyLinear = applyLinearTransformFirstList.begin();
    for ( t = 0; t < transform3dCount; t++ )
    {

      //////////////////////////////////////////////////////////////////////////
      // in all cases, the first transformation is linear transformation
      //////////////////////////////////////////////////////////////////////////


      gkg::RCPointer< gkg::Rotation3d< float > > rotation3d(
                                                 new gkg::Rotation3d< float > );
      gkg::RCPointer< gkg::Transform3d< float > > transform3d;

      if ( transform3dFileNameCounts[ t ] == 1 )
      {

        std::string fileName = fileNameFovToReferenceTransform3ds[
                                                        localOffsetTransform3d ];
        ++ localOffsetTransform3d;

        try
        {

          gkg::RigidTransform3d< float >*
            rigidTransform3d = new gkg::RigidTransform3d< float >;
          std::ifstream is( fileName.c_str() );
          rigidTransform3d->readTrm( is );
          is.close();
          *rotation3d = rigidTransform3d->getRotation3d();
          transform3d.reset( rigidTransform3d );

          transform3ds.push_back( transform3d );
          rotation3ds.push_back( rotation3d );

          if ( verbose )
          {

            std::cout << "- " << fileName 
                      << " -> rigid transformation detected" << std::endl;

          }

        }
        catch ( std::exception& )
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          std::ifstream is( fileName.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          *rotation3d = affineTransform3d->getRotation3d();
          transform3d.reset( affineTransform3d );

          transform3ds.push_back( transform3d );
          rotation3ds.push_back( rotation3d );

          if ( verbose )
          {

            std::cout << "- " << fileName 
                      << " -> affine transformation detected" << std::endl;

          }

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // then if it is a diffeomorphic transformation, reading the vector field
      //////////////////////////////////////////////////////////////////////////

      if ( transform3dFileNameCounts[ t ] == 3 )
      {

        gkg::RCPointer< gkg::NonLinearTransform3d< float > > 
          nonLinearTransform3d;

        nonLinearTransform3d.reset(
                      new gkg::NonLinearTransform3d< float >( *applyLinear ) );
        
        nonLinearTransform3d->readTrm(
                                  fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d ],
                                  fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d + 1 ],
                                  fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d + 2 ]  );
        ++ localOffsetTransform3d;
        ++ localOffsetTransform3d;
        ++ localOffsetTransform3d;
        ++ applyLinear;

        transform3ds.push_back( nonLinearTransform3d );


        if ( verbose )
        {

          std::cout << "- " << fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d - 2 ]
                    << " / " << fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d - 1 ]
                    << " -> diffeomorphic transformation detected" << std::endl;

        }


      }

    }

  }
  GKG_CATCH( "void "
             "DwiPostMortemBlockToWholeImageComposerProcess::readTransform3ds( "
             "const std::vector< std::string >& "
             "fileNameFovToReferenceTransform3ds, "
             "int32_t offsetTransform3d, "
             "const std::vector< int32_t >& transform3dFileNameCounts, "
             "std::list< gkg::RCPointer< gkg::Transform3d< float > > >& "
             "transform3ds, "
             "const std::list< bool >& applyLinearTransformFirstList, "
             "std::list< gkg::RCPointer< gkg::Transform3d< float > > >& "
             "rotation3ds, "
             "bool verbose )" );


}




//
//   DwiPostMortemBlockToWholeImageComposerCommand
//

gkg::DwiPostMortemBlockToWholeImageComposerCommand::
                                  DwiPostMortemBlockToWholeImageComposerCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                                   : gkg::Command( argc, argv,
                                                                   loadPlugin,
                                                                   removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiPostMortemBlockToWholeImageComposerCommand::"
             "DwiPostMortemBlockToWholeImageComposerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiPostMortemBlockToWholeImageComposerCommand::
                                  DwiPostMortemBlockToWholeImageComposerCommand(
           const std::vector< std::string >& fileNameT2Fovs,
           const std::vector< std::string >& fileNameOutputCompositionMask,
           const std::vector< std::string >& fileNameDWFovs,
           const std::vector< std::string >& fileNameBvalFovs,
           const std::vector< std::string >& fileNameBvecFovs,
           const std::vector< std::string >& fileNameFlippingFovs,
           const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
           const std::vector< std::string >& fovToReferenceTransform3dTypes,
           const std::string& fileNameOutputT2Volume,
           const std::string& fileNameOutputDWVolume,
           const std::string& fileNameOutputBVal,
           const std::string& fileNameOutputBVec,
           const std::vector< double >& fovScalings,
           const gkg::Vector3d< int32_t >& referenceSizes,
           const gkg::Vector3d< double >& referenceResolutions,
           double deltaBValue,
           const std::vector< int32_t >& sphericalHarmonicOrders,
           const std::vector< double >& laplaceBeltramiRegularizationFactors,
           int32_t resamplingOrder,
           double background,
           const gkg::BoundingBox< int32_t >& outputBoundingBox,
           const std::string& format,
           bool verbose )
                                                   : gkg::Command()
{

  try
  {

    execute( fileNameT2Fovs,
             fileNameOutputCompositionMask,
             fileNameDWFovs,
             fileNameBvalFovs,
             fileNameBvecFovs,
             fileNameFlippingFovs,
             fileNameFovToReferenceTransform3ds,
             fovToReferenceTransform3dTypes,
             fileNameOutputT2Volume,
             fileNameOutputDWVolume,
             fileNameOutputBVal,
             fileNameOutputBVec,
             fovScalings,
             referenceSizes,
             referenceResolutions,
             deltaBValue,
             sphericalHarmonicOrders,
             laplaceBeltramiRegularizationFactors,
             resamplingOrder,
             background,
             outputBoundingBox,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::DwiPostMortemBlockToWholeImageComposerCommand::"
             "DwiPostMortemBlockToWholeImageComposerCommand( "
             "const std::vector< std::string >& fileNameT2Fovs, "
             "const std::vector< std::string >& fileNameOutputCompositionMask, "
             "const std::vector< std::string >& fileNameDWFovs, "
             "const std::vector< std::string >& fileNameBvalFovs, "
             "const std::vector< std::string >& fileNameBvecFovs, "
             "const std::vector< std::string >& fileNameFlippingFovs, "
             "const std::vector< std::string >& "
             "fileNameFovToReferenceTransform3ds, "
             "const std::vector< std::string >& "
             "fovToReferenceTransform3dTypes, "
             "const std::string& fileNameOutputT2Volume, "
             "const std::string& fileNameOutputDWVolume, "
             "const std::string& fileNameOutputBVal, "
             "const std::string& fileNameOutputBVec, "
             "const std::vector< double >& fovScalings, "
             "const gkg::Vector3d< int32_t >& referenceSizes, "
             "const gkg::Vector3d< double >& referenceResolutions, "
             "double deltaBValue, "
             "const std::vector< int32_t >& sphericalHarmonicOrders, "
             "const std::vector< double >& "
              "laplaceBeltramiRegularizationFactors, "
             "int32_t resamplingOrder, "
             "double background, "
             "const gkg::BoundingBox< int32_t >& outputBoundingBox, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::DwiPostMortemBlockToWholeImageComposerCommand::
                                  DwiPostMortemBlockToWholeImageComposerCommand(
                                             const gkg::Dictionary& parameters )
                                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameT2Fovs );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameOutputCompositionMask );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameDWFovs );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameBvalFovs );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameBvecFovs );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameFlippingFovs );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameFovToReferenceTransform3ds );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fovToReferenceTransform3dTypes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputT2Volume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputDWVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputBVal );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputBVec );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           fovScalings );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          referenceSizes );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           referenceResolutions );
    DECLARE_FLOATING_PARAMETER( parameters, double, deltaBValue );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          sphericalHarmonicOrders );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER(
                                         parameters,
                                         std::vector< double >,
                                         laplaceBeltramiRegularizationFactors );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, resamplingOrder );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          outputBoundingBox );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );


    gkg::Vector3d< int32_t > vector3dOfReferenceSizes( referenceSizes[ 0 ],
                                                       referenceSizes[ 1 ],
                                                       referenceSizes[ 2 ] );
    gkg::Vector3d< double >
      vector3dOfReferenceResolutions( referenceResolutions[ 0 ],
                                      referenceResolutions[ 1 ],
                                      referenceResolutions[ 2 ] );

    gkg::BoundingBox< int32_t > 
      outputBBox( 0, referenceSizes[ 0 ] - 1,
                  0, referenceSizes[ 1 ] - 1,
                  0, referenceSizes[ 2 ] - 1 );
    if ( outputBoundingBox.size() == 6U )
    {
    
      outputBBox.setLowerX( outputBoundingBox[ 0 ] );
      outputBBox.setLowerY( outputBoundingBox[ 1 ] );
      outputBBox.setLowerZ( outputBoundingBox[ 2 ] );
      outputBBox.setUpperX( outputBoundingBox[ 3 ] );
      outputBBox.setUpperY( outputBoundingBox[ 4 ] );
      outputBBox.setUpperZ( outputBoundingBox[ 5 ] );

    }                                                   

    execute( fileNameT2Fovs,
             fileNameOutputCompositionMask,
             fileNameDWFovs,
             fileNameBvalFovs,
             fileNameBvecFovs,
             fileNameFlippingFovs,
             fileNameFovToReferenceTransform3ds,
             fovToReferenceTransform3dTypes,
             fileNameOutputT2Volume,
             fileNameOutputDWVolume,
             fileNameOutputBVal,
             fileNameOutputBVec,
             fovScalings,
             vector3dOfReferenceSizes,
             vector3dOfReferenceResolutions,
             deltaBValue,
             sphericalHarmonicOrders,
             laplaceBeltramiRegularizationFactors,
             resamplingOrder,
             background,
             outputBBox, 
             format,
             verbose );

  }
  GKG_CATCH( "gkg::DwiPostMortemBlockToWholeImageComposerCommand::"
             "DwiPostMortemBlockToWholeImageComposerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiPostMortemBlockToWholeImageComposerCommand::
                                ~DwiPostMortemBlockToWholeImageComposerCommand()
{
}


std::string gkg::DwiPostMortemBlockToWholeImageComposerCommand::getStaticName()
{

  try
  {

    return "DwiPostMortemBlockToWholeImageComposer";

  }
  GKG_CATCH( "std::string gkg::DwiPostMortemBlockToWholeImageComposerCommand::"
             "getStaticName()" );

}


void gkg::DwiPostMortemBlockToWholeImageComposerCommand::parse()
{

  try
  {


    std::vector< std::string > fileNameT2Fovs;
    std::vector< std::string > fileNameOutputCompositionMask;
    std::vector< std::string > fileNameDWFovs;
    std::vector< std::string > fileNameBvalFovs;
    std::vector< std::string > fileNameBvecFovs;
    std::vector< std::string > fileNameFlippingFovs;
    std::vector< std::string > fileNameFovToReferenceTransform3ds;
    std::vector< std::string > fovToReferenceTransform3dTypes;
    std::string fileNameOutputT2Volume;
    std::string fileNameOutputDWVolume;
    std::string fileNameOutputBVal;
    std::string fileNameOutputBVec;
    std::vector< double > fovScalings;
    std::vector< int32_t > vectorOfReferenceSizes;
    std::vector< double > vectorOfReferenceResolutions;
    double deltaBValue;
    std::vector< int32_t > sphericalHarmonicOrders;
    std::vector< double > laplaceBeltramiRegularizationFactors;
    int32_t resamplingOrder = 1;
    double background = 0.0;
    std::vector< int32_t > vectorOfOutputBoundingBox;
    std::string format;
    bool verbose = false;


    gkg::Application application(
                                _argc, _argv,
                                "DWI Post-mortem block to whole brain composer",
                                _loadPlugin );
    application.addSeriesOption( "-t2",
                                 "T2-weighted at b=0s/mm2 Block FOV filename "
                                 "list",
                                 fileNameT2Fovs,
                                 1 );
    application.addSeriesOption( "-m",
                                 "output composition mask filename "
                                 "list",
                                 fileNameOutputCompositionMask,
                                 0 );
    application.addSeriesOption( "-dw",
                                 "Diffusion-weighted Block FOV filename "
                                 "list",
                                 fileNameDWFovs,
                                 1 );
    application.addSeriesOption( "-bval",
                                 ".bval Block FOV filename list",
                                 fileNameBvalFovs,
                                 1 );
    application.addSeriesOption( "-bvec",
                                 ".bvec Block FOV filename list",
                                 fileNameBvecFovs,
                                 1 );
    application.addSeriesOption( "-flip",
                                 "Flipping Block FOV filename list",
                                 fileNameFlippingFovs,
                                 1 );
    application.addSeriesOption( "-t",
                                 "Block to whole brain 3D transform(s) "
                                 "filename list",
                                 fileNameFovToReferenceTransform3ds,
                                 0 );
    application.addSeriesOption( "-types",
                                 "Block to whole brain 3D transform(s) "
                                 "type(s) among :\n"
                                 "- linear (<direct>.trm)\n"
                                 "- linear_and_diffeomorphic "
                                 "(<direct>.trm <direct>.ima <inverse>.ima)\n"
                                 "- diffeomorphic_and_linear "
                                 "(<direct>.trm <direct>.ima <inverse>.ima)"
                                 "(apply linear transform first)\n"
                                 "(default=none)",
                                 fovToReferenceTransform3dTypes,
                                 0 );
    application.addSingleOption( "-ot2",
                                 "Output T2 volume filename",
                                 fileNameOutputT2Volume );
    application.addSingleOption( "-odw",
                                 "Output DW volume filename",
                                 fileNameOutputDWVolume );
    application.addSingleOption( "-obval",
                                 "Output *.bval filename",
                                 fileNameOutputBVal );
    application.addSingleOption( "-obvec",
                                 "Output *.bvec filename",
                                 fileNameOutputBVec );
    application.addSeriesOption( "-s",
                                 "FOV scaling list",
                                 fovScalings,
                                 1 );
    application.addSeriesOption( "-referenceSizes",
                                 "Reference sizes along X, Y and Z axis",
                                 vectorOfReferenceSizes,
                                 3, 3 );
    application.addSeriesOption( "-referenceResolutions",
                                 "Reference resolutions along X, Y and Z axis",
                                 vectorOfReferenceResolutions,
                                 3, 3 );
    application.addSingleOption( "-b",
                                 "Delta b-value in s/mm2",
                                 deltaBValue );
    application.addSeriesOption( "-sh",
                                 "Spherical harmonics order list",
                                 sphericalHarmonicOrders,
                                 1 );
    application.addSeriesOption( "-lb",
                                 "Laplace-Berltrami regularization factor list",
                                 laplaceBeltramiRegularizationFactors,
                                 1 );
    application.addSingleOption( "-resamplingOrder",
                                 "Resampling order (0 to 7) (default=1)",
                                 resamplingOrder,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0.0)",
                                 background,
                                 true );
    application.addSeriesOption( "-outputBoundingBox",
                                 "Output bounding box "
                                 "(default=whole ref volume)",
                                 vectorOfOutputBoundingBox,
                                 0, 6 );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();


    gkg::Vector3d< int32_t > referenceSizes( vectorOfReferenceSizes[ 0 ],
                                             vectorOfReferenceSizes[ 1 ],
                                             vectorOfReferenceSizes[ 2 ] );
    gkg::Vector3d< double >
      referenceResolutions( vectorOfReferenceResolutions[ 0 ],
                            vectorOfReferenceResolutions[ 1 ],
                            vectorOfReferenceResolutions[ 2 ] );


    if ( !vectorOfOutputBoundingBox.empty() &&
         ( vectorOfOutputBoundingBox.size() != 6U ) )
    {

      throw std::runtime_error( "output bounding box should contain 6 scalars "
                                "xinf, yinf, zinf, xsup, ysup, zsup" );

    } 

    gkg::BoundingBox< int32_t > 
      outputBoundingBox( 0, referenceSizes.x - 1,
                         0, referenceSizes.y - 1,
                         0, referenceSizes.z - 1 );
    if ( vectorOfOutputBoundingBox.size() == 6U )
    {
    
      outputBoundingBox.setLowerX( vectorOfOutputBoundingBox[ 0 ] );
      outputBoundingBox.setLowerY( vectorOfOutputBoundingBox[ 1 ] );
      outputBoundingBox.setLowerZ( vectorOfOutputBoundingBox[ 2 ] );
      outputBoundingBox.setUpperX( vectorOfOutputBoundingBox[ 3 ] );
      outputBoundingBox.setUpperY( vectorOfOutputBoundingBox[ 4 ] );
      outputBoundingBox.setUpperZ( vectorOfOutputBoundingBox[ 5 ] );

    }                                                   

    execute( fileNameT2Fovs,
             fileNameOutputCompositionMask,
             fileNameDWFovs,
             fileNameBvalFovs,
             fileNameBvecFovs,
             fileNameFlippingFovs,
             fileNameFovToReferenceTransform3ds,
             fovToReferenceTransform3dTypes,
             fileNameOutputT2Volume,
             fileNameOutputDWVolume,
             fileNameOutputBVal,
             fileNameOutputBVec,
             fovScalings,
             referenceSizes,
             referenceResolutions,
             deltaBValue,
             sphericalHarmonicOrders,
             laplaceBeltramiRegularizationFactors,
             resamplingOrder,
             background,
             outputBoundingBox,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiPostMortemBlockToWholeImageComposerCommand::"
                     "parse()" );

}


void gkg::DwiPostMortemBlockToWholeImageComposerCommand::execute(
           const std::vector< std::string >& fileNameT2Fovs,
           const std::vector< std::string >& fileNameOutputCompositionMask,
           const std::vector< std::string >& fileNameDWFovs,
           const std::vector< std::string >& fileNameBvalFovs,
           const std::vector< std::string >& fileNameBvecFovs,
           const std::vector< std::string >& fileNameFlippingFovs,
           const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
           const std::vector< std::string >& fovToReferenceTransform3dTypes,
           const std::string& fileNameOutputT2Volume,
           const std::string& fileNameOutputDWVolume,
           const std::string& fileNameOutputBVal,
           const std::string& fileNameOutputBVec,
           const std::vector< double >& fovScalings,
           const gkg::Vector3d< int32_t >& referenceSizes,
           const gkg::Vector3d< double >& referenceResolutions,
           double deltaBValue,
           const std::vector< int32_t >& sphericalHarmonicOrders,
           const std::vector< double >& laplaceBeltramiRegularizationFactors,
           int32_t resamplingOrder,
           double background,
           const gkg::BoundingBox< int32_t >& outputBoundingBox,
           const std::string& format,
           bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // launching registration process
    ////////////////////////////////////////////////////////////////////////////

    DwiPostMortemBlockToWholeImageComposerProcess
      dwiPostMortemBlockToWholeImageComposerProcess(
                                           fileNameT2Fovs,
                                           fileNameOutputCompositionMask,
                                           fileNameDWFovs,
                                           fileNameBvalFovs,
                                           fileNameBvecFovs,
                                           fileNameFlippingFovs,
                                           fileNameFovToReferenceTransform3ds,
                                           fovToReferenceTransform3dTypes,
                                           fileNameOutputT2Volume,
                                           fileNameOutputDWVolume,
                                           fileNameOutputBVal,
                                           fileNameOutputBVec,
                                           fovScalings,
                                           referenceSizes,
                                           referenceResolutions,
                                           deltaBValue,
                                           sphericalHarmonicOrders,
                                           laplaceBeltramiRegularizationFactors,
                                           resamplingOrder,
                                           background,
                                           outputBoundingBox,
                                           format,
                                           verbose );
    dwiPostMortemBlockToWholeImageComposerProcess.execute(
                                                          fileNameT2Fovs[ 0 ] );

  }
  GKG_CATCH( "void gkg::DwiPostMortemBlockToWholeImageComposerCommand::"
             "execute( "
             "const std::vector< std::string >& fileNameT2Fovs, "
             "const std::vector< std::string >& fileNameOutputCompositionMask, "
             "const std::vector< std::string >& fileNameDWFovs, "
             "const std::vector< std::string >& fileNameBvalFovs, "
             "const std::vector< std::string >& fileNameBvecFovs, "
             "const std::vector< std::string >& fileNameFlippingFovs, "
             "const std::vector< std::string >& "
             "fileNameFovToReferenceTransform3ds, "
             "const std::vector< std::string >& "
             "fovToReferenceTransform3dTypes, "
             "const std::string& fileNameOutputT2Volume, "
             "const std::string& fileNameOutputDWVolume, "
             "const std::string& fileNameOutputBVal, "
             "const std::string& fileNameOutputBVec, "
             "const std::vector< double >& fovScalings, "
             "const gkg::Vector3d< int32_t >& referenceSizes, "
             "const gkg::Vector3d< double >& referenceResolutions, "
             "double deltaBValue, "
             "const std::vector< int32_t >& sphericalHarmonicOrders, "
             "const std::vector< double >& "
             "laplaceBeltramiRegularizationFactors, "
             "int32_t resamplingOrder, "
             "double background, "
             "const gkg::BoundingBox< int32_t >& outputBoundingBox, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiPostMortemBlockToWholeImageComposerCommand,
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameT2Fovs ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameOutputCompositionMask ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameDWFovs ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameBvalFovs ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameBvecFovs ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameFlippingFovs ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( 
                                          fileNameFovToReferenceTransform3ds ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fovToReferenceTransform3dTypes ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputT2Volume ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputDWVolume ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputBval ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputBvec ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( fovScalings ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( referenceSizes ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( referenceResolutions ) +
    DECLARE_FLOATING_PARAMETER_HELP( deltaBValue ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( sphericalHarmonicOrders ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( 
                                        laplaceBeltramiRegularizationFactors ) +
    DECLARE_INTEGER_PARAMETER_HELP( resamplingOrder ) +
    DECLARE_FLOATING_PARAMETER_HELP( background ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( outputBoundingBox ) +
    DECLARE_STRING_PARAMETER_HELP( format ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
