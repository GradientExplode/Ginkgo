#include <gkg-processing-plugin-functors/VolumeBetweenTwoMeshesIntersection/VolumeBetweenTwoMeshesIntersectionCommand.h>
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


class VolumeBetweenTwoMeshesIntersectionProcess : public gkg::Process
{

  public:

    VolumeBetweenTwoMeshesIntersectionProcess(
                                         const std::string& fileNameOuterMesh,
                                         const std::string& fileNameInnerMesh,
                                         const std::string& fileNameTransform3d,
                                         const std::string& fileNameMask,
                                         const std::string& fileNameOut,
                                         const float& resamplingStep,
                                         const int32_t& resamplingOrder,
                                         const bool& ascii,
                                         const std::string& format,
                                         const bool& verbose );

    const std::string& fileNameOuterMesh;
    const std::string& fileNameInnerMesh;
    const std::string& fileNameTransform3d;
    const std::string& fileNameMask;
    const std::string& fileNameOut;
    const float& resamplingStep;
    const int32_t& resamplingOrder;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void intersect( gkg::Process& process,
                           const std::string& fileNameVolume,
                           const gkg::AnalyzedObject&,
                           const std::string& );

};


VolumeBetweenTwoMeshesIntersectionProcess::
                                      VolumeBetweenTwoMeshesIntersectionProcess(
                                     const std::string& theFileNameOuterMesh,
                                     const std::string& theFileNameInnerMesh,
                                     const std::string& theFileNameTransform3d,
                                     const std::string& theFileNameMask,
                                     const std::string& theFileNameOut,
                                     const float& theResamplingStep,
                                     const int32_t& theResamplingOrder,
                                     const bool& theAscii,
                                     const std::string& theFormat,
                                     const bool& theVerbose )
                                : gkg::Process( "Volume" ),
                                  fileNameOuterMesh( theFileNameOuterMesh ),
                                  fileNameInnerMesh( theFileNameInnerMesh ),
                                  fileNameTransform3d( theFileNameTransform3d ),
                                  fileNameMask( theFileNameMask ),
                                  fileNameOut( theFileNameOut ),
                                  resamplingStep( theResamplingStep ),
                                  resamplingOrder( theResamplingOrder ),
                                  ascii( theAscii ),
                                  format( theFormat ),
                                  verbose( theVerbose )
{

  registerProcess(
            "Volume", gkg::TypeOf< int8_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< int8_t > );
  registerProcess(
            "Volume", gkg::TypeOf< uint8_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< uint8_t > );
  registerProcess(
            "Volume", gkg::TypeOf< int16_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< int16_t > );
  registerProcess(
            "Volume", gkg::TypeOf< uint16_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< uint16_t > );
  registerProcess(
            "Volume", gkg::TypeOf< int32_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< int32_t > );
  registerProcess(
            "Volume", gkg::TypeOf< uint32_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< uint32_t > );
  registerProcess(
            "Volume", gkg::TypeOf< int64_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< int64_t > );
  registerProcess(
            "Volume", gkg::TypeOf< uint64_t >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< uint64_t > );
  registerProcess(
            "Volume", gkg::TypeOf< float >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< float > );
  registerProcess(
            "Volume", gkg::TypeOf< double >::getName(),
            &VolumeBetweenTwoMeshesIntersectionProcess::intersect< double > );

}


template < class T >
void VolumeBetweenTwoMeshesIntersectionProcess::intersect(
                                              gkg::Process& process,
                                              const std::string& fileNameVolume,
                                              const gkg::AnalyzedObject&,
                                              const std::string& )
{

  try
  {

    VolumeBetweenTwoMeshesIntersectionProcess&
      volumeBetweenTwoMeshesIntersectionProcess =
           static_cast< VolumeBetweenTwoMeshesIntersectionProcess& >( process );

    const std::string& fileNameOuterMesh =
      volumeBetweenTwoMeshesIntersectionProcess.fileNameOuterMesh;
    const std::string& fileNameInnerMesh =
      volumeBetweenTwoMeshesIntersectionProcess.fileNameInnerMesh;
    const std::string& fileNameTransform3d =
      volumeBetweenTwoMeshesIntersectionProcess.fileNameTransform3d;
    const std::string& fileNameMask =
      volumeBetweenTwoMeshesIntersectionProcess.fileNameMask;
    const std::string& fileNameOut =
      volumeBetweenTwoMeshesIntersectionProcess.fileNameOut;
    const float& resamplingStep =
      volumeBetweenTwoMeshesIntersectionProcess.resamplingStep;
    const int32_t& resamplingOrder =
      volumeBetweenTwoMeshesIntersectionProcess.resamplingOrder;
    const bool& ascii =
      volumeBetweenTwoMeshesIntersectionProcess.ascii;
    const std::string& format =
      volumeBetweenTwoMeshesIntersectionProcess.format;
    const bool& verbose =
      volumeBetweenTwoMeshesIntersectionProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading volume
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
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

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading outer mesh
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading outer mesh '"
                << fileNameOuterMesh
                << "' : " << std::flush;

    }
    gkg::MeshMap< int32_t, float, 3 > outerMeshMap;
    gkg::Reader::getInstance().read( fileNameOuterMesh, outerMeshMap );

    // sanity checks
    if ( outerMeshMap.vertices.getRankCount() == 0 )
    {

      throw std::runtime_error( "no rank found in the outer mesh map" );

    }
    if ( outerMeshMap.vertices.getRankCount() > 1 )
    {

      throw std::runtime_error( "outer mesh cannot be 4D" );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading inner mesh
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading inner mesh '"
                << fileNameInnerMesh
                << "' : " << std::flush;

    }
    gkg::MeshMap< int32_t, float, 3 > innerMeshMap;
    gkg::Reader::getInstance().read( fileNameInnerMesh, innerMeshMap );

    // sanity checks
    if ( innerMeshMap.vertices.getRankCount() == 0 )
    {

      throw std::runtime_error( "no rank found in the inner mesh map" );

    }
    if ( innerMeshMap.vertices.getRankCount() > 1 )
    {

      throw std::runtime_error( "inner mesh cannot be 4D" );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading or creating mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< uint8_t > maskUInt8;
    if ( !fileNameMask.empty() )
    {

      if ( verbose )
      {

       std::cout << "reading '"
                 << fileNameMask
                 << "' : " << std::flush;

      }
      try
      {

        gkg::Reader::getInstance().read( fileNameMask, maskUInt8 );
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
          gkg::Reader::getInstance().read( fileNameMask,
                                           *pMaskInt16 );
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

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "creating mask  : " << std::flush;

      }
      maskUInt8.reallocate( size, sizeT );
      maskUInt8.setResolution( resolution, resolutionT );
      maskUInt8.fill( 1U );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading or creating 3D transform
    ////////////////////////////////////////////////////////////////////////////
    gkg::RCPointer< gkg::Transform3d< float > > transform3d;

    if ( !fileNameTransform3d.empty() )
    {

      if ( verbose )
      {

        std::cout << "reading '"
                  << fileNameTransform3d
                  << "' : " << std::flush;

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream is( fileNameTransform3d.c_str() );
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
          std::ifstream is( fileNameTransform3d.c_str() );
          affineWoShearingTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineWoShearingTransform3d );

        }
        catch ( std::exception& )
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          std::ifstream is( fileNameTransform3d.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineTransform3d );

        }

      }

      if ( volumeBetweenTwoMeshesIntersectionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "creating identity 3D transform  : " << std::flush;

      }

      transform3d.reset( new gkg::IdentityTransform3d< float > );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating resampler
    ////////////////////////////////////////////////////////////////////////////
    gkg::Resampler< T >* resampler =
      gkg::ResamplerFactory< T >::getInstance().getResampler( resamplingOrder );
    gkg::Resampler< uint8_t >* maskResampler =
      gkg::ResamplerFactory< uint8_t >::getInstance().getResampler( 0 );

    if ( verbose )
    {

      std::cout << "switching to resampler : " << resampler->getName()
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // extracting the list of textures
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "intersecting volume between two meshes : " << std::flush;

    }

    // retrieving outer and inner mesh rank
    int32_t outerRank = outerMeshMap.vertices.getRank( 0 );
    int32_t innerRank = innerMeshMap.vertices.getRank( 0 );

    gkg::TextureMap< std::list< T > > textureMap;

    int32_t step = 0;
    gkg::Vector3d< float > site;
    gkg::Vector3d< float > unitaryDirection;
    float cortexWidth = 0.0;
    T value = 0;
    uint8_t maskValue = 0U;
    int32_t t = 0;

    for ( t = 0; t < sizeT; t++ )
    {

      // allocating list of item list for the current rank
      std::list< std::list< T > > textures;

      // retrieving outer and inner vertices
      const std::list< gkg::Vector3d< float > >&
        outerVertices = outerMeshMap.vertices.getSites( outerRank );
      const std::list< gkg::Vector3d< float > >&
        innerVertices = innerMeshMap.vertices.getSites( innerRank );

      std::list< gkg::Vector3d< float > >::const_iterator
        ov = outerVertices.begin(),
        ove = outerVertices.end();
      std::list< gkg::Vector3d< float > >::const_iterator
        iv = innerVertices.begin();
      while ( ov != ove )
      {

        std::list< T > values;

        // computing the direction between the two sites
        unitaryDirection = *ov - *iv;
        cortexWidth = unitaryDirection.getNorm();
        unitaryDirection.normalize();

        if ( cortexWidth < resamplingStep )
        {

          // initializing site with the inner mesh vertex
          site = *iv;

          maskResampler->resample( maskUInt8, *transform3d,
                                   ( uint8_t )0, site, maskValue,
                                   &resolution );
          if ( maskValue )
          {

            resampler->resample( volume, *transform3d,
                                 ( T )0, site, value,
                                 &resolution );
            values.push_back( value );
            //std::cout << "site cas 1 = " << site;

            //std::cout << value << std::endl;

          }

        }
        else
        {

          // computing the step count between the two sites
          int32_t stepCount = ( int32_t )( cortexWidth / resamplingStep ) + 1;
          float actualResamplingStep = cortexWidth / ( float )stepCount;

          // initializing site with the inner mesh vertex
          site = *iv;

          // sampling values along line provided by inner to outer vertices
          for ( step = 0; step < stepCount; step++ )
          {

            maskResampler->resample( maskUInt8, *transform3d,
                                     ( uint8_t )0, site, maskValue,
                                     &resolution );

            if ( maskValue )
            {

              resampler->resample( volume, *transform3d,
                                   ( T )0, site, value,
                                   &resolution );
              values.push_back( value );
              //std::cout << "value = " << value << std::endl;


            }

            site += unitaryDirection * actualResamplingStep;

          }

        }

        if ( !values.empty() )
        {

          textures.push_back( values );

        }

        ++ ov;
        ++ iv;

      }

      textureMap.addTextures( textures );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing data
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing '"
                << fileNameOut
                << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameOut,
                                      textureMap,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void volumeBetweenTwoMeshesIntersectionProcess::intersect( "
             "gkg::Process& process, "
             "const std::string& fileNameVolume, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VolumeBetweenTwoMeshesIntersectionCommand
//


gkg::VolumeBetweenTwoMeshesIntersectionCommand::
                                      VolumeBetweenTwoMeshesIntersectionCommand(
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
  GKG_CATCH( "gkg::VolumeBetweenTwoMeshesIntersectionCommand::"
             "VolumeBetweenTwoMeshesIntersectionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VolumeBetweenTwoMeshesIntersectionCommand::
                                      VolumeBetweenTwoMeshesIntersectionCommand(
                  			 const std::string& fileNameVolume,
    		  			 const std::string& fileNameOuterMesh,
    		  			 const std::string& fileNameInnerMesh,
    		  			 const std::string& fileNameTransform3d,
    		  			 const std::string& fileNameMask,
    		  			 const std::string& fileNameOut,
    		  			 float resamplingStep,
    		  			 int32_t resamplingOrder,
    		  			 bool ascii,
    		  			 const std::string& format,
    		  			 bool verbose )
                                              : gkg::Command()
{

  try
  {

    execute( fileNameVolume, fileNameOuterMesh, fileNameInnerMesh, 
             fileNameTransform3d, fileNameMask, fileNameOut,
             resamplingStep, resamplingOrder, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeBetweenTwoMeshesIntersectionCommand::"
             "VolumeBetweenTwoMeshesIntersectionCommand( "
             "const std::string& fileNameVolume, "
    	     "const std::string& fileNameOuterMesh, "
    	     "const std::string& fileNameInnerMesh, "
    	     "const std::string& fileNameTransform3d, "
    	     "const std::string& fileNameMask, "
    	     "const std::string& fileNameOut, "
    	     "float resamplingStep, int32_t resamplingOrder, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


gkg::VolumeBetweenTwoMeshesIntersectionCommand::
                                      VolumeBetweenTwoMeshesIntersectionCommand(
                                             const gkg::Dictionary& parameters )
                                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOuterMesh );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInnerMesh );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, float, resamplingStep );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, resamplingOrder );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameVolume, fileNameOuterMesh, fileNameInnerMesh, 
             fileNameTransform3d, fileNameMask, fileNameOut,
             resamplingStep, resamplingOrder, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeBetweenTwoMeshesIntersectionCommand::"
             "VolumeBetweenTwoMeshesIntersectionCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VolumeBetweenTwoMeshesIntersectionCommand::
                                    ~VolumeBetweenTwoMeshesIntersectionCommand()
{
}


std::string gkg::VolumeBetweenTwoMeshesIntersectionCommand::getStaticName()
{

  try
  {

    return "VolumeBetweenTwoMeshesIntersection";

  }
  GKG_CATCH( "std::string gkg::VolumeBetweenTwoMeshesIntersectionCommand::"
             "getStaticName()" );

}


void gkg::VolumeBetweenTwoMeshesIntersectionCommand::parse()
{

  try
  {

    std::string fileNameVolume;
    std::string fileNameOuterMesh;
    std::string fileNameInnerMesh;
    std::string fileNameTransform3d;
    std::string fileNameMask;
    std::string fileNameOut;
    float resamplingStep = 0.1;
    int32_t resamplingOrder = 1;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Volume between tow meshes intersection " 
                                  "builder",
                                  _loadPlugin );
    application.addSingleOption( "-v",
                                 "Input volume file name",
                                 fileNameVolume );
    application.addSingleOption( "-om",
                                 "Input outer mesh file name",
                                 fileNameOuterMesh );
    application.addSingleOption( "-im",
                                 "Input inner mesh file name",
                                 fileNameInnerMesh );
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
    application.addSingleOption( "-resamplingStep",
                                 "Resampling step (default=0.1mm)",
                                 resamplingStep,
                                 true );
    application.addSingleOption( "-resamplingOrder",
                                 "Resampling order (default=1)",
                                 resamplingOrder,
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

    execute( fileNameVolume, fileNameOuterMesh, fileNameInnerMesh, 
             fileNameTransform3d, fileNameMask, fileNameOut,
             resamplingStep, resamplingOrder, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VolumeBetweenTwoMeshesIntersectionCommand::"
                     "parse()" );

}


void gkg::VolumeBetweenTwoMeshesIntersectionCommand::execute(
                  			 const std::string& fileNameVolume,
    		  			 const std::string& fileNameOuterMesh,
    		  			 const std::string& fileNameInnerMesh,
    		  			 const std::string& fileNameTransform3d,
    		  			 const std::string& fileNameMask,
    		  			 const std::string& fileNameOut,
    		  			 float resamplingStep,
    		  			 int32_t resamplingOrder,
    		  			 bool ascii,
    		  			 const std::string& format,
    		  			 bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // launching volume between two meshes intersection process
    ////////////////////////////////////////////////////////////////////////////
    VolumeBetweenTwoMeshesIntersectionProcess 
      volumeBetweenTwoMeshesIntersectionProcess( fileNameOuterMesh,
                                                 fileNameInnerMesh,
                                                 fileNameTransform3d,
                                                 fileNameMask,
                                                 fileNameOut,
                                                 resamplingStep,
                                                 resamplingOrder,
                                                 ascii,
                                                 format,
                                                 verbose );
    volumeBetweenTwoMeshesIntersectionProcess.execute( fileNameVolume );

  }
  GKG_CATCH( "void gkg::VolumeBetweenTwoMeshesIntersectionCommand::execute( "
             "const std::string& fileNameVolume, "
    	     "const std::string& fileNameOuterMesh, "
    	     "const std::string& fileNameInnerMesh, "
    	     "const std::string& fileNameTransform3d, "
    	     "const std::string& fileNameMask, "
    	     "const std::string& fileNameOut, "
    	     "float resamplingStep, int32_t resamplingOrder, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( VolumeBetweenTwoMeshesIntersectionCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolume ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOuterMesh ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameInnerMesh ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameTransform3d ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_FLOATING_PARAMETER_HELP( resamplingStep ) +
    			DECLARE_INTEGER_PARAMETER_HELP( resamplingOrder ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
