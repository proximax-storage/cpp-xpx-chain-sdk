/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <nemcpp/model/account/account_info.h>
#include <string>
#include <nemcpp/model/account/multisig_info.h>
#include <nemcpp/model/account/account_info.h>
#include <nemcpp/model/account/multiple_account_info.h>
#include <nemcpp/model/account/multisig_graph.h>
#include <nemcpp/model/account/account_properties.h>

namespace xpx_sdk {
    class IAccountService {
    public:
        virtual AccountInfo getAccountInfo(const std::string& id);
        virtual MultipleAccountInfo getAccountsInfo(const std::vector<std::string>& ids);
        virtual AccountProperty getAccountProperties(const std::string& id );
        virtual MultipleAccountProperty getAccountsProperties(const std::vector<std::string>& ids);
        virtual MultisigInfo getMultisigInfo(const std::string& id);
        virtual MultisigGraph getMultisigAccountGraphInfo(const std::string& id);
        virtual ~IAccountService(){};
    };
}