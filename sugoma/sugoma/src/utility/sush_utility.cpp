#include "sush_utility.h"
#include <sstream>
#include <fstream>
namespace sugoma 
{
	struct SUSHContext
	{
		PipelineStageFlags stage = 0;
		SUSHResult result;
	};
	void process_include(SUSHContext& ctx, const std::filesystem::path& includePath)
	{
		std::ifstream includeStream(includePath);
		if (!includeStream.is_open()) 
		{
			ctx.result.errors.push_back("Failed to open include file: " + includePath.string());
			return;
		}
		std::stringstream includeContent;
		includeContent << includeStream.rdbuf();
		includeStream.close();
		
		ctx.result.stages[ctx.stage] += includeContent.str() + "\n";
	}
	SUSHResult SUSHUtility::Process(const std::filesystem::path& path)
	{
		SUSHContext ctx{};
		std::ifstream stream(path);
		if (!stream.is_open()) 
		{
			ctx.result.errors.push_back("Failed to open SUSH file: " + path.string());
			return ctx.result;
		}
		std::stringstream sstr;
		sstr << stream.rdbuf();
		stream.close();

		std::string line;
		while (std::getline(sstr, line)) 
		{
			if(line.starts_with("#include")) 
			{
				std::string includePath = line.substr(8); // Skip "#include "
				includePath.erase(remove_if(includePath.begin(), includePath.end(), isspace), includePath.end());
				std::filesystem::path includeFilePath = path.parent_path() / includePath;
				if (!includePath.empty()) process_include(ctx, includeFilePath);
			} 
			else if (line.starts_with("#stage")) 
			{
				if (line.find("vertex") != std::string::npos) 
				{
					ctx.stage = PipelineStageFlagBits::VertexStageBit;
				} 
				else if (line.find("fragment") != std::string::npos) 
				{
					ctx.stage = PipelineStageFlagBits::FragmentStageBit;
				}
				else if(line.find("geometry") != std::string::npos) 
				{
					ctx.stage = PipelineStageFlagBits::GeometryStageBit;
				} 
				else if (line.find("compute") != std::string::npos) 
				{
					ctx.stage = PipelineStageFlagBits::ComputeStageBit;
					ctx.result.kind = PipelineKind::Compute;
				}
			} 
			else if (!line.empty()) 
			{
				ctx.result.stages[ctx.stage] += line + "\n";
			}
		}
		auto it = ctx.result.stages.find(0);
		if(it != ctx.result.stages.end()) 
		{
			for(auto& stage : ctx.result.stages) 
			{
				if (!stage.first) continue;
				stage.second = it->second + stage.second;
			}
		}
		return ctx.result;
	}
}