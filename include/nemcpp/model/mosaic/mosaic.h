
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/utils/comparator.h>

#include <set>

namespace nem2_sdk {
	
	/// Mosaic data.
	struct Mosaic {
		/// Mosaic id.
		MosaicId id;
		
		/// Mosaic amount.
		Amount amount;

        /// Mosaic height
//        Height height;

		/// Generates mosaic id using \a owner key and \a nonce
		static MosaicId GenerateId(const Key& owner, uint32_t nonce);
	};
	
	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const Mosaic& lhs, const Mosaic& rhs);
	
	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const Mosaic& lhs, const Mosaic& rhs);
	
	/// Mosaics container.
	using MosaicContainer = std::set<Mosaic, data_comparator<Mosaic, MosaicId, &Mosaic::id>>;
}
