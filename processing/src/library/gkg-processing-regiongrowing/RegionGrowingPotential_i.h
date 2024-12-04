#ifndef _gkg_processing_regiongrowing_RegionGrowingPotential_i_h_
#define _gkg_processing_regiongrowing_RegionGrowingPotential_i_h_


#include <gkg-processing-regiongrowing/RegionGrowingPotential.h>
#include <gkg-processing-container/RoiMeasure_i.h>


template < class L >
inline
gkg::RegionGrowingPotential< L >::RegionGrowingPotential()
{
}


template < class L >
inline
gkg::RegionGrowingPotential< L >::~RegionGrowingPotential()
{
}


template < class L >
inline
void gkg::RegionGrowingPotential< L >::addRoiMeasure(
                             gkg::RCPointer< gkg::RoiMeasure< L > > roiMeasure,
                             gkg::RCPointer< gkg::PhiFunction > phiFunction,
                             double weight )
{

  try
  {

    _measures[ roiMeasure->getRoi()->getLabel() ] = roiMeasure;
    _phiFunctions[ roiMeasure->getRoi()->getLabel() ] = phiFunction;
    _weights[ roiMeasure->getRoi()->getLabel() ] = weight;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingPotential< L >::addRoiMeasure( "
             "gkg::RCPointer< gkg::RoiMeasure< L > > roiMeasure,"
             "gkg::RCPointer< gkg::PhiFunction > phiFunction, "
             "double weight )" );

}


template < class L >
inline
void gkg::RegionGrowingPotential< L >::initialize()
{

  try
  {

    typename std::map< L, RCPointer< RoiMeasure< L > > >::iterator
      m = _measures.begin(),
      me = _measures.end();
    while ( m != me )
    {

      m->second->initialize();
      ++ m;

    }

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingPotential< L >::initialize()" );

}


template < class L >
inline
void gkg::RegionGrowingPotential< L >::initialize( const L& label )
{

  try
  {

    typename std::map< L, RCPointer< RoiMeasure< L > > >::iterator
      m = _measures.find( label );

    if ( m == _measures.end() )
    {

      throw std::runtime_error( "label not found" );

    }
    m->second->initialize();

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingPotential< L >::initialize( "
             "const L& label )" );

}


template < class L >
inline
std::list< L > gkg::RegionGrowingPotential< L >::getLabels() const
{

  try
  {

    std::list< L > labels;

    typename std::map< L, double >::const_iterator
      w = _weights.begin(),
      we = _weights.end();
    while ( w != we )
    {

      labels.push_back( w->first );
      ++ w;

    }
    return labels;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "std::list< L > "
             "gkg::RegionGrowingPotential< L >::getLabels() const" );

}


template < class L >
inline
bool 
gkg::RegionGrowingPotential< L >::hasMeasureForLabel( const L& label ) const
{

  try
  {

    return ( _measures.find( label ) != _measures.end() );

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "bool gkg::RegionGrowingPotential< L >::hasMeasureForLabel( "
             "const L& label ) const" );

}


template < class L >
inline
gkg::RCPointer< gkg::RoiMeasure< L > >
gkg::RegionGrowingPotential< L >::getRoiMeasure( const L& label ) const
{

  try
  {

    typename
    std::map< L, gkg::RCPointer< gkg::RoiMeasure< L > > >::const_iterator
      m = _measures.find( label );

    if ( m != _measures.end() )
    {

      return m->second;

    }
    else
    {

      throw std::runtime_error( "no roi measure found for the label" );

    }

  }
  GKG_CATCH( "template < class L > "
             "inline " 
             "gkg::RCPointer< gkg::RoiMeasure< L > > "
             "gkg::RegionGrowingPotential< L >::getRoiMeasure( "
            "const L& label ) "
             "const" );

}


template < class L >
inline
gkg::RCPointer< gkg::PhiFunction > 
gkg::RegionGrowingPotential< L >::getPhiFunction( const L& label ) const
{

  try
  {

    typename
    std::map< L, gkg::RCPointer< gkg::PhiFunction > >::const_iterator 
      p = _phiFunctions.find( label );

    if ( p != _phiFunctions.end() )
    {

      return p->second;

    }
    else
    {

      throw std::runtime_error( "no phi function found for the label" );

    }

  }
  GKG_CATCH( "template < class L > "
             "inline " 
             "gkg::RCPointer< gkg::PhiFunction > "
             "gkg::RegionGrowingPotential< L >::getPhiFunction( "
             "const L& label ) "
             "const" );

}


template < class L >
inline
double 
gkg::RegionGrowingPotential< L >::getWeight( const L& label ) const
{

  try
  {

    typename
    std::map< L, double >::const_iterator 
      w = _weights.find( label );

    if ( w != _weights.end() )
    {

      return w->second;

    }
    else
    {

      throw std::runtime_error( "no weight found for the label" );

    }

  }
  GKG_CATCH( "template < class L > "
             "inline " 
             "double "
             "gkg::RegionGrowingPotential< L >::getWeight( const L& label ) "
             "const" );

}


template < class L >
inline
double gkg::RegionGrowingPotential< L >::getValue() const
{

  try
  {

    double value = 0.0;
    typename
    std::map< L, gkg::RCPointer< gkg::RoiMeasure< L > > >::const_iterator
      m = _measures.begin(),
      me = _measures.end();
    typename
    std::map< L, gkg::RCPointer< gkg::PhiFunction > >::const_iterator
      p = _phiFunctions.begin();
    typename
    std::map< L, double >::const_iterator 
      w = _weights.begin();
    while ( m!= me )
    {

      value += w->second * p->second->getValue( m->second->getValue() );
      ++ m;
      ++ p;
      ++ w;

    }
    return value; 

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingPotential< L >::getValue() const" );

}


template < class L >
inline
double gkg::RegionGrowingPotential< L >::getDeltaValue() const
{

  try
  {

    double deltaValue = 0.0;
    typename
    std::map< L, gkg::RCPointer< gkg::RoiMeasure< L > > >::const_iterator 
      m = _measures.begin(),
      me = _measures.end();
    typename
    std::map< L, gkg::RCPointer< gkg::PhiFunction > >::const_iterator 
      p = _phiFunctions.begin();
    typename
    std::map< L, double >::const_iterator 
      w = _weights.begin();
    while( m!= me )
    {

      deltaValue +=
        w->second * ( p->second->getValue( m->second->getNewValue() ) -
                      p->second->getValue( m->second->getValue() ) );
      ++ m;
      ++ p;
      ++ w;

    } 
    return deltaValue;    

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingPotential< L >::getDeltaValue() "
             "const" );

}


template < class L >
inline
double 
gkg::RegionGrowingPotential< L >::getWeightedValueForLabel(
                                                         const L& label ) const
{

  try
  {

#if defined ( GKG_DEBUG ) || defined ( GKG_DEFAULT )

    if ( !hasMeasureForLabel( label ) )
    {

      throw std::runtime_error( "no measure corresponding to label" );

    }

#endif

    typename
    std::map< L, gkg::RCPointer< gkg::RoiMeasure< L > > >::const_iterator 
      m = _measures.find( label );
    typename
    std::map< L, gkg::RCPointer< gkg::PhiFunction > >::const_iterator 
      p = _phiFunctions.find( label );
    typename
    std::map< L, double >::const_iterator 
      w = _weights.find( label );

    return w->second * p->second->getValue( m->second->getValue() );

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingPotential< L >::"
             "getWeightedValueForLabel( const L& label ) "
             "const" );

}


template < class L >
inline
double
gkg::RegionGrowingPotential< L >::getUnweightedValueForLabel( 
                                                         const L& label ) const
{

  try
  {

#if defined ( GKG_DEBUG ) || defined ( GKG_DEFAULT )

    if ( !hasMeasureForLabel( label ) )
    {

      throw std::runtime_error( "no measure corresponding to label" );

    }

#endif

    typename
    std::map< L, gkg::RCPointer< gkg::RoiMeasure< L > > >::const_iterator 
      m = _measures.find( label );
    typename
    std::map< L, gkg::RCPointer< gkg::PhiFunction > >::const_iterator 
      p = _phiFunctions.find( label );

    return p->second->getValue( m->second->getValue() );

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingPotential< L >::"
             "getUnweightedValueForLabel( const L& label ) "
             "const" );

}


template < class L >
inline
double 
gkg::RegionGrowingPotential< L >::getWeightedDeltaValueForLabel( 
                                                         const L& label ) const
{

  try
  {

#if defined ( GKG_DEBUG ) || defined ( GKG_DEFAULT )

    if ( !this->hasMeasureForLabel( label ) )
    {

      throw std::runtime_error( "no measure corresponding to label" );

    }

#endif

    typename
    std::map< L, gkg::RCPointer< gkg::RoiMeasure< L > > >::const_iterator 
      m = _measures.find( label );
    typename
    std::map< L, gkg::RCPointer< gkg::PhiFunction > >::const_iterator 
      p = _phiFunctions.find( label );
    typename
    std::map< L, double >::const_iterator 
      w = _weights.find( label );

    return w->second * ( p->second->getValue( m->second->getNewValue() ) -
                         p->second->getValue( m->second->getValue() ) );

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingPotential< L >::"
             "getWeightedDeltaValueForLabel( "
             "const L& label ) const" );

}


template < class L >
inline
double 
gkg::RegionGrowingPotential< L >::getUnweightedDeltaValueForLabel( 
                                                         const L& label ) const
{

  try
  {

#if defined ( GKG_DEBUG ) || defined ( GKG_DEFAULT )

    if ( !this->hasMeasureForLabel( label ) )
    {

      throw std::runtime_error( "no measure corresponding to label" );

    }

#endif

    typename
    std::map< L, gkg::RCPointer< gkg::RoiMeasure< L > > >::const_iterator 
      m = _measures.find( label );
    typename
    std::map< L, gkg::RCPointer< gkg::PhiFunction > >::const_iterator 
      p = _phiFunctions.find( label );

    return ( p->second->getValue( m->second->getNewValue() ) -
             p->second->getValue( m->second->getValue() ) );

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingPotential< L >::"
             "getUnweightedDeltaValueForLabel( "
             "const L& label ) const" );

}


#endif
