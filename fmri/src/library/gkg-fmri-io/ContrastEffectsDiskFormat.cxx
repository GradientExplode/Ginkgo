#include <gkg-fmri-io/ContrastEffectsDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-fmri-contrast/ContrastEffects.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::ContrastEffectsDiskFormat::ContrastEffectsDiskFormat()
                               : gkg::TypedDiskFormat< gkg::ContrastEffects >()
{

  this->_extensions.insert( getHeaderExtension() );

}


gkg::ContrastEffectsDiskFormat::~ContrastEffectsDiskFormat()
{
}


std::string gkg::ContrastEffectsDiskFormat::getName() const
{

  return "contrast_effects";

}


void gkg::ContrastEffectsDiskFormat::readHeader(
                                             const std::string& name,
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
    if ( objectType != "ContrastEffects" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "void gkg::ContrastEffectsDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


void gkg::ContrastEffectsDiskFormat::writeHeader(
                                   const std::string& /*name*/,
                                   const gkg::HeaderedObject& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "void gkg::ContrastEffectsDiskFormat::writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


void gkg::ContrastEffectsDiskFormat::read( const std::string& name,
                                            gkg::ContrastEffects& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object );

    gkg::GenericObjectList contrastEffects;
    if ( object.getHeader().hasAttribute( "contrast_effects" ) )
    {

      object.getHeader().getAttribute( "contrast_effects", contrastEffects );

      gkg::GenericObjectList::const_iterator
        i = contrastEffects.begin(),
        ie = contrastEffects.end();
      while ( i != ie )
      {

        gkg::ContrastEffects::Information information;

        gkg::Dictionary contrastEffect = ( *i )->getValue< gkg::Dictionary >();

        // getting identifier
        if ( contrastEffect.find( "name" ) != contrastEffect.end() )
        {

          information.name = contrastEffect[ "name" ]->getString();

        }
        else
        {

          throw std::runtime_error( "missing 'name' field" );

        }

        // collecting regressor names
        if ( contrastEffect.find( "regressor_names" ) != contrastEffect.end() )
        {

          gkg::GenericObjectList
            tmp = contrastEffect[ "regressor_names" ]->getValue<
                                                     gkg::GenericObjectList >();
          information.regressorNames.resize( tmp.size() );
          int32_t r;
          for ( r = 0; r < ( int32_t )information.regressorNames.size(); r++ )
          {

            information.regressorNames[ r ] = tmp[ r ]->getString();

          }

        }
        else
        {

          throw std::runtime_error( "missing 'regressor_names' field" );

        }

        // collecting corresponding weights
        if ( contrastEffect.find( "weights" ) != contrastEffect.end() )
        {

          gkg::GenericObjectList
            tmp = contrastEffect[ "weights" ]->getValue<
                                                     gkg::GenericObjectList >();
          information.weights.reallocate( ( int32_t )tmp.size() );
          int32_t w;
          for ( w = 0; w < information.weights.getSize(); w++ )
          {

            information.weights( w ) = tmp[ w ]->getScalar();

          }

        }
        else
        {

          throw std::runtime_error( "missing 'weights' field" );

        }

        // adding contrast effect
        object.addContrastEffect( information );

        ++ i;

      }

    }
    else
    {

      std::runtime_error( "missing 'contrast_effects' attribute" );

    }

  }
  GKG_CATCH( "void gkg::ContrastEffectsDiskFormat::read( "
             "const std::string& name, "
             "gkg::ContrastEffects& object ) const" );

}


void gkg::ContrastEffectsDiskFormat::write( const std::string& /*name*/,
                                            gkg::ContrastEffects& /*object*/,
                                            bool /*ascii*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "void gkg::ContrastEffectsDiskFormat::write( "
             "const std::string& name, "
             "gkg::ContrastEffects& object, "
             "bool ascii ) const" );

}


bool gkg::ContrastEffectsDiskFormat::hasReader() const
{

  return true;

}


bool gkg::ContrastEffectsDiskFormat::hasWriter() const
{

  return false;

}


std::string 
gkg::ContrastEffectsDiskFormat::getHeaderExtension() const
{

  return std::string( ".contrasts" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


//
// registrating standard ContrastEffects disk format(s) for "ContrastEffects"
// object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::ContrastEffects 
                          >::getInstance().registerDiskFormat(
         gkg::ContrastEffectsDiskFormat::getInstance().getName(),
         &gkg::ContrastEffectsDiskFormat::getInstance() );

    return true;

  }
  GKG_CATCH( "ContrastEffects disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
