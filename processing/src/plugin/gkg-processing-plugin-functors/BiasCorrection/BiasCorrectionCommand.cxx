#include <gkg-processing-plugin-functors/BiasCorrection/BiasCorrectionCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-bias/BiasCorrectionFactory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class BiasCorrectionProcess : public gkg::Process
{

  public:

    BiasCorrectionProcess( const std::string& fileNameMask,
                           const std::string& fileNameWeight,
                           const std::string& fileNameOut,
                           const std::string& fileNameBias,
                           const std::string& type,
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters,
                           const bool& ascii,
                           const std::string& format,
                           const bool& verbose );

    const std::string& getFileNameMask() const;
    const std::string& getFileNameWeight() const;
    const std::string& getFileNameOut() const;
    const std::string& getFileNameBias() const;
    const std::string& getType() const;
    const std::vector< double >& getScalarParameters() const;
    const std::vector< std::string >& getStringParameters() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void correct( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _fileNameMask;
    const std::string& _fileNameWeight;
    const std::string& _fileNameOut;
    const std::string& _fileNameBias;
    const std::string& _type;
    const std::vector< double >& _scalarParameters;
    const std::vector< std::string >& _stringParameters;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


BiasCorrectionProcess::BiasCorrectionProcess(
                             const std::string& fileNameMask,
                             const std::string& fileNameWeight,
                             const std::string& fileNameOut,
                             const std::string& fileNameBias,
                             const std::string& type,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             const bool& ascii,
                             const std::string& format,
                             const bool& verbose )
                      : gkg::Process(),
                        _fileNameMask( fileNameMask ),
                        _fileNameWeight( fileNameWeight ),
                        _fileNameOut( fileNameOut ),
                        _fileNameBias( fileNameBias ),
                        _type( type ),
                        _scalarParameters( scalarParameters ),
                        _stringParameters( stringParameters ),
                        _ascii( ascii ),
                        _format( format ),
                        _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &BiasCorrectionProcess::correct< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &BiasCorrectionProcess::correct< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &BiasCorrectionProcess::correct< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &BiasCorrectionProcess::correct< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &BiasCorrectionProcess::correct< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &BiasCorrectionProcess::correct< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &BiasCorrectionProcess::correct< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &BiasCorrectionProcess::correct< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &BiasCorrectionProcess::correct< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &BiasCorrectionProcess::correct< double > );

}


const std::string& BiasCorrectionProcess::getFileNameMask() const
{

  return _fileNameMask;

}


const std::string& BiasCorrectionProcess::getFileNameWeight() const
{

  return _fileNameWeight;

}


const std::string& BiasCorrectionProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& BiasCorrectionProcess::getFileNameBias() const
{

  return _fileNameBias;

}


const std::string& BiasCorrectionProcess::getType() const
{

  return _type;

}


const std::vector< double >& BiasCorrectionProcess::getScalarParameters() const
{

  return _scalarParameters;

}


const std::vector< std::string >& 
BiasCorrectionProcess::getStringParameters() const
{

  return _stringParameters;

}


const bool& BiasCorrectionProcess::getAscii() const
{

  return _ascii;

}


const std::string& BiasCorrectionProcess::getFormat() const
{

  return _format;

}


const bool& BiasCorrectionProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void BiasCorrectionProcess::correct( gkg::Process& process,
                                     const std::string& fileNameIn,
                                     const gkg::AnalyzedObject&,
                                     const std::string& )
{

  try
  {

    BiasCorrectionProcess&
      biasCorrectionProcess = static_cast< BiasCorrectionProcess& >( process );

    // sanity check(s)
    std::string type = biasCorrectionProcess.getType();
    if ( ( type != "bias-correction-2d" ) &&
         ( type != "bias-correction-poly-2d" ) &&
         ( type != "bias-correction-3d" ) )
    {

      throw std::runtime_error( "bad correction type" );

    }

    // reading input
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > in;
    gkg::Reader::getInstance().read( fileNameIn, in );
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // reading mask
    gkg::Volume< int16_t > mask;
    if ( !biasCorrectionProcess.getFileNameMask().empty() )
    {

      if ( biasCorrectionProcess.getVerbose() )
      {

        std::cout << "reading '" << biasCorrectionProcess.getFileNameMask()
                  << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read(
                                biasCorrectionProcess.getFileNameMask(), mask );
      if ( biasCorrectionProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      // collecting size
      gkg::Vector3d< int32_t > size;
      in.getSize( size );

      // collecting resolution
      gkg::Vector3d< double > resolution;
      in.getResolution( resolution );

      // allocating weight
      mask.reallocate( size.x, size.y, size.z );
      mask.setResolution( resolution );

      // filling with 1.0
      mask.fill( 1 );

    }

    // reading weight
    gkg::Volume< float > weight;
    if ( !biasCorrectionProcess.getFileNameWeight().empty() )
    {

      if ( biasCorrectionProcess.getVerbose() )
      {

        std::cout << "reading '" << biasCorrectionProcess.getFileNameWeight()
                  << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read(
                            biasCorrectionProcess.getFileNameWeight(), weight );
      if ( biasCorrectionProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      // collecting size
      gkg::Vector3d< int32_t > size;
      in.getSize( size );

      // collecting resolution
      gkg::Vector3d< double > resolution;
      in.getResolution( resolution );

      // allocating weight
      weight.reallocate( size.x, size.y, size.z );
      weight.setResolution( resolution );

      // filling with 1.0
      weight.fill( 1.0f );

    }

    // creating bias correction algorithm
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "creating " << type << " : " << std::flush;

    }
    gkg::RCPointer< gkg::BiasCorrection< T > >
      biasCorrection( 
        gkg::BiasCorrectionFactory< T >::getInstance().createBiasCorrection(
          type,
          biasCorrectionProcess.getScalarParameters(),
          biasCorrectionProcess.getStringParameters() ) );
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // correcting bias
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "correcting bias : " << std::flush;

    }
    gkg::Volume< T > out;
    gkg::Volume< float > bias;

    biasCorrection->correct( in,
                             mask,
                             weight,
                             out,
                             bias,
                             biasCorrectionProcess.getVerbose() );
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing corrected data
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "writing corrected volume '"
                << biasCorrectionProcess.getFileNameOut()
                << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( biasCorrectionProcess.getFileNameOut(),
                                      out,
                                      biasCorrectionProcess.getAscii(),
                                      biasCorrectionProcess.getFormat() );
    if ( biasCorrectionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing corresponding bias
    if ( !biasCorrectionProcess.getFileNameBias().empty() )
    {

      if ( biasCorrectionProcess.getVerbose() )
      {

        std::cout << "writing bias volume'"
                  << biasCorrectionProcess.getFileNameBias()
                  << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( biasCorrectionProcess.getFileNameBias(),
                                        bias,
                                        biasCorrectionProcess.getAscii(),
                                        biasCorrectionProcess.getFormat() );
      if ( biasCorrectionProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void BiasCorrectionProcess::correct( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class BiasCorrectionCommand
//


gkg::BiasCorrectionCommand::BiasCorrectionCommand( int32_t argc,
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
  GKG_CATCH( "gkg::BiasCorrectionCommand::BiasCorrectionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BiasCorrectionCommand::BiasCorrectionCommand(
                             const std::string& fileNameIn,
                             const std::string& fileNameMask,
                             const std::string& fileNameWeight,
                             const std::string& fileNameOut,
                             const std::string& fileNameBias,
                             const std::string& type,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
{

  try
  {

    execute( fileNameIn, fileNameMask, fileNameWeight, fileNameOut,
             fileNameBias, type, scalarParameters, stringParameters,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::BiasCorrectionCommand::BiasCorrectionCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameWeight, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameBias, "
             "const std::string& type, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::BiasCorrectionCommand::BiasCorrectionCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameWeight );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBias );
    DECLARE_STRING_PARAMETER( parameters, std::string, type );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< double >,
                                           scalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         stringParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameMask, fileNameWeight, fileNameOut,
             fileNameBias, type, scalarParameters, stringParameters,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::BiasCorrectionCommand::BiasCorrectionCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BiasCorrectionCommand::~BiasCorrectionCommand()
{
}


std::string gkg::BiasCorrectionCommand::getStaticName()
{

  try
  {

    return "BiasCorrection";

  }
  GKG_CATCH( "std::string gkg::BiasCorrectionCommand::getStaticName()" );

}


void gkg::BiasCorrectionCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameMask;
    std::string fileNameWeight;
    std::string fileNameOut;
    std::string fileNameBias;
    std::string type = "bias-correction-3d";
    std::vector< double > scalarParameters;
    std::vector< std::string > stringParameters;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Bias correction of the gray level intensity "
                                  "of a volume",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input corrupted volume file name",
                                 fileNameIn );
    application.addSingleOption( "-m",
                                 "Mask int16_t volume file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-w",
                                 "Weight float volume file name",
                                 fileNameWeight,
                                 true );
    application.addSingleOption( "-o",
                                 "Output corrected volume file name",
                                 fileNameOut );
    application.addSingleOption( "-b",
                                 "Output bias file name",
                                 fileNameBias,
                                 true );
    application.addSingleOption( "-type",
                                 "Bias type among:\n"
                                 "- bias-correction-2d\n"
                                 "- bias-correction-poly-2d\n"
                                 "- bias-correction-3d\n"
                                 "(default=bias-correction-3d)",
                                 type,
                                 true );
    application.addSeriesOption( "-scalarParameters",
                                 "Scalar parameters for bias correction"
                                 "Scalar parameters as a vector"
                                 " of double <P1> <P2> <P3> ... <Pn>:\n\n"
                                 "- in case of bias-correction-2d with "
                                 "mean square difference measure,\n"
                                 "   <P1>: reference slice (-1->middle)\n"
                                 "   <P2>: initial parameter\n"
                                 "   <P3>: delta parameter\n"
                                 "   <P4>: maximum Nelder-Mead iteration"
                                         " count\n"
                                 "   <P5>: maximum Nelder-Mead test size\n"
                                 "   <P6>: verbose Nelder-Mead 0 / 1\n"
                                 "   (default=-1/1.0/0.05/1000/1e-4/0)\n\n"
                                 "- in case of bias-correction-2d with "
                                 "entropy measure,\n"
                                 "   <P1>: reference slice (-1->middle)\n"
                                 "   <P2>: initial parameter\n"
                                 "   <P3>: delta parameter\n"
                                 "   <P4>: maximum Nelder-Mead iteration"
                                         " count\n"
                                 "   <P5>: maximum Nelder-Mead test size\n"
                                 "   <P6>: verbose Nelder-Mead 0 / 1\n"
                                 "   <P7>: level count for probability\n"
                                 "   <P8>: smooth probability 0 / 1\n"
                                 "   (default=-1/1.0/0.05/1000/1e-4/0)\n\n"
                                 "- in case of bias-correction-poly-2d,\n"
                                 "   <P1>: maximum fit order\n"
                                 "   <P2>: maximum bias order\n"
                                 "   (default=8/8)\n\n"
                                 "- in case of bias-correction-3d,\n"
                                 "   <P1>: piecewise bias volume X size\n"
                                 "   <P2>: piecewise bias volume Y size\n"
                                 "   <P3>: piecewise bias volume Z size\n"
                                 "   <P4>: entropy factor\n"
                                 "   <P5>: data attachment factor\n"
                                 "   <P6>: regularization factor\n"
                                 "   <P7>: simulated annealing gain K\n"
                                 "   <P8>: simulated annealing Boltzmann"
                                         " mu parameter\n"
                                 "   <P9>: simulated annealing initial"
                                         " temperature\n"
                                 "   <P10>: simulated annealing minimum"
                                          " temperature\n"
                                 "   <P11>: trial count per simulated annealing"
                                          " step\n"
                                 "   <P12>: iteration count per temperature\n"
                                 "   <P13>: simulated annealing maximum test"
                                          " size\n"
                                 "   <P14>: verbose simulated annealing 0 /"
                                          " 1\n"
                                 "   <P15>: level count in entropy "
                                          " processing\n"
                                 "   <P16>: smooth probability 0 / 1\n"
                                 "   (default=to be completed)\n",
                                 scalarParameters );
    application.addSeriesOption( "-stringParameters",
                                 "String parameters for bias correction"
                                 "String parameters as a vector"
                                 " of string <P1> <P2> <P3> ... <Pn>:\n\n"
                                 "- in case of bias-correction-2d,\n"
                                 "   <P1>: measure type mean-square-difference"
                                         " / entropy\n"
                                 "   <P2>: bias type multiplicative /"
                                         " additive\n"
                                 "   <P3>: plane axis x / y / z\n"
                                 "   (default=mean-square-difference/"
                                         "multiplicative/z)\n\n"
                                 "- in case of bias-correction-poly-2d,\n"
                                 "   <P1>: plane axis x / y / z\n"
                                 "   (default=z)\n\n"
                                 "- in case of bias-correction-3d,\n"
                                 "   N/A\n",
                                 stringParameters );
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

    execute( fileNameIn, fileNameMask, fileNameWeight, fileNameOut,
             fileNameBias, type, scalarParameters, stringParameters,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BiasCorrectionCommand::parse()" );

}


void gkg::BiasCorrectionCommand::execute(
                  	     const std::string& fileNameIn,
                             const std::string& fileNameMask,
                             const std::string& fileNameWeight,
                             const std::string& fileNameOut,
                             const std::string& fileNameBias,
                             const std::string& type,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
{

  try
  {

    BiasCorrectionProcess biasCorrectionProcess( fileNameMask,
                                                 fileNameWeight,
                                                 fileNameOut,
                                                 fileNameBias,
                                                 type,
                                                 scalarParameters,
                                                 stringParameters,
                                                 ascii,
                                                 format,
                                                 verbose );

    biasCorrectionProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::BiasCorrectionCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameWeight, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameBias, "
             "const std::string& type, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                BiasCorrectionCommand,
                DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
                DECLARE_STRING_PARAMETER_HELP( fileNameWeight ) +
                DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                DECLARE_STRING_PARAMETER_HELP( fileNameBias ) +
                DECLARE_STRING_PARAMETER_HELP( type ) +
                DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
                DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringParameters ) +
                DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                DECLARE_STRING_PARAMETER_HELP( format ) +
                DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
