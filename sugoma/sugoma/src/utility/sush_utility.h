#pragma once
#include <filesystem>
#include "pipeline/pipeline.h"
#include <unordered_map>
namespace sugoma 
{
	using namespace graphics;
	struct SUSHResult 
	{
		PipelineKind kind = PipelineKind::Graphics;
		std::unordered_map<PipelineStageFlags, std::string> stages;
		std::vector<std::string> errors;
	};
	class SUSHUtility
	{
	public:
		static SUSHResult Process(const std::filesystem::path& path);
	};

}