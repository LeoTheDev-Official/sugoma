#pragma once
#include <string>
#include <vector>
#include "sugoma_gl.h"
#include "resources/resource.h"
namespace sugoma::graphics 
{
    using namespace sugoma::core;
	enum PipelineAttributeType 
	{
        // Float types
        Float = 0x1406, // GL_FLOAT
        FloatVec2 = 0x8B50, // GL_FLOAT_VEC2
        FloatVec3 = 0x8B51, // GL_FLOAT_VEC3
        FloatVec4 = 0x8B52, // GL_FLOAT_VEC4

        // Double types
        Double = 0x140A, // GL_DOUBLE
        DoubleVec2 = 0x8FFC, // GL_DOUBLE_VEC2
        DoubleVec3 = 0x8FFD, // GL_DOUBLE_VEC3
        DoubleVec4 = 0x8FFE, // GL_DOUBLE_VEC4

        // Integer types
        Int = 0x1404, // GL_INT
        IntVec2 = 0x8B53, // GL_INT_VEC2
        IntVec3 = 0x8B54, // GL_INT_VEC3
        IntVec4 = 0x8B55, // GL_INT_VEC4

        // Unsigned integer types
        UnsignedInt = 0x1405, // GL_UNSIGNED_INT
        UnsignedIntVec2 = 0x8DC6, // GL_UNSIGNED_INT_VEC2
        UnsignedIntVec3 = 0x8DC7, // GL_UNSIGNED_INT_VEC3
        UnsignedIntVec4 = 0x8DC8, // GL_UNSIGNED_INT_VEC4

        // Boolean types
        Bool = 0x8B56, // GL_BOOL
        BoolVec2 = 0x8B57, // GL_BOOL_VEC2
        BoolVec3 = 0x8B58, // GL_BOOL_VEC3
        BoolVec4 = 0x8B59, // GL_BOOL_VEC4

        // Matrices (float)
        Mat2 = 0x8B5A, // GL_FLOAT_MAT2
        Mat3 = 0x8B5B, // GL_FLOAT_MAT3
        Mat4 = 0x8B5C, // GL_FLOAT_MAT4
        Mat2x3 = 0x8B65, // GL_FLOAT_MAT2x3
        Mat2x4 = 0x8B66, // GL_FLOAT_MAT2x4
        Mat3x2 = 0x8B67, // GL_FLOAT_MAT3x2
        Mat3x4 = 0x8B68, // GL_FLOAT_MAT3x4
        Mat4x2 = 0x8B69, // GL_FLOAT_MAT4x2
        Mat4x3 = 0x8B6A, // GL_FLOAT_MAT4x3

        // Matrices (double)
        DoubleMat2 = 0x8F46, // GL_DOUBLE_MAT2
        DoubleMat3 = 0x8F47, // GL_DOUBLE_MAT3
        DoubleMat4 = 0x8F48, // GL_DOUBLE_MAT4
        DoubleMat2x3 = 0x8F49, // GL_DOUBLE_MAT2x3
        DoubleMat2x4 = 0x8F4A, // GL_DOUBLE_MAT2x4
        DoubleMat3x2 = 0x8F4B, // GL_DOUBLE_MAT3x2
        DoubleMat3x4 = 0x8F4C, // GL_DOUBLE_MAT3x4
        DoubleMat4x2 = 0x8F4D, // GL_DOUBLE_MAT4x2
        DoubleMat4x3 = 0x8F4E, // GL_DOUBLE_MAT4x3

        // Sampler types (only relevant for uniforms)
        Sampler1D = 0x8B5D, // GL_SAMPLER_1D
        Sampler2D = 0x8B5E, // GL_SAMPLER_2D
        Sampler3D = 0x8B5F, // GL_SAMPLER_3D
        SamplerCube = 0x8B60, // GL_SAMPLER_CUBE
        Sampler1DShadow = 0x8B61, // GL_SAMPLER_1D_SHADOW
        Sampler2DShadow = 0x8B62, // GL_SAMPLER_2D_SHADOW
        Sampler1DArray = 0x8DC0, // GL_SAMPLER_1D_ARRAY
        Sampler2DArray = 0x8DC1, // GL_SAMPLER_2D_ARRAY
        Sampler1DArrayShadow = 0x8DC3, // GL_SAMPLER_1D_ARRAY_SHADOW
        Sampler2DArrayShadow = 0x8DC4, // GL_SAMPLER_2D_ARRAY_SHADOW
        SamplerCubeShadow = 0x8DC5, // GL_SAMPLER_CUBE_SHADOW
        Sampler2DRect = 0x8B63, // GL_SAMPLER_2D_RECT
        Sampler2DRectShadow = 0x8B64, // GL_SAMPLER_2D_RECT_SHADOW
        IntSampler1D = 0x8DC9, // GL_INT_SAMPLER_1D
        IntSampler2D = 0x8DCA, // GL_INT_SAMPLER_2D
        IntSampler3D = 0x8DCB, // GL_INT_SAMPLER_3D
        IntSamplerCube = 0x8DCC, // GL_INT_SAMPLER_CUBE
        IntSampler1DArray = 0x8DCE, // GL_INT_SAMPLER_1D_ARRAY
        IntSampler2DArray = 0x8DCF, // GL_INT_SAMPLER_2D_ARRAY
        UIntSampler1D = 0x8DD1, // GL_UNSIGNED_INT_SAMPLER_1D
        UIntSampler2D = 0x8DD2, // GL_UNSIGNED_INT_SAMPLER_2D
        UIntSampler3D = 0x8DD3, // GL_UNSIGNED_INT_SAMPLER_3D
        UIntSamplerCube = 0x8DD4, // GL_UNSIGNED_INT_SAMPLER_CUBE
        UIntSampler1DArray = 0x8DD6, // GL_UNSIGNED_INT_SAMPLER_1D_ARRAY
        UIntSampler2DArray = 0x8DD7, // GL_UNSIGNED_INT_SAMPLER_2D_ARRAY

        Image1D = 0x904C,
        Image2D = 0x904D,
        Image3D = 0x904E,
        Image2DRect = 0x904F,
        ImageCube = 0x9050,
        ImageBuffer = 0x9051,
        Image1DArray = 0x9052,
        Image2DArray = 0x9053,
        Image2DMS = 0x9054,
        Image2DMSArray = 0x9055,
        IntImage1D = 0x9057,
        IntImage2D = 0x9058,
        IntImage3D = 0x9059,
        IntImage2DRect = 0x905A,
        IntImageCube = 0x905B,
        IntImageBuffer = 0x905C,
        IntImage1DArray = 0x905D,
        IntImage2DArray = 0x905E,
        IntImage2DMS = 0x905F,
        IntImage2DMSArray = 0x9060,
        UIntImage1D = 0x9061,
        UIntImage2D = 0x9062,
        UIntImage3D = 0x9063,
        UIntImage2DRect = 0x9064,
        UIntImageCube = 0x9065,
        UIntImageBuffer = 0x9066,
        UIntImage1DArray = 0x9067,
        UIntImage2DArray = 0x9068,
        UIntImage2DMS = 0x9069,
        UIntImage2DMSArray = 0x906A
	};
	enum PipelineStageFlagBits : uint8_t
	{
		VertexStageBit = (1 << 0),
		GeometryStageBit = (1 << 1),
		TesselationStageBit = (1 << 2),
		FragmentStageBit = (1 << 3),

		ComputeStageBit = (1 << 4)
	};
	using PipelineStageFlags = uint8_t;
    enum class PipelineKind 
    {
        UNKNOWN,
		Graphics,
		Compute
    };
	struct PipelineStageCreateInfo 
	{
		PipelineStageFlagBits stage;
		std::string source;
		std::vector<std::string_view> defines;
	};
    struct PipelineParameterInfo
    {
        std::string name;
        GLHandle location;
        PipelineAttributeType type;
        uint32_t count = 1;
    };
	class PipelineStage 
	{
	public:
		PipelineStage() = delete;
		GLHandle Handle() const;
		virtual void Invalidate(GLHandle program) {};
		virtual ~PipelineStage();
	protected:
		PipelineStage(const PipelineStageCreateInfo& info) {};
	protected:
		GLHandle m_handle = 0;
	};
	struct PipelineCreateInfo 
	{
        const char* pipelineName = "";
		std::vector<PipelineStageCreateInfo> stages;
		std::vector<std::string_view> defines;
	};
	class Pipeline : public Resource 
	{
	public:
		Pipeline() = delete;
		virtual ~Pipeline();
		GLHandle Handle() const;

        const std::unordered_map<std::string, PipelineParameterInfo>& Parameters() const;
		const std::string& PipelineName() const { return m_pipelineName; }
		inline PipelineKind Kind() const { return m_kind; } 
	protected:
		Pipeline(const PipelineCreateInfo& info);
        virtual void Invalidate();
	protected:
		std::vector<PipelineStage*> m_stages;
        std::unordered_map<std::string, PipelineParameterInfo> m_params;
		GLHandle m_handle = 0;
		std::string m_pipelineName;
        PipelineKind m_kind = PipelineKind::UNKNOWN;
	};
    class PipelineLibrary 
    {
    public:
        static Ref<Pipeline> GetPipeline(const std::string& name);
		static void RegisterPipeline(Ref<Pipeline> pipeline);
    };
}