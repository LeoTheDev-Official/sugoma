
modules = { "core", "graphics", "physics", "imgui", "audio" }

build_dir = "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/"
build_int_dir = "%{wks.location}/bin-int/%{prj.name}/%{cfg.buildcfg}/%{cfg.architecture}/"
include_dir = "%{prj.location}/src"
proj_loc = "%{wks.location}/%{prj.name}"
vendor_path = "%{wks.location}/vendor/"
include_common = vendor_path .. "common"

workspace "sugoma"
    configurations { "Debug", "Release", "Deploy" }
    platforms { "x86", "x64" }
    startproject "sugoma_app"
    cppdialect "C++20"
    location "sugoma"

    flags { "MultiProcessorCompile" }
    
    files { "%{prj.location}/**.h", "%{prj.location}/**.cpp", "%{prj.location}/**.hpp" }

    filter "platforms:x86"
        architecture "x86"
    filter "platforms:x64"
        architecture "x86_64"
    filter {}

    defines { "GLM_ENABLE_EXPERIMENTAL" }

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG", "SUGOMA_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "SUGOMA_RELEASE" }
        optimize "On"
        symbols "On"

    filter "configurations:Deploy"
        defines { "SUGOMA_DEPLOY" }
        optimize "Full"
        symbols "Off"

    filter {}

function CreateCppProject(projectName, projectKind)
    project(projectName)
        kind(projectKind)
        language "C++"
        staticruntime "On"

        location (proj_loc)
        targetdir (build_dir)
        objdir (build_int_dir)
        debugdir ("sugoma/" .. build_dir)

        os.mkdir("sugoma/" .. projectName .. "/src")

        defines { "SUGOMA_MODULE_NAME=\"" .. string.upper(projectName) .. "\"" }

        includedirs { include_dir, include_common }
end

CreateCppProject("sugoma", "StaticLib")
links (modules)
function IncludeAll() for _, module in ipairs(modules) do includedirs { "sugoma/" .. module .. "/src" } end end
IncludeAll()
CreateCppProject("sugoma_app", "ConsoleApp")
links {"sugoma"}
IncludeAll()
includedirs { "sugoma/sugoma/src" }
for _, module in ipairs(modules) do
    CreateCppProject(module, "StaticLib")
    local path = "modules/" .. module .. ".lua"
    if(os.isfile(path)) then include(path) end
end