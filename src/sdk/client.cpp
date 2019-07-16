#include <nemcpp/client.h>
#include <nemcpp/client/block.h>

using namespace nem2_sdk;

Client::Client(std::shared_ptr<Config> config) : _config(config) {
	_blockchain = std::make_shared<Blockchain>(config);
}

std::shared_ptr<Blockchain> Client::blockchain() const {
	return _blockchain;
}

InvalidRequest::InvalidRequest(uint16_t code) :
	runtime_error(InvalidRequest::getErrorMessage(code))
{}

std::string InvalidRequest::getErrorMessage(uint16_t code) {
	std::stringstream s;
	s << "Server rejected the request. Error code: " << code;
	return s.str();
}

InvalidJSON::InvalidJSON(const std::runtime_error &error) : runtime_error(error) {}
