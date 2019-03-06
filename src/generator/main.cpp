/*
 * Polygon-4 Data Manager
 * Copyright (C) 2015-2016 lzwdgc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Polygon4/DataManager/Schema/Schema.h>
#include <Polygon4/DataManager/Schema/Parser.h>

#include <primitives/filesystem.h>
#include <primitives/sw/main.h>
#include <primitives/sw/settings.h>
#include <primitives/sw/cl.h>

#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    cl::opt<path> schema_fn(cl::Positional, cl::desc("<schema>"), cl::Required);
    cl::opt<path> out_dir(cl::Positional, cl::desc("<output dir>"), cl::Required);

    cl::ParseCommandLineOptions(argc, argv);

    Tokens ts;
    const auto schema = parse_file(schema_fn, &ts);

    const auto header = out_dir / "include" / "detail";
    const auto src = out_dir / "src" / "detail";

    auto printModule = [&header, &src](const std::string &name, auto module, const fs::path &subdir = "")
    {
        auto save = [](const auto &path, const std::string &s)
        {
            if (s.empty())
                return;
            error_code ec;
            create_directories(path.parent_path(), ec);
            std::ofstream(path.string()) << s;
        };

        module.hpp.emptyLines(1);
        module.cpp.emptyLines(1);

        save(header / subdir / (name + ".h"  ), module.hpp.getText());
        save(src    / subdir / (name + ".cpp"), module.cpp.getText());
    };

    printModule("ObjectInterfaces", schema.printObjectInterfaces());
    printModule("Storage", schema.printStorage());
    printModule("StorageImpl", schema.printStorageImplementation());
    printModule("Enums", schema.printEnums());
    printModule("Tokens", print(ts), "schema");
    printModule("ForwardDeclarations", schema.printForwardDeclarations());

    printModule("Types", schema.printTypes());
    for (auto &c : schema.getAllClasses())
        printModule(c.getName(), schema.printType(c), "types");

    return 0;
}
