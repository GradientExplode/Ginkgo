#include <gkg-graphics-X11-core/TeXCompositor.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <algorithm>


static const int32_t TOLERANCE = INT32_C( 100 );
static const float BADGSR = 4.5;

//
// class BreakSet
//

namespace gkg
{


struct BreakSet 
{

  BreakSet( BreakSet* );
  ~BreakSet();

  void addBreak( Compositor::Index index, int32_t theDemerits );
  void noBreak( FloatCoordinate theNatural,
                FloatCoordinate theStretch,
                FloatCoordinate theShrink );

  int32_t demerits;
  FloatCoordinate natural;
  FloatCoordinate stretch;
  FloatCoordinate shrink;
  Compositor::Index* breaks;
  Compositor::Index size;
  Compositor::Index count;
  BreakSet* next;
  BreakSet* previous;

};


}


gkg::BreakSet::BreakSet( gkg::BreakSet* breakSet )
              : natural( 0 ),
                stretch( 0 ),
                shrink( 0 )
{

  if ( !breakSet )
  {

    size = 20;
    breaks = new gkg::Compositor::Index[ size ];
    demerits = 0;
    count = 0;
    next = this;
    previous = this;

  }
  else
  {

    demerits = breakSet->demerits;
    size = breakSet->size;
    breaks = new gkg::Compositor::Index[ size ];
    count = breakSet->count;
    for ( gkg::Compositor::Index i = 0; i < count; i++ )
    {

      breaks[ i ] = breakSet->breaks[ i ];

    }
    next = breakSet->next;
    previous = breakSet;
    previous->next = this;
    next->previous = this;

  }

}


gkg::BreakSet::~BreakSet()
{

  previous->next = next;
  next->previous = previous;
  delete [] breaks;

}


void gkg::BreakSet::addBreak( gkg::Compositor::Index index, 
                              int32_t theDemerits )
{

  if ( count == size )
  {

    gkg::Compositor::Index theSize = size + 20;
    gkg::Compositor::Index*
      theBreaks = new gkg::Compositor::Index[ theSize ];
    for ( gkg::Compositor::Index i = 0; i < count; i++ )
    {

      theBreaks[ i ] = breaks[ i ];

    }
    delete [] breaks;
    breaks = theBreaks;
    size = theSize;

  }
  breaks[ count ] = index;
  ++ count;
  natural = 0;
  stretch = 0;
  shrink = 0;
  demerits += theDemerits;

}


inline
void gkg::BreakSet::noBreak( gkg::FloatCoordinate theNatural,
                             gkg::FloatCoordinate theStretch,
                             gkg::FloatCoordinate theShrink )
{

  natural += theNatural;
  stretch += theStretch;
  shrink += theShrink;

}


//
// inline functions
//

inline int32_t demerits( int32_t badness, int32_t penalty, int32_t linePenalty )
{

  int32_t lb = linePenalty + ( badness > 0 ? badness : -badness );
  if ( penalty > 0 )
  {

    return lb * lb + penalty * penalty;

  }
  else
  {

    return lb * lb - penalty * penalty;

  }

}


inline int32_t badness( gkg::FloatCoordinate size,
                        gkg::FloatCoordinate natural,
                        gkg::FloatCoordinate stretch,
                        gkg::FloatCoordinate shrink )
{

  int32_t r;
  gkg::FloatCoordinate gsr;
  if ( size >= natural )
  {

    if ( stretch == 0 )
    {

      return gkg::Compositor::PenaltyBad;

    }
    else 
    {

      gsr = ( size - natural ) / stretch;
      if ( gsr > BADGSR )
      {

        return gkg::Compositor::PenaltyBad;

      }
      else 
      {

        r = ( int32_t )( 100 * gsr * gsr * gsr );
        return std::min( r, ( int32_t )gkg::Compositor::PenaltyBad );

      }

    }

  }
  else if ( ( shrink == 0 ) || ( size < natural - shrink ) )
  {

    return -gkg::Compositor::PenaltyBad;

  }
  else
  {

    gsr = ( size - natural ) / shrink;
    if ( gsr < -BADGSR )
    {

      return -gkg::Compositor::PenaltyBad;

    }
    else
    {

      r = ( int32_t )( 100 * gsr * gsr * gsr );
      return std::max( r, -gkg::Compositor::PenaltyBad );

    }

  }

}


//
// static functions
//

static void possibleBreak( gkg::Compositor::Index index,
                           gkg::FloatCoordinate* spans,
                           gkg::Compositor::Index spanCount,
                           gkg::FloatCoordinate natural,
                           gkg::FloatCoordinate stretch,
                           gkg::FloatCoordinate shrink,
                           int32_t penalty,
                           int32_t breakPenalty,
                           gkg::BreakSet* breaks )
{

  gkg::BreakSet* bestBreak = 0;
  gkg::BreakSet* doomed;
  int32_t leastDemerits = 0;
  gkg::BreakSet* breakSet = breaks->next;
  while ( breakSet != breaks )
  {

    gkg::FloatCoordinate span = spans[ std::min( breakSet->count, spanCount-1 ) ];
    breakSet->noBreak( natural, stretch, shrink );
    int32_t breakBadness = badness( span,
                                    breakSet->natural,
                                    breakSet->stretch,
                                    breakSet->shrink );
    bool onlyBreak = breakSet == breaks->next && breakSet->next == breaks;
    if ( penalty <= gkg::Compositor::PenaltyGood )
    {

      int32_t breakDemerits = demerits( breakBadness, penalty, breakPenalty );
      if ( !bestBreak )
      {

        breakSet->addBreak( index, breakDemerits );
        bestBreak = breakSet;
        leastDemerits = breakSet->demerits;

      }
      else if ( breakSet->demerits + breakDemerits < leastDemerits )
      {

        delete bestBreak;
        breakSet->addBreak( index, breakDemerits );
        bestBreak = breakSet;
        leastDemerits = breakSet->demerits;

      }
      else
      {

        if ( !onlyBreak )
        {

          doomed = breakSet;
          breakSet = breakSet->previous;
          delete doomed;

        }

      }

    }
    else if ( breakBadness < -TOLERANCE )
    {

      if ( onlyBreak )
      {

        int32_t breakDemerits = demerits( breakBadness, penalty, breakPenalty );
        breakSet->addBreak( index, breakDemerits );
        bestBreak = breakSet;
        leastDemerits = breakSet->demerits;

      }
      else
      {

        doomed = breakSet;
        breakSet = breakSet->previous;
        delete doomed;

      }

    }
    else if ( breakBadness <= TOLERANCE )
    {

      int32_t breakDemerits = demerits( breakBadness, penalty, breakPenalty );
      if ( !bestBreak )
      {

        new gkg::BreakSet( breakSet );
        breakSet = breakSet->next;
        breakSet->addBreak( index, breakDemerits );
        bestBreak = breakSet;
        leastDemerits = breakSet->demerits;

      }
      else if ( breakSet->demerits + breakDemerits < leastDemerits )
      {

        delete bestBreak;
        new gkg::BreakSet( breakSet );
        breakSet = breakSet->next;
        breakSet->addBreak( index, breakDemerits );
        bestBreak = breakSet;
        leastDemerits = breakSet->demerits;

      }

    }
    breakSet = breakSet->next;

  }

}


//
// class TexCompositor
//

gkg::TeXCompositor::TeXCompositor( int32_t penalty )
                   : gkg::Compositor(),
                     _penalty( penalty )
{
}


gkg::TeXCompositor::~TeXCompositor()
{
}


gkg::Compositor::Index 
gkg::TeXCompositor::compose( gkg::FloatCoordinate* naturals,
                             gkg::FloatCoordinate* stretchs,
                             gkg::FloatCoordinate* shrinks,
                             int32_t* penalties,
                             gkg::Compositor::Index componentCount,
                             gkg::FloatCoordinate* spans,
                             gkg::Compositor::Index spanCount,
                             gkg::Compositor::Index* breaks,
                             gkg::Compositor::Index breakCount )
{

  gkg::BreakSet* bestBreaks = new gkg::BreakSet( 0 );
  new gkg::BreakSet( bestBreaks );
  gkg::FloatCoordinate nat = 0;
  gkg::FloatCoordinate str = 0;
  gkg::FloatCoordinate shr = 0;
  int32_t penalty;
  for ( gkg::Compositor::Index i = 0; i < componentCount; i++ )
  {

    nat += naturals[ i ];
    str += stretchs[ i ];
    shr += shrinks[ i ];
    if ( i == componentCount - 1 )
    {

      penalty = gkg::Compositor::PenaltyGood;

    }
    else
    {

      penalty = penalties[ i ];

    }
    if ( penalty < gkg::Compositor::PenaltyBad )
    {

      possibleBreak( i, spans, spanCount, nat, str, shr, penalty,
                     _penalty, bestBreaks );
      nat = 0;
      str = 0;
      shr = 0;

    }

  }

  gkg::Compositor::Index
    count = std::min( breakCount, bestBreaks->next->count );
  for ( gkg::Compositor::Index j = 0; j < count; j++ )
  {

    breaks[ j ] = bestBreaks->next->breaks[ j ];

  }
  BreakSet* doomed = bestBreaks->next;
  delete doomed;
  delete bestBreaks;
  return count;

}
