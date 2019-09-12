/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "context.h"

namespace ssl = boost::asio::ssl;

using namespace xpx_chain_sdk::internal::network;

namespace ssl = boost::asio::ssl;

Context::Context() :
		_ctx(ssl::context::tls_client)
{
	_ctx.set_default_verify_paths();
	_ctx.set_verify_mode(ssl::verify_peer);
}

ssl::context& Context::get_ssl_context() {
	return _ctx;
}
