rule("module.program")
    on_load(function (target)
        target:set("kind", "binary")
        target:set("rundir", os.projectdir())
        if target:is_plat("windows") and target:has_runtime("MT") then
            target:add("packages", "vc-ltl5")
        end
    end)

rule("module.component")
    on_load(function (target)
        if has_config("development") then
            target:set("kind", "shared")
        else
            target:set("kind", "static")
        end
    end)
