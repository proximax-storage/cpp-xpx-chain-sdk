
#include <xpxchaincpp/model/mosaic/mosaic_property.h>

#include <utility>

namespace xpx_chain_sdk {
	
	bool operator==(const MosaicProperty& lhs, const MosaicProperty& rhs)
	{
		return lhs.id == rhs.id && lhs.value == rhs.value;
	}
	
	bool operator!=(const MosaicProperty& lhs, const MosaicProperty& rhs)
	{
		return !(lhs == rhs);
	}
	
	MosaicProperties::MosaicProperties(MosaicPropertyContainer propertyContainer):
		properties_(std::move(propertyContainer))
	{
		for (uint32_t i = 0; i < to_underlying_type(MosaicPropertyId::Divisibility); ++i) {
			if (!hasProperty(static_cast<MosaicPropertyId>(i))) {
				XPX_CHAIN_SDK_THROW_1(mosaic_error, "required property is not found", i);
			}
		}
	}
	
	MosaicProperties::MosaicProperties(std::initializer_list<MosaicProperty> properties):
		MosaicProperties(MosaicPropertyContainer(properties))
	{ }
	
	MosaicProperties::MosaicProperties(MosaicFlags flags, uint8_t divisibility, std::optional<BlockDuration> duration):
		properties_({ MosaicProperty{ MosaicPropertyId::Flags, to_underlying_type(flags) },
		              MosaicProperty{ MosaicPropertyId::Divisibility, divisibility } })
	{
		if (duration) {
			properties_.emplace(MosaicProperty{ MosaicPropertyId::Duration, *duration });
		}
	}
	
	MosaicFlags MosaicProperties::flags() const
	{
		return getPropertyValue<MosaicFlags>(MosaicPropertyId::Flags);
	}
	
	uint8_t MosaicProperties::divisibility() const
	{
		return getPropertyValue<uint8_t>(MosaicPropertyId::Divisibility);
	}
	
	BlockDuration MosaicProperties::duration() const
	{
		return getPropertyValue<BlockDuration>(MosaicPropertyId::Duration, Eternal_Duration);
	}
	
	bool MosaicProperties::isEternal() const
	{
		return duration() == Eternal_Duration;
	}
	
	MosaicProperty MosaicProperties::getProperty(MosaicPropertyId id, uint64_t defaultValue) const
	{
		return MosaicProperty{ id, getPropertyValue(id, defaultValue) };
	}
	
	bool MosaicProperties::hasProperty(MosaicPropertyId id) const
	{
		return properties_.find(MosaicProperty{ id }) != end();
	}
	
	size_t MosaicProperties::size() const
	{
		return properties_.size();
	}
}
