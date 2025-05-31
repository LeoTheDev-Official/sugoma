 libdirs { vendor_path .. "graphics/lib" }
 includedirs { vendor_path .. "graphics/inc", "%{wks.location}/core/src" }
 links { "core" }
 defines { "GLEW_STATIC" }
 filter "platforms:x86"
        links { "glfw3_mt.lib", "glew32s.lib", "opengl32.lib" }
    filter "platforms:x64"
        links { "glfw3_mtx64.lib", "glew64s.lib", "opengl32.lib" }
    filter {}