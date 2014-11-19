#ifndef _OSMAND_CORE_MAP_COMMON_TYPES_H_
#define _OSMAND_CORE_MAP_COMMON_TYPES_H_

#include <OsmAndCore/stdlib_common.h>

#include <OsmAndCore/QtExtensions.h>

#include <OsmAndCore.h>

namespace OsmAnd
{
    enum class MapSurfaceType
    {
        Undefined = -1,

        Mixed,
        FullLand,
        FullWater,
    };

    enum class AlphaChannelPresence
    {
        Present,
        NotPresent,
        Unknown
    };

    typedef int MapSymbolIntersectionClassId;

    enum class MapStyleRulesetType
    {
        Invalid = -1,

        Point,
        Polyline,
        Polygon,
        Text,
        Order,

        __LAST
    };
    enum : unsigned int {
        MapStyleRulesetTypesCount = static_cast<unsigned int>(MapStyleRulesetType::__LAST)
    };

    enum class MapStyleValueDataType
    {
        Boolean,
        Integer,
        Float,
        String,
        Color,
    };

    union TagValueId
    {
        uint64_t id;
#if !defined(SWIG)
        struct {
            int32_t tagId;
            int32_t valueId;
        };
#else
        // Fake unwrap for SWIG
        int32_t tagId, valueId;
#endif // !defined(SWIG)

#if !defined(SWIG)
        inline operator uint64_t() const
        {
            return id;
        }

        inline TagValueId& operator=(const uint64_t& that)
        {
            id = that;
            return *this;
        }

        inline bool operator==(const TagValueId& that)
        {
            return this->id == that.id;
        }

        inline bool operator!=(const TagValueId& that)
        {
            return this->id != that.id;
        }

        inline bool operator==(const uint64_t& that)
        {
            return this->id == that;
        }

        inline bool operator!=(const uint64_t& that)
        {
            return this->id != that;
        }
#endif // !defined(SWIG)

        static inline TagValueId compose(const int32_t tagId, const int32_t valueId)
        {
            TagValueId result;
            result.tagId = tagId;
            result.valueId = valueId;
            return result;
        }
    };

#if !defined(SWIG)
    static_assert(sizeof(TagValueId) == 8, "TagValueId must be 8 bytes in size");
#endif // !defined(SWIG)
}

#endif // !defined(_OSMAND_CORE_MAP_COMMON_TYPES_H_)
