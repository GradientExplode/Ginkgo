#include <gkg-processing-topology/TopologyClassifier.h>
#include <gkg-core-exception/Exception.h>


// Numbering of neighborhood points according to Malandain
// 
//  0  1  2     9 10 11    17 18 19
//  3  4  5    12  C 13    20 21 22
//  6  7  8    14 15 16    23 24 25
// 
// Numbering according to Mangin
//
// 19 11 23     7  2 15    20 12 24
//  8  3 16     1  0  6     9  4 17
// 21 13 25    10  5 18    22 14 26
//
#define Point0  19
#define Point1  11
#define Point2  23
#define Point3   8
#define Point4   3
#define Point5  16
#define Point6  21
#define Point7  13
#define Point8  25
#define Point9   7
#define Point10  2
#define Point11 15
#define Point12  1
#define Point13  6
#define Point14 10
#define Point15  5
#define Point16 18
#define Point17 20
#define Point18 12
#define Point19 24
#define Point20  9
#define Point21  4
#define Point22 17
#define Point23 22
#define Point24 14
#define Point25 26


gkg::TopologyClassifier::TopologyClassifier()
                        : _component26( 
                                 gkg::Neighborhood3d::Neighborhood3d_26,
                                 gkg::TopologicalComponent::Adjacency3d_26,
                                 gkg::TopologicalComponent::Connectivity3d_26 ),
                          _component18( 
                                 gkg::Neighborhood3d::Neighborhood3d_6,
                                 gkg::TopologicalComponent::Adjacency3d_6,
                                 gkg::TopologicalComponent::Connectivity3d_18 )
{

  try
  {

    _classMatrix[ 0 ][ 0 ] = GKG_TOPOLOGY_UNKNOWN;
    _classMatrix[ 0 ][ 1 ] = GKG_TOPOLOGY_ISOLATED_POINT;
    _classMatrix[ 0 ][ 2 ] = GKG_TOPOLOGY_ISOLATED_POINT;
    _classMatrix[ 0 ][ 3 ] = GKG_TOPOLOGY_ISOLATED_POINT;
    _classMatrix[ 1 ][ 0 ] = GKG_TOPOLOGY_VOLUME_POINT;
    _classMatrix[ 1 ][ 1 ] = GKG_TOPOLOGY_SIMPLE_POINT;
    _classMatrix[ 1 ][ 2 ] = GKG_TOPOLOGY_SURFACE_POINT;
    _classMatrix[ 1 ][ 3 ] = GKG_TOPOLOGY_SURFACES_JUNCTION;
    _classMatrix[ 2 ][ 0 ] = GKG_TOPOLOGY_VOLUME_POINT;
    _classMatrix[ 2 ][ 1 ] = GKG_TOPOLOGY_CURVE_POINT;
    _classMatrix[ 2 ][ 2 ] = GKG_TOPOLOGY_SURFACE_CURVES_JUNCTION;
    _classMatrix[ 2 ][ 3 ] = GKG_TOPOLOGY_SURFACES_CURVES_JUNCTION;
    _classMatrix[ 3 ][ 0 ] = GKG_TOPOLOGY_VOLUME_POINT;
    _classMatrix[ 3 ][ 1 ] = GKG_TOPOLOGY_CURVES_JUNCTION;
    _classMatrix[ 3 ][ 2 ] = GKG_TOPOLOGY_SURFACE_CURVES_JUNCTION;
    _classMatrix[ 3 ][ 3 ] = GKG_TOPOLOGY_SURFACES_CURVES_JUNCTION;

    _neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                       gkg::Neighborhood3d::Neighborhood3d_26 );

  }
  GKG_CATCH( "gkg::TopologyClassifier::TopologyClassifier()" );

}


gkg::TopologyClassifier::~TopologyClassifier()
{
}


void gkg::TopologyClassifier::setStride( int32_t stride )
{

  try
  {

    _neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                               gkg::Neighborhood3d::Neighborhood3d_26, stride );

  }
  GKG_CATCH( "void gkg::TopologyClassifier::setStride( int32_t stride )" );

}


int32_t gkg::TopologyClassifier::binaryDecisionDiagram( int32_t* X, 
                                                        bool& inverse )
{

  try
  {

    inverse = false;

    if ( X[ Point4 ] )
    {

      if ( X[ Point10 ] )
      {

        if ( X[ Point13 ] )
        {

          if ( X[ Point12 ] )
          {

            if ( X[ Point15 ] )
            {

              return !X[ Point21 ];

            }
            else
            {

              Lbl0: if ( X[ Point21 ] )
              {

                return 1;

              }
              else
              {

                return !X[ Point24 ];

              }

            }

          }
          else
          {

            Lbl1: if ( X[ Point15 ] )
            {

              Lbl2: if ( X[ Point20 ] )
              {

                return X[ Point21 ];

              }
              else
              {

                return 1;

              }

            }
            else
            {

              Lbl3: if ( X[ Point14 ] )
              {

                Lbl4: if ( X[ Point20 ] )
                {

                  return 0;

                }
                else
                {

                  Lbl5: if ( X[ Point21 ] )
                  {

                    return 0;

                  }
                  else
                  {

                    return !X[ Point24 ];

                  }

                }

              }
              else
              {

                Lbl6: if ( X[ Point20 ] )
                {

                  goto Lbl0;

                }
                else
                {

                  Lbl7: if ( X[ Point21 ] )
                  {

                    return 1;

                  }
                  else
                  {

                    Lbl8: if ( X[ Point23 ] )
                    {

                      return X[ Point24 ];

                    }
                    else
                    {

                      return 1;

                    }

                  }

                }

              }

            }

          }

        }
        else if ( X[ Point12 ] )
        {

          Lbl9: if ( X[ Point15 ] )
          {

            Lbl10: if ( X[ Point21 ] )
            {

              return 1;

            }
            else
            {

              return !X[ Point22 ];

            }

          }
          else
          {

            Lbl11: if ( X[ Point21 ] )
            {

              return !X[ Point16 ];

            }
            else
            {

              Lbl12: if ( X[ Point16 ] )
              {

                if ( X[ Point22 ] )
                {

                  return 0;

                }
                else
                {

                  return !X[ Point24 ];

                }

              }
              else
              {

                Lbl13: if ( X[ Point22 ] )
                {

                  return !X[ Point24 ];

                }
                else
                {

                  Lbl14: if ( X[ Point24 ] )
                  {

                    return 1;

                  }
                  else
                  {

                    return !X[ Point25 ];

                  }

                }

              }

            }

          }

        }
        else
        {

          Lbl15: if ( X[ Point15 ] )
          {

            Lbl16: if ( X[ Point20 ] )
            {

              return 0;

            }
            else
            {

              Lbl17: if ( X[ Point21 ] )
              {

                return 0;

              }
              else
              {

                return !X[ Point22 ];

              }

            }

          }
          else
          {

            Lbl18: if ( X[ Point14 ] )
            {

              Lbl19: if ( X[ Point20 ] )
              {

                return 0;

              }
              else
              {

                Lbl20: if ( X[ Point21 ] )
                {

                  return 0;

                }
                else
                {

                  goto Lbl12;

                }

              }

            }
            else
            {

              Lbl21: if ( X[ Point20 ] )
              {

                goto Lbl11;

              }
              else
              {

                Lbl22: if ( X[ Point21 ] )
                {

                  return !X[ Point16 ];

                }
                else
                {

                  Lbl23: if ( X[ Point16 ] )
                  {

                    if ( X[ Point22 ] )
                    {

                      return 0;

                    }
                    else
                    {

                      goto Lbl8;

                    }

                  }
                  else
                  {

                    Lbl24: if ( X[ Point22 ] )
                    {

                      goto Lbl8;

                    }
                    else
                    {

                      Lbl25: if ( X[ Point23 ] )
                      {

                        return 0;

                      }
                      else
                      {

                        Lbl26: if ( X[ Point24 ] )
                        {

                          return 0;

                        }
                        else
                        {

                          return !X[ Point25 ];

                        }

                      }

                    }

                  }

                }

              }

            }

          }

        }

      }
      else if ( X[ Point13 ] )
      {

        if ( X[ Point12 ] )
        {

          Lbl27: if ( X[ Point18 ] )
          {

            Lbl28: if ( X[ Point15 ] )
            {

              return X[ Point21 ];

            }
            else
            {

              return 0;

            }

          }
          else if ( X[ Point15 ] )
          {

            return 1;

          }
          else
          {

            goto Lbl5;

          }

        }
        else
        {

          Lbl29: if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              return 0;

            }
            else  if ( X[ Point15 ] )
            {

              Lbl30: if ( X[ Point20 ] )
              {

                return 0;

              }
              else
              {

                return !X[ Point21 ];

              }

            }
            else
            {

              Lbl31: if ( X[ Point14 ] )
              {           

                goto Lbl4;

              }
              else
              {

                Lbl32: if ( X[ Point20 ] )
                {

                  goto Lbl5;

                }
                else if ( X[ Point21 ] )
                {           

                  return 0;

                }
                else
                {           

                  goto Lbl8;

                }

              }

            }

          }
          else
          {

            Lbl33: if ( X[ Point18 ] )
            {

              goto Lbl1;

            }
            else if ( X[ Point15 ] )
            {

              Lbl34: if ( X[ Point17 ] )
              {

                inverse = !inverse;
                goto Lbl30;

              }
              else
              {

                return 1;

              }

            }
            else
            {

              Lbl35: if ( X[ Point14 ] )
              {

                if ( X[ Point17 ] )
                {

                  Lbl36: if ( X[ Point20 ] )
                  {

                    goto Lbl5;

                  }
                  else
                  {

                    return 0;

                  }

                }
                else
                {

                  goto Lbl5;
                }

              }
              else
              {

                Lbl37: if ( X[ Point17 ] )
                {

                  Lbl38: if ( X[ Point20 ] )
                  {

                    inverse = !inverse;
                    goto Lbl5;

                  }
                  else
                  {

                    return X[ Point21 ];

                  }

                }
                else if ( X[ Point20 ] )
                {

                  inverse = !inverse;
                  goto Lbl5;

                }
                else
                {

                  goto Lbl7;

                }

              }

            }

          }

        }

      }
      else if ( X[ Point11 ] )
      {

        if ( X[ Point12 ] )
        {

          Lbl39: if ( X[ Point18 ] )
          {

            return 0;

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl17;

          }
          else
          {

            goto Lbl20;

          }

        }
        else
        {

          Lbl40: if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              return 0;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl16;

            }
            else
            {

              Lbl41: if ( X[ Point14 ] )
              {

                goto Lbl19;

              }
              else
              {

                Lbl42: if ( X[ Point20 ] )
                {

                  goto Lbl20;

                }
                else if ( X[ Point21 ] )
                {

                  return 0;

                }
                else
                {

                  goto Lbl23;

                }

              }

            }

          }
          else
          {

            Lbl43: if ( X[ Point18 ] )
            {

              goto Lbl15;

            }
            else if ( X[ Point15 ] )
            {

              Lbl44: if ( X[ Point17 ] )
              {

                Lbl45: if ( X[ Point20 ] )
                {

                  goto Lbl17;

                }
                else
                {

                  return 0;

                }

              }
              else
              {

                goto Lbl17;

              }

            }
            else
            {

              Lbl46: if ( X[ Point14 ] )
              {

                if ( X[ Point17 ] )
                {

                  Lbl47: if ( X[ Point20 ] )
                  {

                    goto Lbl20;

                  }
                  else
                  {

                    return 0;

                  }

                }
                else
                {

                  goto Lbl20;

                }

              }
              else
              {

                Lbl48: if ( X[ Point17 ] )
                {

                  Lbl49: if ( X[ Point20 ] )
                  {

                    Lbl50: if ( X[ Point21 ] )
                    {

                      return !X[ Point16 ];

                    }
                    else
                    {

                      Lbl51: if ( X[ Point16 ] )
                      {

                        if ( X[ Point22 ] )
                        {

                          return 0;

                        }
                        else
                        {

                          return X[ Point24 ];

                        }

                      }
                      else
                      {

                        Lbl52: if ( X[ Point22 ] )
                        {

                          return X[ Point24 ];

                        }
                        else
                        {

                          return 0;

                        }

                      }

                    }

                  }
                  else
                  {

                    Lbl53: if ( X[ Point21 ] )
                    {

                      return !X[ Point16 ];

                    }
                    else
                    {

                      return 0;

                    }

                  }

                }
                else  if ( X[ Point20 ] )
                {

                  goto Lbl50;

                }
                else
                {

                  goto Lbl22;

                }

              }

            }

          }

        }

      }
      else if ( X[ Point12 ] )
      {

        Lbl54: if ( X[ Point18 ] )
        {

          goto Lbl9;

        }
        else if ( X[ Point15 ] )
        {

          Lbl55: if ( X[ Point21 ] )
          {

            return 1;

          }
          else
          {

            Lbl56: if ( X[ Point19 ] )
            {

              return X[ Point22 ];

            }
            else
            {

              return 1;

            }

          }

        }
        else
        {

          Lbl57: if ( X[ Point21 ] )
          {

            return !X[ Point16 ];

          }
          else
          {

            Lbl58: if ( X[ Point19 ] )
            {

              Lbl59: if ( X[ Point16 ] )
              {

                if ( X[ Point22 ] )
                {

                  return !X[ Point24 ];

                }
                else
                {

                  return 0;

                }

              }
              else
              {

                goto Lbl52;

              }

            }
            else if ( X[ Point16 ] )
            {

              return !X[ Point24 ];

            }
            else
            {

              Lbl60: if ( X[ Point22 ] )
              {

                return X[ Point24 ];

              }
              else
              {

                goto Lbl14;

              }

            }

          }

        }

      }
      else
      {

        Lbl61: if ( X[ Point9 ] )
        {

          if ( X[ Point18 ] )
          {

            goto Lbl15;

          }
          else if ( X[ Point15 ] )
          {

            Lbl62: if ( X[ Point20 ] )
            {

              return 0;

            }
            else if ( X[ Point21 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl56;

            }

          }
          else
          {

            Lbl63: if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                return 0;

              }
              else if ( X[ Point21 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl58;

              }

            }
            else
            {

              Lbl64: if ( X[ Point20 ] )
              {

                goto Lbl57;

              }
              else if ( X[ Point21 ] )
              {

                return !X[ Point16 ];

              }
              else
              {

                Lbl65: if ( X[ Point19 ] )
                {

                  Lbl66: if ( X[ Point16 ] )
                  {

                    Lbl67: if ( X[ Point22 ] )
                    {

                      goto Lbl8;

                    }
                    else
                    {

                      return 0;

                    }

                  }
                  else
                  {

                    return 0;

                  }

                }
                else if ( X[ Point16 ] )
                {

                  goto Lbl8;

                }
                else
                {

                  Lbl68: if ( X[ Point22 ] )
                  {

                    return 0;

                  }
                  else
                  {

                    goto Lbl25;

                  }

                }

              }

            }

          }

        }
        else
        {

          Lbl69: if ( X[ Point18 ] )
          {

            Lbl70: if ( X[ Point15 ] )
            {

              Lbl71: if ( X[ Point20 ] )
              {

                goto Lbl10;

              }
              else
              {

                inverse = !inverse;
                goto Lbl17;

              }

            }
            else
            {

              Lbl72: if ( X[ Point14 ] )
              {

                if ( X[ Point20 ] )
                {

                  goto Lbl11;

                }
                else
                {

                  Lbl73: if ( X[ Point21 ] )
                  {

                    return !X[ Point16 ];

                  }
                  else
                  {

                    goto Lbl59;

                  }

                }

              }
              else
              {

                Lbl74: if ( X[ Point20 ] )
                {

                  Lbl75: if ( X[ Point21 ] )
                  {

                    return X[ Point16 ];

                  }
                  else
                  {

                    Lbl76: if ( X[ Point16 ] )
                    {

                      Lbl77: if ( X[ Point22 ] )
                      {

                        return !X[ Point24 ];

                      }
                      else
                      {

                        return X[ Point24 ];

                      }

                    }
                    else
                    {

                      return 0;

                    }

                  }

                }
                else
                {

                  Lbl78: if ( X[ Point21 ] )
                  {

                    return X[ Point16 ];

                  }
                  else
                  {

                    goto Lbl66;

                  }

                }

              }

            }

          }
          else if ( X[ Point15 ] )
          {

            Lbl79: if ( X[ Point17 ] )
            {

              Lbl80: if ( X[ Point20 ] )
              {

                goto Lbl55;

              }
              else
              {

                return X[ Point21 ];

              }

            }
            else
            {

              goto Lbl55;

            }

          }
          else
          {

            Lbl81: if ( X[ Point14 ] )
            {

              if ( X[ Point17 ] )
              {

                Lbl82: if ( X[ Point20 ] )
                {

                  goto Lbl57;

                }
                else
                {

                  goto Lbl53;

                }

              }
              else
              {

                goto Lbl57;

              }

            }
            else
            {

              Lbl83: if ( X[ Point17 ] )
              {

                Lbl84: if ( X[ Point20 ] )
                {

                  Lbl85: if ( X[ Point21 ] )
                  {

                    return X[ Point16 ];

                  }
                  else
                  {

                    Lbl86: if ( X[ Point19 ] )
                    {

                      Lbl87: if ( X[ Point16 ] )
                      {

                        goto Lbl52;

                      }
                      else
                      {

                        return 0;

                      }

                    }
                    else if ( X[ Point16 ] )
                    {

                      return X[ Point24 ];

                    }
                    else
                    {

                      return 0;

                    }

                  }

                }
                else
                {

                  Lbl88: if ( X[ Point21 ] )
                  {

                    return X[ Point16 ];

                  }
                  else
                  {

                    return 0;

                  }

                }

              }
              else if ( X[ Point20 ] )
              {

                goto Lbl85;

              }
              else if ( X[ Point21 ] )
              {

                return X[ Point16 ];

              }
              else
              {

                goto Lbl65;

              }

            }

          }

        }

      }

    }
    else
    {

      if ( X[ Point10 ] )
      {

        if ( X[ Point13 ] )
        {

          if ( X[ Point7 ] )
          {

            if ( X[ Point12 ] )
            {

              goto Lbl28;

            }
            else if ( X[ Point3 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl1;

            }

          }
          else if ( X[ Point12 ] )
          {

            Lbl89: if ( X[ Point15 ] )
            {

              return X[ Point21 ];

            }
            else
            {

              goto Lbl0;

            }

          }
          else if ( X[ Point3 ] )
          {

            Lbl90: if ( X[ Point15 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl3;

            }

          }
          else
          {

            Lbl91: if ( X[ Point15 ] )
            {

              goto Lbl2;

            }
            else if ( X[ Point14 ] )
            {

              Lbl92: if ( X[ Point20 ] )
              {

                goto Lbl0;

              }
              else
              {

                inverse = !inverse;
                goto Lbl5;

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl6;

            }

          }

        }
        else if ( X[ Point5 ] )
        {

          if ( X[ Point7 ] )
          {

            if ( X[ Point12 ] )
            {

              return 0;

            }
            else if ( X[ Point3 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl15;

            }

          }
          else if ( X[ Point12 ] )
          {

            Lbl93: if ( X[ Point15 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl11;

            }

          }
          else if ( X[ Point3 ] )
          {

            Lbl94: if ( X[ Point15 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl18;

            }

          }
          else
          {

            Lbl95: if ( X[ Point15 ] )
            {

              goto Lbl16;

            }
            else if ( X[ Point14 ] )
            {

              Lbl96: if ( X[ Point20 ] )
              {

                goto Lbl11;

              }
              else
              {

                goto Lbl50;

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl21;

            }

          }

        }
        else if ( X[ Point7 ] )
        {

          if ( X[ Point12 ] )
          {

            goto Lbl9;

          }
          else if ( X[ Point3 ] )
          {

            goto Lbl15;

          }
          else
          {

            goto Lbl70;

          }

        }
        else if ( X[ Point12 ] )
        {

          Lbl97: if ( X[ Point15 ] )
          {

            goto Lbl10;

          }
          else
          {

            Lbl98: if ( X[ Point21 ] )
            {

              Lbl99: if ( X[ Point8 ] )
              {

                return X[ Point16 ];

              }
              else
              {

                return 1;

              }

            }
            else
            {

              Lbl100: if ( X[ Point8 ] )
              {

                goto Lbl76;

              }
              else if ( X[ Point16 ] )
              {

                goto Lbl77;

              }
              else
              {

                goto Lbl13;

              }

            }

          }

        }
        else if ( X[ Point3 ] )
        {

          Lbl101: if ( X[ Point15 ] )
          {

            goto Lbl16;

          }
          else if ( X[ Point14 ] )
          {

            Lbl102: if ( X[ Point20 ] )
            {

              return 0;

            }
            else
            {

              Lbl103: if ( X[ Point21 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl100;

              }

            }

          }
          else
          {

            Lbl104: if ( X[ Point20 ] )
            {

              goto Lbl98;

            }
            else
            {

              Lbl105: if ( X[ Point21 ] )
              {

                goto Lbl99;

              }
              else
              {

                Lbl106: if ( X[ Point8 ] )
                {

                  goto Lbl66;

                }
                else
                {

                  Lbl107: if ( X[ Point16 ] )
                  {

                    goto Lbl67;

                  }
                  else
                  {

                    goto Lbl24;

                  }

                }

              }

            }

          }

        }
        else
        {

          Lbl108: if ( X[ Point15 ] )
          {

            goto Lbl71;

          }
          else if ( X[ Point14 ] )
          {

            Lbl109: if ( X[ Point20 ] )
            {

              goto Lbl98;

            }
            else
            {

              Lbl110: if ( X[ Point21 ] )
              {

                goto Lbl99;

              }
              else
              {

                Lbl111: if ( X[ Point8 ] )
                {

                  goto Lbl87;

                }
                else
                {

                  goto Lbl52;

                }

              }

            }

          }
          else if ( X[ Point6 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl104;

          }

        }

      }
      else if ( X[ Point13 ] )
      {

        if ( X[ Point1 ] )
        {

          if ( X[ Point7 ] )
          {

            if ( X[ Point12 ] )
            {

              return 0;

            }
            else if ( X[ Point3 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl29;

            }

          }
          else if ( X[ Point12 ] )
          {

            if ( X[ Point18 ] )
            {

              return 0;

            }
            else
            {

              Lbl112: if ( X[ Point15 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl5;

              }

            }

          }
          else if ( X[ Point3 ] )
          {

            if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                return 0;

              }
              else if ( X[ Point15 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl31;

              }

            }
            else if ( X[ Point18 ] )
            {

              goto Lbl90;

            }
            else if ( X[ Point15 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl35;

            }

          }
          else if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              return 0;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl30;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                goto Lbl5;

              }
              else
              {

                inverse = !inverse;
                goto Lbl0;

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl32;

            }

          }
          else
          {

            Lbl113: if ( X[ Point18 ] )
            {

              goto Lbl91;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl34;

            }
            else if ( X[ Point14 ] )
            {

              Lbl114: if ( X[ Point17 ] )
              {

                goto Lbl38;

              }
              else
              {

                inverse = !inverse;
                goto Lbl5;

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl37;

            }

          }

        }
        else if ( X[ Point7 ] )
        {

          if ( X[ Point12 ] )
          {

            goto Lbl27;

          }
          else if ( X[ Point3 ] )
          {

            goto Lbl29;

          }
          else if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl1;

            }
            else if ( X[ Point15 ] )
            {

              inverse = !inverse;
              goto Lbl30;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl36;

            }
            else
            {

              goto Lbl38;

            }

          }
          else if ( X[ Point0 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl33;

          }

        }
        else if ( X[ Point12 ] )
        {

          if ( X[ Point18 ] )
          {

            goto Lbl89;

          }
          else
          {

            inverse = !inverse;
            goto Lbl112;

          }

        }
        else if ( X[ Point3 ] )
        {

          if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl90;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl30;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                return 0;

              }
              else
              {

                inverse = !inverse;
                goto Lbl0;

              }

            }
            else 
            {

              goto Lbl38;

            }

          }
          else if ( X[ Point18 ] )
          {

            if ( X[ Point15 ] )
            {

              goto Lbl2;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl92;

            }
            else
            {

              return 0;

            }

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl34;

          }
          else if ( X[ Point14 ] )
          {

            goto Lbl114;

          }
          else
          {

            return 0;

          }

        }
        else if ( X[ Point9 ] )
        {

          if ( X[ Point18 ] )
          {

            goto Lbl91;

          }
          else if ( X[ Point15 ] )
          {

            inverse = !inverse;
            goto Lbl30;

          }
          else if ( X[ Point14 ] )
          {

            goto Lbl38;

          }
          else if ( X[ Point6 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl38;

          }

        }
        else if ( X[ Point0 ] )
        {

          return 0;

        }
        else
        {

          goto Lbl113;

        }

      }
      else if ( X[ Point5 ] )
      {

        if ( X[ Point1 ] )
        {

          if ( X[ Point11 ] )
          {

            if ( X[ Point7 ] )
            {

              if ( X[ Point12 ] )
              {

                return 0;

              }
              else if ( X[ Point3 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl40;

              }

            }
            else if ( X[ Point12 ] )
            {

              if ( X[ Point18 ] )
              {

                return 0;

              }
              else if ( X[ Point15 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl20;

              }

            }
            else if ( X[ Point3 ] )
            {

              if ( X[ Point9 ] )
              {

                if ( X[ Point18 ] )
                {

                  return 0;

                }
                else if ( X[ Point15 ] )
                {

                  return 0;

                }
                else
                {

                  goto Lbl41;

                }

              }
              else if ( X[ Point18 ] )
              {

                goto Lbl94;

              }
              else if ( X[ Point15 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl46;

              }

            }
            else if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                return 0;

              }
              else if ( X[ Point15 ] )
              {

                goto Lbl16;

              }
              else if ( X[ Point14 ] )
              {

                if ( X[ Point20 ] )
                {

                  goto Lbl20;

                }
                else
                {

                  Lbl115: if ( X[ Point21 ] )
                  {

                    return 0;

                  }
                  else
                  {

                    goto Lbl51;

                  }

                }

              }
              else if ( X[ Point6 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl42;

              }

            }
            else
            {

              Lbl116: if ( X[ Point18 ] )
              {

                goto Lbl95;

              }
              else if ( X[ Point15 ] )
              {

                goto Lbl44;

              }
              else if ( X[ Point14 ] )
              {

                Lbl117: if ( X[ Point17 ] )
                {

                  goto Lbl49;

                }
                else
                {

                  goto Lbl50;

                }

              }
              else if ( X[ Point6 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl48;

              }

            }

          }
          else if ( X[ Point7 ] )
          {

            if ( X[ Point12 ] )
            {

              return 0;

            }
            else if ( X[ Point3 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl61;

            }

          }
          else if ( X[ Point12 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl93;

            }
            else if ( X[ Point15 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl57;

            }

          }
          else if ( X[ Point3 ] )
          {

            if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                goto Lbl94;

              }
              else if ( X[ Point15 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl63;

              }

            }
            else if ( X[ Point18 ] )
            {

              if ( X[ Point15 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl72;

              }

            }
            else if ( X[ Point15 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl81;

            }

          }
          else if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl95;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl62;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                goto Lbl57;

              }
              else if ( X[ Point21 ] )
              {

                return !X[ Point16 ];

              }
              else
              {

                goto Lbl86;

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl64;

            }

          }
          else
          {

            Lbl118: if ( X[ Point18 ] )
            {

              Lbl119: if ( X[ Point15 ] )
              {

                goto Lbl71;

              }
              else if ( X[ Point14 ] )
              {

                Lbl120: if ( X[ Point20 ] )
                {

                  goto Lbl75;

                }
                else
                {

                  Lbl121: if ( X[ Point21 ] )
                  {

                    return X[ Point16 ];

                  }
                  else
                  {

                    goto Lbl87;

                  }

                }

              }
              else if ( X[ Point6 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl74;

              }

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl79;

            }
            else if ( X[ Point14 ] )
            {

              Lbl122: if ( X[ Point17 ] )
              {

                goto Lbl84;

              }
              else
              {

                goto Lbl85;

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl83;

            }

          }

        }
        else if ( X[ Point11 ] )
        {

          if ( X[ Point7 ] )
          {

            if ( X[ Point12 ] )
            {

              goto Lbl39;

            }
            else if ( X[ Point3 ] )
            {

              goto Lbl40;

            }
            else if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                goto Lbl15;

              }
              else if ( X[ Point15 ] )
              {

                goto Lbl45;

              }
              else if ( X[ Point14 ] )
              {

                goto Lbl47;

              }
              else
              {

                goto Lbl49;

              }

            }
            else if ( X[ Point0 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl43;

            }

          }
          else if ( X[ Point12 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl93;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl17;

            }
            else
            {

              goto Lbl50;

            }

          }
          else if ( X[ Point3 ] )
          {

            if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                goto Lbl94;

              }
              else if ( X[ Point15 ] )
              {

                goto Lbl16;

              }
              else if ( X[ Point14 ] )
              {

                if ( X[ Point20 ] )
                {

                  return 0;

                }
                else 
                {

                  goto Lbl115;

                }

              }
              else
              {

                goto Lbl49;

              }

            }
            else if ( X[ Point18 ] )
            {

              if ( X[ Point15 ] )
              {

                goto Lbl16;

              }
              else if ( X[ Point14 ] )
              {

                goto Lbl96;

              }
              else
              {

                return 0;

              }

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl44;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl117;

            }
            else
            {

              return 0;

            }

          }
          else if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl95;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl45;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl49;

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl49;

            }

          }
          else if ( X[ Point0 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl116;

          }

        }
        else if ( X[ Point7 ] )
        {

          Lbl123: if ( X[ Point12 ] )
          {

            goto Lbl54;

          }
          else if ( X[ Point3 ] )
          {

            goto Lbl61;

          }
          else if ( X[ Point9 ] )
          {

            Lbl124: if ( X[ Point18 ] )
            {

              goto Lbl70;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl80;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl82;

            }
            else
            {

              goto Lbl84;

            }

          }
          else if ( X[ Point0 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl69;

          }

        }
        else if ( X[ Point12 ] )
        {

          if ( X[ Point18 ] )
          {

            if ( X[ Point15 ] )
            {

              goto Lbl10;

            }
            else
            {

              goto Lbl75;

            }

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl55;

          }
          else
          {

            goto Lbl85;

          }

        }
        else if ( X[ Point3 ] )
        {

          if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              if ( X[ Point15 ] )
              {

                goto Lbl16;

              }
              else if ( X[ Point14 ] )
              {

                if ( X[ Point20 ] )
                {

                  return 0;

                }
                else
                {

                  Lbl125: if ( X[ Point21 ] )
                  {

                    return 0;

                  }
                  else
                  {

                    goto Lbl76;

                  }

                }

              }
              else
              {

                goto Lbl74;

              }

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl62;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                return 0;

              }
              else if ( X[ Point21 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl86;

              }

            }
            else
            {

              goto Lbl84;

            }

          }
          else if ( X[ Point18 ] )
          {

            if ( X[ Point15 ] )
            {

              goto Lbl71;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl120;

            }
            else
            {

              return 0;

            }

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl79;

          }
          else if ( X[ Point14 ] )
          {

            goto Lbl122;

          }
          else
          {

            return 0;

          }

        }
        else if ( X[ Point9 ] )
        {

          if ( X[ Point18 ] )
          {

            goto Lbl119;

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl80;

          }
          else if ( X[ Point14 ] )
          {

            goto Lbl84;

          }
          else if ( X[ Point6 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl84;

          }

        }
        else if ( X[ Point0 ] )
        {

          return 0;

        }
        else
        {

          goto Lbl118;

        }

      }
      else if ( X[ Point1 ] )
      {

        if ( X[ Point11 ] )
        {

          if ( X[ Point7 ] )
          {

            if ( X[ Point12 ] )
            {

              goto Lbl39;

            }
            else if ( X[ Point3 ] )
            {

              goto Lbl40;

            }
            else if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                return 0;

              }
              else if ( X[ Point15 ] )
              {

                Lbl126: if ( X[ Point20 ] )
                {

                  goto Lbl17;

                }
                else
                {

                  inverse = !inverse;
                  goto Lbl10;

                }

              }
              else if ( X[ Point14 ] )
              {

                if ( X[ Point20 ] )
                {

                  goto Lbl20;

                }
                else
                {

                  Lbl127: if ( X[ Point21 ] )
                  {

                    return 0;

                  }
                  else
                  {

                    goto Lbl59;

                  }

                }

              }
              else if ( X[ Point20 ] )
              {

                goto Lbl125;

              }
              else if ( X[ Point21 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl66;

              }

            }
            else
            {

              Lbl128: if ( X[ Point18 ] )
              {

                goto Lbl70;

              }
              else if ( X[ Point15 ] )
              {

                Lbl129: if ( X[ Point17 ] )
                {

                  Lbl130: if ( X[ Point20 ] )
                  {

                    inverse = !inverse;
                    goto Lbl17;

                  }
                  else
                  {

                    return X[ Point21 ];

                  }

                }
                else
                {

                  inverse = !inverse;
                  goto Lbl17;

                }

              }
              else if ( X[ Point14 ] )
              {

                if ( X[ Point17 ] )
                {

                  Lbl131: if ( X[ Point20 ] )
                  {

                    goto Lbl73;

                  }
                  else
                  {

                    goto Lbl53;

                  }

                }
                else
                {

                  goto Lbl73;

                }

              }
              else if ( X[ Point17 ] )
              {

                Lbl132: if ( X[ Point20 ] )
                {

                  goto Lbl121;

                }
                else
                {

                  goto Lbl88;

                }

              }
              else if ( X[ Point20 ] )
              {

                goto Lbl121;

              }
              else
              {

                goto Lbl78;

              }

            }

          }
          else if ( X[ Point12 ] )
          {

            if ( X[ Point18 ] )
            {

              return 0;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl17;

            }
            else
            {

              goto Lbl103;

            }

          }
          else if ( X[ Point3 ] )
          {

            if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                return 0;

              }
              else if ( X[ Point15 ] )
              {

                goto Lbl16;

              }
              else if ( X[ Point14 ] )
              {

                goto Lbl102;

              }
              else
              {

                Lbl133: if ( X[ Point20 ] )
                {

                  goto Lbl103;

                }
                else if ( X[ Point21 ] )
                {

                  return 0;

                }
                else
                {

                  goto Lbl106;

                }

              }

            }
            else if ( X[ Point18 ] )
            {

              goto Lbl101;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl44;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point17 ] )
              {

                if ( X[ Point20 ] )
                {

                  goto Lbl103;

                }
                else
                {

                  return 0;

                }

              }
              else
              {

                goto Lbl103;

              }

            }
            else
            {

              Lbl134: if ( X[ Point17 ] )
              {

                Lbl135: if ( X[ Point20 ] )
                {

                  goto Lbl110;

                }
                else
                {

                  Lbl136: if ( X[ Point21 ] )
                  {

                    goto Lbl99;

                  }
                  else
                  {

                    return 0;

                  }

                }

              }
              else if ( X[ Point20 ] )
              {

                goto Lbl110;

              }
              else
              {

                goto Lbl105;

              }

            }

          }
          else if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              return 0;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl126;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                goto Lbl103;

              }
              else
              {

                Lbl137: if ( X[ Point21 ] )
                {

                  return 0;

                }
                else
                {

                  goto Lbl111;

                }

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl133;

            }

          }
          else
          {

            Lbl138: if ( X[ Point18 ] )
            {

              goto Lbl108;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl129;

            }
            else if ( X[ Point14 ] )
            {

              Lbl139: if ( X[ Point17 ] )
              {

                goto Lbl135;

              }
              else
              {

                goto Lbl110;

              }

            }
            else if ( X[ Point6 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl134;

            }

          }

        }
        else if ( X[ Point7 ] )
        {

          if ( X[ Point12 ] )
          {

            goto Lbl54;

          }
          else if ( X[ Point3 ] )
          {

            goto Lbl61;

          }
          else if ( X[ Point9 ] )
          {

            goto Lbl124;

          }
          else
          {

            return 0;

          }

        }
        else if ( X[ Point12 ] )
        {

          Lbl140: if ( X[ Point18 ] )
          {

            goto Lbl97;

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl55;

          }
          else
          {

            Lbl141: if ( X[ Point21 ] )
            {

              goto Lbl99;

            }
            else
            {

              Lbl142: if ( X[ Point8 ] )
              {

                goto Lbl86;

              }
              else if ( X[ Point19 ] )
              {

                goto Lbl52;

              }
              else if ( X[ Point16 ] )
              {

                return X[ Point24 ];

              }
              else
              {

                goto Lbl60;

              }

            }

          }

        }
        else if ( X[ Point3 ] )
        {

          Lbl143: if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl101;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl62;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                return 0;

              }
              else if ( X[ Point21 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl142;

              }

            }
            else
            {

              Lbl144: if ( X[ Point20 ] )
              {

                goto Lbl141;

              }
              else if ( X[ Point21 ] )
              {

                goto Lbl99;

              }
              else
              {

                Lbl145: if ( X[ Point8 ] )
                {

                  return 0;

                }
                else if ( X[ Point19 ] )
                {

                  return 0;

                }
                else  if ( X[ Point16 ] )
                {

                  return 0;

                }
                else
                {

                  goto Lbl68;

                }

              }

            }

          }
          else if ( X[ Point18 ] )
          {

            Lbl146: if ( X[ Point15 ] )
            {

              goto Lbl71;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl109;

            }
            else
            {

              return 0;

            }

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl79;

          }
          else if ( X[ Point14 ] )
          {

            Lbl147: if ( X[ Point17 ] )
            {

              Lbl148: if ( X[ Point20 ] )
              {

                goto Lbl141;

              }
              else
              {

                goto Lbl136;

              }

            }
            else
            {

             goto Lbl141;

            }

          }
          else
          {

            Lbl149: if ( X[ Point17 ] )
            {

              return 0;

            }
            else if ( X[ Point20 ] )
            {

              return 0;

            }
            else if ( X[ Point21 ] )
            {

              return 0;

            }
            else
            {

              goto Lbl145;

            }

          }

        }
        else if ( X[ Point9 ] )
        {

          Lbl150: if ( X[ Point18 ] )
          {

            goto Lbl108;

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl80;

          }
          else if ( X[ Point14 ] )
          {

            goto Lbl148;

          }
          else if ( X[ Point6 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl144;

          }

        }
        else
        {

          Lbl151: if ( X[ Point18 ] )
          {

            return 0;

          }
          else if ( X[ Point15 ] )
          {

            return 0;

          }
          else if ( X[ Point14 ] )
          {

            return 0;

          }
          else if ( X[ Point6 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl149;

          }

        }

      }
      else if ( X[ Point11 ] )
      {

        if ( X[ Point7 ] )
        {

          if ( X[ Point12 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl9;

            }
            else if ( X[ Point15 ] )
            {

              inverse = !inverse;
              goto Lbl17;

            }
            else
            {

              goto Lbl73;

            }

          }
          else if ( X[ Point3 ] )
          {

            if ( X[ Point9 ] )
            {

              if ( X[ Point18 ] )
              {

                goto Lbl15;

              }
              else if ( X[ Point15 ] )
              {

                Lbl152: if ( X[ Point20 ] )
                {

                  return 0;

                }
                else
                {

                  inverse = !inverse;
                  goto Lbl10;

                }

              }
              else if ( X[ Point14 ] )
              {

                if ( X[ Point20 ] )
                {

                  return 0;

                }
                else
                {

                 goto Lbl127;

                }

              }
              else if ( X[ Point20 ] )
              {

                goto Lbl73;

              }
              else if ( X[ Point21 ] )
              {

                return !X[ Point16 ];

              }
              else
              {

                goto Lbl66;

              }

            }
            else
            {

              goto Lbl128;

            }

          }
          else if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl70;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl130;

            }
            else if ( X[ Point14 ] )
            {

              goto Lbl131;

            }
            else
            {

              goto Lbl132;

            }

          }
          else if ( X[ Point0 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl128;

          }

        }
        else if ( X[ Point12 ] )
        {

          if ( X[ Point18 ] )
          {

            goto Lbl97;

          }
          else if ( X[ Point15 ] )
          {

            inverse = !inverse;
            goto Lbl17;

          }
          else
          {

            goto Lbl110;

          }

        }
        else if ( X[ Point3 ] )
        {

          if ( X[ Point9 ] )
          {

            if ( X[ Point18 ] )
            {

              goto Lbl101;

            }
            else if ( X[ Point15 ] )
            {

              goto Lbl152;

            }
            else if ( X[ Point14 ] )
            {

              if ( X[ Point20 ] )
              {

                return 0;

              }
              else
              {

                goto Lbl137;

              }

            }
            else
            {

              goto Lbl135;

            }

          }
          else if ( X[ Point18 ] ) 
          {

            goto Lbl146;

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl129;

          }
          else if ( X[ Point14 ] )
          {

            goto Lbl139;

          }
          else
          {

            return 0;

          }

        }
        else if ( X[ Point9 ] )
        {

          if ( X[ Point18 ] )
          {

            goto Lbl108;

          }
          else if ( X[ Point15 ] )
          {

            goto Lbl130;

          }
          else if ( X[ Point14 ] )
          {

            goto Lbl135;

          }
          else if ( X[ Point6 ] )
          {

            return 0;

          }
          else
          {

            goto Lbl135;

          }

        }
        else if ( X[ Point0 ] )
        {

          return 0;

        }
        else
        {

          goto Lbl138;

        }

      }
      else if ( X[ Point7 ] )
      {

        if ( X[ Point2 ] )
        {

          return 0;

        }
        else
        {

          goto Lbl123;

        }

      }
      else if ( X[ Point2 ] )
      {

        if ( X[ Point12 ] )
        {

          return 0;

        }
        else if ( X[ Point3 ] )
        {

          return 0;

        }
        else if ( X[ Point9 ] )
        {

          return 0;

        }
        else if ( X[ Point0 ] )
        {

          return 0;

        }
        else
        {

          goto Lbl151;

        }

      }
      else if ( X[ Point12 ] )
      {

        goto Lbl140;

      }
      else if ( X[ Point3 ] )
      {

        goto Lbl143;

      }
      else if ( X[ Point9 ] )
      {

        goto Lbl150;

      }
      else if ( X[ Point0 ] )
      {

        goto Lbl151;

      }
      else if ( X[ Point18 ] )
      {

        goto Lbl108;

      }
      else if ( X[ Point15 ] )
      {

        goto Lbl79;

      }
      else if ( X[ Point14 ] )
      {

        goto Lbl147;

      }
      else if ( X[ Point6 ] )
      {

        goto Lbl149;

      }
      else if ( X[ Point17 ] )
      {

        goto Lbl144;

      }
      else if ( X[ Point20 ] )
      {

        goto Lbl141;

      }
      else if ( X[ Point21 ] )
      {

        goto Lbl99;

      }
      else if ( X[ Point8 ] )
      {

        goto Lbl65;

      }
      else if ( X[ Point19 ] )
      {

        goto Lbl107;

      }
      else if ( X[ Point16 ] )
      {

        goto Lbl8;

      }
      else if ( X[ Point22 ] )
      {

        goto Lbl8;

      }
      else if ( X[ Point23 ] )
      {

        goto Lbl14;

      }
      else
      {

        inverse = !inverse;
        goto Lbl26;

      }  

    }

  }
  GKG_CATCH( "int32_t gkg::TopologyClassifier::binaryDecisionDiagram("
             " int32_t* X, bool& inverse )" );

}


#undef Point0 
#undef Point1 
#undef Point2 
#undef Point3 
#undef Point4 
#undef Point5 
#undef Point6 
#undef Point7 
#undef Point8 
#undef Point9 
#undef Point10
#undef Point11
#undef Point12
#undef Point13
#undef Point14
#undef Point15
#undef Point16
#undef Point17
#undef Point18
#undef Point19
#undef Point20
#undef Point21
#undef Point22
#undef Point23
#undef Point24
#undef Point25
