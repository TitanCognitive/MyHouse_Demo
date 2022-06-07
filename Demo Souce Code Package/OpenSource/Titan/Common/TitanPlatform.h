// Dependencies
#ifndef TITAN_PLATFORM_H
#define TITAN_PLATFORM_H

   // OS Dependencies
   #ifdef WIN32
   #include "Platform/Windows/TitanWindows.h"
   #else
      #ifdef linux
      #include "Platform/Linux/TitanLinux.h"
      #else
      #include "Platform/iOS/TitaniOS.h"
      #endif
   #endif
  
#endif

/*
   End of file
*/
