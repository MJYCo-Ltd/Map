#ifndef MAP_GLOBAL_H
#define MAP_GLOBAL_H

#include<NoQt.h>

#if defined(MAP_LIBRARY)
#  define MAPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAP_GLOBAL_H
