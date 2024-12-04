#ifndef _gkg_processing_regiongrowing_RegionGrowingEnergy_i_h_
#define _gkg_processing_regiongrowing_RegionGrowingEnergy_i_h_


#include <gkg-processing-regiongrowing/RegionGrowingEnergy.h>
#include <gkg-processing-regiongrowing/RegionGrowingPotential_i.h>


template < class L >
inline
gkg::RegionGrowingEnergy< L >::RegionGrowingEnergy()
{
}


template < class L >
inline
gkg::RegionGrowingEnergy< L >::~RegionGrowingEnergy()
{
}


template < class L >
inline
void gkg::RegionGrowingEnergy< L >::addPotential(
                  const std::string& contrastName,
                  const std::string& potentialName,
                  gkg::RCPointer< gkg::RegionGrowingPotential< L > > potential,
                  double weight )
{

  try
  {

    if ( _potentials.find( contrastName ) != _potentials.end() )
    {

      if ( _potentials[ contrastName ].find( potentialName ) !=
            _potentials[ contrastName ].end() )
      {

        throw std::runtime_error(
               "contrast/potential pair already present in map of potentials" );

      }

    }
    _potentials[ contrastName ][ potentialName ] = potential;
    _weights[ contrastName ][ potentialName ] = weight;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingEnergy< L >::addPotential( "
             "const std::string& contrastName, "
             "const std::string& potentialName, "
             "gkg::RCPointer< gkg::RegionGrowingPotential< L > > potential, "
             "double weight )" );

}


template < class L >
inline
std::list< std::string > gkg::RegionGrowingEnergy< L >::getContrastNames() const
{

  try
  {

    std::list< std::string > contrastNames;

    typename 
    std::map<
      std::string,
      std::map< std::string,
                gkg::RCPointer< gkg::RegionGrowingPotential< L > >
              > >::const_iterator
      c = _potentials.begin(),
      ce = _potentials.end();
    while ( c != ce )
    {

      contrastNames.push_back( c->first );
      ++ c;

    }
    return contrastNames;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "std::list< std::string > "
             "gkg::RegionGrowingEnergy< L >::getContrastNames() const" );

}


template < class L >
inline
std::list< std::string > gkg::RegionGrowingEnergy< L >::getPotentialNames(
                                        const std::string& contrastName ) const
{

  try
  {

    std::list< std::string > potentialNames;

    typename 
    std::map< std::string,
      std::map< std::string,
                gkg::RCPointer< gkg::RegionGrowingPotential< L > >
              > >::const_iterator
      c = _potentials.find( contrastName );

    if ( c == _potentials.end() )
    {

      throw std::runtime_error( "contrast name not found" );

    }
    typename std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                     >::const_iterator
      p = c->second.begin(),
      pe = c->second.end();
    while ( p != pe )
    {

      potentialNames.push_back( p->first );
      ++ p;

    }
    return potentialNames;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "std::list< std::string > gkg::RegionGrowingEnergy< L >::"
             "getPotentialNames( "
             "const std::string& contrastName ) const" );

}


template < class L >
inline
gkg::RCPointer< gkg::RegionGrowingPotential< L > >
gkg::RegionGrowingEnergy< L >::getPotential(
                                const std::string& contrastName,
                                const std::string& potentialName ) const
{

  try
  {

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c = _potentials.find( contrastName );

    if ( c == _potentials.end() )
    {

      throw std::runtime_error( "contrast name not found" );

    }
    typename std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                     >::const_iterator
      p = c->second.find( potentialName );
    if ( p == c->second.end() )
    {

      throw std::runtime_error( "potential name not found" );

    }
    return p->second;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "gkg::RCPointer< gkg::RegionGrowingPotential< L > > "
             "gkg::RegionGrowingEnergy< L >::getPotential( "
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );

}


template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getWeight(
                                    const std::string& contrastName,
                                    const std::string& potentialName ) const
{

  try
  {

    std::map< std::string, std::map< std::string, double > >::const_iterator
      c = _weights.find( contrastName );

    if ( c == _weights.end() )
    {

      throw std::runtime_error( "contrast name not found" );

    }
    typename std::map< std::string, double >::const_iterator
      w = c->second.find( potentialName );
    if ( w == c->second.end() )
    {

      throw std::runtime_error( "potential name not found" );

    }
    return w->second;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::getWeight( "
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );

}


template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getValue() const 
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.begin(),
      c1e = _potentials.end();
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.begin();

    while ( c1 != c1e )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.begin(),
        pe = c1->second.end();
      std::map< std::string, double >::const_iterator
        w = c2->second.begin();
      while ( p != pe )
      {

        value += w->second * p->second->getValue();
        ++ p;
        ++ w;

      }
      ++ c1;
      ++ c2;

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::getValue() const" );

}


template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getDeltaValue() const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.begin(),
      c1e = _potentials.end();
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.begin();

    while ( c1 != c1e )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.begin(),
        pe = c1->second.end();
      std::map< std::string, double >::const_iterator
        w = c2->second.begin();
      while ( p != pe )
      {

        deltaValue += w->second * p->second->getDeltaValue();
        ++ p;
        ++ w;

      }
      ++ c1;
      ++ c2;

    }
    return deltaValue; 

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::getDeltaValue() const" );

}


//
// looking at the global energy for a given label
//

template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getValueForLabel( const L& label ) const
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.begin(),
      c1e = _potentials.end();
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.begin();

    while ( c1 != c1e )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.begin(),
        pe = c1->second.end();
      std::map< std::string, double >::const_iterator
        w = c2->second.begin();
      while ( p != pe )
      {

        if ( p->second->hasMeasureForLabel( label ) )
        {

          value += w->second * p->second->getWeightedValueForLabel( label );

        }
        ++ p;
        ++ w;

      }
      ++ c1;
      ++ c2;

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::getValueForLabel( "
             "const L& label ) "
             "const" );
}


template < class L >
inline
double 
gkg::RegionGrowingEnergy< L >::getDeltaValueForLabel( const L& label ) const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.begin(),
      c1e = _potentials.end();
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.begin();

    while ( c1 != c1e )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.begin(),
        pe = c1->second.end();
      std::map< std::string, double >::const_iterator
        w = c2->second.begin();
      while ( p != pe )
      {

        if ( p->second->hasMeasureForLabel( label ) )
        {

          deltaValue += w->second *
                        p->second->getWeightedDeltaValueForLabel( label );

        }
        ++ p;
        ++ w;

      }
      ++ c1;
      ++ c2;

    }
    return deltaValue; 

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::getDeltaValueForLabel( "
             "const L& label ) "
             "const" );

}


//
// looking at the global energy for a given potential
//

template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getValueForPotential(
                                        const std::string& potentialName ) const
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.begin(),
      c1e = _potentials.end();
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.begin();

    while ( c1 != c1e )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      std::map< std::string, double >::const_iterator
        w = c2->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        value += w->second * p->second->getValue();

      }
      ++ c1;
      ++ c2;

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::getValueForPotential( "
             "const std::string& potentialName ) const" );

}


template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getDeltaValueForPotential(
                                        const std::string& potentialName ) const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.begin(),
      c1e = _potentials.end();
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.begin();

    while ( c1 != c1e )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      std::map< std::string, double >::const_iterator
        w = c2->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        deltaValue += w->second * p->second->getDeltaValue();

      }
      ++ c1;
      ++ c2;

    }
    return deltaValue;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::getDeltaValueForPotential( "
             "const std::string& potentialName ) const" );

}


//
// looking at the global energy for a given contrast
//

template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getWeightedValueForContrast(
                                         const std::string& contrastName ) const
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.begin(),
        pe = c1->second.end();
      std::map< std::string, double >::const_iterator
        w = c2->second.begin();
      while ( p != pe )
      {

        value += w->second * p->second->getValue();
        ++ p;
        ++ w;

      }
      ++ c1;
      ++ c2;

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getWeightedValueForContrast( "
             "const std::string& contrastName ) const" );

}


template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getWeightedDeltaValueForContrast(
                                        const std::string& contrastName ) const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.begin(),
        pe = c1->second.end();
      std::map< std::string, double >::const_iterator
        w = c2->second.begin();
      while ( p != pe )
      {

        deltaValue += w->second * p->second->getDeltaValue();
        ++ p;
        ++ w;

      }
      ++ c1;
      ++ c2;

    }
    return deltaValue;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getWeightedDeltaValueForContrast( "
             "const std::string& contrastName ) const" );

}


//
// looking at a given potential for a given contrast
//


template < class L >
inline
void gkg::RegionGrowingEnergy< L >::initializeForContrastAndPotential(
                                              const std::string& contrastName,
                                              const std::string& potentialName )
{

  try
  {

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::iterator
      c1 = _potentials.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::iterator
        p = c1->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        p->second->initialize();

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingEnergy< L >::"
             "initializeForContrastAndPotential( "
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );

}


template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getWeightedValueForContrastAndPotential(
                                        const std::string& contrastName,
                                        const std::string& potentialName ) const
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      std::map< std::string, double >::const_iterator
        w = c2->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        value += w->second * p->second->getValue();

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getWeightedValueForContrastAndPotential("
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );


}


template < class L >
inline
double gkg::RegionGrowingEnergy< L >::getUnweightedValueForContrastAndPotential(
                                        const std::string& contrastName,
                                        const std::string& potentialName ) const
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        value += p->second->getValue();

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getUnweightedValueForContrastAndPotential( "
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );

}


template < class L >
inline
double 
gkg::RegionGrowingEnergy< L >::getWeightedDeltaValueForContrastAndPotential(
                                       const std::string& contrastName,
                                       const std::string& potentialName ) const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      std::map< std::string, double >::const_iterator
        w = c2->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        deltaValue += w->second * p->second->getDeltaValue();

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return deltaValue;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getWeightedDeltaValueForContrastAndPotential("
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );


}


template < class L >
inline
double 
gkg::RegionGrowingEnergy< L >::getUnweightedDeltaValueForContrastAndPotential(
                                      const std::string& contrastName,
                                      const std::string& potentialName ) const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        deltaValue += p->second->getDeltaValue();

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return deltaValue;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getUnweightedDeltaValueForContrastAndPotential( "
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );

}


//
// looking at a given potential for a given label and for a given contrast
//

template < class L >
inline
void gkg::RegionGrowingEnergy< L >::initializeForContrastAndPotentialAndLabel(
                                               const std::string& contrastName,
                                               const std::string& potentialName,
                                               const L& label )
{

  try
  {

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::iterator
      c1 = _potentials.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::iterator
        p = c1->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        p->second->initialize( label );

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingEnergy< L >::"
             "initializeForContrastAndPotential( "
             "const std::string& contrastName, "
             "const std::string& potentialName ) const" );

}


template < class L >
inline
double 
gkg::RegionGrowingEnergy< L >::getWeightedValueForContrastAndPotentialAndLabel(
                                            const std::string& contrastName,
                                            const std::string& potentialName,
                                            const L& label ) const
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      std::map< std::string, double >::const_iterator
        w = c2->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        if ( p->second->hasMeasureForLabel( label ) )
        {

          value += w->second * p->second->getWeightedValueForLabel( label );

        }

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getWeightedValueForContrastAndPotentialAndLabel( "
             "const std::string& contrastName, "
             "const std::string& potentialName, "
             "const L& label ) const" );

}


template < class L >
inline
double 
gkg::RegionGrowingEnergy< L >::
                          getUnweightedValueForContrastAndPotentialAndLabel(
                                            const std::string& contrastName,
                                            const std::string& potentialName,
                                            const L& label ) const
{

  try
  {

    double value = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        if ( p->second->hasMeasureForLabel( label ) )
        {

          value += p->second->getUnweightedValueForLabel( label );

        }

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return value;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getUnweightedValueForContrastAndPotentialAndLabel( "
             "const std::string& contrastName, "
             "const std::string& potentialName, "
             "const L& label ) const" );

}


template < class L >
inline
double 
gkg::RegionGrowingEnergy< L >::
                         getWeightedDeltaValueForContrastAndPotentialAndLabel(
                                            const std::string& contrastName,
                                            const std::string& potentialName,
                                            const L& label ) const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );
    std::map< std::string, std::map< std::string, double > >::const_iterator
      c2 = _weights.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      std::map< std::string, double >::const_iterator
        w = c2->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        if ( p->second->hasMeasureForLabel( label ) )
        {

          deltaValue += w->second *
                        p->second->getWeightedDeltaValueForLabel( label );

        }

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return deltaValue;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getWeightedDeltaValueForContrastAndPotentialAndLabel( "
             "const std::string& contrastName, "
             "const std::string& potentialName, "
             "const L& label ) const" );

}


template < class L >
inline
double 
gkg::RegionGrowingEnergy< L >::
                        getUnweightedDeltaValueForContrastAndPotentialAndLabel(
                                            const std::string& contrastName,
                                            const std::string& potentialName,
                                            const L& label ) const
{

  try
  {

    double deltaValue = 0.0;

    typename 
    std::map< std::string,
              std::map< std::string,
                       gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                      > >::const_iterator
      c1 = _potentials.find( contrastName );

    if ( c1 != _potentials.end() )
    {

      typename std::map< std::string,
                         gkg::RCPointer< gkg::RegionGrowingPotential< L > >
                       >::const_iterator
        p = c1->second.find( potentialName );
      if ( p != c1->second.end() )
      {

        if ( p->second->hasMeasureForLabel( label ) )
        {

          deltaValue += p->second->getUnweightedDeltaValueForLabel( label );

        }

      }
      else
      {

         throw std::runtime_error( "potential name not found" );

      }

    }
    else
    {

       throw std::runtime_error( "contrast name not found" );

    }
    return deltaValue;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "double gkg::RegionGrowingEnergy< L >::"
             "getUnweightedDeltaValueForContrastAndPotentialAndLabel( "
             "const std::string& contrastName, "
             "const std::string& potentialName, "
             "const L& label ) const" );

}


template < class L >
inline
void 
gkg::RegionGrowingEnergy< L >::displayCurrentEnergy( int32_t iteration ) const
{

  try
  {

    std::list< std::string > contrastNames = this->getContrastNames();
    std::list< std::string >::const_iterator
      c = contrastNames.begin(),
      ce = contrastNames.end();

    // time
    std::cout << iteration + 1 << " | " << std::flush;

    // global energy
    std::cout << this->getValue() << " | " << std::flush;

    // global energy for each contrast
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::cout << this->getWeightedValueForContrast( *c ) 
                << " | " << std::flush;
      ++ c;

    }

    // unweighted energy for each contrast and potential
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::list< std::string > potentialNames = this->getPotentialNames( *c );
      std::list< std::string >::const_iterator
        p = potentialNames.begin(),
        pe = potentialNames.end();
      while ( p != pe )
      {

        std::cout << this->getUnweightedValueForContrastAndPotential( *c, *p )
                  << " | " << std::flush;
        ++ p;

      }
      ++ c;

    }

    std::cout << std::endl;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingEnergy< L >::displayCurrentEnergy( "
             "int32_t iteration ) const" );

}




template < class L >
inline
void 
gkg::RegionGrowingEnergy< L >::writeMatPlotLibProlog( std::ostream& os ) const
{

  try
  {

    os << "from pylab import *" << std::endl;
    os << std::endl;
    os << "matplotlib.use( 'PS' )" << std::endl;
    os << std::endl;
    os << "yLabelToRankLut={" << std::endl;

    std::list< std::string > contrastNames = this->getContrastNames();
    std::list< std::string >::const_iterator
      c = contrastNames.begin(),
      ce = contrastNames.end();

    int32_t rank = 1;
    // global energy
    os << "                  'E':"
       << rank << ", " << std::endl;
    ++ rank;

    // global energy for each contrast
    c = contrastNames.begin();
    while ( c != ce )
    {

      os << "                  'E("
         << *c << ")':" << rank << ", " << std::endl;
      ++ rank;
      ++ c;

    }

    // unweighted energy for each contrast and potential
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::list< std::string > potentialNames = this->getPotentialNames( *c );
      std::list< std::string >::const_iterator
        p = potentialNames.begin(),
        pe = potentialNames.end();
      while ( p != pe )
      {

        os << "                  'E("
           << *c << "," << *p << ")':" << rank << ", " << std::endl;
        ++ rank;
        ++ p;

      }
      ++ c;

    }

    // unweighted energy for each contrast / potential / label
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::list< std::string > potentialNames = this->getPotentialNames( *c );
      std::list< std::string >::const_iterator
        p = potentialNames.begin(),
        pe = potentialNames.end();
      while ( p != pe )
      {

        std::list< L >
          labels = this->getPotential( *c, *p )->getLabels();
        typename std::list< L >::const_iterator
          l = labels.begin(),
          le = labels.end();
        while ( l != le )
        {

          os << "                  'E("
             << *c << "," << *p << "," << *l << ")':" << rank << ", "
             << std::endl;
          ++ rank;
          ++ l;

        }
        ++ p;

      }
      ++ c;

    }
    os << "}" << std::endl;
    os << std::endl;
    os << "yLabelNames = {}" << std::endl;
    os << "for k, v in yLabelToRankLut.iteritems():" << std::endl;
    os << "  yLabelNames[ v ] = k" << std::endl;
    os << std::endl;
    os << "table=[" << std::endl;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingEnergy< L >::writeMatPlotLibProlog( "
             "std::ostream& os ) const" );

}


template < class L >
inline
void 
gkg::RegionGrowingEnergy< L >::writeMatPlotLibEpilog( std::ostream& os ) const
{

  try
  {

    os << "]" << std::endl;
    os << std::endl;
    os << "yLabel='E'" << std::endl;
    os << std::endl;

    os << "xs=[]" << std::endl;
    os << "ys=[]" << std::endl;
    os << "for i in range( 0, len( table ) - 1 ):" << std::endl;
    os << "  xs.append( table[ i ][ 0 ] )" << std::endl;
    os << "  ys.append( table[ i ][ yLabelToRankLut[ yLabel ] ] )"
       << std::endl;
    os << std::endl;

    os << std::endl;
    os << "grid(True)" << std::endl;
    os << "gca().yaxis.grid(True, which='minor')" << std::endl;
    os << "xlabel( 'iteration' )" << std::endl;
    os << "ylabel( yLabelNames[ yLabelToRankLut[ yLabel ] ] )" << std::endl;
    os << std::endl;
    os << "plot( xs, ys )" << std::endl;
    os << std::endl;
    os << "show()" << std::endl;
    os << std::endl;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingEnergy< L >::writeMatPlotLibEpilog( "
             "std::ostream& os ) const" );

}


template < class L >
inline
void gkg::RegionGrowingEnergy< L >::writeMatPlotLibCurrentEnergy(
                                              int32_t iteration,
                                              std::ostream& valueOs,
                                              std::ostream& deltaValueOs ) const
{

  try
  {

    std::list< std::string > contrastNames = this->getContrastNames();
    std::list< std::string >::const_iterator
      c = contrastNames.begin(),
      ce = contrastNames.end();

    ////////////////////////////////////////////////////////////////////////////
    // storing value(s)
    ////////////////////////////////////////////////////////////////////////////

    valueOs << "[ " << std::flush;

    // time
    valueOs << iteration << ", " << std::flush;

    // global energy
    valueOs << this->getValue() << ", " << std::flush;

    // global energy for each contrast
    c = contrastNames.begin();
    while ( c != ce )
    {

      valueOs << this->getWeightedValueForContrast( *c ) 
              << ", " << std::flush;
      ++ c;

    }

    // unweighted energy for each contrast and potential
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::list< std::string > potentialNames = this->getPotentialNames( *c );
      std::list< std::string >::const_iterator
        p = potentialNames.begin(),
        pe = potentialNames.end();
      while ( p != pe )
      {

        valueOs << this->getUnweightedValueForContrastAndPotential( *c, *p )
                << ", " << std::flush;
        ++ p;

      }
      ++ c;

    }

    // unweighted energy for each contrast / potential / label
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::list< std::string > potentialNames = this->getPotentialNames( *c );
      std::list< std::string >::const_iterator
        p = potentialNames.begin(),
        pe = potentialNames.end();
      while ( p != pe )
      {

        std::list< L >
          labels = this->getPotential( *c, *p )->getLabels();
        typename std::list< L >::const_iterator
          l = labels.begin(),
          le = labels.end();
        while ( l != le )
        {

          valueOs << this->getUnweightedValueForContrastAndPotentialAndLabel(
                                                                *c, *p, *l )
                  << ", " << std::flush;
          ++ l;

        }
        ++ p;

      }
      ++ c;

    }

    valueOs << "], " << std::endl;


    ////////////////////////////////////////////////////////////////////////////
    // storing delta value(s)
    ////////////////////////////////////////////////////////////////////////////

    deltaValueOs << "[ " << std::flush;

    // time
    deltaValueOs << iteration << ", " << std::flush;

    // global energy variation
    deltaValueOs << this->getDeltaValue() << ", " << std::flush;

    // global energy variation for each contrast
    c = contrastNames.begin();
    while ( c != ce )
    {

      deltaValueOs << this->getWeightedDeltaValueForContrast( *c ) 
                   << ", " << std::flush;
      ++ c;

    }

    // unweighted energy variation for each contrast and potential
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::list< std::string > potentialNames = this->getPotentialNames( *c );
      std::list< std::string >::const_iterator
        p = potentialNames.begin(),
        pe = potentialNames.end();
      while ( p != pe )
      {

        deltaValueOs << this->getUnweightedDeltaValueForContrastAndPotential(
                                                                        *c, *p )
                     << ", " << std::flush;
        ++ p;

      }
      ++ c;

    }

    // unweighted energy variation for each contrast / potential / label
    c = contrastNames.begin();
    while ( c != ce )
    {

      std::list< std::string > potentialNames = this->getPotentialNames( *c );
      std::list< std::string >::const_iterator
        p = potentialNames.begin(),
        pe = potentialNames.end();
      while ( p != pe )
      {

        std::list< L >
          labels = this->getPotential( *c, *p )->getLabels();
        typename std::list< L >::const_iterator
          l = labels.begin(),
          le = labels.end();
        while ( l != le )
        {

          deltaValueOs <<
                  this->getUnweightedDeltaValueForContrastAndPotentialAndLabel(
                                                                    *c, *p, *l )
                       << ", " << std::flush;
          ++ l;

        }
        ++ p;

      }
      ++ c;

    }

    deltaValueOs << "], " << std::endl;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingEnergy< L >::writeMatPlotLibCurrentEnergy( "
             "int32_t iteration, "
             "std::ostream& valueOs, "
             "std::ostream& deltaValueOs ) const" );

}


#endif
