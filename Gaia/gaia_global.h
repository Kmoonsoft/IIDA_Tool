#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(GAIA_LIB)
#  define GAIA_EXPORT Q_DECL_EXPORT
# else
#  define GAIA_EXPORT Q_DECL_IMPORT
# endif
#else
# define GAIA_EXPORT
#endif
