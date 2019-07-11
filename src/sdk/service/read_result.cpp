
#include <nemcpp/service/read_result.h>

namespace nem2_sdk {
	
	ReadResult::ReadResult(ReadResultCode code, size_t consumed, std::string_view description):
		code_(code),
		description_(description),
		consumed_(consumed)
	{ }
	
	ReadResultCode ReadResult::code() const
	{
		return code_;
	}
	
	const std::string& ReadResult::description() const
	{
		return description_;
	}
	
	size_t ReadResult::consumed() const
	{
		return consumed_;
	}
	
	ReadResult::operator bool() const
	{
		return code_ == ReadResultCode::Success;
	}
}
