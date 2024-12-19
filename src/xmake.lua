add_packages(
    "raw_pdb",
    "fast_io",
    "cppitertools",
    "bddisasm"
)

target("dump")
    set_kind("binary")
    add_files("*.cpp")

    add_ldflags("/map", {tools = "link"})

    -- set_rundir(os.projectdir())

    on_load(function (target)
        local map_file = path.join(target:targetdir(), target:basename() .. ".map")
        target:set("runargs", path.unix(map_file))
    end)
