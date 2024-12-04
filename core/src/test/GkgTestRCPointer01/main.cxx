#include <gkg-core-pattern/RCPointer.h>
#include <cassert>
#include <iostream>


class Foo : public gkg::RCObject
{

  public:

    Foo();
    virtual ~Foo();

};


Foo::Foo()
{

  std::cout << ">>> " << this << std::endl;

}


Foo::~Foo()
{

  std::cout << "<<< " << this << std::endl;

}


gkg::RCPointer< Foo > function1();


gkg::RCPointer< Foo > function1()
{

  return gkg::RCPointer< Foo >( new Foo );

}


gkg::RCPointer< Foo > function2( const gkg::RCPointer< Foo >& p );


gkg::RCPointer< Foo > function2( const gkg::RCPointer< Foo >& p )
{

  return p;

}


gkg::RCPointer< Foo > function3( gkg::RCPointer< Foo > p );


gkg::RCPointer< Foo > function3( gkg::RCPointer< Foo > p )
{

  return p;

}


int main() 
{

  std::cout << "testing RCPointer constructors..." << std::endl;
  {

    gkg::RCPointer<Foo> x1, x2;
    assert( x1 == 0 );
    assert( x2 == 0 );
    assert( x1 == x2 );

  }

  std::cout << "testing RCPointer constructors..." << std::endl;
  {

    gkg::RCPointer< Foo > x1 = gkg::RCPointer< Foo >( new Foo );
    gkg::RCPointer< Foo > x2 = gkg::RCPointer< Foo >( new Foo );
    assert( !(x1 == 0) );
    assert( !(x2 == 0) );
    assert( x1 != x2 );

  }

  std::cout << "testing RCPointer::operator=()..." << std::endl;
  {

    gkg::RCPointer< Foo > x1;
    assert( x1 == 0 );
    {

      gkg::RCPointer< Foo > x2 = gkg::RCPointer< Foo >( new Foo );
      x1 = x2;
      assert( x1 == x2 );

    }
    assert( x1 != 0 );

  }

  std::cout << "testing RCPointer::reset()..." << std::endl;
  {

    gkg::RCPointer< Foo > x1;
    assert( x1 == 0 );
    x1.reset( new Foo );
    assert( x1 != 0 );
    x1.reset();
    assert( x1 == 0 );

  }

  std::cout << "testing RCPointer as a return value..." << std::endl;
  {

    gkg::RCPointer< Foo > x1;
    assert( x1 == 0 );
    x1 = function1();
    assert( x1 != 0 );

  }

  std::cout << "testing RCPointer as an argument..." << std::endl;
  {

    gkg::RCPointer< Foo > x1( new Foo );
    assert( x1 != 0 );
    gkg::RCPointer< Foo > x2( function2( x1 ) );
    assert( x1 == x2 );
    gkg::RCPointer< Foo > x3( function3( x1 ) );
    assert( x1 == x3 );
    gkg::RCPointer< Foo > x4( function2( function2( x1 ) ) );
    assert( x1 == x4 );

  }

  return EXIT_SUCCESS;

}
