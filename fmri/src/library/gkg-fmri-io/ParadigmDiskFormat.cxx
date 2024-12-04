#include <gkg-fmri-io/ParadigmDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-fmri-paradigm/Paradigm.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::ParadigmDiskFormat::ParadigmDiskFormat()
                        : gkg::TypedDiskFormat< gkg::Paradigm >()
{

  this->_extensions.insert( getHeaderExtension() );

}


gkg::ParadigmDiskFormat::~ParadigmDiskFormat()
{
}


std::string gkg::ParadigmDiskFormat::getName() const
{

  return "paradigm";

}


void gkg::ParadigmDiskFormat::readHeader( const std::string& name,
                                          gkg::HeaderedObject& object ) const
{

  try
  {

    // reading the meta-information header
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

    // checking that it is a site map
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "Paradigm" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "void gkg::ParadigmDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::ParadigmDiskFormat::writeHeader(
                                   const std::string& /*name*/,
                                   const gkg::HeaderedObject& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "void gkg::ParadigmDiskFormat::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


void gkg::ParadigmDiskFormat::read( const std::string& name,
                                    gkg::Paradigm& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object );


    gkg::GenericObjectList stimuli;
    if ( object.getHeader().hasAttribute( "stimuli" ) )
    {

      object.getHeader().getAttribute( "stimuli", stimuli );

      gkg::GenericObjectList::const_iterator
        i = stimuli.begin(),
        ie = stimuli.end();
      while ( i != ie )
      {

        gkg::Dictionary stimulus = ( *i )->getValue< gkg::Dictionary >();

        // getting identifier
        std::string name;
        if ( stimulus.find( "name" ) != stimulus.end() )
        {

          name = stimulus[ "name" ]->getString();
 
        }
        else
        {

          throw std::runtime_error( "missing 'name' field" );

        }

        // getting type
        std::string type;
        if ( stimulus.find( "type" ) != stimulus.end() )
        {

          type = stimulus[ "type" ]->getValue< std::string >();

        }
        else
        {

          throw std::runtime_error( "missing 'type' field" );

        }

        if ( type == "discrete_stimulus" )
        {

          // collecting times
          gkg::Vector times;
          if ( stimulus.find( "times" ) != stimulus.end() )
          {

            gkg::GenericObjectList 
              tmp = stimulus[ "times" ]->getValue< gkg::GenericObjectList >();
            times.reallocate( ( int32_t )tmp.size() );
            int32_t t;
            for ( t = 0; t < times.getSize(); t++ )
            {

              times( t ) = tmp[ t ]->getScalar();

            }

          }
          else
          {

            throw std::runtime_error( "missing 'times' field" );

          }

          // collecting durations
          gkg::Vector durations;
          if ( stimulus.find( "durations" ) != stimulus.end() )
          {

            gkg::GenericObjectList 
              tmp = stimulus[ "durations" ]->getValue< gkg::GenericObjectList >(
                                                                              );
            durations.reallocate( ( int32_t )tmp.size() );
            int32_t t;
            for ( t = 0; t < durations.getSize(); t++ )
            {

              durations( t ) = tmp[ t ]->getScalar();

            }

          }
          else
          {

            throw std::runtime_error( "missing 'durations' field" );

          }

          // collecting magnitudes
          gkg::Vector magnitudes;
          if ( stimulus.find( "magnitudes" ) != stimulus.end() )
          {

            gkg::GenericObjectList 
              tmp = stimulus[ "magnitudes" ]->getValue<
                                                     gkg::GenericObjectList >();
            magnitudes.reallocate( ( int32_t )tmp.size() );
            int32_t t;
            for ( t = 0; t < magnitudes.getSize(); t++ )
            {

              magnitudes( t ) = tmp[ t ]->getScalar();

            }

          }
          else
          {

            throw std::runtime_error( "missing 'magnitudes' field" );

          }

          // adding discrete stimulus to paradigm
          object.addDiscreteStimulus( name,
                                      times,
                                      durations,
                                      magnitudes );

        }
        else if ( type == "continuous_stimulus" )
        {

          // collecting starting time
          double startingTime;
          if ( stimulus.find( "starting_time" ) != stimulus.end() )
          {

            startingTime = stimulus[ "starting_time" ]->getValue< double >();

          }
          else
          {

            throw std::runtime_error( "missing 'starting_time' field" );

          }

          // collecting sampling period
          double samplingPeriod;
          if ( stimulus.find( "sampling_period" ) != stimulus.end() )
          {

            samplingPeriod = stimulus[ "sampling_period" ]->getValue< double>();

          }
          else
          {

            throw std::runtime_error( "missing 'sampling_period' field" );

          }

          // collecting magnitudess
          gkg::Vector magnitudes;
          if ( stimulus.find( "magnitudes" ) != stimulus.end() )
          {

            gkg::GenericObjectList 
              tmp = stimulus[ "magnitudes" ]->getValue<
                                                     gkg::GenericObjectList >();
            magnitudes.reallocate( ( int32_t )tmp.size() );
            int32_t t;
            for ( t = 0; t < magnitudes.getSize(); t++ )
            {

              magnitudes( t ) = tmp[ t ]->getScalar();

            }

          }
          
          // adding continuous stimulus to paradigm
          object.addContinuousStimulus( name,
                                        startingTime,
                                        samplingPeriod,
                                        magnitudes );

        }
        else
        {

          throw std::runtime_error( "unknown stimulus type" );

        }

        ++ i;

      }

    }
    else
    {

      std::runtime_error( "missing 'stimuli' attribute" );

    }

  }
  GKG_CATCH( "void gkg::ParadigmDiskFormat::read( "
             "const std::string& name, "
             "gkg::Paradigm& object ) const" );

}


void gkg::ParadigmDiskFormat::write( const std::string& /*name*/,
                                     gkg::Paradigm& /*object*/,
                                     bool /*ascii*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "void gkg::ParadigmDiskFormat::write( "
             "const std::string& name, "
             "gkg::Paradigm& object, "
             "bool ascii ) const" );

}


bool gkg::ParadigmDiskFormat::hasReader() const
{

  return true;

}


bool gkg::ParadigmDiskFormat::hasWriter() const
{

  return false;

}


std::string 
gkg::ParadigmDiskFormat::getHeaderExtension() const
{

  return std::string( ".paradigm" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


//
// registrating standard Paradigm disk format(s) for "Paradigm" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Paradigm 
                          >::getInstance().registerDiskFormat(
         gkg::ParadigmDiskFormat::getInstance().getName(),
         &gkg::ParadigmDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "Paradigm disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
