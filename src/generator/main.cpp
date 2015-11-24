#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#include <Polygon4/DataManager/Schema/Schema.h>
#include <Polygon4/DataManager/Schema/Parser.h>

int main(int argc, char *argv[])
try
{
    if (argc < 3)
    {
        printf("Usage: %s schema.txt out_dir\n", argv[0]);
        return 1;
    }

    Tokens ts;
    auto schema = parse_file(argv[1], &ts);

    fs::path p = argv[2];
    auto header = p / "include" / "detail";
    auto src = p / "src" / "detail";

    auto printModule = [&header, &src](const std::string &name, auto &module, const fs::path &subdir = fs::path())
    {
        auto save = [](const auto &path, const std::string &s)
        {
            if (s.empty())
                return;
            boost::system::error_code ec;
            fs::create_directories(path.parent_path(), ec);
            std::ofstream(path.string()) << s;
        };

        module.hpp.emptyLines(1);
        module.cpp.emptyLines(1);

        save(header / subdir / (name + ".h"  ), module.hpp.getText());
        save(src    / subdir / (name + ".cpp"), module.cpp.getText());
    };

    printModule("ObjectInterfaces", schema.printObjectInterfaces());
    printModule("Types", schema.printTypes());
    printModule("Storage", schema.printStorage());
    printModule("StorageImpl", schema.printStorageImplementation());
    printModule("Enums", schema.printEnums());
    printModule("Tokens", print(ts), "schema");

    return 0;
}
catch (std::exception &e)
{
    std::cerr << e.what() << "\n";
    return 1;
}
catch (...)
{
    std::cerr << "unknown exception" << "\n";
    return 1;
}
