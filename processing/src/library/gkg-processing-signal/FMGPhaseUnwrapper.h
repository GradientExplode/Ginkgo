#ifndef _gkg_processing_signal_FMGPhaseUnwrapper_h_
#define _gkg_processing_signal_FMGPhaseUnwrapper_h_


#include <gkg-processing-signal/PhaseUnwrapper.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


template < class T >
class FMGPhaseUnwrapper : public PhaseUnwrapper< T >,
                          public CreatorNoArg< FMGPhaseUnwrapper< T >,
                                               PhaseUnwrapper< T > >
{

  public:

    enum PlaneAxis
    {

      X,
      Y,
      Z

    };

    ~FMGPhaseUnwrapper();

    std::string getName() const;

    void setPlaneAxis( PlaneAxis planeAxis );
    void setCycleCount( int32_t count );
    void setCoarsestSize( int32_t size );
    void setPreSmoothingIterationCount( int32_t count );
    void setPostSmoothingIterationCount( int32_t count );
    void useCongruence( bool value );

    PlaneAxis getPlaneAxis() const;
    int32_t getCycleCount() const;
    int32_t getCoarsestSize() const;
    int32_t getPreSmoothingIterationCount() const;
    int32_t getPostSmoothingIterationCount() const;
    bool getUseCongruence() const;

    void unwrap( Volume< T >& phase, 
                 const Volume< T >& weights, 
                 bool verbose ) const;
    void unwrap( Volume< T >& phase,
                 bool verbose ) const;

    static std::string getStaticName();

  protected:

    friend class CreatorNoArg< FMGPhaseUnwrapper< T >,
                               PhaseUnwrapper< T > >;

    FMGPhaseUnwrapper();

    void unwrapSlice( Volume< T >& wrappedPhase ) const;
    T wrapPhase( const T& value ) const;
    void fullMultigridVCycle( Volume< T >& fineUnwrappedPhase,
                              const Volume< T >& fineGradientX,
                              const Volume< T >& fineGradientY ) const;
    void vCycle( Volume< T >& fineUnwrappedPhase,
                 const Volume< T >& fineGradientX,
                 const Volume< T >& fineGradientY ) const;
    void restrict( const Volume< T >& fineGradientX,
                   const Volume< T >& fineGradientY,
                   const Volume< T >& fineUnwrappedPhase,
                   Volume< T >& coarseGradientX,
                   Volume< T >& coarseGradientY ) const;
    void prolongate( const Volume< T >& coarseUnwrappedPhase,
                     Volume< T >& fineUnwrappedPhase ) const;
    void relax( const Volume< T >& gradientX,
                const Volume< T >& gradientY,
                Volume< T >& unwrappedPhase,
                int32_t iterationCount ) const;
    void congruence( const Volume< T >& sliceWrappedPhase,
                     Volume< T >& sliceUnwrappedPhase ) const;

    PlaneAxis _planeAxis;
    int32_t _cycleCount;
    int32_t _coarsestSize;
    int32_t _preSmoothingIterationCount;
    int32_t _postSmoothingIterationCount;
    bool _useCongruence;

};


}


#endif
