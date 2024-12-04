 #ifndef _gkg_core_pattern_Observer_h_
#define _gkg_core_pattern_Observer_h_


namespace gkg
{


class Observable;


class Observer
{

  public:

    Observer();
    virtual ~Observer();

    virtual void update( const Observable* observable = 0, void* arg = 0 ) = 0;
    virtual void unregisterObservable( Observable* );

};


}


#endif
