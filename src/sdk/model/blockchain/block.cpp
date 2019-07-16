#include <nemcpp/client.h>
#include <nemcpp/model/blockchain/block.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace nem2_sdk;

BlockMeta BlockMeta::from_json(std::istream& json) {
	boost::property_tree::ptree pTree;
	boost::property_tree::read_json(json, pTree);

	try {

	} catch (boost::property_tree::ptree_bad_path& e) {
		throw InvalidJSON(e);
	}
}
