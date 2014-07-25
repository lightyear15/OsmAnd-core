#include "MapPresentationEnvironment.h"
#include "MapPresentationEnvironment_P.h"

#include "MapStyleValue.h"
#include "IExternalResourcesProvider.h"

OsmAnd::MapPresentationEnvironment::MapPresentationEnvironment(
    const std::shared_ptr<const MapStyle>& style_,
    const float displayDensityFactor_ /*= 1.0f*/,
    const QString& localeLanguageId_ /*= QLatin1String("en")*/,
    const std::shared_ptr<const IExternalResourcesProvider>& externalResourcesProvider_ /*= nullptr*/)
    : _p(new MapPresentationEnvironment_P(this))
    , styleBuiltinValueDefs(MapStyle::getBuiltinValueDefinitions())
    , style(style_)
    , displayDensityFactor(displayDensityFactor_)
    , localeLanguageId(localeLanguageId_)
    , externalResourcesProvider(externalResourcesProvider_)
    , dummyMapSection(_p->dummyMapSection)
    , mapPaint(_p->_mapPaint)
    , textPaint(_p->_textPaint)
    , oneWayPaints(_p->_oneWayPaints)
    , reverseOneWayPaints(_p->_reverseOneWayPaints)
{
    _p->initialize();
}

OsmAnd::MapPresentationEnvironment::~MapPresentationEnvironment()
{
}

QHash< std::shared_ptr<const OsmAnd::MapStyleValueDefinition>, OsmAnd::MapStyleValue > OsmAnd::MapPresentationEnvironment::getSettings() const
{
    return _p->getSettings();
}

void OsmAnd::MapPresentationEnvironment::setSettings(const QHash< std::shared_ptr<const MapStyleValueDefinition>, MapStyleValue >& newSettings)
{
    _p->setSettings(newSettings);
}

void OsmAnd::MapPresentationEnvironment::setSettings(const QHash< QString, QString >& newSettings)
{
    _p->setSettings(newSettings);
}

void OsmAnd::MapPresentationEnvironment::applyTo(MapStyleEvaluator& evaluator) const
{
    _p->applyTo(evaluator);
}

void OsmAnd::MapPresentationEnvironment::configurePaintForText(SkPaint& paint, const QString& text, const bool bold, const bool italic) const
{
    _p->configurePaintForText(paint, text, bold, italic);
}

bool OsmAnd::MapPresentationEnvironment::obtainBitmapShader(const QString& name, SkBitmapProcShader* &outShader) const
{
    return _p->obtainBitmapShader(name, outShader);
}

bool OsmAnd::MapPresentationEnvironment::obtainPathEffect(const QString& encodedPathEffect, SkPathEffect* &outPathEffect) const
{
    return _p->obtainPathEffect(encodedPathEffect, outPathEffect);
}

bool OsmAnd::MapPresentationEnvironment::obtainMapIcon(const QString& name, std::shared_ptr<const SkBitmap>& outIcon) const
{
    return _p->obtainMapIcon(name, outIcon);
}

bool OsmAnd::MapPresentationEnvironment::obtainTextShield(const QString& name, std::shared_ptr<const SkBitmap>& outIcon) const
{
    return _p->obtainTextShield(name, outIcon);
}

OsmAnd::ColorARGB OsmAnd::MapPresentationEnvironment::getDefaultBackgroundColor(const ZoomLevel zoom) const
{
    return _p->getDefaultBackgroundColor(zoom);
}

void OsmAnd::MapPresentationEnvironment::obtainShadowRenderingOptions(const ZoomLevel zoom, int& mode, ColorARGB& color) const
{
    _p->obtainShadowRenderingOptions(zoom, mode, color);
}

double OsmAnd::MapPresentationEnvironment::getPolygonAreaMinimalThreshold(const ZoomLevel zoom) const
{
    return _p->getPolygonAreaMinimalThreshold(zoom);
}

unsigned int OsmAnd::MapPresentationEnvironment::getRoadDensityZoomTile(const ZoomLevel zoom) const
{
    return _p->getRoadDensityZoomTile(zoom);
}

unsigned int OsmAnd::MapPresentationEnvironment::getRoadsDensityLimitPerTile(const ZoomLevel zoom) const
{
    return _p->getRoadsDensityLimitPerTile(zoom);
}
