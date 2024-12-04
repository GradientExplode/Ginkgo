#include <gkg-processing-container/DictionaryHelpers.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


//
// for string parameters
//

std::string gkg::getStringValue( const gkg::Dictionary& dictionary,
                             const std::string& key,
                             bool isRequired,
                             const std::string& defaultValue )
{

  try
  {

    std::string value = defaultValue;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      value = i->second->getString();

    }
    return value;

  }
  GKG_CATCH( "std::string gkg::getIntegerParameter( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "bool isRequired, "
             "const std::string& defaultValue )" );

}


//
// for scalar int32_t / float / double parameters
//

int32_t gkg::getIntegerValue( const gkg::Dictionary& dictionary,
                              const std::string& key,
                              bool isRequired,
                              int32_t defaultValue )
{

  try
  {

    int32_t value = defaultValue;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      value = ( int32_t )( i->second->getScalar() + 0.5 );

    }
    return value;

  }
  GKG_CATCH( "int32_t gkg::getIntegerParameter( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "bool isRequired, "
             "int32_t defaultValue )" );

}


float gkg::getFloatValue( const gkg::Dictionary& dictionary,
                          const std::string& key,
                          bool isRequired,
                          float defaultValue )
{

  try
  {

    float value = defaultValue;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      value = ( float )i->second->getScalar();

    }
    return value;

  }
  GKG_CATCH( "float gkg::getFloatValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "bool isRequired, "
             "float defaultValue )" );

}


double gkg::getDoubleValue( const gkg::Dictionary& dictionary,
                            const std::string& key,
                            bool isRequired,
                            double defaultValue )
{

  try
  {

    double value = defaultValue;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      value = i->second->getScalar();

    }
    return value;

  }
  GKG_CATCH( "double gkg::getDoubleValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "bool isRequired, "
             "double defaultValue )" );

}


//
// for Vector3d< int32_t / float / double >
//

gkg::Vector3d< int32_t >
gkg::getVector3dOfIntegerValue( const gkg::Dictionary& dictionary,
                                const std::string& key,
                                bool isRequired,
                                const gkg::Vector3d< int32_t >& defaultValue )
{

  try
  {

    gkg::Vector3d< int32_t > value = defaultValue;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( gol.size() != 3U )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have 3 sub-items" );

      }

      value.x = ( int32_t )( gol[ 0 ]->getScalar() + 0.5 );
      value.y = ( int32_t )( gol[ 1 ]->getScalar() + 0.5 );
      value.z = ( int32_t )( gol[ 2 ]->getScalar() + 0.5 );

    }
    return value;

  }
  GKG_CATCH( "gkg::Vector3d< int32_t > gkg::getVector3dOfIntegerValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "bool isRequired, "
             "const gkg::Vector3d< int32_t >& defaultValue )" );

}


gkg::Vector3d< float >
gkg::getVector3dOfFloatValue( const gkg::Dictionary& dictionary,
                              const std::string& key,
                              bool isRequired,
                              const gkg::Vector3d< float >& defaultValue )
{

  try
  {

    gkg::Vector3d< float > value = defaultValue;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( gol.size() != 3U )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have 3 sub-items" );

      }

      value.x = ( float )gol[ 0 ]->getScalar();
      value.y = ( float )gol[ 1 ]->getScalar();
      value.z = ( float )gol[ 2 ]->getScalar();

    }
    return value;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::getVector3dOfFloatValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "bool isRequired, "
             "const gkg::Vector3d< float >& defaultValue )" );

}


gkg::Vector3d< double >
gkg::getVector3dOfDoubleValue( const gkg::Dictionary& dictionary,
                               const std::string& key,
                               bool isRequired,
                               const gkg::Vector3d< double >& defaultValue )
{

  try
  {

    gkg::Vector3d< double > value = defaultValue;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( gol.size() != 3U )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have 3 sub-items" );

      }

      value.x = gol[ 0 ]->getScalar();
      value.y = gol[ 1 ]->getScalar();
      value.z = gol[ 2 ]->getScalar();

    }
    return value;

  }
  GKG_CATCH( "gkg::Vector3d< double > gkg::getVector3dOfDoubleValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "const gkg::Vector3d< double >& defaultValue, "
             "bool isRequired )" );

}


//
// for std::vector< int32_t / float / double >
//

std::vector< int32_t >
gkg::getVectorOfIntegerValue( const gkg::Dictionary& dictionary,
                              const std::string& key,
                              int32_t expectedItemCount )
{

  try
  {

    std::vector< int32_t > value;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( expectedItemCount >= 1 )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( ( expectedItemCount >= 1 ) &&
           ( ( int32_t ) gol.size() != expectedItemCount ) )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have " +
                  gkg::StringConverter::toString( expectedItemCount ) +
                  " sub-items" );

      }

      int32_t itemCount = ( int32_t )gol.size();
      value.resize( itemCount );

      int32_t index = 0;
      for ( index = 0; index < itemCount; index++ )
      {
      
        value[ index ] = ( int32_t )( gol[ index ]->getScalar() + 0.5 );

      }

    }
    return value;

  }
  GKG_CATCH( "std::vector< int32_t > gkg::getVectorOfIntegerValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "int32_t expectedItemCount )" );

}


std::vector< float >
gkg::getVectorOfFloatValue( const gkg::Dictionary& dictionary,
                            const std::string& key,
                            int32_t expectedItemCount )
{

  try
  {

    std::vector< float > value;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( expectedItemCount >= 1 )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( ( expectedItemCount >= 1 ) &&
           ( ( int32_t ) gol.size() != expectedItemCount ) )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have " +
                  gkg::StringConverter::toString( expectedItemCount ) +
                  " sub-items" );

      }

      int32_t itemCount = ( int32_t )gol.size();
      value.resize( itemCount );

      int32_t index = 0;
      for ( index = 0; index < itemCount; index++ )
      {
      
        value[ index ] = ( float )gol[ index ]->getScalar();

      }

    }
    return value;

  }
  GKG_CATCH( "std::vector< float > gkg::getVectorOfFloatValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "int32_t expectedItemCount )" );

}


std::vector< double >
gkg::getVectorOfDoubleValue( const gkg::Dictionary& dictionary,
                             const std::string& key,
                             int32_t expectedItemCount )
{

  try
  {

    std::vector< double > value;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( expectedItemCount >= 1 )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( ( expectedItemCount >= 1 ) &&
           ( ( int32_t ) gol.size() != expectedItemCount ) )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have " +
                  gkg::StringConverter::toString( expectedItemCount ) +
                  " sub-items" );

      }

      int32_t itemCount = ( int32_t )gol.size();
      value.resize( itemCount );

      int32_t index = 0;
      for ( index = 0; index < itemCount; index++ )
      {
      
        value[ index ] = gol[ index ]->getScalar();

      }

    }
    return value;

  }
  GKG_CATCH( "std::vector< double > gkg::getVectorOfDoubleValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "int32_t expectedItemCount )" );

}


//
// for std::vector< Vector3d< int32_t / float / double > >
//

std::vector< gkg::Vector3d< int32_t > >
gkg::getVectorOfVector3dOfIntegerValue( const gkg::Dictionary& dictionary,
                                        const std::string& key,
                                        int32_t expectedItemCount )
{

  try
  {

    std::vector< gkg::Vector3d< int32_t > > value;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( expectedItemCount >= 1 )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( ( expectedItemCount >= 1 ) &&
           ( ( int32_t ) gol.size() != expectedItemCount ) )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have " +
                  gkg::StringConverter::toString( expectedItemCount ) +
                  " sub-items" );

      }

      int32_t itemCount = ( int32_t )gol.size();
      value.resize( itemCount );

      int32_t index = 0;
      for ( index = 0; index < itemCount; index++ )
      {

        gkg::GenericObjectList xyz =
                             gol[ index ]->getValue< gkg::GenericObjectList >();

        if ( xyz.size() != 3U )
        {

          throw std::runtime_error(
                                   "vector of 3 coordinates x, y, z required" );

        }
      
        value[ index ].x = ( int32_t )( xyz[ 0 ]->getScalar() + 0.5 );
        value[ index ].y = ( int32_t )( xyz[ 1 ]->getScalar() + 0.5 );
        value[ index ].z = ( int32_t )( xyz[ 2 ]->getScalar() + 0.5 );

      }

    }
    return value;

  }
  GKG_CATCH( "std::vector< gkg::Vector3d< int32_t > > "
             "gkg::getVectorOfVector3dOfIntegerValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "int32_t expectedItemCount )" );

}


std::vector< gkg::Vector3d< float > >
gkg::getVectorOfVector3dOfFloatValue( const gkg::Dictionary& dictionary,
                                      const std::string& key,
                                      int32_t expectedItemCount )
{

  try
  {

    std::vector< gkg::Vector3d< float > > value;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( expectedItemCount >= 1 )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( ( expectedItemCount >= 1 ) &&
           ( ( int32_t ) gol.size() != expectedItemCount ) )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have " +
                  gkg::StringConverter::toString( expectedItemCount ) +
                  " sub-items" );

      }

      int32_t itemCount = ( int32_t )gol.size();
      value.resize( itemCount );

      int32_t index = 0;
      for ( index = 0; index < itemCount; index++ )
      {

        gkg::GenericObjectList xyz =
                             gol[ index ]->getValue< gkg::GenericObjectList >();

        if ( xyz.size() != 3U )
        {

          throw std::runtime_error(
                                   "vector of 3 coordinates x, y, z required" );

        }
      
        value[ index ].x = ( float )xyz[ 0 ]->getScalar();
        value[ index ].y = ( float )xyz[ 1 ]->getScalar();
        value[ index ].z = ( float )xyz[ 2 ]->getScalar();

      }

    }
    return value;

  }
  GKG_CATCH( "std::vector< gkg::Vector3d< float > > "
             "gkg::getVectorOfVector3dOfFloatValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "int32_t expectedItemCount )" );

}


std::vector< gkg::Vector3d< double > >
gkg::getVectorOfVector3dOfDoubleValue( const gkg::Dictionary& dictionary,
                                       const std::string& key,
                                       int32_t expectedItemCount )
{

  try
  {

    std::vector< gkg::Vector3d< double > > value;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( expectedItemCount >= 1 )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      gkg::GenericObjectList 
        gol = i->second->getValue< gkg::GenericObjectList >();

      if ( ( expectedItemCount >= 1 ) &&
           ( ( int32_t ) gol.size() != expectedItemCount ) )
      {

        throw std::runtime_error(
                  std::string( "a '" ) + key + "' item must have " +
                  gkg::StringConverter::toString( expectedItemCount ) +
                  " sub-items" );

      }

      int32_t itemCount = ( int32_t )gol.size();
      value.resize( itemCount );

      int32_t index = 0;
      for ( index = 0; index < itemCount; index++ )
      {

        gkg::GenericObjectList xyz =
                             gol[ index ]->getValue< gkg::GenericObjectList >();

        if ( xyz.size() != 3U )
        {

          throw std::runtime_error(
                                   "vector of 3 coordinates x, y, z required" );

        }
      
        value[ index ].x = xyz[ 0 ]->getScalar();
        value[ index ].y = xyz[ 1 ]->getScalar();
        value[ index ].z = xyz[ 2 ]->getScalar();

      }

    }
    return value;

  }
  GKG_CATCH( "std::vector< gkg::Vector3d< double > > "
             "gkg::getVectorOfVector3dOfDoubleValue( "
             "const gkg::Dictionary& dictonary, "
             "const std::string& key, "
             "int32_t expectedItemCount )" );

}


//
// for Dictionary
//

gkg::Dictionary gkg::getDictionaryValue( const gkg::Dictionary& dictionary,
                                         const std::string& key,
                                         bool isRequired )
{

  try
  {

    gkg::Dictionary value;

    gkg::Dictionary::const_iterator i = dictionary.find( key );


    if ( i == dictionary.end() )
    {

      if ( isRequired )
      {

        throw std::runtime_error( 
                            std::string( "a '" ) + key + "' item is required" );

      }

    }
    else
    {

      value = i->second->getValue< gkg::Dictionary >();

    }
    return value;

  }
  GKG_CATCH( "gkg::Dictionary getDictionaryValue( "
             "const gkg::Dictionary& dictionary, "
             "const std::string& key, "
             "bool isRequired )" );

}
