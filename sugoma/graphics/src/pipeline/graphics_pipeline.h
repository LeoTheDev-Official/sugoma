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
	struct PropertyBinding 
	{
		std::string name;
		std::string internalName;
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
		GraphicsPipeline(const PipelineCreateInfo& info, const std::vector<std::string>& publicParams = {});
		~GraphicsPipeline();
		void Invalidate() override;
		
		static Ref<GraphicsPipeline> Create(const PipelineCreateInfo& infoa, const std::vector<std::string>& publicParams = {});
	public:
		const std::vector<PropertyBinding>& PublicParams() const { return m_publicParams; }
	private:
		std::vector<PropertyBinding> m_publicParams;
	};
}