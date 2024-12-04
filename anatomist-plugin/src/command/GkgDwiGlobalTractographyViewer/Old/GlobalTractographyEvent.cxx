#include "GlobalTractographyEvent.h"


gkg::GlobalTractographyEvent::GlobalTractographyEvent( int32_t id )
                           : QEvent( ( QEvent::Type )( QEvent::User + id ) )
{
}
