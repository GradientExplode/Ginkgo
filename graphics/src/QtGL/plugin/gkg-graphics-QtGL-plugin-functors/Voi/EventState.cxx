#include <gkg-graphics-QtGL-plugin-functors/Voi/EventState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>


gkg::EventState::EventState( gkg::GraphicsScene* scene )
               : _scene( scene )
{
}


gkg::EventState::~EventState()
{
}


void gkg::EventState::mousePressEvent( QGraphicsSceneMouseEvent* /* event */ )
{
}


void gkg::EventState::mouseMoveEvent( QGraphicsSceneMouseEvent* /* event */ )
{
}


void gkg::EventState::keyPressEvent( QKeyEvent* /* event */ )
{
}
