#ifndef _gkg_processing_transform_Transform3dManager_h_
#define _gkg_processing_transform_Transform3dManager_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-pattern/Observable.h>
#include <gkg-core-pattern/Observer.h>
#include <gkg-core-pattern/RCPointer.h>
#include <string>
#include <map>
#include <set>
#include <deque>


namespace gkg
{


class Referential;
template < class T > class Transform3d;


template < class T >
class Transform3dManager : public Singleton< Transform3dManager< T > >,
                           public Observable,
                           public Observer
{

  public:

    ~Transform3dManager();

    Transform3d< T >* getTransform3d( const Referential* fromReferential,
                                      const Referential* toReferential );
    bool hasTransform3d( const Referential* fromReferential,
                         const Referential* toReferential ) const;

    void registerTransform3d( const Referential* fromReferential,
                              const Referential* toReferential,
                              RCPointer< Transform3d< T > > transform3d,
                              bool update = true,
                              bool notify = true );
    void unregisterTransform3d( const Referential* fromReferential,
                                const Referential* toReferential,
                                bool update = true,
                                bool notify = true );

  protected:

    friend class Singleton< Transform3dManager< T > >;

    Transform3dManager();

    void update( const Observable* observable, void* arg );

    bool internalHasTransform3d( const Referential* fromReferential,
                                 const Referential* toReferential ) const;
    bool internalHasPath( const Referential* fromReferential,
                          const Referential* toReferential ) const;
    void internalRegisterTransform3d(
                                    const Referential* fromReferential,
                                    const Referential* toReferential,
                                    RCPointer< Transform3d< T > > transform3d );
    void internalUnregisterTransform3d( const Referential* fromReferential,
                                        const Referential* toReferential );

    void internalUpdate( bool notify );
    void internalRemoveSelfCreatedTransform3ds();
    void internalUpdateReferentials();
    void internalAddInverseTransform3ds();
    void internalCompleteTransform3ds();
    std::deque< const Referential* >
       internalGetPath( const Referential* fromReferential,
                        const Referential* toReferential ) const;

    void unregisterObservable( gkg::Observable* );

    std::map< const Referential*,
              std::map< const Referential*,
                        RCPointer< Transform3d< T > > > > _transform3ds;
    std::map< const Referential*,
              std::map< const Referential*,
                        RCPointer< Transform3d< T > > > > _internalTransform3ds;
    std::set< RCPointer< Transform3d< T > > > _selfCreatedTransform3ds;
    std::set< const Referential* > _referentials;

};


}


#endif
