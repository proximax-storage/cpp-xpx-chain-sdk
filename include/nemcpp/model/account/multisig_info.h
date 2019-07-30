#pragma once
#include <string>
#include <nemcpp/model/mosaic/mosaic.h>

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
namespace nem2_sdk {
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