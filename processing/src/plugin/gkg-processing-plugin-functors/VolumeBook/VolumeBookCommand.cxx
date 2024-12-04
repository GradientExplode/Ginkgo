#include <gkg-processing-plugin-functors/VolumeBook/VolumeBookCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cstdio>
#include <fstream>


extern "C"
{

#include <jpeglib.h>

}


template < class T >
void writeJpegData( const gkg::Volume< T >& slice,
                    const T& lowerValue,
                    const T& upperValue,
                    const int32_t& smoothingFactor,
                    const std::string& fileName )
{

  try
  {

    // rescaling data to uint8_t
    gkg::Volume< uint8_t > sliceU8;
    gkg::Rescaler< gkg::Volume< T >, gkg::Volume< uint8_t > >
      rescaler( lowerValue, upperValue, 0U, 255U );
    rescaler.rescale( slice, sliceU8 );

    gkg::Vector3d< double > resolution;
    slice.getResolution( resolution );

    jpeg_compress_struct jpegCompressStruct;
    jpeg_error_mgr jpegErrorManager;
    JSAMPROW rowPointer[ 1 ];

    jpegCompressStruct.err = jpeg_std_error( &jpegErrorManager );
    jpeg_create_compress( &jpegCompressStruct );
 
    jpegCompressStruct.image_width = slice.getSizeX();
    jpegCompressStruct.image_height = slice.getSizeY();
    jpegCompressStruct.in_color_space = JCS_GRAYSCALE;
    jpegCompressStruct.input_components = 1;
    jpegCompressStruct.input_gamma = 1;
    jpeg_set_defaults( &jpegCompressStruct );
    jpegCompressStruct.data_precision = 8;
    jpegCompressStruct.smoothing_factor = smoothingFactor;
    jpeg_set_quality( &jpegCompressStruct, 100, true );
    jpegCompressStruct.density_unit = 1;
    jpegCompressStruct.X_density = ( uint16_t )( 25.4 / resolution.x );
    jpegCompressStruct.Y_density = ( uint16_t )( 25.4 / resolution.y );

    FILE* fp = fopen( fileName.c_str(), "wb" );
    if ( !fp )
    {

      throw std::runtime_error( std::string( "can't open file \'" ) +
                                fileName + "\'" );

    }

    jpeg_stdio_dest( &jpegCompressStruct, fp );
    jpeg_start_compress( &jpegCompressStruct, true );

    uint32_t i = 0U;
    for ( i = 0; i < jpegCompressStruct.image_height; i++ )
    {

      rowPointer[ 0 ] = ( JSAMPROW )&sliceU8( 0, i );
      jpeg_write_scanlines( &jpegCompressStruct, rowPointer, 1 );

    }

    jpeg_finish_compress( &jpegCompressStruct );
    fclose( fp );
    jpeg_destroy_compress( &jpegCompressStruct );


  }
  GKG_CATCH( "template < class T > "
             "void writeJpegData( const gkg::Volume< T >& slice, "
             "const T& lowerValue, "
             "const T& upperValue, "
             "const std::string& fileName )" );


}


class BookProcess : public gkg::Process
{

  public:

    BookProcess( const std::string& fileNameOut,
                 const std::string& title,
                 const std::string& author,
                 const std::string& date,
                 const std::vector< double >& greyLevelRange,
                 const std::vector< double >& pictureSizes,
                 const int32_t& smoothingFactor,
                 const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    const std::string& getDate() const;
    const std::vector< double >& getGreyLevelRange() const;
    const std::vector< double >& getPictureSizes() const;
    const int32_t& getSmoothingFactor() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void makeBook( gkg::Process& process,
                          const std::string& fileNameIn,
                          const gkg::AnalyzedObject&,
                          const std::string& );

    const std::string& _fileNameOut;
    const std::string& _title;
    const std::string& _author;
    const std::string& _date;
    const std::vector< double >& _greyLevelRange;
    const std::vector< double >& _pictureSizes;
    const int32_t& _smoothingFactor;
    const bool& _verbose;

};


BookProcess::BookProcess( const std::string& fileNameOut,
                          const std::string& title,
                          const std::string& author,
                          const std::string& date,
                          const std::vector< double >& greyLevelRange,
                          const std::vector< double >& pictureSizes,
                          const int32_t& smoothingFactor,
                          const bool& verbose)
            : gkg::Process(),
              _fileNameOut( fileNameOut ),
              _title( title ),
              _author( author ),
              _date( date ),
              _greyLevelRange( greyLevelRange ),
              _pictureSizes( pictureSizes ),
              _smoothingFactor( smoothingFactor ),
              _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &BookProcess::makeBook< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &BookProcess::makeBook< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &BookProcess::makeBook< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &BookProcess::makeBook< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &BookProcess::makeBook< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &BookProcess::makeBook< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &BookProcess::makeBook< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &BookProcess::makeBook< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &BookProcess::makeBook< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &BookProcess::makeBook< double > );

}


const std::string& BookProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& BookProcess::getTitle() const
{

  return _title;

}


const std::string& BookProcess::getAuthor() const
{

  return _author;

}


const std::string& BookProcess::getDate() const
{

  return _date;

}


const std::vector< double >& BookProcess::getGreyLevelRange() const
{

  return _greyLevelRange;

}


const std::vector< double >& BookProcess::getPictureSizes() const
{

  return _pictureSizes;

}


const int32_t& BookProcess::getSmoothingFactor() const
{

  return _smoothingFactor;

}


const bool& BookProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void BookProcess::makeBook( gkg::Process& process,
                            const std::string& fileNameIn,
                            const gkg::AnalyzedObject&,
                            const std::string& )
{

  try
  {

    BookProcess&
      bookProcess = static_cast< BookProcess& >( process );

    // reading data
    if ( bookProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );
    if ( bookProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // collecting or processing scales
    T lowerValue = ( T )0;
    T upperValue = ( T )0;
    if ( bookProcess.getGreyLevelRange().empty() )
    {

      if ( bookProcess.getVerbose() )
      {

        std::cout << "processing grey level range : " << std::flush;

      }
      gkg::MinimumFilter< gkg::Volume< T >, T > minimumFilter;
      gkg::MaximumFilter< gkg::Volume< T >, T > maximumFilter;
      minimumFilter.filter( volume, lowerValue );
      maximumFilter.filter( volume, upperValue );

    }
    else
    {

      if ( bookProcess.getGreyLevelRange().size() != 2U )
      {

        throw std::runtime_error( "grey level range requires two bounds" );

      }
      if ( bookProcess.getVerbose() )
      {

        std::cout << "collecting grey level range : " << std::flush;

      }
      lowerValue = ( T )bookProcess.getGreyLevelRange()[ 0 ];
      upperValue = ( T )bookProcess.getGreyLevelRange()[ 1 ];

    }
    if ( bookProcess.getVerbose() )
    {

      std::cout << lowerValue << " -> " << upperValue << std::endl;

    }

    // collecting size(s) and resolution(s)
    int32_t x, y, z;
    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    gkg::Vector3d< double > resolution;
    volume.getResolution( resolution );

    // converting axial slices to JPEG images
    if ( bookProcess.getVerbose() )
    {

      std::cout << "writing axial JPEG images : " << std::flush;

    }

    int32_t s = 0;
    int32_t axialSliceCount = volume.getSizeZ();

    for ( s = 0; s < axialSliceCount; s++ )
    {

      gkg::Volume< T > slice( sizeX, sizeY );
      slice.getHeader().addAttribute( "resolutionX", resolution.x );
      slice.getHeader().addAttribute( "resolutionY", resolution.y );

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          slice( x, y ) = volume( x, y, s );

        }

      }

      std::string fileName = std::string( "axial_" ) +
                             gkg::StringConverter::toString( s ) +
                             ".jpg";
      writeJpegData( slice, lowerValue, upperValue,
                     bookProcess.getSmoothingFactor(),
                     fileName );

    }

    if ( bookProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // converting coronal slices to JPEG images
    if ( bookProcess.getVerbose() )
    {

      std::cout << "writing coronal JPEG images : " << std::flush;

    }

    int32_t coronalSliceCount = volume.getSizeY();
    for ( s = 0; s < coronalSliceCount; s++ )
    {

      gkg::Volume< T > slice( sizeX, sizeZ );
      slice.getHeader().addAttribute( "resolutionX", resolution.x );
      slice.getHeader().addAttribute( "resolutionY", resolution.z );

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          slice( x, z ) = volume( x, s, z );

        }

      }

      std::string fileName = std::string( "coronal_" ) +
                             gkg::StringConverter::toString( s ) +
                             ".jpg";
      writeJpegData( slice, lowerValue, upperValue,
                     bookProcess.getSmoothingFactor(),
                     fileName );

    }

    if ( bookProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // converting axial slices to JPEG images
    if ( bookProcess.getVerbose() )
    {

      std::cout << "writing sagittal JPEG images : " << std::flush;

    }

    int32_t sagittalSliceCount = volume.getSizeX();

    for ( s = 0; s < sagittalSliceCount; s++ )
    {

      gkg::Volume< T > slice( sizeZ, sizeY );
      slice.getHeader().addAttribute( "resolutionX", resolution.z );
      slice.getHeader().addAttribute( "resolutionY", resolution.y );

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          slice( z, y ) = volume( s, sizeY - 1 - y, z );

        }

      }

      std::string fileName = std::string( "sagittal_" ) +
                             gkg::StringConverter::toString( s ) +
                             ".jpg";
      writeJpegData( slice, lowerValue, upperValue,
                     bookProcess.getSmoothingFactor(),
                     fileName );

    }

    if ( bookProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // generating the latex source file
    if ( bookProcess.getVerbose() )
    {

      std::cout << "generating latex source file : " << std::flush;

    }


    std::ofstream os( ( bookProcess.getFileNameOut() + ".tex" ).c_str() );


    os << "\\documentclass[11pt,oneside,a4paper]{book}" << std::endl;
    os << std::endl;
    os << "\\usepackage{graphicx}" << std::endl;
    os << "\\usepackage{url}" << std::endl;
    os << std::endl;
    os << "\\begin{document}" << std::endl;
    os << std::endl;
    os << "\\title{" + bookProcess.getTitle() + "}" << std::endl;
    os << "\\author{" + bookProcess.getAuthor() + "}" << std::endl;
    os << "\\date{" + bookProcess.getDate() + "}" << std::endl;
    os << std::endl;
    os << "\\maketitle" << std::endl;
    os << std::endl;
    os << std::endl;


    os << "%==================================================" << std::endl;
    os << "% Axial" << std::endl;
    os << "%==================================================" << std::endl;
    os << std::endl;
    os << "\\chapter{Axial views}" << std::endl;
    os << std::endl;

    for ( s = 0; s < axialSliceCount - 2; s++ )
    {

      os << "\\clearpage\\begin{figure}[H]" << std::endl;
      os << std::endl;
      os << "  \\begin{center}$" << std::endl;
      os << "    \\begin{array}{c}" << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 0 ] << "cm]{axial_"
         << gkg::StringConverter::toString( s ) << ".jpg} \\\\"
         << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 0 ] << "cm]{axial_"
         << gkg::StringConverter::toString( s + 1 ) << ".jpg} \\\\"
         << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 0 ] << "cm]{axial_"
         << gkg::StringConverter::toString( s + 2 ) << ".jpg}" << std::endl;
      os << "    \\end{array}$" << std::endl;
      os << "  \\end{center}" << std::endl;
      os << std::endl;
      os << "  \\vspace{-0.5cm}" << std::endl;
      os << "  \\caption{axial slices "
         << gkg::StringConverter::toString( s ) << " / "
         << gkg::StringConverter::toString( s + 1 ) << " / "
         << gkg::StringConverter::toString( s + 2 ) << "}"
         << std::endl;
      os << std::endl;
      os << "\\end{figure}" << std::endl;
      os << std::endl;
      os << "\\newpage" << std::endl;
      os << std::endl;

    }


    os << std::endl;
    os << std::endl;
    os << "%==================================================" << std::endl;
    os << "% Coronal" << std::endl;
    os << "%==================================================" << std::endl;
    os << std::endl;
    os << "\\chapter{Coronal views}" << std::endl;
    os << std::endl;

    for ( s = 0; s < coronalSliceCount - 2; s++ )
    {

      os << "\\clearpage\\begin{figure}[H]" << std::endl;
      os << std::endl;
      os << "  \\begin{center}$" << std::endl;
      os << "    \\begin{array}{c}" << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 1 ] << "cm]{coronal_"
         << gkg::StringConverter::toString( s ) << ".jpg} \\\\"
         << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 1 ] << "cm]{coronal_"
         << gkg::StringConverter::toString( s + 1 ) << ".jpg} \\\\"
         << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 1 ] << "cm]{coronal_"
         << gkg::StringConverter::toString( s + 2 ) << ".jpg}" << std::endl;
      os << "    \\end{array}$" << std::endl;
      os << "  \\end{center}" << std::endl;
      os << std::endl;
      os << "  \\vspace{-0.5cm}" << std::endl;
      os << "  \\caption{coronal slices "
         << gkg::StringConverter::toString( s ) << " / "
         << gkg::StringConverter::toString( s + 1 ) << " / "
         << gkg::StringConverter::toString( s + 2 ) << "}"
         << std::endl;
      os << std::endl;
      os << "\\end{figure}" << std::endl;
      os << std::endl;
      os << "\\newpage" << std::endl;
      os << std::endl;

    }

    os << std::endl;
    os << std::endl;
    os << "%==================================================" << std::endl;
    os << "% Sagittal" << std::endl;
    os << "%==================================================" << std::endl;
    os << std::endl;
    os << "\\chapter{Sagittal views}" << std::endl;
    os << std::endl;

    for ( s = 0; s < sagittalSliceCount - 2; s++ )
    {

      os << "\\clearpage\\begin{figure}[H]" << std::endl;
      os << std::endl;
      os << "  \\begin{center}$" << std::endl;
      os << "    \\begin{array}{c}" << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 2 ] << "cm]{sagittal_"
         << gkg::StringConverter::toString( s + 2 ) << ".jpg} \\\\"
         << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 2 ] << "cm]{sagittal_"
         << gkg::StringConverter::toString( s + 1 ) << ".jpg} \\\\"
         << std::endl;
      os << "      \\includegraphics[width="
         << bookProcess.getPictureSizes()[ 2 ] << "cm]{sagittal_"
         << gkg::StringConverter::toString( s ) << ".jpg}" << std::endl;
      os << "    \\end{array}$" << std::endl;
      os << "  \\end{center}" << std::endl;
      os << std::endl;
      os << "  \\vspace{-0.5cm}" << std::endl;
      os << "  \\caption{sagittal slices "
         << gkg::StringConverter::toString( s ) << " / "
         << gkg::StringConverter::toString( s + 1 ) << " / "
         << gkg::StringConverter::toString( s + 2 ) << "}"
         << std::endl;
      os << std::endl;
      os << "\\end{figure}" << std::endl;
      os << std::endl;
      os << "\\newpage" << std::endl;
      os << std::endl;

    }

    os << std::endl;
    os << "\\end{document}" << std::endl;

    os.close();

    if ( bookProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    // converting latex source to pdf
    if ( bookProcess.getVerbose() )
    {

      std::cout << "converting latex source to pdf file : " << std::flush;

    }

    int32_t status = system( ( std::string( "pdflatex " ) +
                               bookProcess.getFileNameOut() + 
			       ".tex" ).c_str() );
			       
    if ( status < 0 )
    {
    
      throw std::runtime_error( "Error executing pdflatex command" );
    
    }

    if ( bookProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void BookProcess::makeBook( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VolumeBookCommand
//


gkg::VolumeBookCommand::VolumeBookCommand( int32_t argc,
                                 	   char* argv[],
                                 	   bool loadPlugin,
                                 	   bool removeFirst )
                      : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::VolumeBookCommand::VolumeBookCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VolumeBookCommand::VolumeBookCommand(
                        	    const std::string& fileNameIn,
                        	    const std::string& fileNameOut,
                        	    const std::vector< double >& greyLevelRange,
                        	    const std::vector< double >& pictureSizes,
                        	    int32_t smoothingFactor,
                        	    const std::string& title,
                        	    const std::string& author,
                        	    const std::string& date,
                        	    bool verbose )
                      : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, greyLevelRange, pictureSizes,
             smoothingFactor, title, author, date, verbose );

  }
  GKG_CATCH( "gkg::VolumeBookCommand::VolumeBookCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::vector< double >& greyLevelRange, "
             "const std::vector< double >& pictureSizes, "
             "int32_t smoothingFactor, const std::string& title, "
             "const std::string& author, const std::string& date, "
             "bool verbose )" );

}


gkg::VolumeBookCommand::VolumeBookCommand( const gkg::Dictionary& parameters )
                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           greyLevelRange );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           pictureSizes );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, smoothingFactor );
    DECLARE_STRING_PARAMETER( parameters, std::string, title );
    DECLARE_STRING_PARAMETER( parameters, std::string, author );
    DECLARE_STRING_PARAMETER( parameters, std::string, date );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, greyLevelRange, pictureSizes,
             smoothingFactor, title, author, date, verbose );

  }
  GKG_CATCH( "gkg::VolumeBookCommand::VolumeBookCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VolumeBookCommand::~VolumeBookCommand()
{
}


std::string gkg::VolumeBookCommand::getStaticName()
{

  try
  {

    return "VolumeBook";

  }
  GKG_CATCH( "std::string gkg::VolumeBookCommand::getStaticName()" );

}


void gkg::VolumeBookCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::vector< double > greyLevelRange;
    std::vector< double > pictureSizes;
    int32_t smoothingFactor = 0;
    std::string title;
    std::string author;
    std::string date;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Making a PDF book from a volume",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output PDF book file name",
                                 fileNameOut );
    application.addSingleOption( "-title",
                                 "Book title",
                                 title );
    application.addSingleOption( "-author",
                                 "Book author",
                                 author );
    application.addSingleOption( "-date",
                                 "Book date",
                                 date );
    application.addSeriesOption( "-range",
                                 "Grey level range (default=auto)",
                                 greyLevelRange );
    application.addSeriesOption( "-pictureSizes",
                                 "Axial / Coronal / Sagittal picture sizes "
                                 "in cm",
                                 pictureSizes,
                                 3, 3 );
    application.addSingleOption( "-smoothing",
                                 "Smoothing factor (default=0)",
                                 smoothingFactor,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn, fileNameOut, greyLevelRange, pictureSizes,
             smoothingFactor, title, author, date, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VolumeBookCommand::parse()" );

}


void gkg::VolumeBookCommand::execute(
                      		    const std::string& fileNameIn,
                      		    const std::string& fileNameOut,
                      		    const std::vector< double >& greyLevelRange,
                      		    const std::vector< double >& pictureSizes,
                      		    int32_t smoothingFactor,
                      		    const std::string& title,
                      		    const std::string& author,
                      		    const std::string& date,
                      		    bool verbose )
{

  try
  {

    //
    // launching process
    //
    BookProcess bookProcess( fileNameOut,
                             title,
                             author,
                             date,
                             greyLevelRange,
                             pictureSizes,
                             smoothingFactor,
                             verbose );
    bookProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::VolumeBookCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::vector< double >& greyLevelRange, "
             "const std::vector< double >& pictureSizes, "
             "int32_t smoothingFactor, const std::string& title, "
             "const std::string& author, const std::string& date, "
             "bool verbose )" );

}


RegisterCommandCreator(
                  VolumeBookCommand,
    		  DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    		  DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		  DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( greyLevelRange ) +
    		  DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( pictureSizes ) +
    		  DECLARE_INTEGER_PARAMETER_HELP( smoothingFactor ) +
    		  DECLARE_STRING_PARAMETER_HELP( title ) +
    		  DECLARE_STRING_PARAMETER_HELP( author ) +
    		  DECLARE_STRING_PARAMETER_HELP( date ) +
    		  DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
