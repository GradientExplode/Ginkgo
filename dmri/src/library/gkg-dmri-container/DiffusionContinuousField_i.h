#ifndef _gkg_dmri_container_DiffusionContinuousField_i_h_
#define _gkg_dmri_container_DiffusionContinuousField_i_h_


#include <gkg-dmri-container/DiffusionContinuousField.h>
#include <gkg-processing-container/ContinuousField_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <list>
#include <iomanip>



template < class T >
inline
gkg::DiffusionContinuousField< T >::DiffusionContinuousField( bool storeItems )
                                   : gkg::ContinuousField< T >( storeItems )
{
}


template < class T >
inline
gkg::DiffusionContinuousField< T >::~DiffusionContinuousField( )
{
}


#endif
