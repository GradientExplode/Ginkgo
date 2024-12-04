#ifndef _gkg_processing_statistics_IntraSubjectStatisticalMap_h_
#define _gkg_processing_statistics_IntraSubjectStatisticalMap_h_


#include <gkg-processing-statistics/StructureBasedStatisticalMap.h>
#include <list>
#include <string>


namespace gkg
{


template < class T > class Volume;


template < class T, class S, class Compare = std::less< S > >
class IntraSubjectStatisticalMap
{

  public:


    typedef void ( *Functor )( const Volume< T >&,
                               const Volume< S >&,
                               const S& background,
                               StructureBasedStatisticalMap< S, Compare >& );

    IntraSubjectStatisticalMap( const std::string& name,
                                const Volume< T >& contrastVolume,
                                const Volume< S >& labelVolume,
                                std::list< Functor >& functors,
                                const S& background = 0 );
    IntraSubjectStatisticalMap(
                     const IntraSubjectStatisticalMap< T, S, Compare >& other );
    virtual ~IntraSubjectStatisticalMap();

    const S& getBackground() const;
    const StructureBasedStatisticalMap< S, Compare >&
      getStatisticalMap() const;

    static void grayLevelMeanAndStandardDeviationFunctor(
                   const Volume< T >& contrastVolume,
                   const Volume< S >& labelVolume,
                   const S& background,
                   StructureBasedStatisticalMap< S, Compare >& statisticalMap );

    static void grayLevelAlphaAndBetaFunctor(
                   const Volume< T >& contrastVolume,
                   const Volume< S >& labelVolume,
                   const S& background,
                   StructureBasedStatisticalMap< S, Compare >& statisticalMap );
    static void surfaceFunctor(
                   const Volume< T >& contrastVolume,
                   const Volume< S >& labelVolume,
                   const S& background,
                   StructureBasedStatisticalMap< S, Compare >& statisticalMap );
    static void volumeFunctor(
                   const Volume< T >& contrastVolume,
                   const Volume< S >& labelVolume,
                   const S& background,
                   StructureBasedStatisticalMap< S, Compare >& statisticalMap );

  protected:

    S _background;
    StructureBasedStatisticalMap< S, Compare > _statisticalMap;

};


}


#endif
