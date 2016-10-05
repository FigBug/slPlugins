/*
Copyright (c) 2013, 2014, Louis P. Santillan <lpsantil@gmail.com>
All rights reserved.

*/
#include <stdio.h>
#include "tap.h"

void plan( unsigned int num )
{
   printf( "1..%d\n",
           num );
}

void ok( unsigned int ok, const char* msg )
{
   static int testnum = 0;
   printf( "%s %d - %s\n",
           ( ok ? "ok" : "not ok" ),
           ++testnum,
           msg );
}
