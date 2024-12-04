#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/StyleImplementation.h>
#include <gkg-core-io/StringConverter.h>
#include <fstream>


gkg::Style::Style()
           : gkg::RCGUIObject()
{

  _styleImplementation = new gkg::StyleImplementation( "" );

}


gkg::Style::Style( const std::string& name )
           : gkg::RCGUIObject()
{

  _styleImplementation = new gkg::StyleImplementation( name );

}


gkg::Style::Style( gkg::Style* parent )
           : gkg::RCGUIObject()
{

  _styleImplementation = new gkg::StyleImplementation( "" );
  parent->append( this );

}


gkg::Style::Style( const std::string& name, gkg::Style* parent )
           : gkg::RCGUIObject()
{

  _styleImplementation = new gkg::StyleImplementation( name );
  parent->append( this );

}


gkg::Style::Style( const gkg::Style& other )
           : gkg::RCGUIObject()
{

  gkg::StyleImplementation* styleImpl = other.getImplementation();
  _styleImplementation = new gkg::StyleImplementation( styleImpl->getName() );
  if ( styleImpl->getParent() )
  {

    styleImpl->append( this );

  }
  styleImpl->update();

  int32_t n = styleImpl->getAliasCount();
  int32_t i;
  for (i = n - 1; i >= 0; i-- )
  {

    setAlias( other.getAlias( i ) );

  }

  n = other.getChildrenCount();
  for ( i = 0; i < n; i++ )
  {

    append( other.getChild( i ) );

  }

  n = other.getAttributeCount();
  for ( i = 0; i < n; i++ )
  {

    std::string name, value;
    if ( other.getAttribute( i, name, value ) )
    {

      addAttribute(  name, value );

    }

  }
  _styleImplementation->modify();

}


gkg::Style::~Style()
{

  gkg::Style* parent = _styleImplementation->getParent();
  if ( parent )
  {

    parent->remove( this );

  }
  delete _styleImplementation;

}


void gkg::Style::setName( const std::string& name )
{

  _styleImplementation->setName( name );

}


const std::string& gkg::Style::getName() const
{

  return _styleImplementation->getName();

}


void gkg::Style::setAlias( const std::string& name )
{

  _styleImplementation->setAlias( name );

}


int32_t gkg::Style::getAliasCount() const
{

  return _styleImplementation->getAliasCount();

}


std::string gkg::Style::getAlias( int32_t index ) const
{

  return _styleImplementation->getAlias( index );

}


gkg::Style* gkg::Style::getParent() const
{

  return _styleImplementation->getParent();

}


// Add a child style.  Implicitly remove the child from its current parent,
// if it has one, and set its parent to this.  Because reparenting may change
// (cached) attributes, we must mark the child modified.
void gkg::Style::append( gkg::Style* style )
{

  gkg::Style* parent = style->getParent();

  if ( parent == this )
  {

    return;

  }
  if ( parent )
  {

    parent->remove( style );

  }

  _styleImplementation->append( style );
  this->addReference();
  style->getImplementation()->setParent( this );
  style->getImplementation()->modify();

}


void gkg::Style::remove( gkg::Style* style )
{

  _styleImplementation->remove( style );
  style->getImplementation()->setParent( 0 );
  this->removeReference();

}


int32_t gkg::Style::getChildrenCount() const
{

  return _styleImplementation->getChildrenCount();

}


gkg::Style* gkg::Style::getChild( int32_t index ) const
{

  return _styleImplementation->getChild( index );

}


void gkg::Style::addAttribute( const std::string& name,
                               const std::string& value,
                               int32_t priority )
{

  _styleImplementation->addAttribute( name, value, priority );

}


void gkg::Style::removeAttribute( const std::string& name )
{

  _styleImplementation->removeAttribute( name );

}


int32_t gkg::Style::getAttributeCount() const
{

  return _styleImplementation->getAttributeCount();

}


bool gkg::Style::getAttribute( int32_t index, 
                               std::string& name,
                               std::string& value ) const
{

  return _styleImplementation->getAttribute( index, name, value );

}


void gkg::Style::loadFile( const std::string& filename, int32_t priority )
{

  std::ifstream is( filename.c_str() );

  if ( !is )
  {

    return;

  }

  is.seekg( 0, std::ios::end );
  int32_t length = is.tellg();

  if ( length > 0 )
  {

    char* buf = new char[ length ];
    is.seekg( 0, std::ios::beg );
    is.read( buf, length );
    is.close();
    std::string buffer( buf );
    delete [] buf;

    loadList( buffer, priority );

  }

}


void gkg::Style::loadList( const std::string& buffer, int32_t priority )
{

  size_t p = 0;
  size_t start = 0;
  for ( p = 0; p < buffer.length(); p++ )
  {

    if ( buffer[ p ] == '\n' )
    {

      if ( ( p > start ) && ( buffer[ p - 1 ] != '\\' ) )
      {

        loadProperty( buffer.substr( start, p - start ), priority );
        start = p + 1;

      }

    }

  }

}


void gkg::Style::loadProperty( const std::string& property, int32_t priority )
{

  _styleImplementation->loadProperty( property, priority );

}


void gkg::Style::addTrigger( const std::string& name, gkg::Action* action )
{

  _styleImplementation->addTrigger( name, action );

}


void gkg::Style::removeTrigger( const std::string& name, gkg::Action* action )
{

  _styleImplementation->removeTrigger( name, action );

}


void gkg::Style::addTriggerAny( gkg::Action* action )
{

  _styleImplementation->addTriggerAny( action );

}


void gkg::Style::removeTriggerAny( gkg::Action* action )
{

  _styleImplementation->removeTriggerAny( action );

}


bool gkg::Style::findAttribute( const std::string& name, 
                                std::string& value ) const
{

  return _styleImplementation->findAttribute( ( gkg::Style* )this, 
                                              name,
                                              value );

}


bool gkg::Style::findAttribute( const std::string& name, int32_t& value ) const
{

  std::string v;
  return findAttribute( name, v ) && gkg::StringConverter::toScalar( v, value );

}


bool gkg::Style::findAttribute( const std::string& name, double& value ) const
{

  std::string v;
  return findAttribute( name, v ) && gkg::StringConverter::toScalar( v, value );

}


bool gkg::Style::findAttribute( const std::string& name,
                                gkg::FloatCoordinate& value ) const
{

  std::string v;
  if ( !findAttribute( name, v ) )
  {

    return false;

  }
  std::string units( v );
  gkg::FloatCoordinate points = 1.0f;
  const char* p = v.c_str();
  const char* end = p + v.length();
  if ( p < end && ( *p == '-' || *p == '+' ) )
  {

    ++ p;

  }

  bool dot = false;
  for ( ; p < end; p++ )
  {

    if ( !dot && ( *p == '.' ) )
    {

      dot = true;

    }
    else if ( !isspace( *p ) && !isdigit( *p ) )
    {

      size_t i = p - v.c_str();
      units = units.substr( i, units.length() - i );
      if ( units == "mm" )
      {

        points = 72.0 / 25.4;

      }
      else if ( units == "cm" )
      {

        points = 72.0 / 2.54;

      }
      else if ( units == "in" )
      {

        points = 72.0;

      }
      else if ( units != "pt" )
      {

        return false;

      }
      v = v.substr( 0, i ); 
      break;

    }

  }
  if ( gkg::StringConverter::toScalar( v, value ) )
  {

    value *= points;
    return true;

  }
  return false;

}


bool gkg::Style::getValueIsOn( const std::string& name ) const
{

  std::string v;
  if ( !findAttribute( name, v ) )
  {

    return false;

  }
  v = gkg::StringConverter::toLower( v );
  return ( v == "on" ) || ( v == "true" );

}


gkg::StyleImplementation* gkg::Style::getImplementation() const
{

  return _styleImplementation;

}
