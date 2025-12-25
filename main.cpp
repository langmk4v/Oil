#include <filesystem>
#include <fstream>
#include <iostream>

#include <string>
#include <vector>

#include "json.hpp"

#define PROJECT_JSON "fire-project.json"

using json = nlohmann::json;

int command(char const* s) {
  std::cout<<s<<std::endl;
  return std::system(s);
}

int main(int argc, char **argv) {

  if (argc == 1) {
    std::cout << "usage:\n"
                 "  oil [option] ...\n"
                 "\n"
                 "options:\n"
                 "  run                Build and run binary.\n"
                 "  build              Build all source files.\n"
                 "  --new <name>       Create a new project.\n"
                 "\n";
    return 0;
  }

  if (argc == 2 && argv[1] == std::string("run")) {
    if(!std::filesystem::exists("fire-project.json")){
      std::cout<<
      "Error:\n"
      "  Here is not in project folder.\n"
      "\n";
      return -1;
    }

    json project = json::parse(std::ifstream(PROJECT_JSON));

    std::cout<<"Compiling project '"<<project["name"]<<"' ...\n";

    std::filesystem::create_directory("build");

    if(command("firec ./src/Main.fire -o ./build/Main.o") != 0 ) {
      goto __fail;
    }

    if(command(std::string("clang ./build/Main.o -o ./build/"+std::string(project["name"])).c_str()) != 0 ) {
      goto __fail;
    }

    return 0;

  __fail:
    std::cout << "\nfailed to compile, stopped.\n\n";
    return -1;
  }

  std::vector<std::string> args;
  while (argc > 0)
    args.push_back(*argv++), argc--;

  for (auto it = args.begin() + 1, end = args.end(); it < end;) {
    if (*it == "--new") {
      if (++it == end) {
        std::cout << "expected project name after '--new'.\n";
        return -1;
      }

      auto &name = *it;

      if (std::filesystem::exists(name)) {
        std::cout << "Error:\n"
                     "  Failed to create a new project folder.\n"
                     "  Already exists file or folder with same name.\n"
                     "\n";
        return -1;
      }

      std::filesystem::create_directory(name);
      std::filesystem::create_directory(name + "/src");

      std::ofstream ofs{name + "/src/Main.fire"};
      ofs << "import std::IO;\n";
      ofs << "\n";
      ofs << "fn main(args: string...) -> int {\n";
      ofs << "  println(\"Hello, World!\");\n";
      ofs << "}\n";
      ofs.flush();
      ofs.close();

      std::ofstream ofs2{name + "/" PROJECT_JSON};
      ofs2 << "{\n";
      ofs2 << "  \"name\": \"" << name << "\",\n";
      ofs2 << "  \"bin\": \"build/"<<name<<"\",\n";
      ofs2 << "  \"version\": \"1.0\",\n";
      ofs2 << "  \"requires\": [ ]\n";
      ofs2 << "}\n";
      ofs2.flush();
      ofs2.close();

      it++;
      continue;
    }
  }
}