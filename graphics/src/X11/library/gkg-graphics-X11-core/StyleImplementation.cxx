#include <gkg-graphics-X11-core/StyleImplementation.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/MacroAction.h>
#include <cctype>
#include <iostream>


gkg::StyleImplementation::StyleImplementation( const std::string& name )
                         : _name( name ),
                           _parent( 0 ),
                           _observers( 0 ),
                           _modified( true )
{
}


gkg::StyleImplementation::~StyleImplementation()
{

  gkg::StyleImplementation::AttributeHashTable::iterator
    t = _attributeTable.begin(), te = _attributeTable.end();

  while ( t != te )
  {

    gkg::StyleImplementation::AttributeEntry* entry = t->second;

    for ( int32_t j = 0; j < entry->used; j++ )
    {

      std::list< Attribute* >* aList = entry->entries[ j ];
      if ( aList )
      {

        std::list< gkg::StyleImplementation::Attribute* >::iterator
          a = aList->begin(), ae = aList->end();
        while ( a != ae )
        {

          deleteAttribute( *a );
          ++ a;

        }
        delete aList;

      }

    }
    delete [] entry->entries;
    delete entry;

    t ++;

  }
  _attributeTable.clear();
  _attributeList.clear();   /// pas besoin de liberer les Attribute* ? strange!
  _aliases.clear();

  std::deque< gkg::Style* >::iterator s = _children.begin(),
                                      se = _children.end();
  while ( s != se )
  {

    ( *s )->getImplementation()->setParent( 0 );
    ++ s;

  }
  _children.clear();
  if ( _observers )
  {

    _observers->removeReference();

  }

}


void gkg::StyleImplementation::modify()
{

  _modified = true;
  if ( _observers )
  {

    _observers->execute();

  }
  if ( !_children.empty() )
  {

    std::deque< gkg::Style* >::iterator s = _children.begin(),
                                        se = _children.end();
    while ( s != se )
    {

      ( *s )->getImplementation()->modify();
      ++ s;

    }

  }

}


void gkg::StyleImplementation::update()
{

  if ( !_modified )
  {

    return;

  }
  if ( _parent )
  {

    _parent->getImplementation()->update();

  }
  _modified = false;

}


void gkg::StyleImplementation::setName( const std::string& name )
{

  _name = name;

}


const std::string& gkg::StyleImplementation::getName() const
{

  return _name;

}


void gkg::StyleImplementation::setAlias( const std::string& name )
{

  _aliases.push_front( name );
  modify();

}


int32_t gkg::StyleImplementation::getAliasCount() const
{

  return ( int32_t )_aliases.size();

}


std::string gkg::StyleImplementation::getAlias( int32_t index ) const
{

  if ( ( index < 0 ) || ( index >= getAliasCount() ) )
  {

    return "";

  }
  return _aliases[ index ];

}


const std::deque< std::string >& gkg::StyleImplementation::getAliases() const
{

  return _aliases;

}


void gkg::StyleImplementation::setParent( gkg::Style* style )
{

  _parent = style;

}


gkg::Style* gkg::StyleImplementation::getParent() const
{

  return _parent;

}


// Add a child style.  Implicitly remove the child from its current parent,
// if it has one, and set its parent to this.  Because reparenting may change
// (cached) attributes, we must mark the child modified.
void gkg::StyleImplementation::append( gkg::Style* style )
{

  _children.push_back( style );

}


void gkg::StyleImplementation::remove( gkg::Style* style )
{

  std::deque< gkg::Style* >::iterator s = _children.begin(),
                                      se = _children.end();

  while ( s != se )
  {

    if ( *s == style )
    {

      _children.erase( s );
      break;

    }
    ++ s;

  }

}


int32_t gkg::StyleImplementation::getChildrenCount() const
{

  return ( int32_t )_children.size();

}


gkg::Style* gkg::StyleImplementation::getChild( int32_t index ) const
{

  if ( ( index < 0 ) || ( index >= getChildrenCount() ) )
  {

    return 0;

  }
  return _children[ index ];

}


gkg::StyleImplementation::Attribute*
gkg::StyleImplementation::addAttribute( const std::string& name,
                                        const std::string& value,
                                        int32_t priority )
{

  std::string theName = name;
  std::deque< std::string > path;
  if ( !parseName( path, theName, priority ) )
  {

    return 0; // irrelevant attribute: A*B where A doesn't match

  }


  gkg::StyleImplementation::AttributeEntry* entry = findEntry( theName );
  if ( !entry )
  {

    entry = new gkg::StyleImplementation::AttributeEntry;
    entry->entries = new std::list< gkg::StyleImplementation::Attribute* >*[
                                                                            3 ];
    entry->available = 3;
    entry->used = 0;
    for ( int32_t i = 0; i < entry->available; i++ )
    {

      entry->entries[ i ] = 0;

    }
    _attributeTable[ theName ] = entry;

  }

  int32_t n = ( int32_t )path.size();
  if ( entry->available <= n )
  {

    int32_t i;
    int32_t newAvailable = n + 5;

    std::list< gkg::StyleImplementation::Attribute* >**
      newList = new std::list< gkg::StyleImplementation::Attribute* >*[ 
                                                             newAvailable ];
    for ( i = 0; i < entry->available; i++ )
    {

      newList[ i ] = entry->entries[ i ];

    }
    for ( i = entry->available; i < newAvailable; i++ )
    {

      newList[ i ] = 0;

    }
    delete entry->entries;
    entry->entries = newList;
    entry->available = newAvailable;

  }

  if ( !entry->entries[ n ] )
  {

    entry->entries[ n ] = new std::list< gkg::StyleImplementation::Attribute* >;

  }

  entry->used = std::max( entry->used, n + 1 );
  std::list< gkg::StyleImplementation::Attribute* >* 
    aList = entry->entries[ n ];

  std::list< gkg::StyleImplementation::Attribute* >::iterator
    a = aList->begin(), ae = aList->end();
  while ( a != ae )
  {

    gkg::StyleImplementation::Attribute* attribute = *a;
    if ( attribute->path == path )
    {

      if ( priority >= attribute->priority )
      {

        attribute->value = parseValue( value );
        attribute->priority = priority;
        if ( attribute->observers )
        {

          attribute->observers->execute();

        }
        modify();

      }
      return attribute;

    }
    ++ a;

  }

  gkg::StyleImplementation::Attribute* newAttribute =
    new gkg::StyleImplementation::Attribute;
  newAttribute->name = theName;
  newAttribute->path = path;
  newAttribute->value = parseValue( value );
  newAttribute->priority = priority;
  newAttribute->observers = 0;

  aList->push_back( newAttribute );

  newAttribute->index = ( int32_t )_attributeList.size();
  _attributeList.push_back( newAttribute );

  modify();

  return newAttribute;

}


void gkg::StyleImplementation::removeAttribute( const std::string& name )
{

  update();
  if ( _attributeTable.empty() )
  {

    return;

  }

  int32_t priority = 0;
  std::string theName = name;
  std::deque< std::string > path;

  if ( !parseName( path, theName, priority ) )
  {

    return;

  }

  gkg::StyleImplementation::AttributeEntry* entry = findEntry( theName );
  int32_t p = ( int32_t )path.size();

  if ( entry && ( entry->used > p ) )
  {

    std::list< gkg::StyleImplementation::Attribute* >*
      aList = entry->entries[ p ];
    if ( aList )
    {

      std::list< gkg::StyleImplementation::Attribute* >::iterator
        a = aList->begin(), ae = aList->end();
      while ( a != ae )
      {

        gkg::StyleImplementation::Attribute* attribute = *a;
        if ( attribute->path == path )
        {

          deleteAttribute( attribute );
          aList->erase( a );
          break;

        }
        ++ a;

      }

      if ( aList->empty() )
      {

        delete aList;
        entry->entries[ p ] = 0;

      }

    }

  }

}


int32_t gkg::StyleImplementation::getAttributeCount() const
{

  return ( int32_t )_attributeList.size();

}


bool gkg::StyleImplementation::getAttribute( int32_t index, 
                                             std::string& name,
                                             std::string& value ) const
{

  if ( _attributeList.empty() ||
       index < 0 ||
       ( index >= ( int32_t )_attributeList.size() ) )
  {

    return false;

  }

  gkg::StyleImplementation::Attribute* attribute = _attributeList[ index ];
  name = attribute->name;
  value = attribute->value;
  return true;

}


void gkg::StyleImplementation::loadProperty( const std::string& property,
                                             int32_t priority )
{

  std::string p = strip( property );

  if ( ( p.length() == 0 ) || ( p[ 0 ] == '!' ) )
  {

    return;

  }

  size_t colon = p.find( ':' );
  if ( colon == std::string::npos )
  {

    missingColon( p );

  }
  else
  {

    std::string name = strip( p.substr( 0, colon ) );
    std::string value = strip( p.substr( colon + 1, p.length() - colon - 1 ) );
    if ( name.length() == 0 )
    {

      badPropertyName();

    }
    else if ( value.length() == 0 )
    {

      badPropertyValue();

    }
    else
    {

      addAttribute( name, value, priority );

    }

  }

}


void gkg::StyleImplementation::addTrigger( const std::string& name,
                                           gkg::Action* action )
{

  std::string value( "undefined" );
  gkg::StyleImplementation::Attribute*
    attribute = addAttribute( name, value, -1000 );
  if ( attribute )
  {

    if ( !attribute->observers )
    {

      attribute->observers = new gkg::MacroAction;
      if ( attribute->observers )
      {

        attribute->observers->addReference();

      }

    }
    attribute->observers->append( action );

  }

}


// if the action parameter is nil, remove all triggers associated with the name.
void gkg::StyleImplementation::removeTrigger( const std::string& name,
                                              gkg::Action* action )
{

  std::string value( "undefined" );
  gkg::StyleImplementation::Attribute*
    attribute = addAttribute( name, value, -1000 );
  if ( attribute )
  {

    gkg::MacroAction* macroAction = attribute->observers;
    if ( !action )
    {

      if ( macroAction )
      {

        macroAction->removeReference();

      }
      attribute->observers = 0;

    }
    else
    {

      int32_t macroCount = macroAction->getCount();
      for ( int32_t i = 0; i < macroCount; i++ )
      {

        if ( macroAction->getAction( i ) == action )
        {

          macroAction->remove( i );
          break;

        }

      }

    }

  }

}


void gkg::StyleImplementation::addTriggerAny( gkg::Action* action )
{

  if ( !_observers )
  {

    _observers = new gkg::MacroAction;
    if ( _observers )
    {

      _observers->addReference();

    }

  }
  _observers->append( action );

}


void gkg::StyleImplementation::removeTriggerAny( gkg::Action* action )
{

  int32_t macroCount = _observers->getCount();
  for ( int32_t i = 0; i < macroCount; i++ )
  {

    if ( _observers->getAction( i ) == action )
    {

      _observers->remove( i );
      break;

    }

  }

}


bool gkg::StyleImplementation::findAttribute( Style* style,
                                              const std::string& name,
                                              std::string& value )
{

  update();

  gkg::StyleImplementation::AttributeEntry* entry = findEntry( name );
  if ( entry )
  {

    std::list< gkg::StyleImplementation::Attribute* >*
      aList = entry->entries[ 0 ];
    if ( aList && !aList->empty() )
    {

      gkg::StyleImplementation::Attribute* attribute = aList->front();
      value = attribute->value;
      return true;

    }

  }

  std::deque< Style* > styleList;
  styleList.push_front( style );
  gkg::StyleImplementation* styleImpl = this;
  for ( Style* s = styleImpl->getParent(); s; s = styleImpl->getParent() )
  {

    styleImpl = s->getImplementation();
    entry = styleImpl->findEntry( name );
    if ( entry )
    {

      if ( ( entry->used > 0 ) &&
           styleImpl->wildcardMatch( *entry, styleList, value ) )
      {

        return true;

      }
      std::list< gkg::StyleImplementation::Attribute* >*
        aList = entry->entries[ 0 ];
      if ( aList )
      {

        gkg::StyleImplementation::Attribute* attribute = aList->front();
        value = attribute->value;
        return true;

      }

    }
    styleList.push_front( style );

  }
  return false;

}


gkg::StyleImplementation::AttributeEntry* 
gkg::StyleImplementation::findEntry( const std::string& name ) const
{

  gkg::StyleImplementation::AttributeEntry* entry = 0;
  
  if ( !_attributeTable.empty() )
  {

    gkg::StyleImplementation::AttributeHashTable::const_iterator
      a = _attributeTable.find( name );
    if ( a != _attributeTable.end() )
    {

      entry = a->second;

    }

  }
  return entry;

}


// Check to see if a given name matches the style's name or any of its aliases.
// Return value:     0 - no match
//                   1 - name match
//                   2 and up - index of alias match plus 2
int32_t gkg::StyleImplementation::matchName( const std::string& name ) const
{

  int32_t match = 0;
  if ( !_name.empty() && ( _name == name ) )
  {

    match = 1;

  }
  else if ( !_aliases.empty() )
  {

    int32_t possibleMatch = 2;
    std::deque< std::string >::const_iterator a = _aliases.begin(),
                                              ae = _aliases.end();
    while ( a != ae )
    {

      if ( *a == name )
      {

        match = possibleMatch;
        break;

      }
      ++ possibleMatch;
      ++ a;

    }

  }
  return match;

}


std::string gkg::StyleImplementation::strip(  const std::string& s ) const
{

  int32_t i = 0;
  int32_t length = s.length();
  for ( i = 0; i < length && isspace( s[ i ] ); i ++ ) {}
  int32_t j = length - 1;
  for ( ; j >= 0 && isspace( s[ j ] ); j-- ) {}
  return s.substr( i, j - i + 1 );

}


void 
gkg::StyleImplementation::missingColon( const std::string& property ) const
{

  std::cerr << "bad property line : " << property << std::endl;

}


void gkg::StyleImplementation::badPropertyName() const
{
}


void gkg::StyleImplementation::badPropertyValue() const
{
}


// Parse a name of the form *A*B*C into the list of names A, B, C.
// Strip the first name (e.g., A) if it matches the style's name or an alias.
bool gkg::StyleImplementation::parseName( std::deque< std::string >& names,
                                          std::string& s, 
                                          int32_t& priority ) const
{

  bool leadingStar = false;
  if ( s[ 0 ] == '*' )
  {

    leadingStar = true;
    s = s.substr( 1, s.length() - 1 );

  }
  bool first = true;
  for ( size_t i = findSeparator( s ); i != std::string::npos;
                                       i = findSeparator( s ) )
  {

    std::string name = s.substr( 0, i );
    if ( first )
    {

      first = false;
      int32_t q = matchName( name );
      if ( q )
      {

        priority += ( q == 1 ) ? 2 : 1;
        s = s.substr( i + 1, s.length() - i - 1 );
        continue;

      }
      else if ( !leadingStar )
      {

        names.clear();
        return false;

      }

    }
    names.push_back( name );
    s = s.substr( i + 1, s.length() - i - 1 );

  }
  return true;

}


std::string gkg::StyleImplementation::parseValue( const std::string& v )
{

  const char* src = v.c_str();
  int length = ( int32_t )v.length();
  const char* srcEnd = src + length;
  char* dstStart = new char[ length + 1 ];
  char* dst = dstStart;
  for ( ; src < srcEnd; src++ )
  {

    if ( *src == '\\' )
    {

      ++ src;
      switch ( *src )
      {

        case '\n':

          // eliminate
          break;

        case 'n':

          *dst++ = '\n';
          break;

        case '\\':

          *dst++ = *src;
          break;

        default:

          *dst++ = '\\';
          *dst++ = *src;
          break;

      }

    }
    else
    {

      *dst++ = *src;

    }

  }
  *dst = '\0';
  std::string value( dstStart, dst - dstStart );
  delete [] dstStart;
  return value;

}


size_t gkg::StyleImplementation::findSeparator( const std::string& s ) const
{

  size_t pos = s.find( '*' );

  if ( pos == std::string::npos )
  {

    pos = s.find( '.' );

  }
  return pos;

}


void gkg::StyleImplementation::deleteAttribute( 
                                gkg::StyleImplementation::Attribute* attribute )
{

  _attributeList.erase( _attributeList.begin() + attribute->index );
  int32_t n = ( int32_t )_attributeList.size();
  for ( int32_t i = attribute->index; i < n; i++ )
  {

    gkg::StyleImplementation::Attribute* attr = _attributeList[ i ];
    -- attr->index;

  }
  if ( attribute->observers )
  {

    attribute->observers->removeReference();

  }
  delete attribute;

}


// Check if the given table entry contains a match for the given list of styles
// and if so copy the value; we start from the end of the style list so that we
// can find the closest match.
bool gkg::StyleImplementation::wildcardMatch(
                          const gkg::StyleImplementation::AttributeEntry& entry,
                          const std::deque< gkg::Style* >& styleList,
                          std::string& value ) const
{

  std::deque< gkg::Style* >::const_reverse_iterator s = styleList.rbegin(),
                                                    se = styleList.rend();

  int32_t i = ( int32_t )styleList.size() - 1;
  while ( s != se )
  {

    gkg::Style* style = *s;
    gkg::StyleImplementation* styleImpl = style->getImplementation();
    if ( !styleImpl->getName().empty() &&
         wildcardMatchName( styleImpl->getName(), entry, styleList, i, value ) )
    {

      return true;

    }

    const std::deque< std::string >& aliases = styleImpl->getAliases();
    if ( !aliases.empty() )
    {

      std::deque< std::string >::const_iterator a = aliases.begin(),
                                                ae = aliases.end();
      while ( a != ae )
      {

        if ( wildcardMatchName( *a, entry, styleList, i, value ) )
        {

          return true;

        }
        ++ a;

      }

    }

    ++ s;
    -- i;

  }
  return false;

}


bool gkg::StyleImplementation::wildcardMatchName(
                          const std::string& name,
                          const gkg::StyleImplementation::AttributeEntry& entry,
                          const std::deque< gkg::Style* >& styleList,
                          int32_t index,
                          std::string& value ) const
{

  int32_t n = std::min( index + 1, entry.used - 1 );
  for ( int32_t i = n; i >= 1; i-- )
  {

    std::list< gkg::StyleImplementation::Attribute* >*
      aList = entry.entries[ i ];
    if ( aList )
    {

      bool foundMatch = false;
      int32_t bestMatch = 0;
      std::list< gkg::StyleImplementation::Attribute* >::const_iterator
        a = aList->begin(), ae = aList->end();
      while ( a != ae )
      {

        gkg::StyleImplementation::Attribute* attribute = *a;
        const std::deque< std::string >& path = attribute->path;
        if ( name == path[ i - 1 ] )
        {

          if ( i == 1 )
          {

            value = attribute->value;
            return true;

          }
          else if ( index != 0 )
          {

            int32_t newMatch = finishMatch( styleList, index - 1, path, i - 2 );
            if ( newMatch > bestMatch )
            {

              foundMatch = true;
              bestMatch = newMatch;
              value = attribute->value;

            }

          }

        }
        ++ a;

      }
      if ( foundMatch )
      {

        return true;

      }

    }

  }
  return false;

}


bool gkg::StyleImplementation::finishMatch( 
                                     const std::deque< gkg::Style* >& styleList,
                                     int32_t sIndex,
                                     const std::deque< std::string >& path,
                                     int32_t pIndex ) const
{

  int32_t matched = 0;
  int32_t sCurrent = sIndex;
  int32_t pCurrent = pIndex;
  while ( ( pCurrent >= 0 ) && ( sCurrent >= 0 ) )
  {

    gkg::StyleImplementation*
      styleImpl = styleList[ sCurrent ]->getImplementation();
    int32_t m = styleImpl->matchName( path[ pCurrent ] );
    if ( m != 0 )
    {

      -- pCurrent;
      matched += m;

    }
    -- sCurrent;

  }
  return matched;

}
