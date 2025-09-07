#pragma once
#include "sugoma_gl.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>

namespace sugoma::graphics 
{
	struct FramebufferSpecification;
	//Store the desired output layout
	//Check for framebuffer / template errors
	//Clear the framebuffer / maintain the data when begining the render pass
	//Set transparency flags, culling flags, etc

	enum CullMode
	{
		Front = 0x0404,
		Back = 0x0405,
		Both = 0x0408
	};
	struct RenderPassBufferClear
	{
		uint32_t fbIndex;
		glm::vec4 value;
	};
	struct RenderPassCullState
	{
		CullMode faceToCull = CullMode::Back;
	};
	enum BlendFactor
	{
		Zero = 0,
		One = 1,
		SrcColor = 0x0300,
		OneMinusSrcColor = 0x0301,
		DstColor = 0x0306,
		OneMinusDstColor = 0x0307,
		SrcAlpha = 0x0302,
		OneMinusSrcAlpha = 0x0303,
		DstAlpha = 0x0304,
		OneMinusDstAlpha = 0x0305,
		ConstantColor = 0x8001,
		OneMinusConstantColor = 0x8002,
		ConstantAlpha = 0x8003,
		OneMinusConstantAlpha = 0x8004,
		SrcAlphaSaturate = 0x0308,
	};
	enum BlendEquation
	{
		Add = 0x8006,
		Subtract = 0x800A,
		ReverseSubtract = 0x800B,
		Min = 0x8007,
		Max = 0x8008
	};

	struct RenderPassBlending
	{
		uint32_t fbIndex;
		BlendFactor sourceFactor = BlendFactor::SrcAlpha;
		BlendFactor destFactor = BlendFactor::OneMinusSrcAlpha;
		BlendEquation equation = BlendEquation::Add;
	};
	enum DepthFunc
	{
		Never = 0x0200,
		Less = 0x0201,
		Equal = 0x0202,
		LessEqual = 0x0203,
		Greater = 0x0204,
		NotEqual = 0x0205,
		GreaterEqual = 0x0206,
		Always = 0x0207
	};

	struct RenderPassDepthState 
	{
		DepthFunc depthFunction = DepthFunc::Less;
		bool depthWrite = true;
	};
	struct RenderPassState 
	{
		std::vector<RenderPassBufferClear> clearState;
		bool enableCulling = false;
		RenderPassCullState cullState;
		bool enableBlending = false;
		std::vector<RenderPassBlending> blendingState;
		bool enableDepth = false;
		RenderPassDepthState depthState;
	};
	struct RenderPassCreateInfo 
	{
		RenderPassState state;
		const FramebufferSpecification* fbSpec;
		const char* debugName;
	};
	class Framebuffer;
	class RenderPass 
	{
	public:
		RenderPass() = default;
		RenderPass(const RenderPassCreateInfo& createInfo);

		void BeginRenderPass(Framebuffer* framebuffer);
		void EndRenderPass();
		
		RenderPassState& State();

	protected:
		void SetupDrawBuffers(Framebuffer* target);
		void ClearBuffers(Framebuffer* target);
		void SetCullState();
		void SetBlendState();
		void SetDepthState();
	private:
		std::string m_name;
		RenderPassState m_state;
		std::vector<int32_t> m_drawBuffers;
	};
}