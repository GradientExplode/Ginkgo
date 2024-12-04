#include <gkg-communication-command/Command.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <cstdlib>
#include <cstring>


gkg::Command::Command()
            : _argc( 0 ),
              _argv( 0 ),
              _loadPlugin( false )
{
}


gkg::Command::Command( int32_t argc, char* argv[], 
                       bool loadPlugin, bool removeFirst )
            : _argc( 0 ),
              _argv( 0 ),
              _loadPlugin( loadPlugin )
{

  try
  {

    filter( argc, argv, removeFirst );

  }
  GKG_CATCH( "gkg::Command::Command( int32_t argc, char* argv[] )" );

}


gkg::Command::Command( const gkg::Dictionary& /*parameters*/ )
            : _argc( 0 ),
              _argv( 0 ),
              _loadPlugin( false )
{
}


gkg::Command::~Command()
{

  clear();

}


void gkg::Command::clear()
{

  try
  {

    if ( _argc && _argv )
    {

      int32_t i = _argc;

      for ( i = 0; i < _argc; i++ )
      {

        if ( _argv[ i ] )
        {

          std::free( _argv[ i ] );

        }

      }

      std::free( _argv );

      _argv = 0;
      _argc = 0;

    }

  }
  GKG_CATCH( "gkg::Command::clear()" );

}


void gkg::Command::filter( int32_t argc, char* argv[], bool removeFirst )
{

  try
  {

    int32_t i = removeFirst ? 1 : 0;
    int32_t j = 0;

    clear();

    if ( argc < 2 )
    {

      _argc = argc;
      i = 0;

    }
    else
    {

      _argc = argc - i;

    }

    _argv = (char**)::calloc( _argc, sizeof( char* ) );

    if ( i )
    {

      size_t length = ::strlen( argv[ i ] ) + 4;

      _argv[ j ] = (char*)::calloc( length, sizeof( char ) );
      ::strcpy( _argv[ j ], "Gkg\0" );
      ::strcat( _argv[ j ], argv[ i ] );
      i++;
      j++;

    }

    while ( i < argc )
    {

      _argv[ j ] = ::strdup( argv[ i ] );
      i++;
      j++;

    }

  }
  GKG_CATCH( "void gkg::Command::filter( int32_t argc, char* argv[] )" );

}


void gkg::Command::getString( const gkg::Dictionary& parameters,
                              const std::string& parameterName,
                              std::string& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {

      result = p->second->getString();

    }

  }
  GKG_CATCH( "void gkg::Command::getString( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "std::string& result ) const" );

}


void gkg::Command::getBoolean( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               bool& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {

      result = ( bool )p->second->getValue< int32_t >();

    }

  }
  GKG_CATCH( "void gkg::Command::getBoolean( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "bool& result ) const" );

}


template < class T >
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               T& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {

      result = ( T )p->second->getValue< int32_t >();

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Command::getInteger( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "T& result ) const" );

}


template < class T >
void gkg::Command::getFloating( const gkg::Dictionary& parameters,
                                const std::string& parameterName,
                                T& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {

      result = ( T )p->second->getScalar();

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Command::getFloating( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "T& result ) const" );

}


void gkg::Command::getVectorOfStrings( const gkg::Dictionary& parameters,
                                       const std::string& parameterName,
                                       std::vector< std::string >& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {    

      try
      {

        gkg::GenericObjectList 
          items = p->second->getValue< gkg::GenericObjectList >();
      
        int32_t itemCount = ( int32_t )items.size();

        result.resize( itemCount );
        int32_t i = 0;
        for ( i = 0; i < itemCount; i++ )
        {

          result[ i ] = items[ i ]->getString();

        }

      }
      catch( std::exception& )
      {

        result.resize( 1U );
        result[ 0 ] = p->second->getString();

      }

    }

  }
  GKG_CATCH( "void gkg::Command::getVectorOfStrings( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "std::vector< std::string >& result ) const" );

}


template < class T >
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< T >& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {    

      try
      {

       gkg::GenericObjectList
         items = p->second->getValue< gkg::GenericObjectList >();
 
       int32_t itemCount = ( int32_t )items.size();

       result.resize( itemCount );
       int32_t i = 0;
       for ( i = 0; i < itemCount; i++ )
       {

         result[ i ] = ( T )( items[ i ]->getScalar() + 0.5 );

       }

      }
      catch( std::exception& )
      {

        result.resize( 1U );
        result[ 0 ] = ( T )( p->second->getScalar() + 0.5 );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Command::getVectorOfIntegers( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "std::vector< T >& result ) const" );

}


template < class T >
void gkg::Command::getVectorOfFloatings( const gkg::Dictionary& parameters,
                                         const std::string& parameterName,
                                         std::vector< T >& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {    

      try
      {

        gkg::GenericObjectList
          items = p->second->getValue< gkg::GenericObjectList >();
 
        int32_t itemCount = ( int32_t )items.size();

        result.resize( itemCount );
        int32_t i = 0;
        for ( i = 0; i < itemCount; i++ )
        {

          result[ i ] = ( T )items[ i ]->getScalar();

        }

      }
      catch( std::exception& )
      {

        result.resize( 1U );
        result[ 0 ] = ( T )p->second->getScalar();

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Command::getVectorOfFloatings( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "std::vector< T >& result ) const" );

}


void gkg::Command::getListOfStrings( const gkg::Dictionary& parameters,
                                     const std::string& parameterName,
                                     std::list< std::string >& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {    

      try
      {

        gkg::GenericObjectList
          items = p->second->getValue< gkg::GenericObjectList >();
 
        int32_t itemCount = ( int32_t )items.size();

        result.clear();
        int32_t i = 0;
        for ( i = 0; i < itemCount; i++ )
        {

          result.push_back( items[ i ]->getString() );

        }

      }
      catch( std::exception& )
      {

        result.push_back( p->second->getString() );

      }

    }

  }
  GKG_CATCH( "void gkg::Command::getVectorOfStrings( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "std::list< std::string >& result ) const" );

}


template < class T >
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< T >& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {    

      try
      {

        gkg::GenericObjectList 
          items = p->second->getValue< gkg::GenericObjectList >();
      
        int32_t itemCount = ( int32_t )items.size();

        result.clear();
        int32_t i = 0;
        for ( i = 0; i < itemCount; i++ )
        {

          result.push_back( ( T )( items[ i ]->getScalar() + 0.5 ));

        }

      }
      catch( std::exception& )
      {

        result.push_back( ( T )( p->second->getScalar() + 0.5 ));

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Command::getVectorOfIntegers( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "std::list< T >& result ) const" );

}


template < class T >
void gkg::Command::getListOfFloatings( const gkg::Dictionary& parameters,
                                       const std::string& parameterName,
                                       std::list< T >& result ) const
{

  try
  {

    gkg::Dictionary::const_iterator p = parameters.find( parameterName );
    if ( p != parameters.end() )
    {    

      try
      {


        gkg::GenericObjectList 
          items = p->second->getValue< gkg::GenericObjectList >();
      
        int32_t itemCount = ( int32_t )items.size();

        result.clear();
        int32_t i = 0;
        for ( i = 0; i < itemCount; i++ )
        {

          result.push_back( ( T )items[ i ]->getScalar() );

        }

      }
      catch( std::exception& )
      {

        result.push_back( ( T )p->second->getScalar() );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Command::getVectorOfFloatings( "
             "const gkg::Dictionary& parameters, "
             "const std::string& parameterName, "
             "std::list< T >& result ) const" );

}


//
// forcing instanciation of getInteger() methods
//

template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               int8_t& result ) const;
template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               uint8_t& result ) const;
template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               int16_t& result ) const;
template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               uint16_t& result ) const;
template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               int32_t& result ) const;
template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               uint32_t& result ) const;
template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               int64_t& result ) const;
template 
void gkg::Command::getInteger( const gkg::Dictionary& parameters,
                               const std::string& parameterName,
                               uint64_t& result ) const;


//
// forcing instanciation of getInteger() methods
//

template 
void gkg::Command::getFloating( const gkg::Dictionary& parameters,
                                const std::string& parameterName,
                                float& result ) const;
template 
void gkg::Command::getFloating( const gkg::Dictionary& parameters,
                                const std::string& parameterName,
                                double& result ) const;


//
// forcing instanciation of getVectorOfIntegers() methods
//

template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< int8_t >& result ) const;
template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< uint8_t >& result ) const;
template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< int16_t >& result ) const;
template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< uint16_t >& result ) const;
template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< int32_t >& result ) const;
template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< uint32_t >& result ) const;
template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< int64_t >& result ) const;
template 
void gkg::Command::getVectorOfIntegers( const gkg::Dictionary& parameters,
                                        const std::string& parameterName,
                                        std::vector< uint64_t >& result ) const;


//
// forcing instanciation of getVectorOfFloatings() methods
//

template 
void gkg::Command::getVectorOfFloatings( const gkg::Dictionary& parameters,
                                         const std::string& parameterName,
                                         std::vector< float >& result ) const;
template 
void gkg::Command::getVectorOfFloatings( const gkg::Dictionary& parameters,
                                         const std::string& parameterName,
                                         std::vector< double >& result ) const;


//
// forcing instanciation of getListOfIntegers() methods
//

template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< int8_t >& result ) const;
template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< uint8_t >& result ) const;
template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< int16_t >& result ) const;
template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< uint16_t >& result ) const;
template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< int32_t >& result ) const;
template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< uint32_t >& result ) const;
template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< int64_t >& result ) const;
template 
void gkg::Command::getListOfIntegers( const gkg::Dictionary& parameters,
                                      const std::string& parameterName,
                                      std::list< uint64_t >& result ) const;


//
// forcing instanciation of getListOfFloatings() methods
//

template 
void gkg::Command::getListOfFloatings( const gkg::Dictionary& parameters,
                                       const std::string& parameterName,
                                       std::list< float >& result ) const;
template 
void gkg::Command::getListOfFloatings( const gkg::Dictionary& parameters,
                                       const std::string& parameterName,
                                       std::list< double >& result ) const;
