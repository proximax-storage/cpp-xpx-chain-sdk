/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once


#include <xpxchaincpp/client/namespace_service.h>

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/model/account/public_account.h>
#include <xpxchaincpp/model/mosaic/mosaic_property.h>
#include <memory>
#include <xpxchaincpp/model/namespace/multiple_namespace_info.h>
#include <xpxchaincpp/model/namespace/namespace_names.h>

namespace xpx_chain_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;

    class NamespaceService {
    public:
        NamespaceService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
                std::shared_ptr<RequestParamsBuilder> builder
        );
        ~NamespaceService() = default;
        NamespaceInfo getNamespaceInfoById(const NamespaceId& id);
		NamespaceInfo getNamespaceInfoByHexId(const std::string& id);
		MultipleNamespaceInfo getNamespaceInfoByAccount(const std::string& accountId);
        MultipleNamespaceInfo getNamespaceInfoByAccounts(const std::vector<std::string>& accountIds);
        NamespaceNames getNamespaceNames(const std::vector<std::string> & namespaceIds);

    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
    };
}


