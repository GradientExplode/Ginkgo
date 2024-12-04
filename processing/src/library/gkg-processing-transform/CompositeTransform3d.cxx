#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineWoShearingTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::CompositeTransform3d< T >::CompositeTransform3d()
                               : gkg::Transform3d< T >(),
                                 gkg::Observer(),
                                 _isComposable( true )
{
}


template < class T >
gkg::CompositeTransform3d< T >::CompositeTransform3d(
                                        const CompositeTransform3d< T >& other )
                               : gkg::Transform3d< T >(),
                                 gkg::Observer(),
                                 _isComposable( true )
{

  try
  {

    std::deque< bool > clonings;
    size_t i, size = other._transforms.size();
    for ( i = 0; i < size; i++ )
    {

      clonings.push_back( true );

    }
    compose( other._transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T > "
             "gkg::CompositeTransform3d< T >::CompositeTransform3d( "
             "const CompositeTransform3d< T >& other )" );

}


template < class T >
gkg::CompositeTransform3d< T >::~CompositeTransform3d()
{

  removeTransforms();

}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::CompositeTransform3d< T >::clone() const
{

  try
  {

    gkg::RCPointer< gkg::Transform3d< T > > transform;
    transform.reset( new gkg::CompositeTransform3d< T >( *this ) );
    return transform;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Transform3d< T > > "
             "gkg::CompositeTransform3d< T >::clone() const" );

}


template < class T >
gkg::CompositeTransform3d< T >& gkg::CompositeTransform3d< T >::operator=( 
                                   const gkg::CompositeTransform3d< T >& other )
{

  try
  {

    removeTransforms();

    std::deque< bool > clonings;
    size_t i, size = other._transforms.size();
    for ( i = 0; i < size; i++ )
    {

      clonings.push_back( true );

    }
    compose( other._transforms, clonings, true, true );
    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::CompositeTransform3d< T >& "
             "gkg::CompositeTransform3d< T >::operator=( "
             "const gkg::CompositeTransform3d< T >& other )" );

}


template < class T >
void gkg::CompositeTransform3d< T >::getDirect( const gkg::Vector3d< T >& from, 
                                                gkg::Vector3d< T >& to ) const
{

  try
  {

    if ( _isComposable )
    {

      _composition.getDirect( from, to );

    }
    else
    {

      typename std::deque< gkg::RCPointer< gkg::Transform3d< T > > >::
                                                                  const_iterator
        t = _transforms.begin(), te = _transforms.end();

      gkg::Vector3d< T > localFrom = from;
      while ( t != te )
      {

        ( *t )->getDirect( localFrom, to );
        localFrom = to;
        ++ t;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::getDirect( "
             "const gkg::Vector3d< T >& from, "
             "gkg::Vector3d< T >& to ) const" );

}


template < class T >
void gkg::CompositeTransform3d< T >::getInverse(
                                                const gkg::Vector3d< T >& to, 
                                                gkg::Vector3d< T >& from ) const
{

  try
  {

    if ( _isComposable )
    {

      _composition.getInverse( to, from );

    }
    else
    {

      typename  std::deque< gkg::RCPointer< gkg::Transform3d< T > > >::
                                                          const_reverse_iterator
        t = _transforms.rbegin(), te = _transforms.rend();

      gkg::Vector3d< T > localTo = to;
      while ( t != te )
      {

        ( *t )->getInverse( localTo, from );
        localTo = from;
        ++ t;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::getInverse( "
             "const gkg::Vector3d< T >& to,  "
             "gkg::Vector3d< T >& from ) const" );

}


template < class T >
bool gkg::CompositeTransform3d< T >::isComposable() const
{

  return _isComposable;

}


template < class T >
bool gkg::CompositeTransform3d< T >::isIndirect() const
{

  try
  {

    bool isIndirect = false;

    if ( _isComposable )
    {

      isIndirect = _composition.isIndirect();

    }
    return isIndirect;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::CompositeTransform3d< T >::isIndirect() const" );

}


template < class T >
void gkg::CompositeTransform3d< T >::compose(
                                            const gkg::Transform3d< T >& other )
{

  try
  {

    _isComposable = _isComposable && other.isComposable();
    gkg::RCPointer< gkg::Transform3d< T > > transform3d = other.clone();
    _transforms.push_back( transform3d );
    transform3d->addObserver( this );
    bool notify = true;
    update( 0, &notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::compose( "
             "const gkg::Transform3d< T >& other )" );

}


template < class T >
void gkg::CompositeTransform3d< T >::compose( gkg::RCPointer< gkg::Transform3d<
                                                                    T > > other,
                                              bool cloning,
                                              bool updating,
                                              bool notify )
{

  try
  {

    if ( !other.isNull() )
    {

      _isComposable = _isComposable && other->isComposable();
      gkg::RCPointer< gkg::Transform3d< T > > transform3d =
          ( cloning ? other->clone() : other );
      _transforms.push_back( transform3d );
      transform3d->addObserver( this );
      if ( updating )
      {

        update( 0, &notify );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::compose( "
             "gkg::RCPointer< gkg::Transform3d< T > > other, "
             "bool cloning, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::CompositeTransform3d< T >::compose(
                                  const std::deque< gkg::RCPointer< gkg::Transform3d< T > > >& others,
                                  const std::deque< bool >& clonings,
                                  bool updating,
                                  bool notify )
{

  try
  {

   size_t i, size = others.size();
   for ( i = 0; i < size; i++ )
   {

     compose( others[ i ],
              clonings[ i ],
              ( i == size - 1 ) ? updating : false,
              ( i == size - 1 ) ? notify : false );

   }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::compose( "
             "const std::deque< gkg::RCPointer< gkg::Transform3d< T > > >& "
             "others, "
             "const std::deque< bool >& clonings, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
const gkg::HomogeneousTransform3d< T >&
gkg::CompositeTransform3d< T >::getComposition() const
{

  try
  {

    if ( !_isComposable )
    {

      throw std::runtime_error(  "not a composable composite transform" );

    }
    return _composition;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::HomogeneousTransform3d< T >& "
             "gkg::CompositeTransform3d< T >::getComposition() const" );

}


template < class T >
void gkg::CompositeTransform3d< T >::removeTransforms()
{

  try
  {

    typename std::deque< gkg::RCPointer< gkg::Transform3d< T > > >::iterator
      t = _transforms.begin(), te = _transforms.end();

    while ( t != te )
    {

      ( *t )->removeObserver( this );
      ++ t;

    }
    _transforms.clear();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::removeTransforms()" );

}


template < class T >
void gkg::CompositeTransform3d< T >::update(
                                          const gkg::Observable* /*observable*/,
                                          void* arg )
{

  try
  {

    bool notify = true;
    if ( arg )
    {

      bool* notifyPointer = static_cast< bool* >( arg );
      if ( notifyPointer )
      {

        notify = *notifyPointer;

      }

    }
    _composition.setIdentity();
    if ( _isComposable )
    {

      typename std::deque< gkg::RCPointer< gkg::Transform3d< T > > >::
                                                                  const_iterator
        t = _transforms.begin(), te = _transforms.end();

      while ( t != te )
      {

        if ( dynamic_cast< gkg::HomogeneousTransform3d< T >* >( t->get() ) )
        {

          _composition = _composition.compose(
               *dynamic_cast< gkg::HomogeneousTransform3d< T >* >( t->get() ) );

        }
        else if ( dynamic_cast< gkg::InverseTransform3d< T >* >( t->get() ) )
        {

          const gkg::Transform3d< T >*
            dt = &dynamic_cast< gkg::InverseTransform3d< T >* >( t->get()
                                                      )->getDirectTransform3d();
          if ( dynamic_cast< const gkg::HomogeneousTransform3d< T >* >( dt ) )
          {

            _composition = _composition.compose(
               dynamic_cast< const gkg::HomogeneousTransform3d< T >* >(
                                    dt )->getInverseHomogeneousTransform3d() );

          }
          else  // inverse transform contains a composite
          {

            _composition = _composition.compose(
               dynamic_cast< const gkg::CompositeTransform3d< T >* >(
                        dt )->_composition.getInverseHomogeneousTransform3d() );

          }

        }
        else if (
               dynamic_cast< gkg::ParameterizedTransform3d< T >* >( t->get() ) )
        {

          if (
          dynamic_cast< gkg::ParameterizedRigidTransform3d< T >* >( t->get() ) )
          {

            _composition = _composition.compose(
              dynamic_cast< gkg::ParameterizedRigidTransform3d< T >* >(
                           t->get() )->getRigidTransform3d().getComposition() );

          }
          else if (
            dynamic_cast< gkg::ParameterizedAffineWoShearingTransform3d< T >* >(
                                                                    t->get() ) )
          {

            _composition = _composition.compose(
            dynamic_cast< gkg::ParameterizedAffineWoShearingTransform3d< T >* >(
                t->get() )->getAffineWoShearingTransform3d().getComposition() );

          }
          else if ( dynamic_cast< gkg::ParameterizedTransform3d< T >* >(
                                                                    t->get() ) )
          {

            _composition = _composition.compose(
            dynamic_cast< gkg::ParameterizedAffineTransform3d< T >* >(
                          t->get() )->getAffineTransform3d().getComposition() );

          }

        }
        else   // it is a composite
        {

          _composition = _composition.compose(
            dynamic_cast< gkg::CompositeTransform3d< T >* >(
                                                     t->get() )->_composition );


        }
        ++ t;

      }

    }
    this->setChanged();
    if ( notify )
    {

      this->notifyObservers( this );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::update( "
             "const gkg::Observable* /*observable*/, "
             "void* arg )" );

}


template < class T >
void 
gkg::CompositeTransform3d< T >::unregisterObservable( Observable* observable )
{

  try
  {

    gkg::Transform3d< T >*  transform3d =
        dynamic_cast< gkg::Transform3d< T >*  >( observable );

    typename std::deque< gkg::RCPointer< gkg::Transform3d< T > > >::iterator
      t = _transforms.begin(), te = _transforms.end();

    while ( t != te )
    {

      if ( t->get() == transform3d )
      {

         ( *t )->removeObserver( this );
         t->reset( new gkg::IdentityTransform3d< T > );
         ( *t )->addObserver( this );

      }
      ++ t;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CompositeTransform3d< T >::unregisterObservable( "
             "Observable* observable )" );

}



template class gkg::CompositeTransform3d< float >;
template class gkg::CompositeTransform3d< double >;

