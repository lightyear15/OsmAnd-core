#ifndef _OSMAND_CORE_MAP_PRESENTATION_ENVIRONMENT_H_
#define _OSMAND_CORE_MAP_PRESENTATION_ENVIRONMENT_H_

#include <OsmAndCore/stdlib_common.h>

#include <OsmAndCore/QtExtensions.h>
#include <QMap>
#include <QList>

#include <OsmAndCore.h>
#include <OsmAndCore/CommonTypes.h>
#include <OsmAndCore/PrivateImplementation.h>
#include <OsmAndCore/IExternalResourcesProvider.h>
#include <OsmAndCore/Map/MapStyle.h>

#include <SkPaint.h>
class SkBitmapProcShader;
class SkPathEffect;
class SkBitmap;

namespace OsmAnd
{
    class MapStyleEvaluator;
    class MapStyleValueDefinition;
    struct MapStyleValue;
    class ObfMapSectionInfo;

    class MapPresentationEnvironment_P;
    class OSMAND_CORE_API MapPresentationEnvironment
    {
        Q_DISABLE_COPY(MapPresentationEnvironment);
    private:
        PrivateImplementation<MapPresentationEnvironment_P> _p;
    protected:
    public:
        MapPresentationEnvironment(
            const std::shared_ptr<const MapStyle>& style,
            const float displayDensityFactor = 1.0f,
            const QString& localeLanguageId = QLatin1String("en"),
            const std::shared_ptr<const IExternalResourcesProvider>& externalResourcesProvider = nullptr);
        virtual ~MapPresentationEnvironment();

        const std::shared_ptr<const MapStyleBuiltinValueDefinitions> styleBuiltinValueDefs;

        const std::shared_ptr<const MapStyle> style;
        const float displayDensityFactor;
        const QString localeLanguageId;
        const std::shared_ptr<const IExternalResourcesProvider> externalResourcesProvider;

        const SkPaint& mapPaint;
        const SkPaint& textPaint;
        void configurePaintForText(SkPaint& paint, const QString& text, const bool bold, const bool italic) const;

        const QList< SkPaint >& oneWayPaints;
        const QList< SkPaint >& reverseOneWayPaints;

        const std::shared_ptr<const ObfMapSectionInfo>& dummyMapSection;

        QHash< std::shared_ptr<const MapStyleValueDefinition>, MapStyleValue > getSettings() const;
        void setSettings(const QHash< std::shared_ptr<const MapStyleValueDefinition>, MapStyleValue >& newSettings);
        void setSettings(const QHash< QString, QString >& newSettings);

        void applyTo(MapStyleEvaluator& evaluator) const;

        bool obtainBitmapShader(const QString& name, SkBitmapProcShader* &outShader) const;
        bool obtainPathEffect(const QString& encodedPathEffect, SkPathEffect* &outPathEffect) const;
        bool obtainMapIcon(const QString& name, std::shared_ptr<const SkBitmap>& outIcon) const;
        bool obtainTextShield(const QString& name, std::shared_ptr<const SkBitmap>& outIcon) const;

        ColorARGB getDefaultBackgroundColor(const ZoomLevel zoom) const;
        void obtainShadowRenderingOptions(const ZoomLevel zoom, int& mode, ColorARGB& color) const;
        double getPolygonAreaMinimalThreshold(const ZoomLevel zoom) const;
        unsigned int getRoadDensityZoomTile(const ZoomLevel zoom) const;
        unsigned int getRoadsDensityLimitPerTile(const ZoomLevel zoom) const;

        enum {
            DefaultShadowLevelMin = 0,
            DefaultShadowLevelMax = 256,
        };
    };
}

#endif // !defined(_OSMAND_CORE_MAP_PRESENTATION_ENVIRONMENT_H_)