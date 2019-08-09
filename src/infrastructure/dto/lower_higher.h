#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace nem2_sdk::internal::dto {
	class LowerHigherDto {
	public:
		uint64_t lower, higher;
		static LowerHigherDto from_json(std::istream& json);
		static LowerHigherDto from_ptree(boost::property_tree::ptree &pTree);
	};
}
