#ifndef _gkg_graphics_X11_core_LookAndFeel_h_
#define _gkg_graphics_X11_core_LookAndFeel_h_


#include <string>
#include <map>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-graphics-X11-core/WidgetFactory.h>
#include <gkg-graphics-X11-core/DialogFactory.h>


namespace gkg
{


class LookAndFeel : public Singleton< LookAndFeel >
{

  public:

    typedef WidgetFactory& ( *WidgetFactoryAccessor )();
    typedef DialogFactory& ( *DialogFactoryAccessor )();

    ~LookAndFeel();

    WidgetFactory* getWidgetFactory();
    DialogFactory* getDialogFactory();

    bool registerFactories( const std::string& lookName,
                            WidgetFactoryAccessor widgetFactoryAccessor,
                            DialogFactoryAccessor dialogFactoryAccessor );

  protected:

    friend class Singleton< LookAndFeel >;

    LookAndFeel();
    void selectFactories();
    bool select( const std::string& look );
    void selectColoringMode();

    std::map< std::string, WidgetFactoryAccessor > _widgetFactoryAccessors;
    std::map< std::string, DialogFactoryAccessor > _dialogFactoryAccessors;
    WidgetFactory* _widgetFactory;
    DialogFactory* _dialogFactory;
    bool _ready;

};


}


#endif
