#ifndef BACKGROUNDIMAGELAYER_H
#define BACKGROUNDIMAGELAYER_H

#include "gaia_global.h"
#include "MapLayer.h"

class GAIA_EXPORT BackgroundImageLayer : public MapLayer
{
public:
	BackgroundImageLayer(const QString& imgFile, QGraphicsItem *parent, const QString& name, const QString& id);
};

#endif // BACKGROUNDIMAGELAYER_H
