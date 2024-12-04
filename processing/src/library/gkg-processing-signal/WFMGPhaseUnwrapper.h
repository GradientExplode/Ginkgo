#ifndef _gkg_processing_signal_WFMGPhaseUnwrapper_h_
#define _gkg_processing_signal_WFMGPhaseUnwrapper_h_


#include <gkg-processing-signal/PhaseUnwrapper.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


template < class T >
class WFMGPhaseUnwrapper : public PhaseUnwrapper< T >,
                           public CreatorNoArg< WFMGPhaseUnwrapper< T >,
                                                PhaseUnwrapper< T > >
{

  public:

    enum PlaneAxis
    {

      X,
      Y,
      Z

    };

    ~WFMGPhaseUnwrapper();

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

    friend class CreatorNoArg< WFMGPhaseUnwrapper< T >,
                               PhaseUnwrapper< T > >;

    WFMGPhaseUnwrapper();

    void unwrapSlice( Volume< T >& wrappedPhase,
                      const Volume< T >& weights ) const;
    T wrapPhase( const T& value ) const;
    void fullMultigridVCycle( Volume< T >& fineUnwrappedPhase,
                              const Volume< T >& fineGradientX,
                              const Volume< T >& fineGradientY,
                              const Volume< T >& fineWeightsX,
                              const Volume< T >& fineWeightsY ) const;
    void vCycle( Volume< T >& fineUnwrappedPhase,
                 const Volume< T >& fineGradientX,
                 const Volume< T >& fineGradientY,
                 const Volume< T >& fineWeightsX,
                 const Volume< T >& fineWeightsY ) const;
    void restrictWeights( const Volume< T >& fineWeightsX,
                          const Volume< T >& fineWeightsY,
                          Volume< T >& coarseWeightsX,
                          Volume< T >& coarseWeightsY ) const;
    void restrict( const Volume< T >& fineGradientX,
                   const Volume< T >& fineGradientY,
                   const Volume< T >& fineWeightsX,
                   const Volume< T >& fineWeightsY,
                   const Volume< T >& fineUnwrappedPhase,
                   Volume< T >& coarseGradientX,
                   Volume< T >& coarseGradientY ) const;
    void prolongate( const Volume< T >& coarseUnwrappedPhase,
                     const Volume< T >& coarseWeightsX,
                     const Volume< T >& coarseWeightsY,
                     Volume< T >& fineUnwrappedPhase ) const;
    void relax( const Volume< T >& gradientX,
                const Volume< T >& gradientY,
                const Volume< T >& weightsX,
                const Volume< T >& weightsY,
                Volume< T >& unwrappedPhase,
                int32_t iterationCount ) const;
    void congruence( const Volume< T >& sliceWrappedPhase,
                     const Volume< T >& sliceWeights,
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
