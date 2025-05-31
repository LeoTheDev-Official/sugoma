#pragma once
#include "pipeline.h"
#include "glm/glm.hpp"
namespace sugoma::graphics 
{
	class ComputeStage : public PipelineStage 
	{
	public:
		ComputeStage() = delete;
		ComputeStage(const PipelineStageCreateInfo& info);
		void Invalidate(GLHandle program) override;
	};
	class ComputePipeline : public Pipeline 
	{
	public:
		ComputePipeline() = delete;
		ComputePipeline(const PipelineCreateInfo& info);
		~ComputePipeline();
		void Invalidate() override;
		const glm::uvec3& GroupSize() const;
	private:
		glm::uvec3 m_groupSize;
	};
}