#ifndef _gkg_core_pattern_Convertion_h_
#define _gkg_core_pattern_Convertion_h_


namespace gkg
{


template < class T, class U >
class Convertion 
{

  protected:

    template < class V, class W >
    struct Helper
    {

      typedef int8_t no;                 // sizeof( no ) == 1
      struct yes { int8_t dummy[ 2 ]; }; // sizeof(yes) >= 2
      static no test(...);
      static yes test( W* );

    };

    typedef Helper< T, U > H;

  public:

    enum 
    {

      exists = sizeof( typename H::yes ) == 
               sizeof( H::test( static_cast< T* >( 0 ) ) )

    };

};


}


#define SUPERSUBCLASS( T, U ) \
                     ( ( bool )::gkg::Convertion< const U, const T >::exists )


#endif
