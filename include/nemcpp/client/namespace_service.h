/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once
#include <nemcpp/model/namespace/namespace_info.h>
#include <nemcpp/model/namespace/multiple_namespace_info.h>
#include <nemcpp/model/namespace/namespace_names.h>
#include <nemcpp/types.h>
/*
 * Info on a single namespace by id

    Info on namespaces aliased with an account

    Info on namespaces aliased with accounts

    Get names of multiple namespaces

    Get ids of linked mosaics

    Get ids of linked accounts
*/
namespace xpx_sdk {
    class INamespaceService {
    public:
        virtual NamespaceInfo getNamespaceInfoById(const NamespaceId& id) = 0;
        virtual MultipleNamespaceInfo getNamespaceInfoByAccount(const std::string& accountId) = 0;
        virtual MultipleNamespaceInfo getNamespaceInfoByAccounts(const std::vector<std::string>& accountIds) = 0;
        virtual NamespaceNames getNamespaceNames(const std::vector<std::string>& namespaceIds) = 0;
		virtual NamespaceInfo getNamespaceInfoById(const std::string& id) = 0;
//        virtual std::vector<string> getAccountIds(); //maybe in future if will know how to do it
        virtual ~INamespaceService() = default;
    };
}