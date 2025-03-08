local path_src = "src"
local path_shd = path_src .. "/shader"

local path_lib = "lib"
local path_sokol_tools = path_lib .. "/sokol-tools-bin/bin"

local path_build = "build"
local path_bin = path_build .. "/bin"

local target_name = "C3DGame"

local host_system = os.host()
local host_arch = os.hostarch()

local libs = {}

if host_system == "macosx" or host_system == "linux" then
    libs.sdl = {
        include = os.outputof("pkg-config -cflags-only-I sdl3"):gsub("-I", ""),
        path = os.outputof("pkg-config -libs-only-L sdl3"):gsub("-L", ""),
        lib = os.outputof("pkg-config -libs-only-l sdl3"):gsub("-l", ""),
    }
end

libs.sokol = { include = path_lib .. "/sokol" }

libs.cglm = { include = path_lib .. "/cglm/include" }

libs

local sokol_shdc

if host_system == "macosx" then
    if host_arch == "ARM64" then
        sokol_shdc = path_sokol_tools .. "/osx_arm64/sokol-shdc"
    else
        sokol_shdc = path_sokol_tools .. "/osx/sokol_shdc"
    end
elseif host_system == "linux" then
    if host_arch == "ARM64" then
        sokol_shdc = path_sokol_tools .. "/linux_arm64/sokol-shdc"
    else
        sokol_shdc = path_sokol_tools .. "/linux/sokol-shdc"
    end
elseif host_system == "windows" then
    sokol_shdc = path_sokol_tools .. "/win32/sokol-shdc.exe"
end

local sokol_shader = os.matchfiles(path_shd .. "/*.glsl")

local shdc_commands = function()
    local m = {}
    for  i, v  in ipairs(sokol_shader) do
        m[i] = string.format("../%s --input ../%s --output ../%s.h --slang glsl300es:glsl410 --format=sokol_impl", sokol_shdc, v, v)
    end
    return m
end

local debug_define = "DEBUG"

workspace("C3DGame")
do
    location(path_build)
    configurations({ "DEBUG", "RELEASE" })
    platforms({ "MacOS_Arm", "MacOS", "Windows", "Linux" })
end

project("C3DGame")
do
    kind("ConsoleApp")

    language("C")
    cdialect("C99")

    targetdir(path_bin .. "/%{cfg.buildcfg}")
    targetname(target_name)
    targetprefix("%{cfg.plattform}")

    prebuildcommands(shdc_commands())

    objdir(path_build .. "/obj")

    files({
        (path_src .. "/*.c")
    })

    externalincludedirs({ libs.sdl.include, libs.sokol.include })

    libdirs({ libs.sdl.path })
    links({ libs.sdl.lib })

    filter({ "configurations:RELEASE" })
        optimize("Full")
        symbols("Off")

    filter({ "configurations:DEBUG" })
        -- optimize("On")
        symbols("On")
        targetsuffix("-debug")
        defines({ debug_define })

    filter({ "platforms:MacOS*" })
        links("OpenGl.framework")

    filter({ "platforms:MacOS_Arm" })
        architecture("ARM64")

    filter({})
end
