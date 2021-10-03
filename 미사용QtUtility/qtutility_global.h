#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTUTILITY_LIB)
#  define QTUTILITY_EXPORT Q_DECL_EXPORT
# else
#  define QTUTILITY_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTUTILITY_EXPORT
#endif
