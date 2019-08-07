#pragma once

/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/


#include <nemcpp/client/account_service.h>
#include <infrastructure/network/http.h>
#include <nemcpp/sdk.h>



namespace xpx_sdk {
    class AccountService : public IAccountService {
    public:
        AccountService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
                internal::network::RequestParamsBuilder builder);

        AccountInfo getAccountInfo(const std::string& id) override;
        MultipleAccountInfo getAccountsInfo(const std::vector<std::string>& ids) override;
        AccountProperty getAccountProperties(const std::string& id ) override;
        MultipleAccountProperty getAccountsProperties(const std::vector<std::string>& ids) override;
        MultisigInfo getMultisigInfo(const std::string& id) override;
        MultisigGraph getMultisigAccountGraphInfo(const std::string& id) override;
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
        internal::network::RequestParamsBuilder _builder;
    };
}


