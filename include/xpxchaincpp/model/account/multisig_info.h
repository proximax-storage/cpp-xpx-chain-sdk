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