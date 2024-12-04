#include <gkg-communication-socket/IOStreamBase.h>
#include <gkg-core-cppext/Limits.h>


// assume the peer machine has the same endian architecture but
// disable binary I/O by default anyway.  The two machines should
// always negotiate these two options before enabling binary I/O.

gkg::IOStreamBase::IOStreamBase()
                  : _binary( false ),
                    _swapped( false )
{
}


gkg::IOStreamBase::~IOStreamBase()
{
}


// Get and set IOStreamBase's extra state information.  Verify that the
// assumptions about sizes of integral types hold before enabling
// binary I/O; if they do not hold, then refuse to enable binary I/O.
bool gkg::IOStreamBase::getBinary() const
{

  return _binary;

}


void gkg::IOStreamBase::setBinary( bool binary )
{

  if ( binary )
  {

    // don't use consts; cfront has wrong idea of their values
    gkg::IOStreamBase::UINT64 max64 = ~0ULL;
    gkg::IOStreamBase::UINT32 max32 = ~0U;
    gkg::IOStreamBase::UINT16 max16 = ( gkg::IOStreamBase::UINT16 )~0U;

    if ( ( max64 != std::numeric_limits< uint64_t >::max() ) || 
         ( max32 != std::numeric_limits< uint32_t >::max() ) || 
         ( max16 != std::numeric_limits< uint16_t >::max() ) )
    {

      binary = false;

    }

  }
  _binary = binary;

}


bool gkg::IOStreamBase::getSwapped() const
{

  return _swapped;

}


void gkg::IOStreamBase::setSwapped( bool swapped )
{

  _swapped = swapped;

}

