#ifndef _gkg_graphics_X11_core_Style_h_
#define _gkg_graphics_X11_core_Style_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Action;
class StyleImplementation;


class Style : public RCGUIObject
{

  public:

    Style();
    Style( const std::string& name );
    Style( Style* parent );
    Style( const std::string& name, Style* parent );
    Style( const Style& other );
    virtual ~Style();

    virtual void setName( const std::string& name );
    virtual const std::string& getName() const;

    virtual void setAlias( const std::string& name );
    virtual int32_t getAliasCount() const;
    virtual std::string getAlias( int32_t index ) const;

    virtual Style* getParent() const;
    virtual void append( Style* style );
    virtual void remove( Style* style );
    virtual int32_t getChildrenCount() const;
    virtual Style* getChild( int32_t index ) const;

    virtual void addAttribute( const std::string& name,
                               const std::string& value,
                               int32_t priority = 0 );
    virtual void removeAttribute( const std::string& name );
    virtual int32_t getAttributeCount() const;
    virtual bool getAttribute( int32_t index, 
                               std::string& name,
                               std::string& value ) const;

    virtual void loadFile( const std::string& filename, int32_t priority = 0 );
    virtual void loadList( const std::string& buffer, int32_t priority = 0 );
    virtual void loadProperty( const std::string& property, 
                               int32_t priority = 0 );

    virtual void addTrigger( const std::string& name, Action* action );
    virtual void removeTrigger( const std::string& name, Action* action = 0 );
    virtual void addTriggerAny( Action* action );
    virtual void removeTriggerAny( Action* action );

    virtual bool findAttribute( const std::string& name,
                                std::string& value ) const;
    virtual bool findAttribute( const std::string& name,
                                int32_t& value ) const;
    virtual bool findAttribute( const std::string& name,
                                double& value ) const;
    virtual bool findAttribute( const std::string& name,
                                FloatCoordinate& value ) const;

    virtual bool getValueIsOn( const std::string& name ) const;

    StyleImplementation* getImplementation() const;

  private:

    Style& operator=( const Style& );   // not implemented

    StyleImplementation* _styleImplementation;

};


}


#endif
