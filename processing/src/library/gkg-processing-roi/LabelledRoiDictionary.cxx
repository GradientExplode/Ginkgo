#include <gkg-processing-roi/LabelledRoiDictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>


gkg::LabelledRoiDictionary::LabelledRoiDictionary( const std::string& name )
                           : gkg::HeaderedObject(),
                             _name( name )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", 
                          std::string( "LabelledRoiDictionary" ) );


  }
  GKG_CATCH( "gkg::LabelledRoiDictionary::LabelledRoiDictionary( "
             "const std::string& name )" );

}

gkg::LabelledRoiDictionary::LabelledRoiDictionary( 
                                       const gkg::LabelledRoiDictionary& other )
                           : gkg::HeaderedObject( other ),
                             _name( other._name )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", 
                          std::string( "LabelledRoiDictionary" ) );

  }
  GKG_CATCH( "gkg::LabelledRoiDictionary::LabelledRoiDictionary( "
             "const gkg::LabelledRoiDictionary& other )" );

}


gkg::LabelledRoiDictionary::LabelledRoiDictionary(
                                      const gkg::HeaderedObject& headeredObject,
                                      const std::string& name )
                           : gkg::HeaderedObject(),
                             _name( name )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", 
                           std::string( "LabelledRoiDictionary" ) );

    if ( headeredObject.getHeader().hasAttribute( _name ) )
    {

      gkg::Dictionary dictionary;
      headeredObject.getHeader().getAttribute( _name, dictionary );
      _header.addAttribute( _name, dictionary );

    }
    else
    {

      throw std::runtime_error( std::string( "no dictionary '" ) +
                                _name + "' present in the header" );

    }

  }
  GKG_CATCH( "gkg::LabelledRoiDictionary::LabelledRoiDictionary( "
             "const gkg::HeaderedObject& headeredObject,"
             "const std::string& dictionaryName )" );

}

gkg::LabelledRoiDictionary::LabelledRoiDictionary(
                                             const gkg::Dictionary& dictionary,
                                             const std::string& name )
                           : gkg::HeaderedObject(),
                             _name( name )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", 
                          std::string( "LabelledRoiDictionary" ) );
    _header.addAttribute( name, dictionary );

  }
  GKG_CATCH( "gkg::LabelledRoiDictionary::LabelledRoiDictionary( "
             "const gkg::Dictionary& dictionary, "
             "const std::string& dictionaryName )" );

}


gkg::LabelledRoiDictionary::~LabelledRoiDictionary()
{
}


gkg::LabelledRoiDictionary& 
gkg::LabelledRoiDictionary::operator=( const gkg::LabelledRoiDictionary& other )
{

   try
   {

    this->HeaderedObject::operator=( other );
     return *this; 

   } 
   GKG_CATCH( "gkg::LabelledRoiDictionary&"
              "gkg::LabelledRoiDictionary::operator=(" 
              "const gkg::LabelledRoiDictionary& other )" );

}


std::string gkg::LabelledRoiDictionary::getName() const
{

  return _name;

}


gkg::Dictionary gkg::LabelledRoiDictionary::getDictionary() const
{

  try
  {

    gkg::Dictionary dictionary;
    if ( _header.hasAttribute( _name ) )
    {

      _header.getAttribute( _name, dictionary );

    }
    else
    {

      throw std::runtime_error( std::string( "no dictionary '" ) +
                                _name + "' present in the header" );

    }

    return dictionary;

  }
  GKG_CATCH( "gkg::Dictionary "
             "gkg::LabelledRoiDictionary::getDictionary() const" );

}


int32_t gkg::LabelledRoiDictionary::getRoiCount() const
{

  try
  {

    return ( int32_t )getDictionary().size();

  }
  GKG_CATCH( "int32_t gkg::LabelledRoiDictionary::getRoiCount() const" );

}

gkg::Dictionary
gkg::LabelledRoiDictionary::getRoiDictionary( const std::string& roiKey ) const
{

   try
   {

      gkg::Dictionary dictionary = getDictionary();
      gkg::Dictionary::iterator i = dictionary.find( roiKey );

      if ( i != dictionary.end() )
      {

        return i->second->getValue< gkg::Dictionary >();

      }
      else
      {

        throw std::runtime_error( "no ROI dictionary '" + roiKey + 
                                  "' present in the header" );

      }

   }
   GKG_CATCH( "gkg::Dictionary "
              "gkg::LabelledRoiDictionary::getRoiDictionary( "
	      "const std::string& roiKey ) const" );
}


std::string 
gkg::LabelledRoiDictionary::getRoiAcronym( const std::string& roiKey ) const
{

  try
  {

    return getRoiDictionary( roiKey )[ "acronym" ]->getString();

  }
  GKG_CATCH( "std::string gkg::LabelledRoiDictionary::getRoiAcronym( "
             "const std::string& roiKey ) const" );

}



template < class T >
void gkg::LabelledRoiDictionary::getRoiLabel( const std::string& roiKey,
                                              T& label ) const
{

  try
  {

    label = ( T )getRoiDictionary( roiKey )[ "label" ]->getScalar();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::LabelledRoiDictionary::getRoiLabel( "
             "const std::string& roiKey, T& label ) const" );

}


double gkg::LabelledRoiDictionary::getConfidenceLevel( 
                                               const std::string& roiKey ) const
{

  try
  {

    return getRoiDictionary( roiKey )[ "confidence_level" ]->getScalar();

  }
  GKG_CATCH( "double gkg::LabelledRoiDictionary::getConfidenceLevel( "
             "const std::string& roiKey ) const" );

}


template < class T >
void gkg::LabelledRoiDictionary::getAdjacencies(
                             std::set< std::pair< T, T > >& adjacencies ) const
{

  try
  {

    gkg::Dictionary dictionary = getDictionary();
    adjacencies.clear();

    gkg::Dictionary::iterator 
      r = dictionary.begin(),
      re = dictionary.end();

    gkg::GenericObjectList genericObjectList;

    while( r != re )
    {

      T label = ( T )0;
      getRoiLabel( r->first, label );
      gkg::Dictionary roiDictionary = r->second->getValue< gkg::Dictionary >();

      genericObjectList = roiDictionary.find( "adjacent_label" )->second
                                         ->getValue< gkg::GenericObjectList >();

      std::vector< T > adjacentRois( genericObjectList.size() );

      int32_t i;
      for( i = 0; i < ( int32_t )adjacentRois.size(); i++ )
      {

        adjacentRois[ i ] = ( T )genericObjectList[ i ]->getScalar();


      }

      typename std::vector< T >::iterator
        a = adjacentRois.begin(),
        ae = adjacentRois.end();
      while ( a != ae )
      {

        adjacencies.insert( std::make_pair( label, *a ) );

        ++ a;

      }

      ++ r;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::LabelledRoiDictionary::getAdjacencies( "
             "std::set< std::pair< T, T > >& adjacencies ) const" );

}


void gkg::LabelledRoiDictionary::addSemantic(
                                    gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    headeredObject.addSemantic( "__labelled_ROI_dictionary__",
                                _name,
                                gkg::Semantic( 
                                gkg::TypeOf< gkg::Dictionary >::getName() ) );

  }
  GKG_CATCH( "void gkg::LabelledRoiDictionary::addSemantic( "
             "gkg::HeaderedObject& headeredObject ) const" );
}


void gkg::LabelledRoiDictionary::addToHeaderedObject(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    addSemantic( headeredObject );
    if ( headeredObject.getHeader().hasAttribute( _name ) )
    {

      throw std::runtime_error( std::string( "'" ) + _name +
                                "' attribute already present" );

    }
    headeredObject.getHeader().addAttribute( _name, getDictionary() );

  }
  GKG_CATCH( "void gkg::LabelledRoiDictionary::addDictionary( "
             "const gkg::HeaderedObject& headeredObject ) const" );

}


//
// forcing instanciations
//

template void gkg::LabelledRoiDictionary::getRoiLabel< uint8_t >(
                const std::string& roiKey, uint8_t& label ) const;
template void gkg::LabelledRoiDictionary::getRoiLabel< int8_t >(
                const std::string& roiKey, int8_t& label ) const;
template void gkg::LabelledRoiDictionary::getRoiLabel< uint16_t >(
                const std::string& roiKey, uint16_t& label ) const;
template void gkg::LabelledRoiDictionary::getRoiLabel< int16_t >(
                const std::string& roiKey, int16_t& label ) const;
template void gkg::LabelledRoiDictionary::getRoiLabel< uint32_t >(
                const std::string& roiKey, uint32_t& label ) const;
template void gkg::LabelledRoiDictionary::getRoiLabel< int32_t >(
                const std::string& roiKey, int32_t& label ) const;
template void gkg::LabelledRoiDictionary::getRoiLabel< uint64_t >(
                const std::string& roiKey, uint64_t& label ) const;
template void gkg::LabelledRoiDictionary::getRoiLabel< int64_t >(
                const std::string& roiKey, int64_t& label ) const;


template void gkg::LabelledRoiDictionary::getAdjacencies< uint8_t >(
                std::set< std::pair< uint8_t, uint8_t > >& adjacencies ) const;
template void gkg::LabelledRoiDictionary::getAdjacencies< int8_t >(
                std::set< std::pair< int8_t, int8_t > >& adjacencies ) const;
template void gkg::LabelledRoiDictionary::getAdjacencies< uint16_t >(
                std::set< std::pair< uint16_t, uint16_t > >& adjacencies ) const;
template void gkg::LabelledRoiDictionary::getAdjacencies< int16_t >(
                std::set< std::pair< int16_t, int16_t > >& adjacencies ) const;
template void gkg::LabelledRoiDictionary::getAdjacencies< uint32_t >(
                std::set< std::pair< uint32_t, uint32_t > >& adjacencies ) const;
template void gkg::LabelledRoiDictionary::getAdjacencies< int32_t >(
                std::set< std::pair< int32_t, int32_t > >& adjacencies ) const;
template void gkg::LabelledRoiDictionary::getAdjacencies< uint64_t >(
                std::set< std::pair< uint64_t, uint64_t > >& adjacencies ) const;
template void gkg::LabelledRoiDictionary::getAdjacencies< int64_t >(
                std::set< std::pair< int64_t, int64_t > >& adjacencies ) const;
