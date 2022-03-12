#ifndef _OSMAND_CORE_WEATHER_TILE_RESOURCE_PROVIDER_P_H_
#define _OSMAND_CORE_WEATHER_TILE_RESOURCE_PROVIDER_P_H_

#include "stdlib_common.h"

#include "QtExtensions.h"
#include "ignore_warnings_on_external_includes.h"
#include <QDir>
#include <QMutex>
#include <QQueue>
#include <QSet>
#include <QPair>
#include <QReadWriteLock>
#include <QThreadPool>
#include <QWaitCondition>
#include "restore_internal_warnings.h"

#include "OsmAndCore.h"
#include "CommonTypes.h"
#include "PrivateImplementation.h"
#include "WeatherTileResourceProvider.h"
#include "TileSqliteDatabase.h"

namespace OsmAnd
{
    class WeatherTileResourceProvider_P Q_DECL_FINAL
        : public std::enable_shared_from_this<WeatherTileResourceProvider_P>
    {
        Q_DISABLE_COPY_AND_MOVE(WeatherTileResourceProvider_P);
    private:
        class OSMAND_CORE_API ObtainValueTask : public QRunnable
        {
            Q_DISABLE_COPY_AND_MOVE(ObtainValueTask);
        private:
            std::shared_ptr<WeatherTileResourceProvider_P> _provider;
            
        protected:
        public:
            ObtainValueTask(
                 const std::shared_ptr<WeatherTileResourceProvider_P> provider,
                 const std::shared_ptr<WeatherTileResourceProvider::ValueRequest> request,
                 const WeatherTileResourceProvider::ObtainValueAsyncCallback callback,
                 const bool collectMetric = false);
            virtual ~ObtainValueTask();
            
            const std::shared_ptr<WeatherTileResourceProvider::ValueRequest> request;
            const WeatherTileResourceProvider::ObtainValueAsyncCallback callback;
            const bool collectMetric;

            virtual void run() Q_DECL_OVERRIDE;
        };
        
        class OSMAND_CORE_API ObtainTileTask : public QRunnable
        {
            Q_DISABLE_COPY_AND_MOVE(ObtainTileTask);
        private:
            std::shared_ptr<WeatherTileResourceProvider_P> _provider;

            sk_sp<const SkImage> createTileImage(
                const QHash<BandIndex, sk_sp<const SkImage>>& bandImages,
                const QList<BandIndex>& bands);

            void obtainRasterTile();
            void obtainContourTile();

        protected:
        public:
            ObtainTileTask(
                 const std::shared_ptr<WeatherTileResourceProvider_P> provider,
                 const std::shared_ptr<WeatherTileResourceProvider::TileRequest> request,
                 const WeatherTileResourceProvider::ObtainTileDataAsyncCallback callback,
                 const bool collectMetric = false);
            virtual ~ObtainTileTask();
            
            const std::shared_ptr<WeatherTileResourceProvider::TileRequest> request;
            const WeatherTileResourceProvider::ObtainTileDataAsyncCallback callback;
            const bool collectMetric;

            virtual void run() Q_DECL_OVERRIDE;
        };
                
        class OSMAND_CORE_API DownloadGeoTileTask : public QRunnable
        {
            Q_DISABLE_COPY_AND_MOVE(DownloadGeoTileTask);
        private:
            std::shared_ptr<WeatherTileResourceProvider_P> _provider;
            
        protected:
        public:
            DownloadGeoTileTask(
                 const std::shared_ptr<WeatherTileResourceProvider_P> provider,
                 const std::shared_ptr<WeatherTileResourceProvider::DownloadGeoTileRequest> request,
                 const WeatherTileResourceProvider::DownloadGeoTilesAsyncCallback callback,
                 const bool collectMetric = false);
            virtual ~DownloadGeoTileTask();
            
            const std::shared_ptr<WeatherTileResourceProvider::DownloadGeoTileRequest> request;
            const WeatherTileResourceProvider::DownloadGeoTilesAsyncCallback callback;
            const bool collectMetric;

            virtual void run() Q_DECL_OVERRIDE;
        };
        
    private:
        QThreadPool *_threadPool;

        QHash<BandIndex, float> _bandOpacityMap;

        mutable QReadWriteLock _lock;
        int _priority;

        ZoomLevel _lastRequestedZoom;
        QList<BandIndex> _lastRequestedBands;
        int _requestVersion;

        int getAndDecreasePriority();
        
        mutable QMutex _geoTilesInProcessMutex;
        std::array< QSet< TileId >, ZoomLevelsCount > _geoTilesInProcess;
        QWaitCondition _waitUntilAnyGeoTileIsProcessed;
        
        mutable QReadWriteLock _geoDbLock;
        std::shared_ptr<TileSqliteDatabase> _geoTilesDb;

        mutable QMutex _rasterTilesInProcessMutex;
        std::array< QSet< TileId >, ZoomLevelsCount > _rasterTilesInProcess;
        QWaitCondition _waitUntilAnyRasterTileIsProcessed;

        mutable QReadWriteLock _rasterDbLock;
        QHash<BandIndex, std::shared_ptr<TileSqliteDatabase>> _rasterTilesDbMap;
        std::shared_ptr<OsmAnd::TileSqliteDatabase> createRasterTilesDatabase(BandIndex band);

        mutable QMutex _contourTilesInProcessMutex;
        std::array< QSet< TileId >, ZoomLevelsCount > _contourTilesInProcess;
        QWaitCondition _waitUntilAnyContourTileIsProcessed;

    protected:
        WeatherTileResourceProvider_P(
            WeatherTileResourceProvider* const owner,
            const QDateTime& dateTime,
            const QHash<BandIndex, float>& bandOpacityMap,
            const QHash<BandIndex, QString>& bandColorProfilePaths,
            const QString& localCachePath,
            const QString& projResourcesPath,
            const uint32_t tileSize = 256,
            const float densityFactor = 1.0f,
            const std::shared_ptr<const IWebClient>& webClient = std::shared_ptr<const IWebClient>(new WebClient())
        );
        
    public:
        ~WeatherTileResourceProvider_P();

        ImplementationInterface<WeatherTileResourceProvider> owner;

        const std::shared_ptr<const IWebClient> webClient;
        
        const QDateTime dateTime;
        const QHash<BandIndex, QString> bandColorProfilePaths;

        const QString localCachePath;
        const QString projResourcesPath;
        const uint32_t tileSize;
        const float densityFactor;

        void obtainValueAsync(
            const WeatherTileResourceProvider::ValueRequest& request,
            const WeatherTileResourceProvider::ObtainValueAsyncCallback callback,
            const bool collectMetric = false);
        
        void obtainDataAsync(
            const WeatherTileResourceProvider::TileRequest& request,
            const WeatherTileResourceProvider::ObtainTileDataAsyncCallback callback,
            const bool collectMetric = false);

        void downloadGeoTilesAsync(
            const WeatherTileResourceProvider::DownloadGeoTileRequest& request,
            const WeatherTileResourceProvider::DownloadGeoTilesAsyncCallback callback,
            const bool collectMetric = false);
        
        const QHash<BandIndex, float> getBandOpacityMap() const;
        void setBandOpacityMap(const QHash<BandIndex, float>& bandOpacityMap);

        int getCurrentRequestVersion() const;
        int getAndUpdateRequestVersion(
            const std::shared_ptr<WeatherTileResourceProvider::TileRequest>& request = nullptr);

        bool obtainGeoTile(
            const TileId tileId,
            const ZoomLevel zoom,
            QByteArray& outData,
            bool forceDownload = false);

        void lockGeoTile(const TileId tileId, const ZoomLevel zoom);
        void unlockGeoTile(const TileId tileId, const ZoomLevel zoom);
        void lockRasterTile(const TileId tileId, const ZoomLevel zoom);
        void unlockRasterTile(const TileId tileId, const ZoomLevel zoom);
        void lockContourTile(const TileId tileId, const ZoomLevel zoom);
        void unlockContourTile(const TileId tileId, const ZoomLevel zoom);

        std::shared_ptr<TileSqliteDatabase> getGeoTilesDatabase();
        std::shared_ptr<TileSqliteDatabase> getRasterTilesDatabase(BandIndex band);

        bool closeProvider();
        
    friend class OsmAnd::WeatherTileResourceProvider;
    };
}

#endif // !defined(_OSMAND_CORE_WEATHER_TILE_RESOURCE_PROVIDER_P_H_)
