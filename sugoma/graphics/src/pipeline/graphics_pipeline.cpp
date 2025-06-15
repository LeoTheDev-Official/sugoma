#include "graphics_pipeline.h"
#include "debug/logger.h"

#include <GL/glew.h>
namespace sugoma::graphics 
{
	void __process_vertex_input(GLHandle handle, std::vector<PropertyInfo>& layout) 
	{
		int count;
		glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &count);
		for (int i = 0; i < count; ++i) {
			GLchar name[256];
			GLsizei length;
			GLint size;
			GLenum type;

			glGetActiveAttrib(handle, i, sizeof(name), &length, &size, &type, name);
			GLint location = glGetAttribLocation(handle, name);

			PropertyInfo cmp{};
			cmp.size = size;
			cmp.name = std::string(name, length);
			cmp.type = (PipelineAttributeType)type;
			layout.push_back(cmp);
		}
	}




	VertexStage::VertexStage(const PipelineStageCreateInfo& info) : PipelineStage(info)
	{
		m_handle = GLUtility::CompileShader(info.source, GL_VERTEX_SHADER);
	}
	void VertexStage::Invalidate(GLHandle program)
	{
		__process_vertex_input(program, m_input_layout);
	}
	FragmentStage::FragmentStage(const PipelineStageCreateInfo& info) : PipelineStage(info)
	{
		m_handle = GLUtility::CompileShader(info.source, GL_FRAGMENT_SHADER);
	}

	void FragmentStage::Invalidate(GLHandle program)
	{

	}

	GraphicsPipeline::GraphicsPipeline(const PipelineCreateInfo& info, const std::vector<std::string>& publicParams) : Pipeline(info)
	{
		m_kind = PipelineKind::Graphics;
		PipelineStageFlagBits flags = (PipelineStageFlagBits)0;
		for (auto& stage : info.stages) 
		{
			if (flags & stage.stage) 
			{
				sugoma_error("Multiple graphics pipeline stage infos assigned to the same stage. Stage : " << stage.stage);
				continue;
			}
			switch (stage.stage)
			{
			case VertexStageBit:
				m_stages.push_back(new VertexStage(stage));
				break;
			case GeometryStageBit:
				break;
			case TesselationStageBit:
				break;
			case FragmentStageBit:
				m_stages.push_back(new FragmentStage(stage));
				break;
			}
		}
		m_handle = glCreateProgram();
		for (auto& stage : m_stages)
			glAttachShader(m_handle, stage->Handle());
		GLUtility::LinkProgram(m_handle);
		Invalidate();
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		for (auto& stage : m_stages)
			delete stage;
		glDeleteProgram(m_handle);
	}

	void GraphicsPipeline::Invalidate()
	{
		for (auto& stage : m_stages)
			stage->Invalidate(m_handle);
		Pipeline::Invalidate(); //fetch pipeline parameters
		m_publicParams.clear();
		for (auto& param : m_params)
			if (param.second.name.find("mat_") == 0)
				m_publicParams.push_back({ .name = param.second.name.substr(4), .internalName = param.second.name});
	}

	Ref<GraphicsPipeline> GraphicsPipeline::Create(const PipelineCreateInfo& info, const std::vector<std::string>& publicParams)
	{
		auto ref = Resources::Create<GraphicsPipeline>(info, publicParams);
		if(info.pipelineName != "") PipelineLibrary::RegisterPipeline(ref);
		return ref;
	}

}