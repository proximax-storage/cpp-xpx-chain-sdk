#include "context.h"

using namespace nem2_sdk::internal::network;

Context::Context() :
		_ctx(ssl::context::tls_client)
{
	_ctx.set_default_verify_paths();
	_ctx.set_verify_mode(ssl::verify_peer);
}

ssl::context& Context::get_ssl_context() {
	return _ctx;
}