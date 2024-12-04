#include <gkg-graphics-X11-core/Discretionary.h>


gkg::Discretionary::Discretionary( int32_t penalty, gkg::Glyph* glyph )
                   : gkg::Glyph(),
                     _penalty( penalty ),
                     _noBreak( glyph ),
                     _preBreak( glyph ),
                     _inBreak( 0 ),
                     _postBreak( 0 )
{

  if ( _noBreak )
  {

    _noBreak->addReference();

  }
  if ( _preBreak )
  {

    _preBreak->addReference();

  }

}


gkg::Discretionary::Discretionary( int32_t penalty,
                                   gkg::Glyph* no,
                                   gkg::Glyph* pre,
                                   gkg::Glyph* in,
                                   gkg::Glyph* post )
                   : Glyph(),
                     _penalty( penalty ),
                     _noBreak( no ),
                     _preBreak( pre ),
                     _inBreak( in ),
                     _postBreak( post )
{

  if ( _noBreak )
  {

    _noBreak->addReference();

  }
  if ( _preBreak )
  {

    _preBreak->addReference();

  }
  if ( _inBreak )
  {

    _inBreak->addReference();

  }
  if ( _postBreak )
  {

    _postBreak->addReference();

  }

}


gkg::Discretionary::~Discretionary()
{

  if ( _noBreak )
  {

    _noBreak->removeReference();

  }
  if ( _preBreak )
  {

    _preBreak->removeReference();

  }
  if ( _inBreak )
  {

    _inBreak->removeReference();

  }
  if ( _postBreak )
  {

    _postBreak->removeReference();

  }

}    


void gkg::Discretionary::request(
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _noBreak )
  {

    _noBreak->request( geometryRequisition );

  }
  geometryRequisition.setPenalty( _penalty );

}


gkg::Glyph* gkg::Discretionary::compose( gkg::Glyph::BreakType breakType )
{

  switch ( breakType )
  {

    case gkg::Glyph::NoBreak:

      return _noBreak;

    case gkg::Glyph::PreBreak:

      return _preBreak;

    case gkg::Glyph::InBreak:

      return _inBreak;

    case gkg::Glyph::PostBreak:

      return _postBreak;

    default:

      return 0;

  }

}

