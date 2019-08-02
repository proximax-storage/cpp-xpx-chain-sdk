#include <nemcpp/client/network_service.h>
#include <memory>

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

