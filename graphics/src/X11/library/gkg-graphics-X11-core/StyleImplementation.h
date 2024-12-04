#ifndef _gkg_graphics_X11_core_StyleImplementation_h_
#define _gkg_graphics_X11_core_StyleImplementation_h_


#include <string>
#include <list>
#include <deque>    // like a vector but with push_front inserter
#include <gkg-graphics-X11-core/MacroAction.h>
#include <map>


namespace gkg
{


class Style;


class StyleImplementation
{

  public:

    struct Attribute
    {

      std::string name;
      std::deque< std::string > path;
      std::string value;
      int32_t priority;
      MacroAction* observers;
      int32_t index;

    };

    struct AttributeEntry
    {

      std::list< Attribute* >** entries;
      int32_t available;
      int32_t used;

    };

    typedef std::map< std::string, AttributeEntry* > AttributeHashTable;

    StyleImplementation( const std::string& name );
    ~StyleImplementation();

    void modify();
    void update();

    void setName( const std::string& name );
    const std::string& getName() const;

    void setAlias( const std::string& name );
    int32_t getAliasCount() const;
    std::string getAlias( int32_t index ) const;
    const std::deque< std::string >& getAliases() const;

    void setParent( Style* style );
    Style* getParent() const;
    void append( Style* style );
    void remove( Style* style );
    int32_t getChildrenCount() const;
    Style* getChild( int32_t index ) const;

    Attribute* addAttribute( const std::string& name,
                             const std::string& value,
                             int32_t priority );
    void removeAttribute( const std::string& name );
    int32_t getAttributeCount() const;
    bool getAttribute( int32_t index, 
                       std::string& name,
                       std::string& value ) const;

    void loadProperty( const std::string& property, int32_t priority );

    void addTrigger( const std::string& name, Action* action );
    void removeTrigger( const std::string& name, Action* action );
    void addTriggerAny( Action* action );
    void removeTriggerAny( Action* action );

    bool findAttribute( Style* style,
                        const std::string& name,
                        std::string& value );
    AttributeEntry* findEntry( const std::string& name ) const;
    int32_t matchName( const std::string& name ) const;

  private:

    std::string strip(  const std::string& s ) const;
    void missingColon( const std::string& property ) const;
    void badPropertyName() const;
    void badPropertyValue() const;

    bool parseName( std::deque< std::string >& names,
                    std::string& s, int32_t& priority ) const;
    std::string parseValue( const std::string& v );
    size_t findSeparator( const std::string& s ) const;
    void deleteAttribute( Attribute* attribute );

    bool wildcardMatch( const AttributeEntry& entry,
                        const std::deque< Style* >& styleList,
                        std::string& value ) const;
    bool wildcardMatchName( const std::string& name,
                            const AttributeEntry& entry,
                            const std::deque< Style* >& styleList,
                            int32_t index,
                            std::string& value ) const;
    bool finishMatch( const std::deque< Style* >& styleList,
                      int32_t sIndex,
                      const std::deque< std::string >& path,
                      int32_t pIndex ) const;

    std::string _name;
    std::deque< std::string > _aliases;
    Style* _parent;
    AttributeHashTable _attributeTable;
    std::deque< Attribute* > _attributeList;
    std::deque< Style* > _children;
    MacroAction* _observers;
    bool _modified;

};


}


#endif

