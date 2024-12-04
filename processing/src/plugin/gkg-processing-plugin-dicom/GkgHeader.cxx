#include <gkg-processing-plugin-dicom/GkgHeader.h>
#include <gkg-core-object/GenericObject_i.h>


gkg::GkgHeader::GkgHeader( gkg::HeaderedObject& header )
              : pdcm::HeaderProxy(),
                _header( header.getHeader() )
{
}


void gkg::GkgHeader::addAttribute( const std::string& name, int32_t value )
{

  _header.addAttribute( name, value );

}


void gkg::GkgHeader::addAttribute( const std::string& name, double value )
{

  _header.addAttribute( name, value );

}


void gkg::GkgHeader::addAttribute( const std::string& name, 
                                   const std::string& value )
{

  _header.addAttribute( name, value );

}


void gkg::GkgHeader::addAttribute( const std::string& name, 
                                   const std::vector< int32_t >& value )
{

  int32_t i = 0;
  gkg::GenericObjectList vec( value.size() );
  std::vector< int32_t >::const_iterator
    v = value.begin(),
    ve = value.end();

  while ( v != ve )
  {

    vec[ i ] = *v;
    ++v;
    i++;

  }

  _header.addAttribute( name, vec );

}


void gkg::GkgHeader::addAttribute( const std::string& name, 
                                   const std::vector< double >& value )
{

  int32_t i = 0;
  gkg::GenericObjectList vec( value.size() );
  std::vector< double >::const_iterator
    v = value.begin(),
    ve = value.end();

  while ( v != ve )
  {

    vec[ i ] = *v;
    ++v;
    i++;

  }

  _header.addAttribute( name, vec );

}


void gkg::GkgHeader::addAttribute( const std::string& name, 
                                   const std::vector< std::string >& value )
{

  int32_t i = 0;
  gkg::GenericObjectList vec( value.size() );
  std::vector< std::string >::const_iterator
    v = value.begin(),
    ve = value.end();

  while ( v != ve )
  {

    vec[ i ] = *v;
    ++v;
    i++;

  }

  _header.addAttribute( name, vec );

}


void gkg::GkgHeader::addAttribute(
                             const std::string& name, 
                             const std::vector< std::vector< double > >& value )
{

  int32_t i = 0;
  gkg::GenericObjectList vecvec( value.size() );
  std::vector< std::vector< double > >::const_iterator
    v = value.begin(),
    ve = value.end();

  while ( v != ve )
  {

    int32_t j = 0;
    gkg::GenericObjectList vec( v->size() );
    std::vector< double >::const_iterator
      d = v->begin(),
      de = v->end();

    while ( d != de )
    {

      vec[ j ] = *d;
      ++d;
      j++;

    }

    vecvec[ i ] = vec;
    ++v;
    i++;

  }

  _header.addAttribute( name, vecvec );

}


bool gkg::GkgHeader::hasAttribute( const std::string& name )
{

  return _header.hasAttribute( name );

}
