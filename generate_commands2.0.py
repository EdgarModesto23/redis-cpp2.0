import os
import sys
import json
from pathlib import Path


CPP_HEADER_TEMPLATE = """#pragma once
#include <memory>
#include <optional>
#include <string>
#include "database.hpp"
#include "command.hpp"

class {class_name} : public ICommand {{
public:
  explicit {class_name}(std::shared_ptr<Database> db, std::string &buf{ctor_args}) noexcept;

  ~{class_name}() override = default;

  {class_name}(const {class_name} &) = delete;
  {class_name} &operator=(const {class_name} &) = delete;
  {class_name}({class_name} &&) noexcept = default;
  {class_name} &operator=({class_name} &&) noexcept = delete;

  void serveRequest() override;

private:
  std::shared_ptr<Database> db_;
  std::string &buff_;
{member_vars}
}};
"""


CPP_SOURCE_TEMPLATE = """#include "{header_name}"
#include <string>

{class_name}::{class_name}(std::shared_ptr<Database> db, std::string &buf{ctor_args}) noexcept
    : db_(db), buff_(buf){ctor_inits}
{{
}}

void {class_name}::serveRequest() {{
  // TODO: Implement command logic for {class_name}
}}
"""


def cpp_type(arg_type):
    """Map Redis JSON argument types to C++."""
    mapping = {
        "string": "std::string",
        "integer": "long long",
        "double": "double",
        "key": "std::string",
        "pattern": "std::string",
        "unix-time": "long long",
        "pure-token": "std::string",
    }
    return mapping.get(arg_type, "std::string")


def generate_cpp_class(json_path, out_dir):
    with open(json_path, "r") as f:
        data = json.load(f)

    # Each JSON file has one top-level key: the command name
    command_name = list(data.keys())[0]
    spec = data[command_name]
    class_name = command_name.capitalize() + "Command"

    arguments = spec.get("arguments", [])

    ctor_args = ""
    ctor_inits = ""
    member_vars = ""

    for arg in arguments:
        name = arg["name"]
        arg_type = arg["type"]
        is_optional = arg.get("optional", False)

        cpp_t = cpp_type(arg_type)
        if is_optional:
            member_decl = f"  std::optional<{cpp_t}> {name}_;\n"
            ctor_arg = f", std::optional<{cpp_t}> {name}"
            ctor_init = f", {name}_({name})"
        else:
            member_decl = f"  {cpp_t} {name}_;\n"
            ctor_arg = f", {cpp_t} {name}"
            ctor_init = f", {name}_({name})"

        member_vars += member_decl
        ctor_args += ctor_arg
        ctor_inits += ctor_init

    header_name = f"{class_name}.hpp"

    header_code = CPP_HEADER_TEMPLATE.format(
        class_name=class_name,
        ctor_args=ctor_args,
        member_vars=member_vars,
    )

    source_code = CPP_SOURCE_TEMPLATE.format(
        class_name=class_name,
        ctor_args=ctor_args,
        ctor_inits=ctor_inits,
        header_name=header_name,
    )

    # Write files
    header_path = out_dir / f"{class_name}.hpp"
    source_path = out_dir / f"{class_name}.cc"

    with open(header_path, "w") as f:
        f.write(header_code)

    with open(source_path, "w") as f:
        f.write(source_code)

    print(f"Generated {class_name}.hpp and {class_name}.cc")


def main():
    if len(sys.argv) != 3:
        print("Usage: generate_commands.py <json_dir> <output_dir>")
        sys.exit(1)


    json_dir = Path(sys.argv[1])
    out_dir = Path(sys.argv[2])

    if not json_dir.exists():
        print("Input JSON directory does not exist.")
        sys.exit(1)

    out_dir.mkdir(parents=True, exist_ok=True)

    for file in json_dir.glob("*.json"):
        out_file = out_dir / (file.stem + ".hpp") 
        if out_file.exists():
            print(f"Skipping {file.name}: class file already exists.")
            continue
        generate_cpp_class(file, out_dir)



if __name__ == "__main__":
    main()
