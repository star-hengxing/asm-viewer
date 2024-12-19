#include <map>
#include <vector>
#include <string>
#include <string_view>

#include <Windows.h>

#include <cppitertools/enumerate.hpp>
#include <bddisasm/bddisasm.h>
#include <fast_io.h>

#include "base/base.hpp"

using namespace fast_io::manipulators;

struct PE
{
    PIMAGE_DOS_HEADER dos;
    PIMAGE_NT_HEADERS nt;

    void init(std::byte* data)
    {
        dos = (PIMAGE_DOS_HEADER)data;
        nt = (PIMAGE_NT_HEADERS)((std::byte*)dos + dos->e_lfanew);
    }

    bool is_dos() noexcept
    {
        return dos->e_magic == IMAGE_DOS_SIGNATURE;
    }

    bool is_pe() noexcept
    {
        return nt->Signature == IMAGE_NT_SIGNATURE;
    }

    bool is_64() noexcept
    {
        return nt->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 && nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    }

    bool is_dll() noexcept
    {
        return nt->FileHeader.Characteristics & IMAGE_FILE_DLL;
    }
};

constexpr bool is_space(char ch) noexcept
{
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

std::string_view parse_string(const char* current) noexcept
{
    char ch = *current;
    auto const start = current;
    while (!is_space(ch))
    {
        current += 1;
        ch = *current;
    }
    return {start, static_cast<usize>(current - start)};
}

int main(int argc, char** argv)
{
    // if (argc != 2)
    // {
    //     return -1;
    // }
    auto const link_map_path = std::string_view{"dump.map"};
    auto const link_map_file = fast_io::native_file_loader{link_map_path};

    std::vector<std::string_view> current_line_info;
    std::vector<std::vector<std::string_view>> info;
    const char* current = link_map_file.data();
    auto const end = link_map_file.data() + link_map_file.size();
    while (current < end)
    {
        auto const ch = *current;
        if (ch == '\n')
        {
            if (!current_line_info.empty())
            {
                info.push_back(current_line_info);
                current_line_info.clear();
            }
            current += 1;
            continue;
        }
        else if (is_space(ch))
        {
            current += 1;
            continue;
        }

        auto str = parse_string(current);
        current_line_info.push_back(str);
        current += str.size() + 1;
    }

    struct symbol_info
    {
        std::string_view name;
        usize offset;
        usize size;
    };

    // std::vector<symbol_info> symbol_table;
    // for (auto const& line_info : info)
    // {
    //     auto const file = line_info.back();
    //     if (file.ends_with("cpp.obj"))
    //     {
    //         int offset = 0;
    //         auto name = line_info[1];
    //         auto colon_pos = line_info[0].find_last_of(':');
    //         if (colon_pos != std::string_view::npos)
    //         {
    //             auto const substr = std::string{line_info[0].substr(colon_pos + 1)};
    //             offset = std::stoi(substr.c_str(), 0, 16);
    //         }

    //         if (offset != 0)
    //         {
    //             symbol_table.emplace_back(name, offset);
    //         }
    //     }
    // }

    // for (auto&& [i, info] : iter::enumerate(symbol_table))
    // {
    //     info.size = symbol_table[i + 1].offset - info.offset;
    // // }

    // symbol_info main_symbol{};
    // for (auto i : symbol_table)
    // {
    //     if (i.name == "main")
    //     {
    //         main_symbol = i;
    //     }
    // }

    // auto pe_file_path = std::string{link_map_path};
    // pe_file_path.replace(pe_file_path.find(".map"), 4, ".exe");
    // auto const pe_file = fast_io::native_file_loader{pe_file_path};

    // PE pe;
    // pe.init(reinterpret_cast<std::byte*>(pe_file.data()));

    // auto section = IMAGE_FIRST_SECTION(pe.nt);
    // if (std::string_view{(char*)section->Name} != ".text")
    // {
    //     return -1;
    // }

    // {
    //     auto const start = pe_file.data() + section->PointerToRawData + main_symbol.offset;
    //     auto const code = std::string_view{start, main_symbol.size};

    //     INSTRUX ix;
    //     auto code_p = (const u8*)code.data();
    //     isize code_size = code.size();
    //     char text[ND_MIN_BUF_SIZE]{};
    //     while (code_size > 0)
    //     {
    //         NDSTATUS status = ::NdDecodeEx(&ix, code_p, code_size, ND_CODE_64, ND_DATA_64);
    //         if (!ND_SUCCESS(status))
    //         {
    //             fast_io::io::println("Decode failed with error ", status);
    //             return -1;
    //         }
    //         ::NdToText(&ix, 0, sizeof(text), text);
    //         fast_io::io::println(text);

    //         code_p += ix.Length;
    //         code_size -= ix.Length;
    //     }
    // }
}
