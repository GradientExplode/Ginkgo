#ifndef _gkg_processing_container_FrontEvolutionRoi_i_h_
#define _gkg_processing_container_FrontEvolutionRoi_i_h_


#include <gkg-processing-container/FrontEvolutionRoi.h>
#include <gkg-processing-container/Roi_i.h>
#include <gkg-processing-container/Front_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <list>


template < class T >
inline
gkg::FrontEvolutionRoi< T >::FrontEvolutionRoi(
                             gkg::RCPointer< gkg::Volume< T > > labelVolume,
                             const T& label,
                             int32_t stride )
                            : gkg::Roi< T >( labelVolume, label, stride ),
                              _front( labelVolume, label, stride )
{
}

template < class T >
inline
gkg::FrontEvolutionRoi< T >::FrontEvolutionRoi(
                                     const gkg::FrontEvolutionRoi< T >& other )
                            : gkg::Roi< T >( other ),
                              _front( other._front )
{
}


template < class T >
inline
gkg::FrontEvolutionRoi< T >::~FrontEvolutionRoi()
{
}


template < class T >
inline
const gkg::Front< T >& gkg::FrontEvolutionRoi< T >::getFront() const
{

  return _front;

}


template < class T >
inline
void gkg::FrontEvolutionRoi< T >::evaluateSiteAddition(
                                         const gkg::Vector3d< int32_t >& site )
{

  try
  {

    typename std::map< std::string,
                       gkg::RCPointer< gkg::RoiMeasure< T > > >::iterator
      m = this->_measures.begin(),
      me = this->_measures.end();

    while ( m != me )
    {

      m->second->processNewValue( site, true );
      ++ m;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontEvolutionRoi< T >::evaluateSiteAddition( "
             "const gkg::Vector3d< int32_t >& site )" );

}


template < class T >
inline
void gkg::FrontEvolutionRoi< T >::evaluateSiteRemoval(
                                         const gkg::Vector3d< int32_t >& site )
{

  try
  {

    typename std::map< std::string,
                       gkg::RCPointer< gkg::RoiMeasure< T > > >::iterator
      m = this->_measures.begin(),
      me = this->_measures.end();

    while ( m != me )
    {

      m->second->processNewValue( site, false );
      ++ m;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontEvolutionRoi< T >::evaluateSiteRemoval( "
             "const gkg::Vector3d< int32_t >& site )" );

}


template < class T >
inline
void gkg::FrontEvolutionRoi< T >::resetEvaluation()
{

  try
  {

    typename std::map< std::string,
                       gkg::RCPointer< gkg::RoiMeasure< T > > >::iterator
      m = this->_measures.begin(),
      me = this->_measures.end();

    while ( m != me )
    {

      m->second->resetNewValue();
      ++ m;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontEvolutionRoi< T >::resetEvaluation()" );

}


template < class T >
inline
void gkg::FrontEvolutionRoi< T >::addSite(
                                         const gkg::Vector3d< int32_t >& site )
{

  try
  {

    // effectively adding site
    _front.addSite( site );

    // and modifying measure(s)
    typename std::map< std::string,
                       gkg::RCPointer< gkg::RoiMeasure< T > > >::iterator
      m = this->_measures.begin(),
      me = this->_measures.end();

    while ( m != me )
    {

      m->second->processNewValue( site, true );
      m->second->applyNewValue();
      ++ m;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontEvolutionRoi< T >::addSite( "
             "const gkg::Vector3d< int32_t >& site )" );

}


template < class T >
inline
void gkg::FrontEvolutionRoi< T >::removeSite(
                                         const gkg::Vector3d< int32_t >& site )
{

  try
  {

    // effectively adding site
    _front.removeSite( site );

    // and modifying measure(s)
    typename std::map< std::string,
                       gkg::RCPointer< gkg::RoiMeasure< T > > >::iterator
      m = this->_measures.begin(),
      me = this->_measures.end();

    while ( m != me )
    {

      m->second->processNewValue( site, false );
      m->second->applyNewValue();
      ++ m;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontEvolutionRoi< T >::removeSite( "
             "const gkg::Vector3d< int32_t >& site )" );

}


template < class T >
inline
void gkg::FrontEvolutionRoi< T >::getRandomSite( 
                                         gkg::Vector3d< int32_t >& site ) const
{

  try
  {

    _front.getRandomSite( site );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::FrontEvolutionRoi< T >::getRandomSite( "
             "gkg::Vector3d< int32_t >& site ) const" );

}


#endif
