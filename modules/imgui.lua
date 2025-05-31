libdirs { vendor_path .. "graphics/lib" }
links { "glfw3.lib", "core", "graphics" }
includedirs { vendor_path .. "graphics/inc", "%{wks.location}/core/src", "%{wks.location}/graphics/src"}