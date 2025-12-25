#include <iostream>
#include <fstream>
#include <filesystem>

#include <string>
#include <vector>

int main(int argc, char** argv) {

  if(argc==1){
    std::cout<<
    "usage:\n"
    "  oil [option] ...\n"
    "\n"
    "options:\n"
    "  --new <name>       Create a new project.\n"
    "\n"
    ;
    return 0;
  }

  std::vector<std::string> args;
  while(argc>0)
    args.push_back(*argv++), argc--;

  for(auto it=args.begin()+1,end=args.end();it<end;){
    if(*it=="--new"){
      if(++it==end) {std::cout<<"expected project name after '--new'.\n";return -1;}

      auto& name = *it;

      if(std::filesystem::exists(name)){
        std::cout<<
        "Error:\n"
        "  Failed to create a new project folder.\n"
        "  Already exists file or folder with same name.\n"
        "\n";
        return -1;
      }

      std::filesystem::create_directory(name);
      std::filesystem::create_directory(name+"/src");

      std::ofstream ofs {name+"/src/Main.fire"};
      ofs<<"import std::IO;\n";
      ofs<<"\n";
      ofs<<"fn main(args: string...) -> int {\n";
      ofs<<"  println(\"Hello, World!\");\n";
      ofs<<"}\n";
      ofs.flush();
      ofs.close();

      std::ofstream ofs2 { name+"/package.json"};
      ofs2<<"{\n";
      ofs2<<"  \"name\": \""<<name<<"\",\n";
      ofs2<<"  \"version\": \"1.0\"\n";
      ofs2<<"  \"requires\": [ ]\n";
      ofs2<<"}\n";
      ofs2.flush();
      ofs2.close();

      it++;
    }
  }

}