 #include <gkg-processing-plugin-functors/PottsRegularizer/PottsRegularizerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>


//
// inline functions to compute potentials and energies
//

template < class T >
inline
double differentFromFunction( const T& value1, const T& value2 )
{

  if ( value1 != value2 )
  {

    return 1.0;

  }
  return 0.0;

}


template < class T >
inline 
double getPottsPenalty( const gkg::Vector3d< int32_t >& neighbor,
                        const gkg::Vector3d< int32_t >& voxel,
                        const gkg::Volume< T >& regularizedVolume )
{

  try
  {

    return differentFromFunction( regularizedVolume( neighbor ),
                                  regularizedVolume( voxel ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double getPottsPenalty( "
             "const gkg::Vector3d< int32_t >& neighbor, "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::Volume< T >& regularizedVolume )" );

}


template < class T >
inline 
double getAttachmentPenalty( const gkg::Vector3d< int32_t >& voxel,
                             const gkg::Volume< T >& oversampledVolume,
                             const gkg::Volume< T >& regularizedVolume )
{

  try
  {

    return differentFromFunction( oversampledVolume( voxel ),
                                  regularizedVolume( voxel ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double getAttachmentPenalty( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::Volume< T >& oversampledVolume, "
             "const gkg::Volume< T >& regularizedVolume )" );

}



template < class T >
inline 
double computePottsPotential(
                 const gkg::Vector3d< int32_t >& voxel,
                 const gkg::Volume< T >& regularizedVolume,
                 const gkg::Volume< uint8_t >& mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets,
                 const std::vector< double >& neighborInverseDistances )
{

  try
  {

    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      o = neighborOffsets.begin(),
      oe = neighborOffsets.end();
    std::vector< double >::const_iterator id = neighborInverseDistances.begin();
    gkg::Vector3d< int32_t > neighbor;
    double pottsPotential = 0.0;
    while ( o != oe )
    {

      neighbor = voxel + *o;

      if ( boundingBox.contains( neighbor ) )
      {

        if ( mask( neighbor ) )
        {

          pottsPotential += *id * differentFromFunction( 
                                                  regularizedVolume( neighbor ),
                                                  regularizedVolume( voxel ) );

        }

      }
      ++ o;
      ++ id;

    }
    return pottsPotential;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double computePottsPotential( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::Volume< T >& regularizedVolume, "
             "const gkg::Volume< uint8_t >& mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets, "
             "const std::vector< double >& neighborInverseDistances )" );

}


template < class T >
inline 
double computePottsEnergy(
                 const gkg::Volume< T >& regularizedVolume,
                 const gkg::Volume< uint8_t >& mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const std::vector< gkg::Vector3d< int32_t > >& voxelsInMask,
                 const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets,
                 const std::vector< double >& neighborInverseDistances )
{

  try
  {

    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      v = voxelsInMask.begin(),
      ve = voxelsInMask.end();
    double pottsEnergy = 0.0;
    while ( v != ve )
    {

      pottsEnergy += computePottsPotential( *v,
                                            regularizedVolume,
                                            mask,
                                            boundingBox,
                                            neighborOffsets,
                                            neighborInverseDistances );
      ++ v;

    }
    return pottsEnergy;

  }
  GKG_CATCH( "template < class T > "
             "inline  "
             "double computePottsEnergy( "
             "const gkg::Volume< T >& regularizedVolume, "
             "const gkg::Volume< uint8_t >& mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::vector< gkg::Vector3d< int32_t > >& voxelsInMask, "
             "const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets, "
             "const std::vector< double >& neighborInverseDistances )" );

}


template < class T >
inline 
double computeAttachmentPotential( const gkg::Vector3d< int32_t >& voxel,
                                   const gkg::Volume< T >& oversampledVolume,
                                   const gkg::Volume< T >& regularizedVolume )
 
{

  try
  {

    return getAttachmentPenalty( voxel, oversampledVolume, regularizedVolume );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double computeAttachmentPotential( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::Volume< T >& oversampledVolume, "
             "const gkg::Volume< T >& regularizedVolume )" );

}


template < class T >
inline 
double computeAttachmentEnergy(
                 const gkg::Volume< T >& oversampledVolume,
                 const gkg::Volume< T >& regularizedVolume,
                 const gkg::Volume< uint8_t >& mask )
{

  try
  {

    typename gkg::Volume< T >::const_iterator o = oversampledVolume.begin(),
                                              oe = oversampledVolume.end();
    typename gkg::Volume< T >::const_iterator r = regularizedVolume.begin();
    gkg::Volume< uint8_t >::const_iterator m = mask.begin();
    double attachmentEnergy = 0.0;
    while ( o != oe )
    {

      if ( *m )
      {

        attachmentEnergy += differentFromFunction( *o, *r );

      }
      ++ o;
      ++ r;
      ++ m;

    }
    return attachmentEnergy;

  }
  GKG_CATCH( "template < class T > "
             "inline  "
             "double computeAttachmentEnergy( "
             "const gkg::Volume< T >& oversampledVolume, "
             "const gkg::Volume< T >& regularizedVolume, "
             "const gkg::Volume< uint8_t >& mask )" );

}


template < class T >
inline 
double computeEnergy(
                 const gkg::Volume< T >& oversampledVolume,
                 const gkg::Volume< T >& regularizedVolume,
                 const gkg::Volume< uint8_t >& mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const std::vector< gkg::Vector3d< int32_t > >& voxelsInMask,
                 const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets,
                 const std::vector< double >& neighborInverseDistances,
                 double regularizationFactor )
{

  try
  {

    return regularizationFactor * 
           computePottsEnergy( regularizedVolume,
                               mask,
                               boundingBox,
                               voxelsInMask,
                               neighborOffsets,
                               neighborInverseDistances ) +
           computeAttachmentEnergy( oversampledVolume,
                                    regularizedVolume,
                                    mask );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double computeEnergy( "
             "const gkg::Volume< T >& oversampledVolume, "
             "const gkg::Volume< T >& regularizedVolume, "
             "const gkg::Volume< uint8_t >& mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::vector< gkg::Vector3d< int32_t > >& voxelsInMask, "
             "const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets, "
             "const std::vector< double >& neighborInverseDistances, "
             "double regularizationFactor )" );
  

}


template < class T >
inline 
void getOtherLabelsInNeighborhood(
                 const gkg::Vector3d< int32_t >& voxel,
                 const gkg::Volume< T >&  regularizedVolume,
                 const gkg::Volume< uint8_t >&  mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets,
                 std::set< T >& otherLabels )
{

  try
  {

    const T& currentLabel = regularizedVolume( voxel );

    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      o = neighborOffsets.begin(),
      oe = neighborOffsets.end();
    gkg::Vector3d< int32_t > neighbor;
    while ( o != oe )
    {

      neighbor = voxel + *o;

      if ( boundingBox.contains( neighbor ) )
      {

        if ( mask( neighbor ) )
        {

          const T& label = regularizedVolume( neighbor );
          if ( label != currentLabel )
          {

            otherLabels.insert( label );

          }

        }

      }
      ++ o;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void getOtherLabelsInNeighborhood( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::Volume< T >&  regularizedVolume, "
             "const gkg::Volume< uint8_t >&  mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets, "
             "std::set< T >& otherLabels )" );


}



template < class T >
inline 
double getEnergyContribution(
                 const gkg::Vector3d< int32_t >& voxel,
                 const gkg::Volume< T >& oversampledVolume,
                 const gkg::Volume< T >& regularizedVolume,
                 const gkg::Volume< uint8_t >& mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets,
                 const std::vector< double >& neighborInverseDistances,
                 double regularizationFactor )
{

  try
  {

    double energyContribution = 0.0;

    // first adding the contribution of the central voxel
    energyContribution += regularizationFactor * 
                          computePottsPotential( voxel,
                                                 regularizedVolume,
                                                 mask,
                                                 boundingBox,
                                                 neighborOffsets,
                                                 neighborInverseDistances ) +
                          computeAttachmentPotential( voxel,
                                                      oversampledVolume,
                                                      regularizedVolume );

    // and then those of the neighbors
    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      o = neighborOffsets.begin(),
      oe = neighborOffsets.end();
    gkg::Vector3d< int32_t > neighbor;
    while ( o != oe )
    {

      neighbor = voxel + *o;

      if ( boundingBox.contains( neighbor ) )
      {

        if ( mask( neighbor ) )
        {

          energyContribution +=
                             regularizationFactor *
                             computePottsPotential( neighbor,
                                                    regularizedVolume,
                                                    mask,
                                                    boundingBox,
                                                    neighborOffsets,
                                                    neighborInverseDistances ) +
                             computeAttachmentPotential( neighbor,
                                                         oversampledVolume,
                                                         regularizedVolume );

        }

      }
      ++ o;

    }

    return energyContribution;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double getEnergyContribution( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::Volume< T >& oversampledVolume, "
             "const gkg::Volume< T >& regularizedVolume, "
             "const gkg::Volume< uint8_t >& mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const std::vector< gkg::Vector3d< int32_t > >& neighborOffsets, "
             "const std::vector< double >& neighborInverseDistances, "
             "double regularizationFactor )" );


}


//
// class PottsRegularizerProcess
//

class PottsRegularizerProcess : public gkg::Process
{

  public:

    PottsRegularizerProcess( const std::string& theFileNameOut,
                             const std::string& theFileNameOversampled,
                             const std::string& theFileNameEnergy,
                             double theOversamplingFactor,
                             double theRegularizationFactor,
                             const std::string& theNeighborhoodType,
                             const std::string& theMode,
                             const double& theThreshold1,
                             const double& theThreshold2,
                             const int32_t& theMaximumIterationCount,
                             const bool& theAscii,
                             const std::string& theFormat,
                             const bool& theVerbose );

    std::string fileNameOut;
    std::string fileNameOversampled;
    std::string fileNameEnergy;
    double oversamplingFactor;
    double regularizationFactor;
    std::string neighborhoodType;
    std::string mode;
    double threshold1;
    double threshold2;
    int32_t maximumIterationCount;
    bool ascii;
    std::string format;
    bool verbose;

  private:

    template < class T >
    static void regularize( gkg::Process& process,
                            const std::string& fileNameIn,
                            const gkg::AnalyzedObject&,
                            const std::string& );


};


PottsRegularizerProcess::PottsRegularizerProcess( 
                                      const std::string& theFileNameOut,
                                      const std::string& theFileNameOversampled,
                                      const std::string& theFileNameEnergy,
                                      double theOversamplingFactor,
                                      double theRegularizationFactor,
                                      const std::string& theNeighborhoodType,
                                      const std::string& theMode,
                                      const double& theThreshold1,
                                      const double& theThreshold2,
                                      const int32_t& theMaximumIterationCount,
                                      const bool& theAscii,
                                      const std::string& theFormat,
                                      const bool& theVerbose )
                        : gkg::Process( "Volume" ),
                          fileNameOut( theFileNameOut ),
                          fileNameOversampled( theFileNameOversampled ),
                          fileNameEnergy( theFileNameEnergy ),
                          oversamplingFactor( theOversamplingFactor ),
                          regularizationFactor( theRegularizationFactor ),
                          neighborhoodType( theNeighborhoodType ),
                          mode( theMode ),
                          threshold1( theThreshold1 ),
                          threshold2( theThreshold2 ),
                          maximumIterationCount( theMaximumIterationCount ),
                          ascii( theAscii ),
                          format( theFormat ),
                          verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &PottsRegularizerProcess::regularize< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &PottsRegularizerProcess::regularize< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &PottsRegularizerProcess::regularize< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &PottsRegularizerProcess::regularize< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &PottsRegularizerProcess::regularize< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &PottsRegularizerProcess::regularize< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &PottsRegularizerProcess::regularize< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &PottsRegularizerProcess::regularize< uint64_t > );

  }
  GKG_CATCH( "PottsRegularizerProcess::PottsRegularizerProcess( "
             "const std::string& theFileNameOut, "
             "const std::string& theFileNameOversampled, "
             "const std::string& theFileNameEnergy, "
             "double theOversamplingFactor, "
             "double theRegularizationFactor, "
             "const std::string& theNeighborhoodType, "
             "const std::string& theMode, "
             "const double& theThreshold1, "
             "const double& theThreshold2, "
             "const int32_t& theMaximumIterationCount, "
             "const bool& theAscii," 
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void PottsRegularizerProcess::regularize( gkg::Process& process,
                                          const std::string& fileNameIn,
                                          const gkg::AnalyzedObject&,
                                          const std::string& )
{

  try
  {

    PottsRegularizerProcess&
      pottsRegularizerProcess = static_cast< PottsRegularizerProcess& >(
                                                                      process );

    const std::string& fileNameOut =
      pottsRegularizerProcess.fileNameOut;
    const std::string& fileNameOversampled =
      pottsRegularizerProcess.fileNameOversampled;
    const std::string& fileNameEnergy =
      pottsRegularizerProcess.fileNameEnergy;
    const double& oversamplingFactor =
      pottsRegularizerProcess.oversamplingFactor;
    const double& regularizationFactor =
      pottsRegularizerProcess.regularizationFactor;
    const std::string& neighborhoodType =
      pottsRegularizerProcess.neighborhoodType;
    const std::string& mode =
      pottsRegularizerProcess.mode;
    const double& threshold1 =
      pottsRegularizerProcess.threshold1;
    const double& threshold2 =
      pottsRegularizerProcess.threshold2;
    int32_t maximumIterationCount =
      pottsRegularizerProcess.maximumIterationCount;
    const bool& ascii =
      pottsRegularizerProcess.ascii;
    const std::string& format =
      pottsRegularizerProcess.format;
    const bool& verbose =
      pottsRegularizerProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );


    ////////////////////////////////////////////////////////////////////////////
    // reading input data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );

    gkg::Vector3d< int32_t > inputSize;
    volumeIn.getSize( inputSize );

    gkg::Vector3d< double > inputResolution;
    volumeIn.getResolution( inputResolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // oversampling input data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "oversampling (X" << oversamplingFactor << "): "
                << std::flush;

    }

    gkg::Vector3d< int32_t > oversampledSize;
    oversampledSize.x = ( int32_t )( inputSize.x * oversamplingFactor );
    oversampledSize.y = ( int32_t )( inputSize.y * oversamplingFactor );
    oversampledSize.z = ( int32_t )( inputSize.z * oversamplingFactor );

    gkg::Vector3d< double >
      oversampledResolution = inputResolution / oversamplingFactor;
    gkg::Volume< T > oversampledVolume( oversampledSize );
    oversampledVolume.setResolution( oversampledResolution );

    gkg::Resampler< T >*
      resampler = gkg::ResamplerFactory< T >::getInstance().getResampler( 0 );

    gkg::IdentityTransform3d< float > identityTransform3d;

    resampler->resample( volumeIn,
                         identityTransform3d,
                         ( T )threshold1,
                         oversampledVolume,
                         verbose );

    gkg::BoundingBox< int32_t > boundingBox( oversampledVolume );


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving oversampled input data
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameOversampled.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving oversampled input data : " << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameOversampled,
                                        oversampledVolume,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building collecting voxels within the mask
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

      std::cout << "inspecting oversampled volume : " << std::flush;

    }

    gkg::RCPointer< gkg::TestFunction< T > >
      testFunction(
                gkg::TestFunctionFactory< T >::getInstance().createTestFunction( 
                                                                 mode,
	                                                         threshold1,
                                                                 threshold2 ) );

    std::vector< gkg::Vector3d< int32_t > > voxelsInMask;
    voxelsInMask.reserve( oversampledSize.x *
                          oversampledSize.y *
                          oversampledSize.z );
    std::set< T > labelSet;

    gkg::Volume< uint8_t > mask( oversampledSize );
    mask.fill( 0U );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < oversampledSize.z; z++ )
    {

      for ( y = 0; y < oversampledSize.y; y++ )
      {

        for ( x = 0; x < oversampledSize.x; x++ )
        {

          const T& value = oversampledVolume( x, y, z );
          if ( testFunction->getState( value ) )
          {

            voxelsInMask.push_back( gkg::Vector3d< int32_t >( x, y, z ) );
            labelSet.insert( value );
            mask( x, y, z ) = 1U;

          }

        }

      }

    }

    int32_t voxelCount = ( int32_t )voxelsInMask.size();
    std::vector< T > labels( labelSet.begin(), labelSet.end() );
    if ( maximumIterationCount == -1 )
    {

      maximumIterationCount = 2 * voxelCount;

    }

    if ( verbose )
    {

      std::cout << "voxelCount=" << voxelCount << " / labels=";
      typename std::vector< T >::const_iterator 
        l = labels.begin(),
        le = labels.end();
      while ( l != le )
      {

        std::cout << *l << " ";
        ++ l;

      }

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating neighborhood
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating neighborhood " << neighborhoodType << " : "
                << std::flush;

    }
    std::list< gkg::Vector3d< int32_t > > neighborOffsetList;
    if ( neighborhoodType == "4xy" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood2d_4xy );

    }
    else if ( neighborhoodType == "4xz" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood2d_4xz );

    }
    else if ( neighborhoodType == "4yz" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood2d_4yz );

    }
    else if ( neighborhoodType == "8xy" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood2d_8xy );

    }
    else if ( neighborhoodType == "8xz" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood2d_8xz );

    }
    else if ( neighborhoodType == "8yz" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood2d_8yz );

    }
    else if ( neighborhoodType == "6" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood3d_6 );

    }
    else if ( neighborhoodType == "18" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood3d_18 );

    }
    else if ( neighborhoodType == "26" )
    {

      neighborOffsetList = gkg::Neighborhood3d::getInstance().
                  getNeighborOffsets( gkg::Neighborhood3d::Neighborhood3d_26 );

    }

    std::vector< gkg::Vector3d< int32_t > >
      neighborOffsets( neighborOffsetList.begin(), neighborOffsetList.end() );


    std::vector< double > neighborInverseDistances( neighborOffsets.size() );
    std::vector< double >::iterator id = neighborInverseDistances.begin(),
                                    ide = neighborInverseDistances.end();
    std::vector< gkg::Vector3d< int32_t > >::const_iterator
                                                    o = neighborOffsets.begin();
    while ( id != ide )
    {

      *id = 1.0 / o->getDoubleNorm();
      ++ id;
      ++ o;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // regularizing data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "regularizing : " << std::flush;

    }

    gkg::Volume< T > regularizedVolume = oversampledVolume;

    double energy = computeEnergy( oversampledVolume,
                                   regularizedVolume,
                                   mask,
                                   boundingBox,
                                   voxelsInMask,
                                   neighborOffsets,
                                   neighborInverseDistances,
                                   regularizationFactor );

    std::ofstream os;

    if ( !fileNameEnergy.empty() )
    {

      os.open( fileNameEnergy.c_str() );
      os << 0 << "     " << energy << std::endl;

    }

    if ( verbose )
    {

      std::cout << std::setw( 15 ) << 0
                << " / "
                << std::setw( 15 ) << maximumIterationCount
                << " -> E="
                << std::setw( 20 ) << energy
                << std::flush;

    }

    std::vector< int32_t > randomIndices( voxelCount );
    int32_t iterationCount = 0;
    int32_t voxelIndex = 0;
    while ( iterationCount < maximumIterationCount )
    {

      voxelIndex = factory->getUniformRandomUInt32( randomGenerator, 
                                                    voxelCount - 1 );

      const gkg::Vector3d< int32_t >& 
        voxel = voxelsInMask[ voxelIndex ];

      std::set< T > otherLabelsInNeighborhood;
      getOtherLabelsInNeighborhood( voxel,
                                    regularizedVolume,
                                    mask,
                                    boundingBox,
                                    neighborOffsets,
                                    otherLabelsInNeighborhood );

      double currentEnergyContribution = getEnergyContribution(
                                                       voxel,
                                                       oversampledVolume,
                                                       regularizedVolume,
                                                       mask,
                                                       boundingBox,
                                                       neighborOffsets,
                                                       neighborInverseDistances,
                                                       regularizationFactor );

      const T& currentLabel = regularizedVolume( voxel );

      typename std::set< T >::const_iterator
        l = otherLabelsInNeighborhood.begin(),
        le = otherLabelsInNeighborhood.end();
      T bestLabel = currentLabel;
      double otherEnergyContribution = 0.0;
      double bestEnergyContribution = currentEnergyContribution;
      while ( l != le )
      {

        regularizedVolume( voxel ) = *l;
        otherEnergyContribution = getEnergyContribution(
                                                       voxel,
                                                       oversampledVolume,
                                                       regularizedVolume,
                                                       mask,
                                                       boundingBox,
                                                       neighborOffsets,
                                                       neighborInverseDistances,
                                                       regularizationFactor );

        if ( otherEnergyContribution < bestEnergyContribution )
        {

          bestLabel = *l;
          bestEnergyContribution = otherEnergyContribution;

        }
        ++ l;

      }

      energy += bestEnergyContribution - currentEnergyContribution;
      regularizedVolume( voxel ) = bestLabel;


      if ( ( iterationCount % 5000 == 0 ) && verbose )
      {

        std::cout << gkg::Eraser( 59 );
        std::cout << std::setw( 15 ) << iterationCount
                  << " / "
                  << std::setw( 15 ) << maximumIterationCount
                  << " -> E="
                  << std::setw( 20 ) << energy
                  << std::flush;

      }
      if ( !fileNameEnergy.empty() && ( iterationCount % 1000 == 0 ) )
      {

        os << iterationCount << "     " << energy << std::endl;

      }

      ++ iterationCount;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 59 );
      std::cout << std::setw( 15 ) << iterationCount
                << " / "
                << std::setw( 15 ) << maximumIterationCount
                << " -> E="
                << std::setw( 20 ) << energy
                << std::flush;

    }

    if ( !fileNameEnergy.empty() )
    {

      os.close();

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing result
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      regularizedVolume,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "template < class T > "
             "void PottsRegularizerProcess::binarize( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class PottsRegularizerCommand
//


gkg::PottsRegularizerCommand::PottsRegularizerCommand( int32_t argc,
                                                       char* argv[],
                                                       bool loadPlugin,
                                                       bool removeFirst )
                             : gkg::Command( argc, argv, 
                                             loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::PottsRegularizerCommand::PottsRegularizerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::PottsRegularizerCommand::PottsRegularizerCommand(
                                         const std::string& fileNameIn,
                                         const std::string& fileNameOut,
                                         const std::string& fileNameOversampled,
                                         const std::string& fileNameEnergy,
                                         double oversamplingFactor,
                                         double regularizationFactor,
                                         const std::string& neighborhoodType,
                                         const std::string& mode,
                                         double threshold1,
                                         double threshold2,
                                         int32_t maximumIterationCount,
                                         bool ascii,
                                         const std::string& format,
                                         bool verbose )
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             fileNameOversampled,
             fileNameEnergy,
             oversamplingFactor,
             regularizationFactor,
             neighborhoodType,
             mode,
             threshold1,
             threshold2,
             maximumIterationCount,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::PottsRegularizerCommand::PottsRegularizerCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameOversampled, "
             "const std::string& fileNameEnergy, "
             "double oversamplingFactor, "
             "double regularizationFactor, "
             "const std::string& neighborhoodType, "
             "const std::string& mode, "
             "double threshold1, "
             "double threshold2, "
             "int32_t maximumIterationCount, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbos )" );

}


gkg::PottsRegularizerCommand::PottsRegularizerCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOversampled );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameEnergy );
    DECLARE_FLOATING_PARAMETER( parameters, double, oversamplingFactor );
    DECLARE_FLOATING_PARAMETER( parameters, double, regularizationFactor );
    DECLARE_STRING_PARAMETER( parameters, std::string, neighborhoodType );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumIterationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             fileNameOversampled,
             fileNameEnergy,
             oversamplingFactor,
             regularizationFactor,
             neighborhoodType,
             mode,
             threshold1,
             threshold2,
             maximumIterationCount,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::PottsRegularizerCommand::PottsRegularizerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::PottsRegularizerCommand::~PottsRegularizerCommand()
{
}


std::string gkg::PottsRegularizerCommand::getStaticName()
{

  try
  {

    return "PottsRegularizer";

  }
  GKG_CATCH( "std::string gkg::PottsRegularizerCommand::getStaticName()" );

}


void gkg::PottsRegularizerCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string fileNameOversampled;
    std::string fileNameEnergy;
    double oversamplingFactor = 2.0;
    double regularizationFactor = 0.5;
    std::string neighborhoodType = "";
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    int32_t maximumIterationCount = -1;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Potts regularizer",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-ov",
                                 "Oversampled input file name",
                                 fileNameOversampled,
                                 true );
    application.addSingleOption( "-e",
                                 "Output energy file name",
                                 fileNameEnergy,
                                 true );
    application.addSingleOption( "-oversamplingFactor",
                                 "Oversampling factor (default=2.0)",
                                 oversamplingFactor,
                                 true );
    application.addSingleOption( "-regularizationFactor",
                                 "Potts regularization factor (default=0.5)",
                                 regularizationFactor,
                                 true );
    application.addSingleOption( "-n",
                                 "Neighborhood type (default=26) :\n"
                                 "- 4xy\n"
                                 "- 4xz\n"
                                 "- 4yz\n"
                                 "- 8xy\n"
                                 "- 8xz\n"
                                 "- 8yz\n"
                                 "- 6\n"
                                 "- 18\n"
                                 "- 26\n",
                                 neighborhoodType,
                                 true );
    application.addSingleOption( "-m",
                                 "Threshold mode\n"
                                 " - lt -> lower than\n"
                                 " - le -> lower or equal to\n"
                                 " - eq -> equal to\n"
                                 " - di -> different from\n"
                                 " - ge -> greater or equal to\n"
                                 " - gt -> greater than\n"
                                 " - be -> between or equal\n"
                                 " - bt -> between\n"
                                 " - oe -> outside or equal\n"
                                 " - ou -> outside\n",
                                 mode );
    application.addSingleOption( "-t1",
                                 "First threshold",
                                 threshold1 );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-maximumIterationCount",
                                 "Maximum iteration count "
                                 "(default=2 x #oversampled voxels "
                                 "in the mask)",
                                 maximumIterationCount,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             fileNameOversampled,
             fileNameEnergy,
             oversamplingFactor,
             regularizationFactor,
             neighborhoodType,
             mode,
             threshold1,
             threshold2,
             maximumIterationCount,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::PottsRegularizerCommand::parse()" );

}


void gkg::PottsRegularizerCommand::execute(
                                         const std::string& fileNameIn,
                                         const std::string& fileNameOut,
                                         const std::string& fileNameOversampled,
                                         const std::string& fileNameEnergy,
                                         double oversamplingFactor,
                                         double regularizationFactor,
                                         const std::string& neighborhoodType,
                                         const std::string& mode,
                                         double threshold1,
                                         double threshold2,
                                         int32_t maximumIterationCount,
                                         bool ascii,
                                         const std::string& format,
                                         bool verbose )
{

  try
  {

    //
    // sanity check(s)
    //

    if ( ( mode != "lt" ) &&
         ( mode != "le" ) &&
         ( mode != "eq" ) &&
         ( mode != "di" ) &&
         ( mode != "ge" ) &&
         ( mode != "gt" ) &&
         ( mode != "be" ) &&
         ( mode != "bt" ) &&
         ( mode != "oe" ) &&
         ( mode != "ou" ) )
    {

      throw std::runtime_error( "invalid threshold mode" );

    }


    if ( oversamplingFactor < 1.0 )
    {

      throw std::runtime_error( "oversamplingFactor must be greater than 1.0" );

    }

    if ( regularizationFactor <= 0.0 )
    {

      throw std::runtime_error( 
                              "regularizationFactor must be greater than 0.0" );

    }

    if ( ( neighborhoodType != "4xy" ) &&
         ( neighborhoodType != "4xz" ) &&
         ( neighborhoodType != "4yz" ) &&
         ( neighborhoodType != "8xy" ) &&
         ( neighborhoodType != "8xz" ) &&
         ( neighborhoodType != "8yz" ) &&
         ( neighborhoodType != "6" ) &&
         ( neighborhoodType != "18" ) &&
         ( neighborhoodType != "26" ) )
    {

      throw std::runtime_error( "bad neighborhood type" );

    }

         
    //
    // launching Potts regularizer process
    //

    PottsRegularizerProcess pottsRegularizerProcess( fileNameOut,
                                                     fileNameOversampled,
                                                     fileNameEnergy,
                                                     oversamplingFactor,
                                                     regularizationFactor,
                                                     neighborhoodType,
                                                     mode,
                                                     threshold1,
                                                     threshold2,
                                                     maximumIterationCount,
                                                     ascii,
                                                     format,
                                                     verbose );

    pottsRegularizerProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::PottsRegularizerCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameOversampled, "
             "const std::string& fileNameEnergy, "
             "double oversamplingFactor, "
             "double regularizationFactor, "
             "const std::string& neighborhoodType, "
             "const std::string& mode, "
             "double threshold1, "
             "double threshold2, "
             "int32_t maximumIterationCount, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( PottsRegularizerCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOversampled ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameEnergy ) +
                        DECLARE_FLOATING_PARAMETER_HELP( oversamplingFactor ) +
                        DECLARE_FLOATING_PARAMETER_HELP( 
                                                        regularizationFactor ) +
                        DECLARE_STRING_PARAMETER_HELP( neighborhoodType ) +
                        DECLARE_STRING_PARAMETER_HELP( mode ) +
                        DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
                        DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
                        DECLARE_INTEGER_PARAMETER_HELP( 
                                                       maximumIterationCount ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
