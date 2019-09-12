/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <xpxchaincpp/model/mosaic/mosaic.h>

/*
 "multisig":

{

    "account": "string",
    "accountAddress": "string",
    "minApproval": 0,
    "minRemoval": 0,
    "cosignatories":

[

    "string"

],
"multisigAccounts":

    [
        "string"
    ]

}*/
namespace xpx_chain_sdk {
    class MultisigInfo {
    public:
        std::string account;
        std::string accountAddress;
        uint64_t minApproval;
        uint64_t minRemoval;
        std::vector<std::string> cosignatories;
        std::vector<std::string> multisigAccounts;
    };
}