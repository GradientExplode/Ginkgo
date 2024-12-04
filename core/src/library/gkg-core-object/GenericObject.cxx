#include <gkg-core-object/GenericObject_i.h>


gkg::GenericObject::GenericObject()
                   : gkg::RCPointer< gkg::BaseObject >()
{
}


gkg::GenericObject::GenericObject( const gkg::GenericObject& other )
                   : gkg::RCPointer< gkg::BaseObject >( other )
{
}


gkg::GenericObject::GenericObject( gkg::BaseObject* thing )
                   : gkg::RCPointer< gkg::BaseObject >( thing )
{
}


gkg::GenericObject::~GenericObject()
{
}


gkg::GenericObject& 
gkg::GenericObject::operator=( const gkg::GenericObject& other )
{

  gkg::RCPointer< gkg::BaseObject >::operator=( other );
  return *this;

}


gkg::GenericObject& gkg::GenericObject::operator=( gkg::BaseObject* thing )
{

  gkg::RCPointer< gkg::BaseObject >::operator=( 
                                   gkg::RCPointer< gkg::BaseObject >( thing ) );
  return *this;

}


