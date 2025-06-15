#pragma once
#include <string_view>
#include "glm/glm.hpp"
namespace sugoma::graphics 
{
    enum class ShaderStage 
    {
        Global,
		Vertex,
		Fragment,
		Geometry,
		TessellationControl,
		TessellationEvaluation,
		Compute,
		RayGeneration,
		Intersection,
		AnyHit,
		ClosestHit,
		Miss,
		Callable,
		Task,
		Mesh
    };
    enum class FieldType {
        // Scalar Types
        Bool,
        Int,
        UInt,
        Float,
        Double,

        // Vector Types - Float
        Vec2,
        Vec3,
        Vec4,

        // Vector Types - Double
        DVec2,
        DVec3,
        DVec4,

        // Vector Types - Integer
        IVec2,
        IVec3,
        IVec4,
        UVec2,
        UVec3,
        UVec4,

        // Vector Types - Boolean
        BVec2,
        BVec3,
        BVec4,

        //// Matrix Types - Float
        //Mat2,
        //Mat3,
        //Mat4,
        //Mat2x3,
        //Mat3x2,
        //Mat2x4,
        //Mat4x2,
        //Mat3x4,
        //Mat4x3,

        //// Matrix Types - Double
        //DMat2,
        //DMat3,
        //DMat4,
        //DMat2x3,
        //DMat3x2,
        //DMat2x4,
        //DMat4x2,
        //DMat3x4,
        //DMat4x3,

        // Sampler Types - Float
        Sampler1D,
        Sampler2D,
        Sampler3D,
        SamplerCube,
        Sampler2DRect,
        Sampler1DArray,
        Sampler2DArray,
        SamplerBuffer,
        Sampler2DMS,
        Sampler2DMSArray,
        SamplerCubeArray,

        // Sampler Types - Integer
        ISampler1D,
        ISampler2D,
        ISampler3D,
        ISamplerCube,
        ISampler2DRect,
        ISampler1DArray,
        ISampler2DArray,
        ISamplerBuffer,
        ISampler2DMS,
        ISampler2DMSArray,
        ISamplerCubeArray,

        // Sampler Types - Unsigned Integer
        USampler1D,
        USampler2D,
        USampler3D,
        USamplerCube,
        USampler2DRect,
        USampler1DArray,
        USampler2DArray,
        USamplerBuffer,
        USampler2DMS,
        USampler2DMSArray,
        USamplerCubeArray,

        // Sampler Types - Shadow
        Sampler1DShadow,
        Sampler2DShadow,
        SamplerCubeShadow,
        Sampler2DRectShadow,
        Sampler1DArrayShadow,
        Sampler2DArrayShadow,
        SamplerCubeArrayShadow,

        // Image Types - Float
        Image1D,
        Image2D,
        Image3D,
        ImageCube,
        Image2DRect,
        Image1DArray,
        Image2DArray,
        ImageCubeArray,
        ImageBuffer,
        Image2DMS,
        Image2DMSArray,

        // Image Types - Integer
        IImage1D,
        IImage2D,
        IImage3D,
        IImageCube,
        IImage2DRect,
        IImage1DArray,
        IImage2DArray,
        IImageCubeArray,
        IImageBuffer,
        IImage2DMS,
        IImage2DMSArray,

        // Image Types - Unsigned Integer
        UImage1D,
        UImage2D,
        UImage3D,
        UImageCube,
        UImage2DRect,
        UImage1DArray,
        UImage2DArray,
        UImageCubeArray,
        UImageBuffer,
        UImage2DMS,
        UImage2DMSArray,

        // Special/Custom Types
        Struct,
        Array
    };
    enum class BufferLayout 
    {
		None,
		Std140,
		Std430,
		Packed,
		Interleaved
    };
}