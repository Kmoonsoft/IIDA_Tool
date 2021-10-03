#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(INSIGHT_LIB)
#  define INSIGHT_EXPORT Q_DECL_EXPORT
# else
#  define INSIGHT_EXPORT Q_DECL_IMPORT
# endif
#else
# define INSIGHT_EXPORT
#endif
