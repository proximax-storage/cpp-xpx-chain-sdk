#include <nemcpp/client/network_service.h>
#include <memory>
#include <nemcpp/config.h>
#include <sdk/client/blockchain_service.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/utils/read_json.h>

namespace xpx_sdk {
    class NetworkService :public INetworkService {
    public:
        NetworkService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
                internal::network::RequestParamsBuilder builder
        );
        NetworkInfo getNetworkInfo() override;
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
        internal::network::RequestParamsBuilder _builder;
    };
}

