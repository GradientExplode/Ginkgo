#ifndef _gkg_processing_roi_FrontPottsMeasure_i_h_
#define _gkg_processing_roi_FrontPottsMeasure_i_h_

#include <gkg-processing-roi/FrontPottsMeasure.h>
#include <gkg-processing-container/RoiMeasure_i.h>
#include <gkg-processing-container/FrontEvolutionRoi_i.h>
#include <gkg-processing-roi/AtlasInformation_i.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <set>
#include <deque>
#include <utility>


#define BACKGROUND_LABEL  0
#define POTTS_INFINITY  1000.0


template < class T >
inline
gkg::FrontPottsMeasure< T >::FrontPottsMeasure( 
       gkg::RCPointer< gkg::FrontEvolutionRoi< T > > frontEvolutionRoi,
       gkg::RCPointer< gkg::AtlasInformation< std::string > > atlasInformation,
       std::map< T, gkg::RCPointer< gkg::FrontEvolutionRoi< T > > >&
                                                            frontEvolutionRois )
                            : gkg::RoiMeasure< T >( 
                                gkg::RCPointer< gkg::Roi< T > >(
                                  frontEvolutionRoi.getPointer() ) ),
                              _frontEvolutionRoi( frontEvolutionRoi ),
                              _atlasInformation( atlasInformation ),
                              _frontEvolutionRois( frontEvolutionRois )
{

  try
  {

    initialize();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::FrontPottsMeasure< T >::FrontPottsMeasure( "
             "gkg::RCPointer< gkg::FrontEvolutionRoi< T > > frontEvolutionRoi, "
             "gkg::RCPointer< gkg::AtlasInformation< std::string > > "
             "atlasInformation,"
             "std::map< T, gkg::RCPointer< gkg::FrontEvolutionRoi< T > > >&" 
             "frontEvolutionRois )" );

}


template < class T >
inline
gkg::FrontPottsMeasure< T >::~FrontPottsMeasure()
{
}


template < class T >
inline
void gkg::FrontPottsMeasure< T >::initialize()
{

  try
  {

    this->_value = 0.0;

    double stride = this->_roi->getStride();
    _strideFactor = stride * stride;

    // getting reference to label volume
    const gkg::Volume< T >& labelVolume = 
                                    *this->_frontEvolutionRoi->getLabelVolume();

    // the size of potts matrix is number of structure + 1 (for background)
    // the structure count taken from atlas information does not take into 
    // consideration the background because it is not present in the 
    // structureBasedStatisticalMap

    int32_t structureCount = _atlasInformation->getStructureCount();
    _KPotts.reallocate( structureCount + 1,
                        structureCount + 1 );
    _KPotts.fill( POTTS_INFINITY );

    // obtaining the adjacencies
    std::set< std::pair< T, T > > adjacencies;
    _atlasInformation->getLabelledRoiDictionary().getAdjacencies( adjacencies );

    // looping over adjacencies
    typename std::set< std::pair< T, T > >::const_iterator 
      a = adjacencies.begin(),
      ae = adjacencies.end();
    while ( a != ae )
    {

      _KPotts( _atlasInformation->getRank( a->first ),
               _atlasInformation->getRank( a->second ) ) = 1.0;

      _KPotts( _atlasInformation->getRank( a->first ),
               _atlasInformation->getRank( a->first ) ) = 0.0; 

      ++ a;

    }

    // building label volume bounding box    
    _boundingBox = gkg::BoundingBox< int32_t >( labelVolume );

    // getting sites of the front
    const std::deque< Vector3d< int32_t > >& sites =
      _frontEvolutionRoi->getFront().getSites();

    // getting 26-neighborhood
    _neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets( 
                                         gkg::Neighborhood3d::Neighborhood3d_26,
                                         this->_roi->getStride() );

    // looping over sites and neighbors of the front
    std::list< Vector3d< int32_t > >::const_iterator 
      n = _neighbors.begin(),
      ne = _neighbors.end();

    std::deque< Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();

    int32_t i = _atlasInformation->getRank( _frontEvolutionRoi->getLabel() );
    int32_t j = 0;

    gkg::Vector3d< int32_t > currentSite;
    while ( s != se )
    {

      n = _neighbors.begin();
      while ( n != ne )
      {

        currentSite = *s + *n;
        if ( _boundingBox.contains( currentSite ) )
        {

          j = _atlasInformation->getRank( labelVolume( currentSite ) );

          this->_value += _KPotts( i, j );

        }
        ++ n;

      }
      ++ s;

    }

    this->_value *= _strideFactor;
    this->_newValue = this->_value; 

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "void gkg::FrontPottsMeasure< T >::initialize()" );

}


template < class T >
inline
void gkg::FrontPottsMeasure< T >::processNewValue( 
                                           const gkg::Vector3d< int32_t >& site,
                                           bool addOrRemove )
{

  try
  {

    // getting reference to label volume
    const gkg::Volume< T >& labelVolume = 
                                    *this->_frontEvolutionRoi->getLabelVolume();
    T roiLabel = _frontEvolutionRoi->getLabel();
    T neighborLabel = 0;

    if ( addOrRemove )
    {

      int32_t i = _atlasInformation->getRank( roiLabel );
      int32_t j = 0;

      // looping over sites and neighbors of the front
      std::list< Vector3d< int32_t > >::const_iterator 
        n = _neighbors.begin(),
        ne = _neighbors.end();
      gkg::Vector3d< int32_t > neighbor;
      while ( n != ne )
      {

        neighbor = site + *n;
        if ( _boundingBox.contains( neighbor ) )
        {

          neighborLabel = labelVolume( neighbor );
          j = _atlasInformation->getRank( neighborLabel );

          this->_newValue += _KPotts( i, j ) * _strideFactor;

          if ( neighborLabel != BACKGROUND_LABEL )
          {

            gkg::RCPointer< gkg::FrontEvolutionRoi< T > >&
            frontEvolutionRoiOfNeighbor = _frontEvolutionRois[ neighborLabel ];
            if ( frontEvolutionRoiOfNeighbor->getFront().hasSite( neighbor ) )
            {

              gkg::FrontPottsMeasure< T >*
                frontPottsMeasureOfNeighbor =
                  static_cast< gkg::FrontPottsMeasure< T >* >(
                    frontEvolutionRoiOfNeighbor->getMeasure(
                                        "front_potts_measure" ).getPointer() );
              frontPottsMeasureOfNeighbor->setNewValue(
                              frontPottsMeasureOfNeighbor->getNewValue() +
                              _KPotts( i, j )* _strideFactor );

            }

          }

        }
        ++ n;

      }

    }
    else
    {

      int32_t i = _atlasInformation->getRank( _frontEvolutionRoi->getLabel() );
      int32_t j = 0;

      // looping over sites and neighbors of the front
      std::list< Vector3d< int32_t > >::const_iterator 
        n = _neighbors.begin(),
        ne = _neighbors.end();
      gkg::Vector3d< int32_t > neighbor;
      this->_newValue = this->_value;
      while ( n != ne )
      {

        neighbor = site + *n;
        if ( _boundingBox.contains( neighbor ) )
        {

          neighborLabel = labelVolume( neighbor );
          j = _atlasInformation->getRank( labelVolume( neighbor ) );

          this->_newValue -= _KPotts( i, j ) * _strideFactor;

          if ( neighborLabel != BACKGROUND_LABEL )
          {

            gkg::RCPointer< gkg::FrontEvolutionRoi< T > >&
            frontEvolutionRoiOfNeighbor = _frontEvolutionRois[ neighborLabel ];
            if ( frontEvolutionRoiOfNeighbor->getFront().hasSite( neighbor ) )
            {

              gkg::FrontPottsMeasure< T >*
                frontPottsMeasureOfNeighbor =
                  static_cast< gkg::FrontPottsMeasure< T >* >(
                    frontEvolutionRoiOfNeighbor->getMeasure(
                                        "front_potts_measure" ).getPointer() );
              frontPottsMeasureOfNeighbor->setNewValue(
                                frontPottsMeasureOfNeighbor->getNewValue() -
                                _KPotts( i, j )* _strideFactor );

            }

          }

        } 
        ++ n;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontPottsMeasure< T >::processNewValue( "
             "const gkg::Vector3d< int32_t >& site, "
             "bool addOrRemove )" );

}


template < class T >
inline
void gkg::FrontPottsMeasure< T >::setNewValue( double value )
{

  try
  {

    this->_newValue = value;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontPottsMeasure< T >::setNewValue( double value )" );

}


#undef BACKGROUND_LABEL
#undef POTTS_INFINITY


#endif
