#include <gkg-processing-transform/Transform3dManager.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/Referential.h>
#include <gkg-core-exception/Exception.h>
#include <list>


template < class T >
gkg::Transform3dManager< T >::Transform3dManager()
                             : gkg::Observable(),
                               gkg::Observer()
{
}


template < class T >
gkg::Transform3dManager< T >::~Transform3dManager()
{

  _transform3ds.clear();
  internalRemoveSelfCreatedTransform3ds();

}


template < class T >
gkg::Transform3d< T >* 
gkg::Transform3dManager< T >::getTransform3d(
                                        const gkg::Referential* fromReferential,
                                        const gkg::Referential* toReferential )
{

  try
  {

    // trying to look for transform among existing ones
    typename std::map< const gkg::Referential*,
                       std::map< const gkg::Referential*,
                     gkg::RCPointer< gkg::Transform3d< T > > > >::const_iterator
      m = _transform3ds.find( fromReferential );
    if ( m != _transform3ds.end() )
    {

      typename std::map< const gkg::Referential*,
      gkg::RCPointer< gkg::Transform3d< T > > >::const_iterator
        t = ( *m ).second.find( toReferential );
      if ( t != ( *m ).second.end() )
      {

        return ( *t ).second.get();

      }

    }

    throw std::runtime_error(  "unable to find path between 'fromReferential'"
                               " and 'toReferential'" );
    return 0;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Transform3d< T >* "
             "gkg::Transform3dManager< T >::getTransform3d( "
             "const gkg::Referential* fromReferential, "
             "const gkg::Referential* toReferential )" );

}


template < class T >
bool gkg::Transform3dManager< T >::hasTransform3d(
                                   const gkg::Referential* fromReferential,
                                   const gkg::Referential* toReferential ) const
{

  typename std::map< const gkg::Referential*,
                     std::map< const gkg::Referential*,
                    gkg::RCPointer< gkg::Transform3d< T > > > >::const_iterator
    m = _transform3ds.find( fromReferential );
  if ( m != _transform3ds.end() )
  {

    typename std::map< const gkg::Referential*,
                      gkg::RCPointer< gkg::Transform3d< T > > >::const_iterator
      t = ( *m ).second.find( toReferential );
    if ( t != ( *m ).second.end() )
    {

      return true;

    }

  }
  return false;

}


template < class T >
void gkg::Transform3dManager< T >::registerTransform3d(
                            const gkg::Referential* fromReferential,
                            const gkg::Referential* toReferential,
                            gkg::RCPointer< gkg::Transform3d< T > > transform3d,
                            bool update,
                            bool notify )
{

  try
  {

    if ( fromReferential == toReferential )
    {

      throw std::runtime_error(
                 "'from' referential must be different from 'to' referential" );
      return;

    }

    unregisterTransform3d( fromReferential, toReferential, false, false );
    internalRegisterTransform3d( fromReferential, toReferential, transform3d );
    this->setChanged();

    if ( update )
    {

      internalUpdate( notify );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Transform3dManager< T >::registerTransform3d( "
             "const gkg::Referential* fromReferential, "
             "const gkg::Referential* toReferential, "
             "gkg::Transform3d< T >* transform3d, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Transform3dManager< T >::unregisterTransform3d(
                                        const gkg::Referential* fromReferential,
                                        const gkg::Referential* toReferential,
                                        bool update,
                                        bool notify )
{

  try
  {

    // no existing 3D transform
    if ( !hasTransform3d( fromReferential, toReferential ) )
    {

      return;

    }

    // if path is self created, then we cannot remove it, because it could cause
    // conflicting situtations
    if ( !internalHasPath( fromReferential, toReferential ) )
    {

      throw std::runtime_error(
                 "cannot remove self created transform(s), possible conflict" );
      return;

    }

    // if path was registered, remove it
    if ( internalHasTransform3d( fromReferential, toReferential ) )
    {

      internalUnregisterTransform3d( fromReferential, toReferential );

    }
    else
    {

      internalUnregisterTransform3d( toReferential, fromReferential );

    }
    this->setChanged();
    if ( update )
    {

      internalUpdate( notify );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Transform3dManager< T >::unregisterTransform3d( "
             "const gkg::Referential* fromReferential, "
             "const gkg::Referential* toReferential, "
             "bool update, "
             "bool notify )" );

}


template < class T >
bool gkg::Transform3dManager< T >::internalHasTransform3d(
                                   const gkg::Referential* fromReferential,
                                   const gkg::Referential* toReferential ) const
{

  // trying to find out if 'fromReferential->toReferential' was registered
  typename std::map< const gkg::Referential*,
                     std::map< const gkg::Referential*,
                     gkg::RCPointer< gkg::Transform3d< T > > > >::const_iterator
    m = _internalTransform3ds.find( fromReferential );
  if ( m != _internalTransform3ds.end() )
  {

    typename std::map< const gkg::Referential*,
                       gkg::RCPointer< gkg::Transform3d< T > > >::const_iterator
      t = ( *m ).second.find( toReferential );
    if ( t != ( *m ).second.end() )
    {

      return true;

    }

  }
  return false;

}


template < class T >
bool gkg::Transform3dManager< T >::internalHasPath(
                                   const gkg::Referential* fromReferential,
                                   const gkg::Referential* toReferential ) const
{

  return internalHasTransform3d( fromReferential, toReferential ) ||
         internalHasTransform3d( toReferential, fromReferential );

}


template < class T >
void gkg::Transform3dManager< T >::internalRegisterTransform3d(
                                        const gkg::Referential* fromReferential,
                                        const gkg::Referential* toReferential,
                           gkg::RCPointer< gkg::Transform3d< T > > transform3d )
{

  _internalTransform3ds[ fromReferential ][ toReferential ] = transform3d;
  transform3d->addObserver( this );

}


template < class T >
void gkg::Transform3dManager< T >::internalUnregisterTransform3d(
                                        const gkg::Referential* fromReferential,
                                        const gkg::Referential* toReferential )
{

  typename std::map< const gkg::Referential*,
                     std::map< const gkg::Referential*,
                           gkg::RCPointer< gkg::Transform3d< T > > > >::iterator
    m = _internalTransform3ds.find( fromReferential );
  if ( m != _internalTransform3ds.end() )
  {

    typename std::map< const gkg::Referential*,
                       gkg::RCPointer< gkg::Transform3d< T > > >::iterator
      t = ( *m ).second.find( toReferential );
    if ( t != ( *m ).second.end() )
    {

      ( *m ).second.erase( t );
      ( *t ).second->removeObserver( this );
      if ( ( *m ).second.empty() )
      {

        _internalTransform3ds.erase( m );

      }

    }

  }

}


template < class T >
void gkg::Transform3dManager< T >::update( const Observable* /*observable*/,
                                           void* arg )
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
  internalUpdate( notify );

}

template < class T >
void gkg::Transform3dManager< T >::internalUpdate( bool notify )
{

  // removing self created transform(s)
  internalRemoveSelfCreatedTransform3ds();

  // updating referential set
  internalUpdateReferentials();

  // initializing 3D transform map with internal registered transform3d(s)
  _transform3ds = _internalTransform3ds;

  // adding inverse 3D transform(s)
  internalAddInverseTransform3ds();

  // completing all other (from inverse) self created 3D transform(s)
  internalCompleteTransform3ds();

  // now, notifying every 3D transform manager observer that things have changed
  if ( notify )
  {

    notifyObservers( this );

  }

}


template < class T >
void gkg::Transform3dManager< T >::internalRemoveSelfCreatedTransform3ds()
{

  _selfCreatedTransform3ds.clear();

}


template < class T >
void gkg::Transform3dManager< T >::internalUpdateReferentials()
{

  _referentials.clear();

  typename std::map< const gkg::Referential*,
                     std::map< const gkg::Referential*,
                     gkg::RCPointer< gkg::Transform3d< T > > > >::const_iterator
    m = _internalTransform3ds.begin(), me = _internalTransform3ds.end();
  while ( m != me )
  {

    const gkg::Referential* fromReferential = ( *m ).first;

    _referentials.insert( fromReferential );

    typename std::map< const gkg::Referential*,
                       gkg::RCPointer< gkg::Transform3d< T > > >::const_iterator
      t = ( *m ).second.begin(), te = ( *m ).second.end();
    while ( t != te )
    {

      const gkg::Referential* toReferential = ( *t ).first;
      _referentials.insert( toReferential );
      ++ t;

    }
    ++ m;

  }

}


template < class T >
void gkg::Transform3dManager< T >::internalAddInverseTransform3ds()
{

  typename std::map< const gkg::Referential*,
                     std::map< const gkg::Referential*,
                           gkg::RCPointer< gkg::Transform3d< T > > > >::iterator
    m = _internalTransform3ds.begin(), me = _internalTransform3ds.end();
  while ( m != me )
  {

    const gkg::Referential* fromReferential = ( *m ).first;

    typename std::map< const gkg::Referential*,
                       gkg::RCPointer< gkg::Transform3d< T > > >::iterator
      t = ( *m ).second.begin(), te = ( *m ).second.end();
    while ( t != te )
    {

      const gkg::Referential* toReferential = ( *t ).first;
      gkg::RCPointer< gkg::InverseTransform3d< T > > inverseTransform3d;
      inverseTransform3d.reset( new gkg::InverseTransform3d< T >(
                                                           ( ( *t ).second ) ) );
      _transform3ds[ toReferential ][ fromReferential ] = inverseTransform3d;
      _selfCreatedTransform3ds.insert( inverseTransform3d );
      ++ t;

    }
    ++ m;

  }

}


template < class T >
void gkg::Transform3dManager< T >::internalCompleteTransform3ds()
{

  std::set< const Referential* >::const_iterator
    r1 = _referentials.begin(), r1e = _referentials.end();
  std::set< const Referential* >::const_iterator
    r2, r2e = _referentials.end();

  while ( r1 != r1e )
  {

    r2 = _referentials.begin();
    while ( r2 != r2e )
    {

      const Referential* fromReferential = *r1;
      const Referential* toReferential = *r2;

      // the path doesn't actually exist, we try to build it
      if ( r1 != r2 && !internalHasPath( fromReferential, toReferential ) )
      {

        std::deque< const gkg::Referential* >
          path = internalGetPath(  fromReferential, toReferential );
        if ( !path.empty() )
        {

          // adding direct transform
          std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transform3ds;
          std::deque< bool > clonings;
          size_t i;
          for ( i = 0; i < path.size() - 1; i++ )
          {

            transform3ds.push_back(
                                  _transform3ds[ path[ i ] ][ path[ i + 1 ] ] );
            clonings.push_back( false );

          }
          gkg::RCPointer< gkg::CompositeTransform3d< T > > compositeTransform3d;
          compositeTransform3d.reset( new gkg::CompositeTransform3d< T > );
          compositeTransform3d->compose( transform3ds, clonings );
          _transform3ds[ fromReferential ][ toReferential ] =
            compositeTransform3d;
          _selfCreatedTransform3ds.insert( compositeTransform3d );

          // adding inverse transform
          gkg::RCPointer< gkg::InverseTransform3d< T > > inverseTransform3d;
          inverseTransform3d.reset( new gkg::InverseTransform3d< T >(
                                                        compositeTransform3d ) );
          _transform3ds[ toReferential ][ fromReferential ] = 
           inverseTransform3d;
          _selfCreatedTransform3ds.insert( inverseTransform3d );

        }

      }
      ++ r2;

    }
    ++ r1;

  }

}


template < class T >
std::deque< const gkg::Referential* >
gkg::Transform3dManager< T >::internalGetPath(
                                        const Referential* fromReferential,
                                        const Referential* toReferential ) const
{

  std::list< std::deque< const gkg::Referential* > > currentPaths, newPaths;
  std::deque< const gkg::Referential* > initialPath;
  std::set< const gkg::Referential* > done;

  // initializing the path with 'fromReferential'
  initialPath.push_back( fromReferential );
  currentPaths.push_back( initialPath );
  done.insert( fromReferential );

  bool found = false;
  bool hasProgressed = true;
  std::deque< const gkg::Referential* > path;

  // looping until path is found or propagation is not possible anymore
  // we are working with _transform3ds which is just the same as
  // _internalTransform3ds but with inverse 3D transforms
  while ( !found && hasProgressed )
  {

    std::list< std::deque< const gkg::Referential* > >::const_iterator
      l = currentPaths.begin(), le = currentPaths.end();
    hasProgressed = false;
    // looping over the previous partial paths
    while ( l != le )
    {

      const std::deque< const gkg::Referential* >& currentPath = *l;
      
      typename std::map< const gkg::Referential*,
                         std::map< const gkg::Referential*,
                     gkg::RCPointer< gkg::Transform3d< T > > > >::const_iterator
        m = _transform3ds.find( currentPath.back() );
      // if the last source referential of the current path has en entry in the
      // 3D transform table, then proceed
      if ( m != _transform3ds.end() )
      {

        typename std::map< const gkg::Referential*,
                       gkg::RCPointer< gkg::Transform3d< T > > >::const_iterator
          t = ( *m ).second.begin(), te = ( *m ).second.end();
        while ( t != te )
        {

          const gkg::Referential* currentToReferential = ( *t ).first;
          // if the destination referential has not already been visited,
          // then proceed
          if ( done.find( currentToReferential ) == done.end() )
          {

            // creating a new path which is the current path plus one more
            // referential
            std::deque< const gkg::Referential* > newPath( currentPath );
            newPath.push_back( currentToReferential );
            // stipulating that now the new referential was visited
            done.insert( currentToReferential );
            // putting that new path into the new path list
            newPaths.push_back( newPath );
            // telling that the front has progressed
            hasProgressed = true;
            // if the last new referential is the target 'toReferential', then
            // global path is found
            if ( currentToReferential == toReferential )
            {

              path = newPath;
              found = true;
              break;

            }

          }
          ++ t;

        }
        // do not need to continue if global path is found
        if ( found )
        {

          break;

        }

      }
      ++ l;

    }
    // if global path not still found and front has progressed,
    // initializing current paths with new paths
    if ( !found && hasProgressed )
    {

      currentPaths = newPaths;
      newPaths.clear();

    }

  }
  return path;

}


template < class T >
void gkg::Transform3dManager< T >::unregisterObservable(
                                                   gkg::Observable* observable )
{

  gkg::Transform3d< T >* transform3d = dynamic_cast< gkg::Transform3d< T >* >(
                                                                   observable );
  typename std::map< const gkg::Referential*,
                     std::map< const gkg::Referential*,
                    gkg::RCPointer< gkg::Transform3d< T > > > >::const_iterator
    m = _internalTransform3ds.begin(), me = _internalTransform3ds.end();
  while ( m != me )
  {

    typename std::map< const gkg::Referential*,
                      gkg::RCPointer< gkg::Transform3d< T > > >::const_iterator
      t = ( *m ).second.begin(), te = ( *m ).second.end();
    while ( t != te )
    {

      if ( ( *t ).second.get() == transform3d )
      {

        unregisterTransform3d( ( *m ).first, ( *t ).first );
        return;

      }
      ++ t;

    }
    ++ m;

  }


}


template class gkg::Transform3dManager< float >;
template class gkg::Transform3dManager< double >;
