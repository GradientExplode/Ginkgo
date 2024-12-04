#ifndef _gkg_core_pattern_Observable_h_
#define _gkg_core_pattern_Observable_h_


#include <gkg-core-cppext/StdInt.h>
#include <set>


namespace gkg
{


class Observer;


class Observable
{

  public:

    Observable();
    virtual ~Observable();

    void addObserver( Observer* observer );
    void removeObserver( Observer* observer );
    void removeObservers();

    int32_t getObserverCount() const;

    void notifyObservers( void* arg = 0 );
    void notifyUnregisterObservers();

    bool hasChanged() const;
    void setChanged();

  protected:

    void clearChanged();

  private:

    bool _changed;
    bool _updating;

    std::set< Observer* > _observers;
    std::set< Observer* > _toBeAdded;
    std::set< Observer* > _toBeRemoved;

};



}


#endif
