/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once
#include <nemcpp/model/namespace/namespace_info.h>
#include <nemcpp/types.h>
/*
 * Info on a single namespace by id

    Info on namespaces aliased with an account

    Info on namespaces aliased with accounts

    Get names of multiple namespaces

    Get ids of linked mosaics

    Get ids of linked accounts
*/
namespace nem2_sdk {
    class INamespaceService {
    public:
        virtual NamespaceInfo getNamespaceInfoById(const NamespaceId& id);
        virtual std::vector<NamespaceInfo> getNamespaceInfoByAccount();
        virtual std::vector<NamespaceInfo> getNamespaceInfoByAccounts();
        virtual std::vector<MosaicId> getMosaicIds();
//        virtual std::vector<AccountId> getAccountIds();
    };
}