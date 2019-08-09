#include "lower_higher.h"
#include "height.h"
#include <nemcpp/client.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace nem2_sdk::internal::dto;

HeightDto HeightDto::from_json(std::istream& json) {
	boost::property_tree::ptree pTree;
	boost::property_tree::read_json(json, pTree);

	return HeightDto::from_ptree(pTree);
}

HeightDto HeightDto::from_ptree(boost::property_tree::ptree &pTree) {
	try {
		HeightDto dto{LowerHigherDto::from_ptree(pTree.get_child("height")).higher};
		return dto;
	} catch (boost::property_tree::ptree_bad_path& e) {
		throw InvalidJSON(e);
	}
}
