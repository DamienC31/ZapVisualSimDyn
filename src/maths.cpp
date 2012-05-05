#include "freereign.h"

//Optimise TODO
vertex3D crossProduct(vertex3D v1, vertex3D v2, vertex3D v3)
{
  vertex3D a,b;
  vertex3D cpx;

  a.x = (v1.x - v2.x);
  a.y = (v1.y - v2.y);
  a.z = (v1.z - v2.z);

  b.x = (v2.x - v3.x);
  b.y  =(v2.y - v3.y);
  b.z = (v2.z - v3.z);

  cpx.x = ( (a.y * b.z) - (b.y * a.z) );
  cpx.y = ( (a.z * b.x) - (b.z * a.x) );
  cpx.z = ( (a.x * b.y) - (b.x * a.y) );

  return cpx;
}


float sqr(float num)
{
	return (float)num*num;
}
