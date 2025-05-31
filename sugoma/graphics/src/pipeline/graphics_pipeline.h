#pragma once
#include "pipeline.h"
#include "buffers/vertex_array_object.h"

namespace sugoma::graphics 
{
	struct PropertyInfo 
	{
		std::string name;
		uint16_t location;
		PipelineAttributeType type;
		int16_t size;
	};
	class VertexStage : public PipelineStage
	{
	public:
		VertexStage() = delete;
		VertexStage(const PipelineStageCreateInfo& info);
		void Invalidate(GLHandle program) override;
	private:
		std::vector<PropertyInfo> m_input_layout;
	};
	class FragmentStage : public PipelineStage 
	{
	public:
		FragmentStage() = delete;
		FragmentStage(const PipelineStageCreateInfo& info);
		void Invalidate(GLHandle program) override;
	private:

	};

	class GraphicsPipeline : public Pipeline 
	{
	public:
		GraphicsPipeline() = delete;
		GraphicsPipeline(const PipelineCreateInfo& info);
		~GraphicsPipeline();
		void Invalidate() override;
		
	};
}