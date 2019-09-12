/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/bitwise_enum.h>
#include <xpxchaincpp/utils/comparator.h>

#include <initializer_list>
#include <optional>
#include <set>

namespace xpx_chain_sdk {
	
	/// Mosaic property ids.
	enum class MosaicPropertyId: uint8_t {
		// required properties
		Flags = 0,
		Divisibility,
		
		// optional properties
		Duration
	};
	
	/// Mosaic flags.
	enum class MosaicFlags: uint8_t {
		None = 0x00,
		Supply_Mutable = 0x01,
		Transferable = 0x02,
		Levy_Mutable = 0x04,
		All = 0x07
	};
	
	XPX_CHAIN_SDK_BITWISE_ENUM(MosaicFlags);
	
	/// Mosaic property.
	struct MosaicProperty {
		/// Property id.
		MosaicPropertyId id;
		
		/// Property value.
		uint64_t value;
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const MosaicProperty& lhs, const MosaicProperty& rhs);
	
	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const MosaicProperty& lhs, const MosaicProperty& rhs);
	
	/// Mosaic properties set.
	using MosaicPropertyContainer = std::set<MosaicProperty, data_comparator<MosaicProperty, MosaicPropertyId, &MosaicProperty::id>>;
	
	/// Mosaic properties container.
	class MosaicProperties {
	public:
		/// Creates mosaic properties from \a propertySet.
		/// \note Throws \a mosaic_error if \a propertySet does not contain all of the required properties.
		MosaicProperties(MosaicPropertyContainer propertyContainer);
		
		/// Creates mosaic properties from \a properties.
		/// \note Throws \a mosaic_error if \a properties does not contain all of the required properties.
		MosaicProperties(std::initializer_list<MosaicProperty> properties);
		
		/// Creates mosaic properties.
		explicit MosaicProperties(MosaicFlags flags,
		                          uint8_t divisibility,
		                          std::optional<BlockDuration> duration = std::nullopt);
		
		/// Returns mosaic flags.
		MosaicFlags flags() const;
		
		/// Returns mosaic divisibility.
		/// \note  The divisibility determines up to what decimal place the mosaic can be divided into. For example,
		///        divisibility of 3 means that a mosaic can be divided into smallest parts of 0.001 mosaics.
		uint8_t divisibility() const;
		
		/// Returns mosaic duration.
		BlockDuration duration() const;
		
		/// Returns \c true if mosaic is eternal.
		bool isEternal() const;
		
		/// Returns property value or \a defaultValue if property does not exist.
		template<typename T = uint64_t>
		T getPropertyValue(MosaicPropertyId id, T defaultValue = static_cast<T>(0)) const
		{
			auto it = properties_.find(MosaicProperty{ id });
			return it != properties_.end() ? static_cast<T>(it->value) : defaultValue;
		}
		
		/// Returns property initialized to \a defaultValue if it does not exist.
		MosaicProperty getProperty(MosaicPropertyId id, uint64_t defaultValue = 0) const;
		
		/// Returns \c true if property exists.
		bool hasProperty(MosaicPropertyId id) const;
		
		/// Returns number of properties.
		size_t size() const;
		
		/// Returns a constant iterator to the first property.
		auto begin() const
		{ return properties_.begin(); }
		
		/// Returns a constant iterator to the element following the last property.
		auto end() const
		{ return properties_.end(); }
		
		/// Returns a constant iterator to the first optional property.
		auto optionalBegin() const
		{ return properties_.upper_bound(MosaicPropertyId::Divisibility); }
		
	private:
		MosaicPropertyContainer properties_;
	};
}
