#include "compute_pipeline.h"
#include <GL/glew.h>
namespace sugoma::graphics 
{
	ComputeStage::ComputeStage(const PipelineStageCreateInfo& info):
		PipelineStage(info)
	{
		m_handle = GLUtility::CompileShader(info.source, GL_COMPUTE_SHADER);
	}
	void ComputeStage::Invalidate(GLHandle program)
	{
		//PipelineStage::Invalidate(program);
	}
	ComputePipeline::ComputePipeline(const PipelineCreateInfo& info):
		Pipeline(info)
	{
		for (auto& stageInfo : info.stages)
			if (stageInfo.stage & PipelineStageFlagBits::ComputeStageBit)
				m_stages.push_back(new ComputeStage(stageInfo));
		m_handle = glCreateProgram();
		for (auto& stage : m_stages) glAttachShader(m_handle, stage->Handle());
		GLUtility::LinkProgram(m_handle);
		Invalidate();
	}
	ComputePipeline::~ComputePipeline()
	{
		for (auto& stage : m_stages)
			delete stage;
		glDeleteProgram(m_handle);
	}
	void ComputePipeline::Invalidate()
	{
		for (auto& stage : m_stages)
			stage->Invalidate(m_handle);
		Pipeline::Invalidate();
		GLint workGroupSize[3];
		glGetProgramiv(m_handle, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
		m_groupSize = { workGroupSize[0], workGroupSize[1], workGroupSize[2] };
	}
	const glm::uvec3& ComputePipeline::GroupSize() const { return m_groupSize; }
}