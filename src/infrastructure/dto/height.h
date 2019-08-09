#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace nem2_sdk::internal::dto {
	class HeightDto {
	public:
		uint64_t height;
		static HeightDto from_json(std::istream& json);
		static HeightDto from_ptree(boost::property_tree::ptree &pTree);
	};
}
