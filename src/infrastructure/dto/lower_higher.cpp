#include "lower_higher.h"
#include <nemcpp/client.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace nem2_sdk::internal::dto;

LowerHigherDto LowerHigherDto::from_json(std::istream& json) {
	boost::property_tree::ptree pTree;
	boost::property_tree::read_json(json, pTree);

	try {
		auto lower = pTree.get_child("height").back().second.get_value<uint64_t>();
		auto higher = pTree.get_child("height").front().second.get_value<uint64_t>();
		LowerHigherDto dto{lower, higher};
		return dto;
	} catch (boost::property_tree::ptree_bad_path& e) {
		throw InvalidJSON(e);
	}
}
