#include <gkg-graphics-X11-core/MacroAction.h>


gkg::MacroAction::MacroAction( gkg::Action* a0,
                               gkg::Action* a1,
                               gkg::Action* a2,
                               gkg::Action* a3 )
                 : gkg::Action()
{

  if ( a0 )
  {

    a0->addReference();
    _actions.push_back( a0 );

  }
  if ( a1 )
  {

    a1->addReference();
    _actions.push_back( a1 );

  }
  if ( a2 )
  {

    a2->addReference();
    _actions.push_back( a2 );

  }
  if ( a3 )
  {

    a3->addReference();
    _actions.push_back( a3 );

  }

}


gkg::MacroAction::~MacroAction()
{

  std::deque< gkg::Action* >::iterator a = _actions.begin(),
                                       ae = _actions.end();
  while ( a != ae )
  {

    ( *a )->removeReference();
    ++ a;

  }
  _actions.clear();

}


void gkg::MacroAction::prepend( gkg::Action* action )
{

  if ( action )
  {

    action->addReference();

  }
  _actions.push_front( action );

}


void gkg::MacroAction::append( gkg::Action* action )
{

  if ( action )
  {

    action->addReference();

  }
  _actions.push_back( action );

}


void gkg::MacroAction::insert( int32_t i, gkg::Action* action )
{

  if ( action )
  {

    action->addReference();

  }
  _actions.insert( _actions.begin() + i, action );

}

void gkg::MacroAction::remove( int32_t i )
{

  if ( ( i >= 0 ) && ( i < ( int32_t )_actions.size() ) )
  {

    gkg::Action* action = _actions[ i ];
    if ( action )
    {

      action->addReference();

    }
    _actions.erase( _actions.begin() + i );

  }

}


int32_t gkg::MacroAction::getCount() const
{

  return ( int32_t )_actions.size();

}


gkg::Action* gkg::MacroAction::getAction( int32_t i ) const
{

  if ( ( i >= 0 ) && ( i < ( int32_t )_actions.size() ) )
  {

    return _actions[ i ];

  }
  return 0;

}

void gkg::MacroAction::execute()
{

  std::deque< gkg::Action* >::iterator a = _actions.begin(),
                                       ae = _actions.end();
  while ( a != ae )
  {

    gkg::Action* action = *a;
    if ( action )
    {

      action->execute();

    }
    ++ a;

  }

}
