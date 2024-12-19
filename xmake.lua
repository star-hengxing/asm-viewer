set_project("asm-viewer")

set_version("0.0.1")

set_xmakever("2.9.6")

set_allowedmodes("debug", "release")

add_rules("mode.debug", "mode.release")

includes("src", "xmake")

if has_config("development") then
    add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

    set_languages("c++20")

    if is_plat("windows") then
        set_runtimes("MD")
        add_cxflags("/permissive-", {tools = "cl"})
    end

    set_encodings("source:utf-8")
end
