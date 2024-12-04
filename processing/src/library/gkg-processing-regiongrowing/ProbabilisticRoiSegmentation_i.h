#ifndef _gkg_processing_regiongrowing_ProbabilisticRoiSegmentation_i_h_
#define _gkg_processing_regiongrowing_ProbabilisticRoiSegmentation_i_h_


#include <gkg-processing-regiongrowing/ProbabilisticRoiSegmentation.h>
#include <gkg-processing-regiongrowing/RegionGrowingAlgorithm_i.h>
#include <gkg-processing-regiongrowing/RegionGrowingEnergy_i.h>
#include <gkg-processing-regiongrowing/RegionGrowingPotential_i.h>
#include <gkg-processing-container/FrontEvolutionRoi_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-roi/AtlasInformation_i.h>
#include <gkg-processing-roi/DataAttachmentMeasure_i.h>
#include <gkg-processing-roi/VolumeMeasure_i.h>
#include <gkg-processing-roi/SurfaceMeasure_i.h>
#include <gkg-processing-roi/FrontPottsMeasure_i.h>
#include <gkg-processing-roi/ProbabilisticAtlasMeasure_i.h>
#include <gkg-processing-topology/TopologyClassifier_i.h>
#include <gkg-processing-topology/HomotopicHoleFiller_i.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-morphology/ConnectedComponents.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>


#define BACKGROUND_LABEL            0
#define BACKGROUND_CONFIDENCE_LEVEL 1.0


template < class A, class C, class L >
inline
gkg::ProbabilisticRoiSegmentation< A, C, L >::ProbabilisticRoiSegmentation(
          const std::string& structureDictionaryName,
          const std::map< std::string,
                          gkg::RCPointer< gkg::Volume< A > > >& atlasVolumes,
          const std::map< std::string,
                          gkg::RCPointer< gkg::Volume< C > > >& contrastVolumes,
          const std::map< std::string,
                          std::map< L, gkg::RCPointer< 
                             gkg::PhiFunction > > >& dataAttachmentPhiFunctions,
          const std::map< std::string,
                          std::map< L, gkg::RCPointer<
                             gkg::Transform3d< float > > > >& 
                                                      labelToAtlasTransform3ds,
          const std::map< std::string,
                          std::map< std::string, double > >& weights,
          int32_t stride,
          bool verbose )
                                             : gkg::RegionGrowingAlgorithm< L >(
                                                                              ),
                                               _structureDictionaryName(
                                                      structureDictionaryName ),
                                               _atlasVolumes( atlasVolumes ),
                                               _contrastVolumes(
                                                              contrastVolumes ),
                                               _dataAttachmentPhiFunctions(
                                                   dataAttachmentPhiFunctions ),
                                               _labelToAtlasTransform3ds(
                                                     labelToAtlasTransform3ds ),
                                               _weights( weights ),
                                               _stride( stride ),
                                               _identityPhiFunction(
                                                 new gkg::IdentityPhiFunction ),
                                               _verbose( verbose ),
                                               _randomGenerator(
                                                   gkg::RandomGenerator::Gfsr4 )
{

  try
  {

    // sanity checks : checking that all contrast volumes are in the same
    // referential (ie same size, resolution, Talairach information)
    // also checking that weights size(s) correspond to atlas & contrast size(s)
    // also checking that atlasVolumes / contrastVolumes / weights keys are the 
    // same

    // initializing the label volume
    if( _verbose )
    {

      std::cout << "initializing label volume" << std::flush;

    }
    initializeLabelVolume();
    if( _verbose )
    {

      std::cout << gkg::Eraser( 25 ) << std::flush;

    }

    // initializing energy
    if( _verbose )
    {

      std::cout << "initializing energy" << std::flush;

    }
    initializeEnergy();
    if( _verbose )
    {

      std::cout << gkg::Eraser( 19 ) << std::flush;

    }

    // setting the stride of the topological classifier
    gkg::TopologyClassifier::getInstance().setStride( _stride );

    // creating the list of the 26-neighbors
    _neighbor26s = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                     gkg::Neighborhood3d::Neighborhood3d_26, _stride );

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "ProbabilisticRoiSegmentation( "
             "const std::map< std::string, "
             "gkg::RCPointer< gkg::Volume< A > > >& atlasVolumes, "
             "const std::map< std::string, "
             "gkg::RCPointer< gkg::Volume< C > > >& contrastVolumes, "
             "const std::map< std::string, "
             "std::map< L, gkg::RCPointer< "
             "gkg::PhiFunction > > >& dataAttachmentPhiFunctions, "
             "const std::map< std::string, "
             "std::map< L, gkg::RCPointer< "
             "gkg::Transform3d< float > > > >& "
             "labelToAtlasTransform3ds, "
             "const std::map< std::string, "
             "std::map< std::string, double > >& weights, "
             "int32_t stride, "
             "bool verbose )" );

}

template < class A, class C, class L >
inline
gkg::ProbabilisticRoiSegmentation< A, C, L >::ProbabilisticRoiSegmentation(
          gkg::RCPointer< gkg::Volume< L > > labelVolume,
          const std::string& structureDictionaryName,
          const std::map< std::string,
                          gkg::RCPointer< gkg::Volume< A > > >& atlasVolumes,
          const std::map< std::string,
                          gkg::RCPointer< gkg::Volume< C > > >& contrastVolumes,
          const std::map< std::string,
                          std::map< L, gkg::RCPointer< 
                             gkg::PhiFunction > > >& dataAttachmentPhiFunctions,
          const std::map< std::string,
                          std::map< L, gkg::RCPointer<
                             gkg::Transform3d< float > > > >& 
                                                      labelToAtlasTransform3ds,
          const std::map< std::string,
                          std::map< std::string, double > >& weights,
          int32_t stride,
          bool verbose )
                                             : gkg::RegionGrowingAlgorithm< L >(
                                                                  labelVolume ),
                                               _structureDictionaryName(
                                                      structureDictionaryName ),
                                               _atlasVolumes( atlasVolumes ),
                                               _contrastVolumes(
                                                              contrastVolumes ),
                                               _dataAttachmentPhiFunctions(
                                                   dataAttachmentPhiFunctions ),
                                               _labelToAtlasTransform3ds(
                                                     labelToAtlasTransform3ds ),
                                               _weights( weights ),
                                               _stride( stride ),
                                               _identityPhiFunction(
                                                 new gkg::IdentityPhiFunction ),
                                               _verbose( verbose ),
                                               _randomGenerator(
                                                   gkg::RandomGenerator::Gfsr4 )
{

  try
  {

    // sanity checks : checking that all contrast volumes are in the same
    // referential (ie same size, resolution, Talairach information)
    // also checking that weights size(s) correspond to atlas & contrast size(s)
    // also checking that atlasVolumes / contrastVolumes / weights keys are the 
    // same

    // initializing energy
    if( _verbose )
    {

      std::cout << "initializing energy" << std::flush;

    }
    initializeEnergy();
    if( _verbose )
    {

      std::cout << gkg::Eraser( 19 ) << std::flush;

    }

    // setting the stride of the topological classifier
    gkg::TopologyClassifier::getInstance().setStride( _stride );

    // creating the list of the 26-neighbors
    _neighbor26s = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                     gkg::Neighborhood3d::Neighborhood3d_26, _stride );

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "ProbabilisticRoiSegmentation( "
             "RCPointer< Volume< L > > labelVolume"
             "const std::map< std::string, "
             "gkg::RCPointer< gkg::Volume< A > > >& atlasVolumes, "
             "const std::map< std::string, "
             "gkg::RCPointer< gkg::Volume< C > > >& contrastVolumes, "
             "const std::map< std::string, "
             "std::map< L, gkg::RCPointer< "
             "gkg::PhiFunction > > >& dataAttachmentPhiFunctions, "
             "const std::map< std::string, "
             "std::map< L, gkg::RCPointer< "
             "gkg::Transform3d< float > > > >& "
             "labelToAtlasTransform3ds, "
              "const std::map< std::string, "
             "std::map< std::string, double > >& weights, "
             "int32_t stride, "
             "bool verbose )" );

}

template < class A, class C, class L >
inline
gkg::ProbabilisticRoiSegmentation< A, C, L >::~ProbabilisticRoiSegmentation()
{
}


template < class A, class C, class L >
inline
const std::string& 
gkg::ProbabilisticRoiSegmentation< A, C, L >::getStructureDictionaryName() const
{

  try
  {

    return _structureDictionaryName;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "const std::string& "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "getStructureDictionaryName() const" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::Volume< A > >
gkg::ProbabilisticRoiSegmentation< A, C, L >::getAtlasVolume( 
                                         const std::string& contrastName ) const
{

  try
  {

    typename std::map< std::string, 
                       gkg::RCPointer< gkg::Volume< A > > >::const_iterator
      a = _atlasVolumes.find( contrastName );

    if ( a == _atlasVolumes.end() )
    {

      throw std::runtime_error( "no atlas associated to this contrast name " );

    }
    return a->second;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::RCPointer< gkg::Volume< A > > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::getAtlasVolume( "
             "const std::string& contrastName ) const" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::Volume< C > >
gkg::ProbabilisticRoiSegmentation< A, C, L >::getContrastVolume( 
                                         const std::string& contrastName ) const
{

  try
  {

    typename std::map< std::string, 
                       gkg::RCPointer< gkg::Volume< C > > >::const_iterator
      c = _contrastVolumes.find ( contrastName );

    if ( c == _contrastVolumes.end() )
    {

      throw std::runtime_error( 
                       "no contrast volume associated to this contrast name " );

    }
    return c->second;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::RCPointer< gkg::Volume< C > > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::getContrastVolume( "
             "const std::string& contrastName ) const" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::PhiFunction >
gkg::ProbabilisticRoiSegmentation< A, C, L >::getDataAttachmentPhiFunction( 
                                                const std::string& contrastName,
                                                const L& label ) const
{

  try
  {

    typename
    std::map< std::string,
              std::map< L,
                        gkg::RCPointer< gkg::PhiFunction > > >::const_iterator
      pc = _dataAttachmentPhiFunctions.find( contrastName );

    if ( pc == _dataAttachmentPhiFunctions.end() )
    {

      throw std::runtime_error(
          "no data attachment phi function associated to this contrast name " );

    }

    typename std::map< L,
                       gkg::RCPointer< gkg::PhiFunction > >::const_iterator
      p = pc->second.find( label );

    if ( p == pc->second.end() )
    {

      throw std::runtime_error( 
      "no data attachment phi function for this label in the given contrast " );

    }
    return p->second;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "gkg::RCPointer< gkg::PhiFunction > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "getDataAttachmentPhiFunction( "
             "const std::string& contrastName, "
             "const L& label ) const" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::Transform3d< float > >
gkg::ProbabilisticRoiSegmentation< A, C, L >::getLabelToAtlasTransform3d( 
                                                const std::string& contrastName,
                                                const L& label ) const
{

  try
  {

    typename
    std::map< std::string,
              std::map< L,
                        gkg::RCPointer< gkg::Transform3d< float > >
                      > >::const_iterator
      tc = _labelToAtlasTransform3ds.find( contrastName );

    if ( tc == _labelToAtlasTransform3ds.end() )
    {

      throw std::runtime_error(
                         "no 3D transform associated to this contrast name " );

    }

    typename std::map< L,
                       gkg::RCPointer< gkg::Transform3d< float > >
                     >::const_iterator
      t = tc->second.find( label );

    if ( t == tc->second.end() )
    {

      throw std::runtime_error( 
                      "no 3D transform for this label in the given contrast " );

    }
    return t->second;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "getLabelToAtlasTransform3d( "
             "const std::string& contrastName, "
             "const L& label ) const" );

}


template < class A, class C, class L >
inline
double 
gkg::ProbabilisticRoiSegmentation< A, C, L >::getWeight( 
                                        const std::string& contrastName,
                                        const std::string& potentialName ) const
{

  try
  {

    std::map< std::string, std::map< std::string, double > >::const_iterator
      w = _weights.find( contrastName );

    if ( w == _weights.end() )
    {

      throw std::runtime_error( "no weight associated to this contrast name " );

    }

    std::map< std::string, double >::const_iterator
      pw = w->second.find( potentialName );

    if ( pw == w->second.end() )
    {

      throw std::runtime_error( 
              "no weight associated to this potential in the given contrast " );

    }
    return pw->second;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "double "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::getWeight( "
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );

}


template< class A, class C, class L >
inline
int32_t gkg::ProbabilisticRoiSegmentation< A, C, L >::getStride() const
{

  return _stride;


}


template < class A, class C, class L >
inline
L gkg::ProbabilisticRoiSegmentation< A, C, L >::getLabel( int32_t rank ) const

{

  try
  {

    if ( rank >= ( int32_t )_rankToLabelLut.size() )
    {

      throw std::runtime_error( "bad rank" );

    }
    return _rankToLabelLut.find( rank )->second;

  }
  GKG_CATCH( "template < class A, class C, class L >"
             "inline"
             "L gkg::ProbabilisticRoiSegmentation< A, C, L >::getLabel(" 
             "int32_t rank ) const" ); 

}


template < class A, class C, class L >
inline
void gkg::ProbabilisticRoiSegmentation< A, C, L >::getRandomSite( 
                                gkg::Vector3d< int32_t >& site, L& label ) const

{

  try
  {

    bool suitableSiteFound = false;

    while( ! suitableSiteFound )
    {

      // selecting randomly a front and setting the label
      int32_t randomFrontRank = ( int32_t )
        gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory()->
                           getUniformRandomUInt32( _randomGenerator,
                                                   _frontEvolutionRois.size() );
      label = getLabel( randomFrontRank );

      // selecting randomly a site from the selected front
      // if the random front corresponds to the background, any site is suitable
      if ( label == BACKGROUND_LABEL )
      {

        typename 
        std::map< L,
                  gkg::RCPointer< gkg::FrontEvolutionRoi< L > > >
                                                                ::const_iterator
          f = _frontEvolutionRois.find( label );
        if ( f != _frontEvolutionRois.end() )
        {

          f->second->getRandomSite( site );
          suitableSiteFound = true;

        }
        else
        {

          throw std::runtime_error( "no front evolution ROI with given label" );

        }

      }
      // else the site has to be a simple topological point
      else
      {
 
        gkg::EqualToFunction< L > equalToFunction( label );
        bool isASimplePoint = false;
        typename 
        std::map< L,
                  gkg::RCPointer< gkg::FrontEvolutionRoi< L > > >
                                                                ::const_iterator
          f = _frontEvolutionRois.find( label );
        if ( f != _frontEvolutionRois.end() )
        {

          int32_t maximumTrialCount = f->second->getFront().getSiteCount();
          int32_t trialCount = 0;
          while ( !isASimplePoint && ( trialCount < maximumTrialCount ) )
          {

            f->second->getRandomSite( site );
            isASimplePoint = 
                 gkg::TopologyClassifier::getInstance().isSimplePoint(
                                                            *this->_labelVolume,
                                                            equalToFunction,
                                                            site );
            if( isASimplePoint )
            {

              suitableSiteFound = true;

            }
            ++ trialCount;

          }

        }
        else
        {

          throw std::runtime_error( "no front evolution ROI with given label" );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "inline "
             "getRandomSite( "
             "gkg::Vector3d< int32_t >& site, L& label ) const" ); 

}


template < class A, class C, class L >
inline
std::set< L > 
gkg::ProbabilisticRoiSegmentation< A, C, L >::getNewPutativeLabels(
                                           const gkg::Vector3d< int32_t >& site,
                                           const L& label ) const
{

  try
  {

    // allocating the set of putative new labels
    std::set< L > newPutativeLabels;

    // looping over neighbor(s)
    gkg::Vector3d< int32_t > neighbor;
    L neighborLabel;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      n = _neighbor26s.begin(),
      ne = _neighbor26s.end();
    while ( n != ne )
    {

      neighbor = site + *n;

      // checking that the neighbor is contained in the label volume bounding
      // box
      if ( this->_labelVolumeBoundingBox.contains( neighbor ) )
      {

        // obtaining the label of the current neighbor
        neighborLabel = ( *this->_labelVolume )( neighbor );

        // consider the neighbor label only if it is different from the label
        // provided as argument to the method
        if ( neighborLabel != label )
        {

          if ( neighborLabel == BACKGROUND_LABEL )
          {

            if ( isBelongingToRoiMask( neighborLabel, site ) )
            {

              newPutativeLabels.insert( neighborLabel );

            } 

          }
          else
          {

            gkg::EqualToFunction< int16_t > equalToFunction( neighborLabel );
            if ( isBelongingToRoiMask( neighborLabel, site ) && 
                 gkg::TopologyClassifier::getInstance().isSimplePoint(
                                                            *this->_labelVolume,
                                                            equalToFunction,
                                                            site ) )
            {

              newPutativeLabels.insert( neighborLabel );

            }

          }

        }

      }

      ++ n;

    }
    return newPutativeLabels;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "std::set< L > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "getNewPutativeLabels( "
             "const gkg::Vector3d< int32_t >& site, "
             "const L& label ) const" );

}


template < class A, class C, class L >
inline
bool gkg::ProbabilisticRoiSegmentation< A, C, L >::isBelongingToRoiMask(
                                    const L& label,
                                    const gkg::Vector3d< int32_t >& site ) const
{

  try
  {

    // a site belongs to a ROI mask if at least it is contained into one of
    // the ROI mask among all the atlases

    typename std::map< std::string,
                       gkg::RCPointer< gkg::Volume< A > > >::const_iterator
      a = _atlasVolumes.begin(),
      ae = _atlasVolumes.end();
    typename std::map< std::string,
                       std::map< L, int32_t > >::const_iterator
      l = _contrastAndLabelToRankInAtlas.begin();
    typename std::map< L, int32_t >::const_iterator r;
    gkg::Vector3d< int32_t > to; 
    while ( a != ae )
    {

      // finding the 3D transform map for the current contrast
      typename 
      std::map< std::string,
              std::map< L, RCPointer< Transform3d< float > > > >::const_iterator
        tm = _labelToAtlasTransform3ds.find( a->first );
      if ( tm == _labelToAtlasTransform3ds.end() )
      {

        throw std::runtime_error( "contrast name not found" );

      }

      // finding the 3D transform for the current label
      typename 
      std::map< L, RCPointer< Transform3d< float > > >::const_iterator
        t = tm->second.find( label );
      if ( t == tm->second.end() )
      {

        throw std::runtime_error( "label not found" );

      }

      // obtaining the voxel coordinate into the atlas frame
      t->second->getFullIntegerDirect( site, to );


      const std::map< L, int32_t >& labelToRankInAtlas = l->second;
      r = labelToRankInAtlas.find( label );
      if ( r != labelToRankInAtlas.end() )
      {

        if ( ( *a->second )( to, r->second ) )
        {

          return true;

        }

      }
      ++ a;
      ++ l;

    }
    return false;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "bool gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "isBelongingToRoiMask( "
             "const L& label, "
             "const gkg::Vector3d< int32_t >& site ) const" );

}


template < class A, class C, class L >
inline
double gkg::ProbabilisticRoiSegmentation< A, C, L >::getDeltaEnergy(
                                          const gkg::Vector3d< int32_t >& site,
                                          const L& oldLabel,
                                          const L& newLabel )
{

  try
  {

    // evaluating the removal of the site from the old label
    _frontEvolutionRois[ oldLabel ]->evaluateSiteRemoval( site );

    // evaluating the addition of the site to the new label
    _frontEvolutionRois[ newLabel ]->evaluateSiteAddition( site );

    // processing the energy discrepancy
    double deltaEnergy = this->_energy->getDeltaValue();

    // resetting the current evaluation
    typename std::map< L, RCPointer< FrontEvolutionRoi< L > > >::iterator
      f = _frontEvolutionRois.begin(),
      fe = _frontEvolutionRois.end();
    while ( f != fe )
    {

      f->second->resetEvaluation();
      ++ f;

    }

    return deltaEnergy;

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "double gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "getDeltaEnergy( "
             "const gkg::Vector3d< int32_t >& site, "
             "const L& oldLabel, "
             "const L& newLabel )" );

}


template < class A, class C, class L >
inline
void gkg::ProbabilisticRoiSegmentation< A, C, L >::applyModification(
                                          const gkg::Vector3d< int32_t >& site,
                                          const L& oldLabel,
                                          const L& newLabel )
{

  try
  {

    _frontEvolutionRois[ oldLabel ]->removeSite( site );
    _frontEvolutionRois[ newLabel ]->addSite( site );

    gkg::Vector3d< int32_t > offset;

    for ( offset.z = 0; offset.z < _stride; offset.z++ )
    {

      for ( offset.y = 0; offset.y < _stride; offset.y++ )
      {

        for ( offset.x = 0; offset.x < _stride; offset.x++ )
        {

          ( *this->_labelVolume )( site + offset ) = newLabel;

        }

      }

    }

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "void gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "applyModification( "
             "const gkg::Vector3d< int32_t >& site, "
             "const L& oldLabel, "
             "const L& newLabel )" );

}

template < class A, class C, class L >
inline
void gkg::ProbabilisticRoiSegmentation< A, C, L >::initializeLabelVolume()
{

  try
  {

    typename std::map< std::string,
                       gkg::RCPointer< gkg::Volume< A > > >::const_iterator
      a = _atlasVolumes.begin(),
      ae = _atlasVolumes.end();

    // in this preliminary release, we assume that all contrast volumes are
    // in the same referential, so we only need to collect information
    // considering the first contrast volume
    typename std::map< std::string,
                       gkg::RCPointer< gkg::Volume< C > > >::const_iterator
      c = _contrastVolumes.begin();

    // collecting size and resolution of the first contrast volume
    const gkg::Volume< C >& firstContrast = *( c->second );
    int32_t contrastSizeX = firstContrast.getSizeX();
    int32_t contrastSizeY = firstContrast.getSizeY();
    int32_t contrastSizeZ = firstContrast.getSizeZ();
    gkg::Vector3d< double > contrastResolution;
    firstContrast.getResolution( contrastResolution );

    // processing Talairach bounding box
    const gkg::Volume< A >& firstAtlas = *( a->second );
    gkg::BoundingBox< int32_t > atlasBoundingBox( firstAtlas );

    // allocating label volume
    this->_labelVolume = gkg::RCPointer< gkg::Volume< L > >(
                           new gkg::Volume< L >( contrastSizeX,
                                                 contrastSizeY,
                                                 contrastSizeZ ) );
    this->_labelVolume->getHeader().addAttribute( "resolutionX",
                                                  contrastResolution.x );
    this->_labelVolume->getHeader().addAttribute( "resolutionY",
                                                  contrastResolution.y );
    this->_labelVolume->getHeader().addAttribute( "resolutionZ",
                                                  contrastResolution.z );
    this->_labelVolume->fill( ( L )0 );

    // allocating the label volume bounding box
    this->_labelVolumeBoundingBox = gkg::BoundingBox< int32_t >(
                                                         *this->_labelVolume );

    // filling label volume
    gkg::Vector3d< int32_t > from;
    gkg::Vector3d< int32_t > to;
    gkg::Vector3d< int32_t > offset;
    std::set< L > labelSet;
    gkg::Volume< L > labelVolume( contrastSizeX,
                                  contrastSizeY,
                                  contrastSizeZ );
    labelVolume.fill( 0 );

    gkg::RCPointer< gkg::Transform3d< float > >
      transform3d = _labelToAtlasTransform3ds.begin()->second.begin()->second;
    for ( from.z = 0; from.z < contrastSizeZ; from.z += _stride )
    {

      for ( from.y = 0; from.y < contrastSizeY; from.y += _stride )
      {

        for ( from.x = 0; from.x < contrastSizeX; from.x += _stride )
        {

          transform3d->getFullIntegerDirect( from, to );

          if ( atlasBoundingBox.contains( to ) )
          {

            a = _atlasVolumes.begin();
            while ( a != ae )
            {

              int32_t atlasSizeT = ( * a->second ).getSizeT();
              L label = ( *a->second )( to, atlasSizeT - 1 );

              if ( label )
              {

                labelSet.insert( label );
                for ( offset.z = 0; offset.z < _stride; offset.z++ )
                {

                  for ( offset.y = 0; offset.y < _stride; offset.y++ )
                  {

                    for ( offset.x = 0; offset.x < _stride; offset.x++ )
                    {

                      labelVolume( from + offset ) = label;

                    }

                  }

                }

              }
              ++ a;

            }

          }

        }

      }

    }

    // selecting the biggest connected components per roi
    typename std::set< L >::const_iterator
      l = labelSet.begin(),
      le = labelSet.end();
    gkg::Volume< L > roiVolume( contrastSizeX,
                                contrastSizeY,
                                contrastSizeZ );
    while( l != le )
    {

      gkg::EqualToFunction< L > testFunction( *l );
      gkg::ConnectedComponents< L, L > 
        connectedComponents( testFunction,
                             0,
                             1,
                             1,
                             true,
                             gkg::Neighborhood3d::Neighborhood3d_26 );
      connectedComponents.getComponents( labelVolume, roiVolume );
      for ( from.z = 0; from.z < contrastSizeZ; from.z++ )
      {

        for ( from.y = 0; from.y < contrastSizeY; from.y++ )
        {

          for ( from.x = 0; from.x < contrastSizeX; from.x++ )
          {

            if ( roiVolume( from ) )
            {

              ( * this->_labelVolume )( from ) = *l;

            }

          }

        }

      }

      // performing homotopic erosion in order to preserve the topology
      // by filling the holes inside the connected component
      gkg::HomotopicHoleFiller< L > homotopicHoleFiller( *l );
      homotopicHoleFiller.fillHoles( *this->_labelVolume );

      ++ l;

    }

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "void gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeLabelVolume()" );

}


template < class A, class C, class L >
inline
void gkg::ProbabilisticRoiSegmentation< A, C, L >::initializeEnergy()
{

  try
  {

    typename std::map< std::string,
                        gkg::RCPointer< gkg::Volume< A > > >::const_iterator
      a = _atlasVolumes.begin(),
      ae = _atlasVolumes.end();

    // looping over atlas(es) and contrast(s)
    int32_t rank = 0;
    while( a != ae )
    {

      // getting const reference to the atlas itself
      std::string contrastName = a->first;
      const gkg::Volume< A >& atlas = *a->second;

      // obtaining atlas information
      gkg::RCPointer< gkg::AtlasInformation< std::string >  >
        atlasInformation( new gkg::AtlasInformation< std::string >(
                                                       atlas,
                                                       _structureDictionaryName,
                                                       contrastName ) );

      // collecting the size of the database from which the atlas is built
      int32_t dataBaseCount = atlasInformation->getDataBaseCount();

      // obtaining a reference to the structure based statistical  map
      const gkg::StructureBasedStatisticalMap< std::string >&
        structureBasedStatisticalMap = 
           atlasInformation->getStructureBasedStatisticalMap();

      // obtaining the labelled ROI dictionary
      const gkg::LabelledRoiDictionary&
        labelledRoiDictionary = atlasInformation->getLabelledRoiDictionary();

      // obtaining the explicit dictionary
      gkg::Dictionary dictionary = labelledRoiDictionary.getDictionary();

      // defining potentials for:
      // - data attachment
      // - volume
      // - surface
      // - front Potts
      // - probabilistic atlas
      gkg::RCPointer< gkg::RegionGrowingPotential< L > >
        potentialOfDataAttachment( new gkg::RegionGrowingPotential< L > );
      gkg::RCPointer< gkg::RegionGrowingPotential< L > >
        potentialOfVolume( new gkg::RegionGrowingPotential< L > );
      gkg::RCPointer< gkg::RegionGrowingPotential< L> >
        potentialOfSurface( new gkg::RegionGrowingPotential< L > );
      gkg::RCPointer< gkg::RegionGrowingPotential< L > >
        potentialOfFrontPotts( new gkg::RegionGrowingPotential< L > );
      gkg::RCPointer< gkg::RegionGrowingPotential< L > >
        potentialOfProbabilisticAtlas(  new gkg::RegionGrowingPotential< L > );

      // looping over ROI(s) for the current atlas/contrast
      double mean = 0.0;
      double standardDeviation = 0.0;
      std::string roiName;
      gkg::Dictionary::const_iterator
        r = dictionary.begin(),
        re = dictionary.end();
      while ( r != re )
      {

        // obtaining ROI name
        roiName = r->first;

        // obtaining label for the current ROI
        L label = ( L )0;
        labelledRoiDictionary.getRoiLabel( roiName, label );

        // obtaining confidence level for the current ROI
        double confidenceLevel = labelledRoiDictionary.getConfidenceLevel(
                                                                      roiName );

        // initializing front evolution ROI for the current label
        initializeFrontEvolutionRoi( label, rank );

        // adding the convertion from the label to the rank in the atlas
        _contrastAndLabelToRankInAtlas[ contrastName ][ label ] =
                                             atlasInformation->getRank( label );

        // initializing data attachment measure for the current label
        gkg::RCPointer< gkg::FrontEvolutionRoi< L > >
          frontEvolutionRoi = _frontEvolutionRois[ label ];

        gkg::RCPointer< gkg::RoiMeasure< L > > 
           dataAttachmentMeasure = initializeDataAttachmentMeasure(
                                                                   contrastName,
                                                                   label );

        potentialOfDataAttachment->addRoiMeasure( dataAttachmentMeasure,
                                                  _identityPhiFunction,
                                                  confidenceLevel );

        frontEvolutionRoi->addMeasure( a->first + "_data_attachment_measure",
                                       dataAttachmentMeasure );

        if ( ( label != BACKGROUND_LABEL ) && ( a->first == "t1" ) )
        {

          // initializing volume measure for the current label
          mean = structureBasedStatisticalMap.getStatisticalParameterMap(
                   roiName, "volume" ).getStatisticalParameter( "mean" );
          standardDeviation = 
            structureBasedStatisticalMap.getStatisticalParameterMap(
                   roiName, "volume" ).getStatisticalParameter( 
                                                  "standard_deviation" );
          gkg::RCPointer< gkg::RoiMeasure< L > > 
            volumeMeasure = initializeVolumeMeasure( label );
          frontEvolutionRoi->addMeasure( "volume_measure", volumeMeasure );
          potentialOfVolume->addRoiMeasure(
            volumeMeasure,
            gkg::RCPointer< gkg::PhiFunction >(
              new gkg::NormalizedSquaredCenterPhiFunction( 
                                                          mean, 
                                                          standardDeviation ) ),
            confidenceLevel );

          // initializing surface measure for the current label
          mean = structureBasedStatisticalMap.getStatisticalParameterMap(
                   roiName, "surface" ).getStatisticalParameter( "mean" );
          standardDeviation = 
            structureBasedStatisticalMap.getStatisticalParameterMap(
                   roiName, "surface" ).getStatisticalParameter( 
                                                   "standard_deviation" );
          gkg::RCPointer< gkg::RoiMeasure< L > > 
            surfaceMeasure = initializeSurfaceMeasure( label );
          frontEvolutionRoi->addMeasure( "surface_measure", surfaceMeasure );
          potentialOfSurface->addRoiMeasure(
            surfaceMeasure,
            gkg::RCPointer< gkg::PhiFunction >(
              new gkg::NormalizedSquaredCenterPhiFunction( 
                                                          mean,
                                                          standardDeviation ) ),
            confidenceLevel );

          // initializing front Pott's measure for the current label
          gkg::RCPointer< gkg::RoiMeasure< L > > 
            frontPottsMeasure = initializeFrontPottsMeasure( label,
                                                             atlasInformation );
          frontEvolutionRoi->addMeasure( "front_potts_measure",
                                         frontPottsMeasure );
          potentialOfFrontPotts->addRoiMeasure( frontPottsMeasure,
                                                _identityPhiFunction,
                                                confidenceLevel );

        }

        // initializing probabilistic atlas measure for the current label
        gkg::RCPointer< gkg::RoiMeasure< L > > 
          probabilisticAtlasMeasure = initializeProbabilisticAtlasMeasure( 
                                                                label,
                                                                a->second,
                                                                contrastName,
                                                                dataBaseCount );
        frontEvolutionRoi->addMeasure( "probabilistic_atlas_measure",
                                       probabilisticAtlasMeasure );
        potentialOfProbabilisticAtlas->addRoiMeasure( probabilisticAtlasMeasure,
                                                      _identityPhiFunction,
                                                      confidenceLevel );
        ++ r;

      }

      // adding potentials for current contrast/atlas to energy
      this->_energy->addPotential(
                             contrastName,
                             "data_attachment_potential",
                             potentialOfDataAttachment,
                             getWeight( contrastName, "data_attachment" ) );

      if ( contrastName == "t1" )
      {

        this->_energy->addPotential(
                             contrastName,
                             "volume_potential",
                             potentialOfVolume,
                             getWeight( contrastName, "volume" ) );
        this->_energy->addPotential(
                             contrastName,
                             "surface_potential",
                             potentialOfSurface,
                             getWeight( contrastName, "surface" ) );
        this->_energy->addPotential(
                             contrastName,
                             "front_potts_potential",
                             potentialOfFrontPotts,
                             getWeight( contrastName, "front_potts" ) );
        this->_energy->addPotential(
                             contrastName,
                             "probabilistic_atlas_potential",
                             potentialOfProbabilisticAtlas,
                             getWeight( contrastName, "probabilistic_atlas" ) );

      }

      ++ a;

    }

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "void gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeEnergy()" );

}


template < class A, class C, class L >
inline
void gkg::ProbabilisticRoiSegmentation< A, C, L >::initializeFrontEvolutionRoi(
                                                 const L& label, int32_t& rank )
{

  try
  {

    // if the front evolution roi associated to this label is not created yet
    if ( _frontEvolutionRois.find( label ) == _frontEvolutionRois.end() )
    {

      gkg::RCPointer< gkg::FrontEvolutionRoi< L > >
        frontEvolutionRoi( new gkg::FrontEvolutionRoi< L >( this->_labelVolume,
                                                            label,
                                                            _stride ) );
      _frontEvolutionRois[ label ] = frontEvolutionRoi;
      _rankToLabelLut[ rank ] = label;
      ++ rank;

    }

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "void gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeFrontEvolutionRoi( "
             "const L& label, int32_t& rank )" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::RoiMeasure< L > > 
gkg::ProbabilisticRoiSegmentation< A, C, L >::initializeDataAttachmentMeasure(
                                                const std::string& contrastName,
                                                const L& label )
{

  try
  {

    return gkg::RCPointer< gkg::RoiMeasure< L > >(
             new gkg::DataAttachmentMeasure< L, C >( 
                        _frontEvolutionRois[ label ],
                        getContrastVolume( contrastName ),
                        getDataAttachmentPhiFunction( contrastName, label ) ) );

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::RCPointer< gkg::RoiMeasure< L > >  "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeDataAttachmentMeasure( "
             "const std::string& contrastName, "
             "const L& label )" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::RoiMeasure< L > > 
gkg::ProbabilisticRoiSegmentation< A, C, L >::initializeVolumeMeasure(
                                                                const L& label )
{

  try
  {

    return gkg::RCPointer< gkg::RoiMeasure< L > >(
             new gkg::VolumeMeasure< L >( _frontEvolutionRois[ label ] ) );

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::RCPointer< gkg::RoiMeasure > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeVolumeMeasure( "
             "const L& label )" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::RoiMeasure< L > > 
gkg::ProbabilisticRoiSegmentation< A, C, L >::initializeSurfaceMeasure(
                                                                const L& label )
{

  try
  {

    return gkg::RCPointer< gkg::RoiMeasure< L > >(
             new gkg::SurfaceMeasure< L >( _frontEvolutionRois[ label ] ) );

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::RCPointer< gkg::RoiMeasure< L > >  "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeSurfaceMeasure( const L& label )" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::RoiMeasure< L > > 
gkg::ProbabilisticRoiSegmentation< A, C, L >::initializeFrontPottsMeasure(
       const L& label,
       gkg::RCPointer< gkg::AtlasInformation< std::string > > atlasInformation )
{

  try
  {

    return gkg::RCPointer< gkg::RoiMeasure< L > >(
             new gkg::FrontPottsMeasure< L >( _frontEvolutionRois[ label ],
                                              atlasInformation,
                                              _frontEvolutionRois ) );

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::RCPointer< gkg::RoiMeasure< L > > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeFrontPottsMeasure( "
             "const L& label, "
             "gkg::RCPointer< gkg::AtlasInformation< std::string > > "
             "atlasInformation )" );

}


template < class A, class C, class L >
inline
gkg::RCPointer< gkg::RoiMeasure< L > > 
gkg::ProbabilisticRoiSegmentation< A, C, L >::
                                          initializeProbabilisticAtlasMeasure(
                                       const L& label,
                                       gkg::RCPointer< gkg::Volume< A > > atlas,
                                       const std::string& contrastName,
                                       int32_t dataBaseCount )
{

  try
  {

    return gkg::RCPointer< gkg::RoiMeasure< L > >( 
             new gkg::ProbabilisticAtlasMeasure< L, A >(
                   _frontEvolutionRois[ label ],
                   atlas,
                   _structureDictionaryName,
                   contrastName,
                   dataBaseCount,
                   this->getLabelToAtlasTransform3d( contrastName, label ) ) );

  }
  GKG_CATCH( "template < class A, class C, class L > "
             "inline "
             "gkg::RCPointer< gkg::RoiMeasure< L > > "
             "gkg::ProbabilisticRoiSegmentation< A, C, L >::"
             "initializeProbabilisticAtlasMeasure( "
             "const L& label, "
             "gkg::RCPointer< gkg::Volume< A > > atlas, "
             "const std::string& contrastName, "
             "int32_t dataBaseCount )" );

}


#endif
