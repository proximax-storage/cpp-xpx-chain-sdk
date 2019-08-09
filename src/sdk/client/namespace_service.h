/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once


#include <nemcpp/client/namespace_service.h>

#include <nemcpp/types.h>
#include <nemcpp/model/account/public_account.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <memory>
#include <infrastructure/network/context.h>
#include <infrastructure/network/http.h>
#include <nemcpp/model/namespace/multiple_namespace_info.h>

namespace xpx_sdk {

    class NamespaceService : public INamespaceService {
    public:
        NamespaceService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
                internal::network::RequestParamsBuilder builder
        );
        NamespaceInfo getNamespaceInfoById(const NamespaceId& id) override;
		NamespaceInfo getNamespaceInfoById(const std::string& id) override;
		MultipleNamespaceInfo getNamespaceInfoByAccount(const std::string& accountId) override;
        MultipleNamespaceInfo getNamespaceInfoByAccounts(const std::vector<std::string>& accountIds) override;
        NamespaceNames getNamespaceNames(const std::vector<std::string> & namespaceIds) override;
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
        internal::network::RequestParamsBuilder _builder;
    };
}


