#include <gkg-graphics-X11-LF-Motif/MotifWidgetFactory.h>
#include <gkg-graphics-X11-LF-Motif/MotifDialogFactory.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>


bool registrated = gkg::LookAndFeel::getInstance().registerFactories(
                               gkg::MotifWidgetFactory::getStaticName(),
                               &gkg::MotifWidgetFactory::getAccessor,
                               &gkg::MotifDialogFactory::getAccessor );
