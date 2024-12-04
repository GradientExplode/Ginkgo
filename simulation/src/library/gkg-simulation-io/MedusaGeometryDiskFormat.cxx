#include <gkg-simulation-io/MedusaGeometryDiskFormat.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-io/DefaultAsciiItemReaderOfCell_i.h>
#include <gkg-simulation-io/DefaultAsciiItemWriterOfCell_i.h>
#include <gkg-simulation-io/DefaultBinaryItemReaderOfCell_i.h>
#include <gkg-simulation-io/DefaultBinaryItemWriterOfCell_i.h>
#include <gkg-simulation-io/DefaultBSwapItemReaderOfCell_i.h>
#include <gkg-simulation-io/DefaultBSwapItemWriterOfCell_i.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/NotOpenException.h>
#include <fstream>


gkg::MedusaGeometryDiskFormat::MedusaGeometryDiskFormat()
                              : gkg::TypedDiskFormat< gkg::VirtualTissue >()
{

  try
  {

    this->_extensions.insert( ".medusageometry" );
    this->_extensions.insert( getHeaderExtension() );

  }
  GKG_CATCH( "gkg::MedusaGeometryDiskFormat::"
             "MedusaGeometryDiskFormat()" );

}


gkg::MedusaGeometryDiskFormat::~MedusaGeometryDiskFormat()
{
}


std::string gkg::MedusaGeometryDiskFormat::getName() const
{

  try
  {

    return "medusa-geometry";

  }
  GKG_CATCH( "std::string "
             "gkg::MedusaGeometryDiskFormat::getName() const" );

}


void gkg::MedusaGeometryDiskFormat::readHeader(
                              const std::string& name,
                              gkg::HeaderedObject& object ) const
{

  try
  {


    // first we add all VirtualTissue specific attributes
    object.addSemantic( "__medusageometry__", "byte_order",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );
    object.addSemantic( "__medusageometry__", "io_mode",
                       gkg::Semantic( gkg::TypeOf< std::string >::getName() ) );


    // reading meta-information
    try
    {

      this->gkg::DiskFormat::readHeader( name, object );

    }
    catch ( gkg::FileNotFoundException& e )
    {

      // std::cerr << e.what() << ", "
      //           << "meta-information not present"
      //           << std::endl;

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( e.what() );

    }


    // checking if it is a Medusa geometry
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "MedusaGeometry" )
    {

      throw gkg::ObjectTypeException(
                        this->getNameWithoutExtension( name ) +
                        getHeaderExtension() );

    }


    // checking that population count exists and is valid
    int32_t populationCount = 0;
    if ( object.getHeader().hasAttribute( "population_count" ) )
    {

      object.getHeader().getAttribute( "population_count",
                                       populationCount );
      if ( populationCount < 0 )
      {

        throw std::runtime_error( "invalid population count" );

      }

    }
    else
    {

       throw std::runtime_error( "population count not present" );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "void gkg::MedusaGeometryDiskFormat::readHeader( "
             "const std::string& name, "
             "gkg::HeaderedObject& object ) const" );

}


void gkg::MedusaGeometryDiskFormat::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "void gkg::MedusaGeometryDiskFormat::writeHeader( "
             "const std::string& name, "
             "const gkg::HeaderedObject& object ) const" );

}


void gkg::MedusaGeometryDiskFormat::read( const std::string& name,
                                          gkg::VirtualTissue& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object );


    // checking whether the medusa geometry is degenerated
    int32_t maximumAtomCountExceeded = 0;
    if ( object.getHeader().hasAttribute( "maximum_atom_count_exceeded" ) )
    {

      object.getHeader().getAttribute( "maximum_atom_count_exceeded",
                                       maximumAtomCountExceeded );

    }

    if ( !maximumAtomCountExceeded )
    {

      // getting byte order
      bool bswap = false;
      if ( object.getHeader().hasAttribute( "byte_order" ) )
      {

        std::string byteOrderName;
        object.getHeader().getAttribute( "byte_order", byteOrderName );

        bswap =
          ( gkg::ByteOrder::getInstance().getTypeFromName( byteOrderName ) !=
            gkg::ByteOrder::getInstance().getCurrentType() ? true : false );

      }

      // getting IO mode
      bool ascii = false;
      if ( object.getHeader().hasAttribute( "io_mode" ) )
      {

        std::string ioModeName;
        object.getHeader().getAttribute( "io_mode", ioModeName );
        if ( gkg::IOMode::getInstance().getTypeFromName( ioModeName ) ==
             gkg::IOMode::Ascii )
        {

          ascii = true;

        }

      }

      // collecting the field of view
      gkg::BoundingBox< float > fieldOfView( 0.0f, 0.0f, 0.0f,
                                             0.0f, 0.0f, 0.0f );
      if ( object.getHeader().hasAttribute( "field_of_view" ) )
      {

        gkg::GenericObjectList golFieldOfView;
 
        object.getHeader().getAttribute( "field_of_view",
                                         golFieldOfView );

        if ( golFieldOfView.size() != 6U )
        {

          throw std::runtime_error(
            "the size of field_of_view is not 6" );

        }
        fieldOfView.setLowerX( ( float )golFieldOfView[ 0 ]->getScalar() );
        fieldOfView.setLowerY( ( float )golFieldOfView[ 1 ]->getScalar() );
        fieldOfView.setLowerZ( ( float )golFieldOfView[ 2 ]->getScalar() );
        fieldOfView.setUpperX( ( float )golFieldOfView[ 3 ]->getScalar() );
        fieldOfView.setUpperY( ( float )golFieldOfView[ 4 ]->getScalar() );
        fieldOfView.setUpperZ( ( float )golFieldOfView[ 5 ]->getScalar() );

      }
      else
      {

        throw std::runtime_error( "missing 'field_of_view' attribute" );

      }

      // checking that population count exists and is valid
      int32_t populationCount = 0;
      if ( object.getHeader().hasAttribute( "population_count" ) )
      {

        object.getHeader().getAttribute( "population_count",
                                         populationCount );
        if ( populationCount < 0 )
        {

          throw std::runtime_error( "invalid population count" );

        }

      }
      else
      {

        throw std::runtime_error( "missing 'population_count' attribute" );

      }

      // collecting the population types
      std::vector< std::string > populationTypes;
      if ( object.getHeader().hasAttribute( "population_types" ) )
      {

        gkg::GenericObjectList golPopulationTypes;
 
        object.getHeader().getAttribute( "population_types",
                                         golPopulationTypes );

        if ( ( int32_t )golPopulationTypes.size() != populationCount )
        {

          throw std::runtime_error(
          "the size of population_types and population count are incoherent" );

        }
        populationTypes.resize( populationCount );
        int32_t populationId = 0;
        for ( populationId = 0; populationId < populationCount; populationId++ )
        {

          populationTypes[ populationId ] =
                                golPopulationTypes[ populationId ]->getString();

        }

      }
      else
      {

        throw std::runtime_error( "missing 'population_types' attribute" );

      }

      // collecting the population labels
      std::vector< std::string > populationLabels;
      if ( object.getHeader().hasAttribute( "population_labels" ) )
      {

        gkg::GenericObjectList golPopulationLabels;
      
        object.getHeader().getAttribute( "population_labels",
                                         golPopulationLabels );

        if ( ( int32_t )golPopulationLabels.size() != populationCount )
        {

          throw std::runtime_error( 
          "the size of population_labels and population count are incoherent" );

        }
        populationLabels.resize( populationCount );
        int32_t populationId = 0;
        for ( populationId = 0; populationId < populationCount; populationId++ )
        {

          populationLabels[ populationId ] =
                              golPopulationLabels[ populationId ]->getString();

        }

      }
      else
      {

        throw std::runtime_error( "missing 'population_types' attribute" );

      }

      gkg::ItemReader< int32_t >& itemR_Int32 =
        gkg::DefaultItemIOFactory< int32_t >::getInstance().getReader(
                                                                 ascii,
                                                                 bswap );
      gkg::ItemReader< gkg::Cell >& itemR_Cell =
        gkg::DefaultItemIOFactory< gkg::Cell >::getInstance().
                                                            getReader(
                                                                 ascii,
                                                                 bswap );

      std::string fileNameMedusaGeometry =
                                        this->getNameWithoutExtension( name ) +
                                        ".medusageometry";
      std::ifstream is( fileNameMedusaGeometry.c_str() );

      // reading the population id
      int32_t populationCountInBinaryFile = 0;
      itemR_Int32.read( is, populationCountInBinaryFile );

      if ( populationCountInBinaryFile != populationCount )
      {

        throw std::runtime_error( "inconsistent binary and header files" );

      }

      std::vector< gkg::RCPointer< gkg::Population > >
        populations( populationCount );

      int32_t populationIndex = 0;
      for ( populationIndex = 0; populationIndex < populationCount;
            populationIndex++ )
      {

        // reading the population id
        int32_t populationId = 0;
        itemR_Int32.read( is, populationId );

        // reading the population cell count
        int32_t cellCount = 0;
        itemR_Int32.read( is, cellCount );

        //creating the adequate cell vector
        std::vector< gkg::RCPointer< gkg::Cell > >
          cells( cellCount );

        // reading the cell(s)
        int32_t cellIndex = 0;
        for ( cellIndex = 0; cellIndex < cellCount; cellIndex++ )
        {

          cells[ cellIndex ].reset( new gkg::Cell );
          itemR_Cell.read( is, *cells[ cellIndex ] );

        }

        populations[ populationIndex ].reset(
          new gkg::Population( populationId,
                               populationTypes[ populationId ],
                               &object,
                               cells ) );

      }

      is.close();

      // finally adding the populations that were read to the current 
      // virtual tissue
      object.addPopulations( fieldOfView,
                             populations,
                             populationTypes,
                             populationLabels );

    }

  }
  GKG_CATCH( "void gkg::MedusaGeometryDiskFormat::read( "
             "const std::string& name, "
             "gkg::ParticleMap& object ) const" );

}


void gkg::MedusaGeometryDiskFormat::write( const std::string& name,
                                           gkg::VirtualTissue& object,
                                           bool ascii ) const
{

  try
  {

    // setting IO mode
    std::string ioModeName =
      gkg::IOMode::getInstance().getNameFromType( ascii ? gkg::IOMode::Ascii :
                                                          gkg::IOMode::Binary );
    if ( object.getHeader().hasAttribute( "io_mode" ) )
    {

      object.getHeader()[ "io_mode" ] = ioModeName;
      
    }
    else
    {

      object.getHeader().addAttribute( "io_mode", ioModeName );

    }

    // setting byte order
    std::string byteOrderName = gkg::ByteOrder::getInstance().getCurrentName();
    if ( object.getHeader().hasAttribute( "byte_order" ) )
    {

      object.getHeader()[ "byte_order" ] = byteOrderName;

    }
    else
    {

      object.getHeader().addAttribute( "byte_order", byteOrderName );

    }

    // checking whether the medusa geometry is degenerated
    int32_t maximumAtomCountExceeded = 0;
    if ( object.getHeader().hasAttribute( "maximum_atom_count_exceeded" ) )
    {

      object.getHeader().getAttribute( "maximum_atom_count_exceeded",
                                       maximumAtomCountExceeded );

    }

    // writing header(s) meta-information
    writeHeader( name, object );


    // writing core data <*.medusageometry>
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".medusageometry" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException(
                               this->getNameWithoutExtension( name ) +
                               ".medusageometry" );

    }


    if ( !maximumAtomCountExceeded )
    {


      gkg::ItemWriter< int32_t >& itemW_Int32 =
        gkg::DefaultItemIOFactory< int32_t >::getInstance().getWriter(
                                                                 ascii,
                                                                 false );
      gkg::ItemWriter< gkg::Cell >& itemW_Cell =
        gkg::DefaultItemIOFactory< gkg::Cell >::getInstance().
                                                            getWriter(
                                                                 ascii,
                                                                 false );

      // writing the population count
      itemW_Int32.write( os, object.getPopulationCount() );

      // looping over cell population(s)()
      gkg::VirtualTissue::const_iterator
        p = object.begin(),
        pe = object.end();
      while ( p != pe )
      {

        const gkg::Population& population = **p;

        // writing the population id
        itemW_Int32.write( os, population.getId() );

        // writing the population cell count
        itemW_Int32.write( os, population.getCellCount() );

        // then looping over cell(s)
        gkg::Population::const_iterator
          c = population.begin(),
          ce = population.end();
        while ( c != ce )
        {

          const gkg::Cell& cell = **c;

          // writing the cell
          itemW_Cell.write( os, cell );

          ++ c;

        }

        ++ p;

      }

    }

    os.close();

  }
  GKG_CATCH( "void gkg::MedusaGeometryDiskFormat::write( "
             "const std::string& name, "
             "gkg::ParticleMap& object, "
             "bool ascii ) const" );

}


bool gkg::MedusaGeometryDiskFormat::hasReader() const
{

  return true;

}


bool gkg::MedusaGeometryDiskFormat::hasWriter() const
{

  return true;

}


std::string gkg::MedusaGeometryDiskFormat::getHeaderExtension() const
{

  try
  {

    return std::string( ".medusageometry" ) + 
           gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

  }
  GKG_CATCH( "std::string "
             "gkg::MedusaGeometryDiskFormat::getHeaderExtension() const" );

}


//
// registrating MedusaGeometry disk format for "VirtualTissue" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::VirtualTissue  
                          >::getInstance().registerDiskFormat(
         gkg::MedusaGeometryDiskFormat::getInstance().getName(),
         &gkg::MedusaGeometryDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "MedusaGeometry disk format registration: " )

}


static bool initialized __attribute__((unused)) = initialize();
