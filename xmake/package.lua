-- dev

-- if is_mode("debug") then
--     add_requireconfs("*", {configs = {shared = true}})
-- end

-- set_policy("package.install_locally", true)

add_requires(
    "raw_pdb",
    "fast_io",
    "cppitertools",
    "bddisasm"
)

-- if is_plat("windows") and is_mode("release") then
--     add_requires("vc-ltl5")
-- end

-- test

-- add_requires("boost_ut", {optional = true})

package("raw_pdb")
    set_homepage("https://github.com/MolecularMatters/raw_pdb")
    set_description("A C++11 library for reading Microsoft Program DataBase PDB files")
    set_license("BSD-2-Clause")

    add_urls("https://github.com/MolecularMatters/raw_pdb.git")

    add_versions("2024.10.15", "479cd28468481f4df43cb392953da7e6636c70b6")

    on_install("!macosx and !iphoneos", function (package)
        io.writefile("xmake.lua", [[
            add_rules("mode.debug", "mode.release")
            set_languages("c++20")
            target("raw_pdb")
                set_kind("$(kind)")
                add_files("src/*.cpp")
                set_pcxxheader("src/PDB_PCH.h")
                add_headerfiles("src/(Foundation/*.h)", "src/*.h|PDB_PCH.h", {prefixdir = "raw_pdb"})
                add_installfiles("raw_pdb.natvis", {prefixdir = "include/natvis"})
                if is_plat("windows") and is_kind("shared") then
                    add_rules("utils.symbols.export_all", {export_classes = true})
                end
        ]])
        import("package.tools.xmake").install(package)
    end)

    on_test(function (package)
        assert(package:has_cxxfuncs("PDB::ValidateFile(0, 0)", {includes = {"cstddef", "raw_pdb/PDB.h"}}))
    end)
