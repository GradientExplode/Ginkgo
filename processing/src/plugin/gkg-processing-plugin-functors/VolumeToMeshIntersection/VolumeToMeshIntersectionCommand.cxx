#include <gkg-processing-plugin-functors/VolumeToMeshIntersection/VolumeToMeshIntersectionCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <map>
#include <vector>


class VolumeToMeshIntersectionProcess : public gkg::Process
{

  public:

    VolumeToMeshIntersectionProcess( const std::string& fileNameMesh,
                                     const std::string& fileNameTransform3d,
                                     const std::string& fileNameMask,
                                     const std::string& fileNameOut,
                                     const double& outerDepth,
                                     const double& innerDepth,
                                     const double& resamplingStep,
                                     const int32_t& resamplingOrder,
                                     const std::string& operation,
                                     const bool& ascii,
                                     const std::string& format,
                                     const bool& verbose );

    const std::string& fileNameMesh;
    const std::string& fileNameTransform3d;
    const std::string& fileNameMask;
    const std::string& fileNameOut;
    const double& outerDepth;
    const double& innerDepth;
    const double& resamplingStep;
    const int32_t& resamplingOrder;
    const std::string& operation;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void intersect( gkg::Process& process,
                           const std::string& fileNameVolume,
                           const gkg::AnalyzedObject&,
                           const std::string& );

    template < class T >
    static float getIntegration( const std::vector< T >& values,
                                 int32_t realValueCount );
    template < class T >
    static T getLikelyhood( const std::vector< T >& values,
                            int32_t realValueCount );

};


VolumeToMeshIntersectionProcess::VolumeToMeshIntersectionProcess(
                                     const std::string& theFileNameMesh,
                                     const std::string& theFileNameTransform3d,
                                     const std::string& theFileNameMask,
                                     const std::string& theFileNameOut,
                                     const double& theOuterDepth,
                                     const double& theInnerDepth,
                                     const double& theResamplingStep,
                                     const int32_t& theResamplingOrder,
                                     const std::string& theOperation,
                                     const bool& theAscii,
                                     const std::string& theFormat,
                                     const bool& theVerbose )
                                : gkg::Process( "Volume" ),
                                  fileNameMesh( theFileNameMesh ),
                                  fileNameTransform3d( theFileNameTransform3d ),
                                  fileNameMask( theFileNameMask ),
                                  fileNameOut( theFileNameOut ),
                                  outerDepth( theOuterDepth ),
                                  innerDepth( theInnerDepth ),
                                  resamplingStep( theResamplingStep ),
                                  resamplingOrder( theResamplingOrder ),
                                  operation( theOperation ),
                                  ascii( theAscii ),
                                  format( theFormat ),
                                  verbose( theVerbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &VolumeToMeshIntersectionProcess::intersect< double > );

}


template < class T >
float VolumeToMeshIntersectionProcess::getIntegration(
                        const std::vector< T >& values, int32_t realValueCount )
{

  try
  {

    
    float integral = 0.0;
    
    typename std::vector< T >::const_iterator v = values.begin(),
                                              ve = values.end();
    int32_t currentValueIndex = 0;
    while ( ( v != ve ) && ( currentValueIndex < realValueCount ) )
    {

      integral += ( float )*v;

      ++ currentValueIndex;
      ++ v;

    }

    return integral;

  }
  GKG_CATCH( "template < class T > "
             "float VolumeToMeshIntersectionProcess::getIntegration( "
             "const std::vector< T >& values, int32_t realValueCount )" );

}


template < class T >
T VolumeToMeshIntersectionProcess::getLikelyhood(
                        const std::vector< T >& values, int32_t realValueCount )
{

  try
  {

    std::map< T, int32_t > histogram;

    typename std::vector< T >::const_iterator v = values.begin(),
                                              ve = values.end();
    int32_t currentValueIndex = 0;
    while ( ( v != ve ) && ( currentValueIndex < realValueCount ) )
    {

      ++ histogram[ *v ];
      ++ currentValueIndex;
      ++ v;

    }

    typename std::map< T, int32_t >::const_iterator
      t = histogram.begin(),
      te = histogram.end();
    
    T bestChoice = t->first;
    int32_t bestCount = t->second;
    ++ t;
    while ( t != te )
    {

      if ( t->second > bestCount )
      {

        bestChoice = t->first;
        bestCount = t->second;

      }
      ++ t;

    }

    return bestChoice;

  }
  GKG_CATCH( "template < class T > "
             "float VolumeToMeshIntersectionProcess::getLikelyhood( "
             "const std::vector< T >& values, int32_t realValueCount )" );

}


template < class T >
void VolumeToMeshIntersectionProcess::intersect(
                                              gkg::Process& process,
                                              const std::string& fileNameVolume,
                                              const gkg::AnalyzedObject&,
                                              const std::string& )
{

  try
  {

    VolumeToMeshIntersectionProcess&
      volumeToMeshIntersectionProcess =
                     static_cast< VolumeToMeshIntersectionProcess& >( process );

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( !( ( volumeToMeshIntersectionProcess.operation == "integration" ) ||
            ( volumeToMeshIntersectionProcess.operation == "likelyhood" ) ||
            ( volumeToMeshIntersectionProcess.operation == "labelling" ) ) )
    {

      throw std::runtime_error( "bad operation" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading volume
    ////////////////////////////////////////////////////////////////////////////
    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "reading volume '"
                << fileNameVolume
                << "' : " << std::flush;

    }
    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameVolume, volume );

    gkg::Vector3d< int32_t > size( 1, 1, 1 );
    int32_t sizeT = 1;
    volume.getSize( size, sizeT );

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    double resolutionT = 1.0;
    volume.getResolution( resolution, resolutionT );

    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading mesh
    ////////////////////////////////////////////////////////////////////////////
    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "reading mesh '"
                << volumeToMeshIntersectionProcess.fileNameMesh
                << "' : " << std::flush;

    }
    gkg::MeshMap< int32_t, float, 3 > meshMap;
    gkg::Reader::getInstance().read(
                        volumeToMeshIntersectionProcess.fileNameMesh, meshMap );
    if ( meshMap.vertices.getRankCount() > 1 )
    {

      throw std::runtime_error( "mesh cannot be 4D" );

    }
    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading or creating mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< uint8_t > maskUInt8;
    if ( !volumeToMeshIntersectionProcess.fileNameMask.empty() )
    {

      if ( volumeToMeshIntersectionProcess.verbose )
      {

       std::cout << "reading '"
                 << volumeToMeshIntersectionProcess.fileNameMask
                 << "' : " << std::flush;

      }
      try
      {

        gkg::Reader::getInstance().read(
                    volumeToMeshIntersectionProcess.fileNameMask, maskUInt8 );
        if ( ! ( maskUInt8.hasSameSize( volume ) &&
                 maskUInt8.hasSameResolution( volume, true ) ) )
        {

          throw std::runtime_error( 
                 "mask size and/or resolution incompatible with input volume" );

        }

      }
      catch( std::exception& )
      {

        try
        {

          
          gkg::Volume< int16_t >* pMaskInt16 = new gkg::Volume< int16_t >;
          gkg::Reader::getInstance().read(
                    volumeToMeshIntersectionProcess.fileNameMask, *pMaskInt16 );
          if ( ! ( pMaskInt16->hasSameSize( volume ) &&
                   pMaskInt16->hasSameResolution( volume, true ) ) )
          {

            throw std::runtime_error( 
                 "mask size and/or resolution incompatible with input volume" );

          }

          gkg::GreaterThanFunction< int16_t > greaterThanFunction( 0 );
          gkg::Binarizer< gkg::Volume< int16_t >,
                          gkg::Volume< uint8_t > >
            binarizer( greaterThanFunction, 1U, 0U );
 
          binarizer.binarize( *pMaskInt16, maskUInt8 );

          delete pMaskInt16;

        }
        catch( std::exception& )
        {

           throw std::runtime_error( "unable to read mask" );

        }

      }

      if ( volumeToMeshIntersectionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( volumeToMeshIntersectionProcess.verbose )
      {

       std::cout << "creating mask  : " << std::flush;

      }
      maskUInt8.reallocate( size, sizeT );
      maskUInt8.setResolution( resolution, resolutionT );
      maskUInt8.fill( 1U );

      if ( volumeToMeshIntersectionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading or creating 3D transform
    ////////////////////////////////////////////////////////////////////////////


    gkg::RCPointer< gkg::Transform3d< float > > transform3d;

    if ( !volumeToMeshIntersectionProcess.fileNameTransform3d.empty() )
    {

      if ( volumeToMeshIntersectionProcess.verbose )
      {

       std::cout << "reading '"
                 << volumeToMeshIntersectionProcess.fileNameTransform3d
                 << "' : " << std::flush;

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream 
          is( volumeToMeshIntersectionProcess.fileNameTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3d.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::AffineWoShearingTransform3d< float >*
            affineWoShearingTransform3d =
              new gkg::AffineWoShearingTransform3d< float >;
          std::ifstream 
            is( volumeToMeshIntersectionProcess.fileNameTransform3d.c_str() );
          affineWoShearingTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineWoShearingTransform3d );

        }
        catch ( std::exception& )
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          std::ifstream 
            is( volumeToMeshIntersectionProcess.fileNameTransform3d.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineTransform3d );

        }

      }

      if ( volumeToMeshIntersectionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( volumeToMeshIntersectionProcess.verbose )
      {

       std::cout << "creating identity 3D transform  : " << std::flush;

      }

      transform3d.reset( new gkg::IdentityTransform3d< float > );

      if ( volumeToMeshIntersectionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating resampler
    ////////////////////////////////////////////////////////////////////////////

    gkg::Resampler< T >* resampler =
      gkg::ResamplerFactory< T >::getInstance().getResampler(
                              volumeToMeshIntersectionProcess.resamplingOrder );
    gkg::Resampler< uint8_t >* maskResampler =
      gkg::ResamplerFactory< uint8_t >::getInstance().getResampler( 0 );

    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "switching to resampler : " << resampler->getName()
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // extracting texture
    ////////////////////////////////////////////////////////////////////////////

    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "intersecting volume to mesh : " << std::flush;

    }

    int32_t rank = meshMap.vertices.getRank( 0 );
    std::list< gkg::Vector3d< float > > normals = meshMap.getNormals( rank );
    std::list< gkg::Vector3d< float > >::const_iterator
      n = normals.begin();

    gkg::RCPointer< gkg::TextureMap< float > > textureMapFloat;
    gkg::RCPointer< gkg::TextureMap< T > > textureMapT;
    float textureFloat = 0;
    T textureT = 0;

    if ( volumeToMeshIntersectionProcess.operation == "integration" )
    {

      textureMapFloat.reset( new gkg::TextureMap< float > );

    }
    else if ( volumeToMeshIntersectionProcess.operation == "likelyhood" )
    {

      textureMapFloat.reset( new gkg::TextureMap< float > );

    }
    else if ( volumeToMeshIntersectionProcess.operation == "labelling" )
    {

      textureMapT.reset( new gkg::TextureMap< T > );

    }


    int32_t backwardStepCount = ( int32_t )( 
                           volumeToMeshIntersectionProcess.outerDepth /
                           volumeToMeshIntersectionProcess.resamplingStep + 1 );
    int32_t forwardStepCount = ( int32_t )( 
                           volumeToMeshIntersectionProcess.innerDepth /
                           volumeToMeshIntersectionProcess.resamplingStep + 1 );
    int32_t step = 0;

    std::vector< T > values( backwardStepCount + 1 + forwardStepCount );
    int32_t realValueCount = 0;

    gkg::Vector3d< float > site;
    T value = 0;
    uint8_t maskValue = 0U;
    int32_t t = 0;
    for ( t = 0; t < sizeT; t++ )
    {

      std::list< float > texturesFloat;
      std::list< T > texturesT;

      const std::list< gkg::Vector3d< float > >&
        vertices = meshMap.vertices.getSites( rank );


      std::list< gkg::Vector3d< float > >::const_iterator
        v = vertices.begin(),
        ve = vertices.end();
      while ( v != ve )
      {


        site = *v;
        resampler->resample( volume, *transform3d, 
                             ( T )0, site, value,
                             &resolution );
        //texture = ( float )value;
        realValueCount = 0;
        values[ realValueCount ] = value;
        ++ realValueCount;

        site = *v;
        for ( step = 1; step < backwardStepCount; step++ )
        {

          site += *n * ( float )volumeToMeshIntersectionProcess.resamplingStep;
          maskResampler->resample( maskUInt8, *transform3d,
                                   ( uint8_t )0, site, maskValue,
                                   &resolution );
          if ( maskValue )
          {

            resampler->resample( volume, *transform3d,
                                 ( T )0, site, value,
                                 &resolution );
            values[ realValueCount ] = value;
            ++ realValueCount;

          }
          else
          {

            if ( ( float )step / ( float )backwardStepCount > 0.1 )
            {

              break;

            }

          }

        }

        site = *v;
        for ( step = 1; step < forwardStepCount; step++ )
        {

          site -= *n * ( float )volumeToMeshIntersectionProcess.resamplingStep;
          maskResampler->resample( maskUInt8, *transform3d,
                                   ( uint8_t )0, site, maskValue,
                                   &resolution );
          if ( maskValue )
          {

            resampler->resample( volume, *transform3d,
                                 ( T )0, site, value,
                                 &resolution );
            values[ realValueCount ] = value;
            ++ realValueCount;

          }
          else
          {

            if ( ( float )step / ( float )forwardStepCount > 0.1 )
            {

              break;

            }

          }

        }

        if ( volumeToMeshIntersectionProcess.operation == "integration" )
        {

           textureFloat = getIntegration( values, realValueCount );
           texturesFloat.push_back( textureFloat );

        }
        else if ( volumeToMeshIntersectionProcess.operation == "likelyhood" )
        {

           textureFloat = ( float )getLikelyhood( values, realValueCount );
           texturesFloat.push_back( textureFloat );

        }
        else if ( volumeToMeshIntersectionProcess.operation == "labelling" )
        {

           textureT = getLikelyhood( values, realValueCount );
           texturesT.push_back( textureT );

        }

        ++ n;
        ++ v;

      }

      if ( volumeToMeshIntersectionProcess.operation == "integration" )
      {

        textureMapFloat->addTextures( texturesFloat );

      }
      else if ( volumeToMeshIntersectionProcess.operation == "likelyhood" )
      {

        textureMapFloat->addTextures( texturesFloat );

      }
      else if ( volumeToMeshIntersectionProcess.operation == "labelling" )
      {

        textureMapT->addTextures( texturesT );

      }

    }

    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing data
    ////////////////////////////////////////////////////////////////////////////

    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "writing '"
                << volumeToMeshIntersectionProcess.fileNameOut
                << "' : " << std::flush;

    }

    if ( volumeToMeshIntersectionProcess.operation == "integration" )
    {

      gkg::Writer::getInstance().write( 
                                    volumeToMeshIntersectionProcess.fileNameOut,
                                    *textureMapFloat,
                                    volumeToMeshIntersectionProcess.ascii,
                                    volumeToMeshIntersectionProcess.format );

    }
    else if ( volumeToMeshIntersectionProcess.operation == "likelyhood" )
    {

      gkg::Writer::getInstance().write( 
                                    volumeToMeshIntersectionProcess.fileNameOut,
                                    *textureMapFloat,
                                    volumeToMeshIntersectionProcess.ascii,
                                    volumeToMeshIntersectionProcess.format );

    }
    else if ( volumeToMeshIntersectionProcess.operation == "labelling" )
    {

      gkg::Writer::getInstance().write( 
                                    volumeToMeshIntersectionProcess.fileNameOut,
                                    *textureMapT,
                                    volumeToMeshIntersectionProcess.ascii,
                                    volumeToMeshIntersectionProcess.format );

    }
    if ( volumeToMeshIntersectionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void VolumeToMeshIntersectionProcess::intersect( "
             "gkg::Process& process, "
             "const std::string& fileNameVolume, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VolumeToMeshIntersectionCommand
//


gkg::VolumeToMeshIntersectionCommand::VolumeToMeshIntersectionCommand(
                                 			      int32_t argc,
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
  GKG_CATCH( "gkg::VolumeToMeshIntersectionCommand::"
             "VolumeToMeshIntersectionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VolumeToMeshIntersectionCommand::VolumeToMeshIntersectionCommand(
                                     	 const std::string& fileNameMesh,
                                     	 const std::string& fileNameVolume,
    				     	 const std::string& fileNameTransform3d,
    				     	 const std::string& fileNameMask,
    				     	 const std::string& fileNameOut,
    				     	 double outerDepth,
    				     	 double innerDepth,
    				     	 double resamplingStep,
    				     	 int32_t resamplingOrder,
    				     	 const std::string& operation,
    				     	 bool ascii,
    				     	 const std::string& format,
    				     	 bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameMesh, fileNameVolume, fileNameTransform3d, 
             fileNameMask, fileNameOut, outerDepth, innerDepth,
             resamplingStep, resamplingOrder, operation,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeToMeshIntersectionCommand::"
             "VolumeToMeshIntersectionCommand( "
             "const std::string& fileNameMesh, "
             "const std::string& fileNameVolume, "
    	     "const std::string& fileNameTransform3d, "
    	     "const std::string& fileNameMask, "
    	     "const std::string& fileNameOut, "
    	     "double outerDepth, double innerDepth, double resamplingStep, "
    	     "int32_t resamplingOrder, const std::string& operation, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


gkg::VolumeToMeshIntersectionCommand::VolumeToMeshIntersectionCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMesh );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, double, outerDepth );
    DECLARE_FLOATING_PARAMETER( parameters, double, innerDepth );
    DECLARE_FLOATING_PARAMETER( parameters, double, resamplingStep );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, resamplingOrder );
    DECLARE_STRING_PARAMETER( parameters, std::string, operation );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMesh, fileNameVolume, fileNameTransform3d, 
             fileNameMask, fileNameOut, outerDepth, innerDepth,
             resamplingStep, resamplingOrder, operation,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeToMeshIntersectionCommand::"
             "VolumeToMeshIntersectionCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VolumeToMeshIntersectionCommand::~VolumeToMeshIntersectionCommand()
{
}


std::string gkg::VolumeToMeshIntersectionCommand::getStaticName()
{

  try
  {

    return "VolumeToMeshIntersection";

  }
  GKG_CATCH( "std::string gkg::VolumeToMeshIntersectionCommand::"
             "getStaticName()" );

}


void gkg::VolumeToMeshIntersectionCommand::parse()
{

  try
  {

    std::string fileNameMesh;
    std::string fileNameVolume;
    std::string fileNameTransform3d;
    std::string fileNameMask;
    std::string fileNameOut;
    double outerDepth;
    double innerDepth;
    double resamplingStep = 0.1;
    int32_t resamplingOrder = 1;
    std::string operation = "integration";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Volume to mesh intersection builder",
                                  _loadPlugin );
    application.addSingleOption( "-v",
                                 "Input volume file name",
                                 fileNameVolume );
    application.addSingleOption( "-m",
                                 "Input mesh file name",
                                 fileNameMesh );
    application.addSingleOption( "-t",
                                 "Volume to mesh 3D transform (default=id)",
                                 fileNameTransform3d,
                                 true );
    application.addSingleOption( "-M",
                                 "Input mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-o",
                                 "Output texture file name",
                                 fileNameOut );
    application.addSingleOption( "-outerDepth",
                                 "Outer depth from surface in mm along normal",
                                 outerDepth );
    application.addSingleOption( "-innerDepth",
                                 "Inner depth from surface in mm along normal",
                                 innerDepth );
    application.addSingleOption( "-resamplingStep",
                                 "Resampling step (default=0.1mm)",
                                 resamplingStep,
                                 true );
    application.addSingleOption( "-resamplingOrder",
                                 "Resampling order (default=1)",
                                 resamplingOrder,
                                 true );
    application.addSingleOption( "-op",
                                 "Operation done along the normal: \n"
                                 "- integration\n"
                                 "- likelyhood\n"
                                 "- labelling\n"
                                 "(default=integration)",
                                 operation,
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

    execute( fileNameMesh, fileNameVolume, fileNameTransform3d, 
             fileNameMask, fileNameOut, outerDepth, innerDepth,
             resamplingStep, resamplingOrder, operation,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VolumeToMeshIntersectionCommand::parse()" );

}


void gkg::VolumeToMeshIntersectionCommand::execute(
                  	                 const std::string& fileNameMesh,
                                     	 const std::string& fileNameVolume,
    				     	 const std::string& fileNameTransform3d,
    				     	 const std::string& fileNameMask,
    				     	 const std::string& fileNameOut,
    				     	 double outerDepth,
    				     	 double innerDepth,
    				     	 double resamplingStep,
    				     	 int32_t resamplingOrder,
    				     	 const std::string& operation,
    				     	 bool ascii,
    				     	 const std::string& format,
    				     	 bool verbose )
{

  try
  {

    //
    // launching volume to mesh intersection process
    //
    VolumeToMeshIntersectionProcess 
      volumeToMeshIntersectionProcess( fileNameMesh,
                                       fileNameTransform3d,
                                       fileNameMask,
                                       fileNameOut,
                                       outerDepth,
                                       innerDepth,
                                       resamplingStep,
                                       resamplingOrder,
                                       operation,
                                       ascii,
                                       format,
                                       verbose );
    volumeToMeshIntersectionProcess.execute( fileNameVolume );

  }
  GKG_CATCH( "void gkg::VolumeToMeshIntersectionCommand::execute( "
             "const std::string& fileNameMesh, "
             "const std::string& fileNameVolume, "
    	     "const std::string& fileNameTransform3d, "
    	     "const std::string& fileNameMask, "
    	     "const std::string& fileNameOut, "
    	     "double outerDepth, double innerDepth, double resamplingStep, "
    	     "int32_t resamplingOrder, const std::string& operation, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( VolumeToMeshIntersectionCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameMesh ) +         
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolume ) +       
    			DECLARE_STRING_PARAMETER_HELP( fileNameTransform3d ) +  
    			DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +         
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +	        
    			DECLARE_FLOATING_PARAMETER_HELP( outerDepth ) +         
    			DECLARE_FLOATING_PARAMETER_HELP( innerDepth ) +         
    			DECLARE_FLOATING_PARAMETER_HELP( resamplingStep ) +     
    			DECLARE_INTEGER_PARAMETER_HELP( resamplingOrder ) +     
    			DECLARE_STRING_PARAMETER_HELP( operation ) +	        
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +	        
    			DECLARE_STRING_PARAMETER_HELP( format ) +	        
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );	        
