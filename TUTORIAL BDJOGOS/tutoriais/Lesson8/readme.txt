I have included the headers, libraries, and dll's necessary for you to compile the sources using gcc/mingw/dev-cpp. The header files provided for the OggVorbis sdk are not mingw compatible though. To get them to work you need make sure these types are defined like so:

   /* MinGW */
   typedef long long ogg_int64_t;
   typedef long int ogg_int32_t;
   typedef unsigned long ogg_uint32_t;
   typedef short int ogg_int16_t;

where they would normally be for cygwin in os_types.h.

Other than that just copy the include folder to your compilers include folder, and copy the lib files in your compilers lib directory.