#include <gkg-dmri-plugin-functors/DwiBundleMapDensityMask/DwiBundleMapDensityMaskCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <vector>


FORCE_HARD_LINK_WITH_CONNECTOMIST


//
// class DensityMaskProcess
//

class BundleMapDensityMaskProcess : public gkg::Process
{

  public:

    BundleMapDensityMaskProcess( const std::string& fileNameT2,
				 const float& step,
				 const std::string& fileNameOut,
				 const std::string& outputFormat,
				 const bool& normalized,
                                 const bool& singleFile,
                                 const bool& useThickFascicles,
                                 const float& fascicleRadius,
                                 const int32_t& sphereOrientationCount,
				 const bool& ascii,
				 const bool& verbose );

    const std::string& getFileNameT2() const;
    const float& getStep() const;
    const std::string& getFileNameOut() const;
    const std::string& getOutputFormat() const;
    const bool& getNormalized() const;
    const bool& getSingleFile() const;
    const bool& useThickFascicles() const;
    const float& getFascicleRadius() const;
    const int32_t& getSphereOrientationCount() const;
    const bool& getAscii() const;
    const bool& getVerbose() const;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _fileNameT2;
    const float& _step;
    const std::string& _fileNameOut;
    const std::string& _outputFormat;
    const bool& _normalized;
    const bool& _singleFile;
    const bool& _useThickFascicles;
    const float& _fascicleRadius;
    const int32_t& _sphereOrientationCount;
    const bool& _ascii;
    const bool& _verbose;

};


BundleMapDensityMaskProcess::BundleMapDensityMaskProcess(
                                    const std::string& fileNameT2,
				    const float& step,
				    const std::string& fileNameOut,
				    const std::string& outputFormat,
				    const bool& normalized,
				    const bool& singleFile,
                                    const bool& useThickFascicles,
                                    const float& fascicleRadius,
                                    const int32_t& sphereOrientationCount,
				    const bool& ascii,
				    const bool& verbose )
                               : gkg::Process(),
                                 _fileNameT2( fileNameT2 ),
                                 _step( step ),
                                 _fileNameOut( fileNameOut ),
                                 _outputFormat( outputFormat ),
                                 _normalized( normalized ),
                                 _singleFile( singleFile ),
                                 _useThickFascicles( useThickFascicles ),
                                 _fascicleRadius( fascicleRadius ),
                                 _sphereOrientationCount( 
                                                       sphereOrientationCount ),
                                 _ascii( ascii ),
                                 _verbose( verbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &BundleMapDensityMaskProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &BundleMapDensityMaskProcess::operate< int16_t > );

}


const std::string& 
BundleMapDensityMaskProcess::getFileNameT2() const
{

  return _fileNameT2;

}


const float& 
BundleMapDensityMaskProcess::getStep() const
{

  return _step;

}


const std::string& 
BundleMapDensityMaskProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& 
BundleMapDensityMaskProcess::getOutputFormat() const
{

  return _outputFormat;

}


const bool& 
BundleMapDensityMaskProcess::getNormalized() const
{

  return _normalized;

}


const bool& 
BundleMapDensityMaskProcess::getSingleFile() const
{

  return _singleFile;

}


const bool& 
BundleMapDensityMaskProcess::useThickFascicles() const
{

  return _useThickFascicles;

}


const float& 
BundleMapDensityMaskProcess::getFascicleRadius() const
{

  return _fascicleRadius;

}


const int32_t& 
BundleMapDensityMaskProcess::getSphereOrientationCount() const
{

  return _sphereOrientationCount;

}


const bool& 
BundleMapDensityMaskProcess::getAscii() const
{

  return _ascii;

}


const bool& 
BundleMapDensityMaskProcess::getVerbose() const
{

  return _verbose;

}



template < class L >
void BundleMapDensityMaskProcess::operate( gkg::Process& process,
 					   const std::string& fileNameIn,
 					   const gkg::AnalyzedObject&,
 					   const std::string& )
{

  try
  {

    BundleMapDensityMaskProcess&
      bundleMapDensityMaskProcess =
        static_cast< BundleMapDensityMaskProcess& >( process );

    bool singleFile = bundleMapDensityMaskProcess.getSingleFile();


    ////////////////////////////////////////////////////////////////////////////
    // reading input T2-weighted (b=0) volume
    ////////////////////////////////////////////////////////////////////////////

    if ( bundleMapDensityMaskProcess.getVerbose() )
    {

      std::cout << "reading '"
                << bundleMapDensityMaskProcess.getFileNameT2()
                << "' : " << std::flush;

    }
    

    gkg::Volume< float > t2;
    gkg::TypedVolumeReaderProcess< float > typedVolumeReaderProcess( t2 );
    typedVolumeReaderProcess.execute(
                                  bundleMapDensityMaskProcess.getFileNameT2() );


    gkg::Vector3d< int32_t > size;
    t2.getSize( size );

    gkg::Vector3d< double > resolution;
    t2.getResolution( resolution );

    if ( bundleMapDensityMaskProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;
      std::cout << "input T2 volume size : " << size << std::endl;
      std::cout << "input T2 volume resolution : " << resolution << std::endl;
      

    }
    
    ////////////////////////////////////////////////////////////////////////////
    // reading input bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( bundleMapDensityMaskProcess.getVerbose() )
    {
     
      std::cout << "reading input '" << fileNameIn << "' bundle map : "
                << std::flush;
     
    }
    gkg::BundleMap< L > bundleMap;
    gkg::Reader::getInstance().read( fileNameIn, bundleMap );
    int32_t bundleCount = bundleMap.getBundleCount();

    if ( bundleMapDensityMaskProcess.getVerbose() )
    {
     
      std::cout << "done " << std::endl;
     
    }

    ////////////////////////////////////////////////////////////////////////////
    // creating and filling density mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > densityMask;

    if ( singleFile )
    {

      densityMask = gkg::Volume< float >( size, bundleCount );

    }
    else
    {

      densityMask = gkg::Volume< float >( size );

    }
    densityMask.setResolution( resolution );
    densityMask.fill( 0.0 );
    gkg::BoundingBox< int32_t > boundingBox( densityMask );


    ////////////////////////////////////////////////////////////////////////////
    // selecting fiber bundles
    ////////////////////////////////////////////////////////////////////////////

    std::list< gkg::Vector3d< float > > sphereDistributedPoints;
    sphereDistributedPoints.push_back(
                                      gkg::Vector3d< float >( 0.0, 0.0, 0.0 ) );
    if ( bundleMapDensityMaskProcess.useThickFascicles() )
    {

      float fascicleRadius = bundleMapDensityMaskProcess.getFascicleRadius();
      int32_t sphereOrientationCount =
                        bundleMapDensityMaskProcess.getSphereOrientationCount();
      int32_t currentOrientationCount = 0;
      float radius = 0.0;
      float step = bundleMapDensityMaskProcess.getStep();
      for ( radius = step; radius <= fascicleRadius; radius += step )
      {

        currentOrientationCount =
                       std::max( 6,
                                 ( int32_t )( ( float )sphereOrientationCount *
                                              ( radius / fascicleRadius ) *
                                              ( radius / fascicleRadius ) ) );
        if ( currentOrientationCount % 2 )
        {

          ++ currentOrientationCount;

        }
        gkg::AntipodallySymmetricUniformOrientationSet
          antipodallySymmetricUniformOrientationSet( currentOrientationCount );

        gkg::AntipodallySymmetricUniformOrientationSet::const_iterator
          o = antipodallySymmetricUniformOrientationSet.begin(),
          oe = antipodallySymmetricUniformOrientationSet.end();
        while ( o != oe )
        {

          sphereDistributedPoints.push_back( *o );
          ++ o;

        }

      }

    }
  

    ////////////////////////////////////////////////////////////////////////////
    // selecting fiber bundles
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > voxel;

    typename gkg::BundleMap< L >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    int32_t bundleIndex = 0;
    while ( b != be )
    {

      //////////////////////////////////////////////////////////////////////////
      // computing density mask
      //////////////////////////////////////////////////////////////////////////

      if ( bundleMapDensityMaskProcess.getVerbose() )
      {

        std::cout << "processing bundle '" << b->first << "' : " << std::flush;

      }

      if ( !singleFile )
      {

        densityMask.fill( 0.0 );

      }
      float densitySum = 0.0;

      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;

      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        
        float length = fiber.getLength();

        int32_t newPointCount =
         ( int32_t )( length / bundleMapDensityMaskProcess.getStep() + 0.5 );

        gkg::LightCurve3d< float > 
          resampledFiber = fiber.getEquidistantCurve( newPointCount );
        gkg::LightCurve3d< float >::const_iterator
          s = resampledFiber.begin(),
          se = resampledFiber.end();
        while ( s != se )
        {

          std::list< gkg::Vector3d< float > >::const_iterator
            o = sphereDistributedPoints.begin(),
            oe = sphereDistributedPoints.end();

          while ( o != oe )
          {

            voxel.x = ( int32_t )( ( s->x + o->x ) / resolution.x + 0.5 );
            voxel.y = ( int32_t )( ( s->y + o->y ) / resolution.y + 0.5 );
            voxel.z = ( int32_t )( ( s->z + o->z ) / resolution.z + 0.5 );

            if ( boundingBox.contains( voxel ) )
            {

              if ( singleFile )
              {

                ++ densityMask( voxel, bundleIndex );

              }
              else
              {

                ++ densityMask( voxel );

              }
              if ( bundleMapDensityMaskProcess.getNormalized() )
              {

                densitySum += 1.0f;

              }

            }

            ++ o;

          }

          ++ s;

        }

        ++ f;

      }

      if ( bundleMapDensityMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }


      //////////////////////////////////////////////////////////////////////////
      // normalizing density mask if required
      //////////////////////////////////////////////////////////////////////////

      if ( bundleMapDensityMaskProcess.getNormalized() )
      {

        if ( bundleMapDensityMaskProcess.getVerbose() )
        {

          std::cout << "normalizing density mask for bundle '"
                    << b->first << "' : " << std::flush;

        }

        if ( densitySum > 0.0f )
        {

          for ( voxel.z = 0; voxel.z < size.z; voxel.z++ )
          {

            for ( voxel.y = 0; voxel.y < size.y; voxel.y++ )
            {

              for ( voxel.x = 0; voxel.x < size.x; voxel.x++ )
              {

                densityMask( voxel, ( singleFile ? bundleIndex : 0 ) )
                  /= densitySum;

              }

            }

          }

        }


        if ( bundleMapDensityMaskProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // writing bundle density mask if not in single file mode
      //////////////////////////////////////////////////////////////////////////

      if ( !singleFile )
      {

        std::string fileNameOut = bundleMapDensityMaskProcess.getFileNameOut() +
                                  '_' +
                                  gkg::StringConverter::toString( b->first );
        if ( bundleMapDensityMaskProcess.getVerbose() )
        {

          std::cout << "writing '" << fileNameOut << "' : " << std::flush;

        }

        gkg::Writer::getInstance().write(
 			     fileNameOut,
 			     densityMask,
 			     bundleMapDensityMaskProcess.getAscii(),
 			     bundleMapDensityMaskProcess.getOutputFormat() );

        if ( bundleMapDensityMaskProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

      }

      ++ bundleIndex;
      ++ b;

    }


    //////////////////////////////////////////////////////////////////////////
    // writing output density mask
    //////////////////////////////////////////////////////////////////////////

    if ( singleFile )
    {

      if ( bundleMapDensityMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << bundleMapDensityMaskProcess.getFileNameOut()
                  << "' : " << std::flush;

      }

      gkg::Writer::getInstance().write(
                               bundleMapDensityMaskProcess.getFileNameOut(),
                               densityMask,
                               bundleMapDensityMaskProcess.getAscii(),
                               bundleMapDensityMaskProcess.getOutputFormat() );

      if ( bundleMapDensityMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapDensityMaskProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapDensityMaskCommand
//

gkg::DwiBundleMapDensityMaskCommand::DwiBundleMapDensityMaskCommand(
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
  GKG_CATCH( "gkg::DwiBundleMapDensityMaskCommand::"
             "DwiBundleMapDensityMaskCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapDensityMaskCommand::DwiBundleMapDensityMaskCommand(
                                          const std::string& fileNameT2,
                                          const std::string& fileNameBundleMap,
                                          const std::string& fileNameOut,
                                          float step,
                                          const std::string& outputFormat,
                                          bool normalized,
                                          bool singleFile,
                                          const bool& useThickFascicles,
                                          const float& fascicleRadius,
                                          const int32_t& sphereOrientationCount,
                                          bool ascii,
                                          bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameBundleMap,
             fileNameOut,
             step,
             outputFormat,
             normalized,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapDensityMaskCommand::"
             "DwiBundleMapDensityMaskCommand( "
             " const std::string& fileNameT2, "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "float step, "
             "const std::string& outputFormat, "
             "bool normalized, "
             "bool singleFile, "
             "const bool& useThickFascicles, "
             "const float& fascicleRadius, "
             "const int32_t& sphereOrientationCount, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiBundleMapDensityMaskCommand::DwiBundleMapDensityMaskCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, normalized );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, singleFile );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, useThickFascicles );
    DECLARE_FLOATING_PARAMETER( parameters, float, fascicleRadius );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sphereOrientationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameBundleMap,
             fileNameOut,
             step,
             outputFormat,
             normalized,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapDensityMaskCommand::"
             "DwiBundleMapDensityMaskCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapDensityMaskCommand::~DwiBundleMapDensityMaskCommand()
{
}


std::string gkg::DwiBundleMapDensityMaskCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapDensityMask";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiBundleMapDensityMaskCommand::getStaticName()" );

}


void gkg::DwiBundleMapDensityMaskCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameBundleMap;
    std::string fileNameOut;
    float step = 0.1;
    std::string outputFormat;
    bool normalized = false;
    bool singleFile = false;
    bool useThickFascicles = false;
    float fascicleRadius = 0.5;
    int32_t sphereOrientationCount = 100;
    bool ascii = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "computing fiber density in each voxel ",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map",
                                 fileNameBundleMap );
    application.addSingleOption( "-t2",
                                 "Input T2(b=0)-weighted volume",
                                 fileNameT2 );
    application.addSingleOption( "-s",
                                 "step ",
                                 step,
				 true );
    application.addSingleOption( "-o",
                                 "Output fiber densities file name",
                                 fileNameOut );
    application.addSingleOption( "-of",
                                 "Output format name",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-normalized",
                                 "Normalizing density map (default=false)",
                                 normalized,
                                 true );
    application.addSingleOption( "-singleFile",
                                 "Create a single 4D density mask including all"
                                 " the bundle density masks along the 4th "
                                 "dimension instead of one output density mask "
                                 "per bundle(default=false)",
                                 singleFile,
                                 true );
    application.addSingleOption( "-useThickFascicles",
                                 "Take into account the thickness of numerical "
                                 "fibers that correspond to faiscicles "
                                 "(default=false)",
                                 useThickFascicles,
                                 true );
    application.addSingleOption( "-fascicleRadius",
                                 "Fascicle radius (default=0.5mm)",
                                 fascicleRadius,
                                 true );
    application.addSingleOption( "-sphereOrientationCount",
                                 "Maximum sphere orientation count when taking "
                                 "into account fascicle thickness "
                                 "(default=100)",
                                 sphereOrientationCount,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 


    execute( fileNameT2,
             fileNameBundleMap,
             fileNameOut,
             step,
             outputFormat,
             normalized,
             singleFile,
             useThickFascicles,
             fascicleRadius,
             sphereOrientationCount,
             ascii,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMapDensityMaskCommand::parse()" );

}


void gkg::DwiBundleMapDensityMaskCommand::execute(
                                          const std::string& fileNameT2,
                                          const std::string& fileNameBundleMap,
                                          const std::string& fileNameOut,
                                          float step,
                                          const std::string& outputFormat,
                                          bool normalized,
                                          bool singleFile,
                                          const bool& useThickFascicles,
                                          const float& fascicleRadius,
                                          const int32_t& sphereOrientationCount,
                                          bool ascii,
                                          bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( step <= 0 )
    {

       throw std::runtime_error( "step must be strictly positive" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    BundleMapDensityMaskProcess 
      bundleMapDensityMaskProcess( fileNameT2,
                                   step,
                                   fileNameOut,
                                   outputFormat,
                                   normalized,
                                   singleFile,
                                   useThickFascicles,
                                   fascicleRadius,
                                   sphereOrientationCount,
                                   ascii,
                                   verbose );
    bundleMapDensityMaskProcess.execute( fileNameBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapDensityMaskCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameBundleMap, "
             "const std::string& fileNameOut, "
             "float step, "
             "const std::string& outputFormat, "
             "bool normalized, "
             "bool singleFile, "
             "const bool& useThickFascicles, "
             "const float& fascicleRadius, "
             "const int32_t& sphereOrientationCount, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleMapDensityMaskCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( normalized ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( singleFile ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( useThickFascicles ) +
    DECLARE_FLOATING_PARAMETER_HELP( fascicleRadius ) +
    DECLARE_INTEGER_PARAMETER_HELP( sphereOrientationCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
