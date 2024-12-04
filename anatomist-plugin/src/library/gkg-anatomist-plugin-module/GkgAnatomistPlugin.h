#ifndef _gkg_anatomist_plugin_module_GkgAnatomistPlugin_h_
#define _gkg_anatomist_plugin_module_GkgAnatomistPlugin_h_

#include <anatomist/application/module.h>
#include <string>


#include <iostream>


namespace gkg
{


class GkgAnatomistPlugin : public anatomist::Module
{

  public:

    GkgAnatomistPlugin();
    virtual ~GkgAnatomistPlugin();

    virtual std::string name() const;
    virtual std::string description() const;

  protected:

    virtual void objectsDeclaration();
    virtual void controlsDeclaration();
    virtual void actionsDeclaration();

  };

}


#endif
