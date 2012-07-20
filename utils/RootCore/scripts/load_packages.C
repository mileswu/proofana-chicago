#include <fstream>
#include <TSystem.h>

void load_packages ()
{
  const char *root_core_dir = gSystem->Getenv ("ROOTCOREBIN");
  if (root_core_dir == 0)
    throw std::string ("ROOTCOREBIN not set, try sourcing RootCoreDir/setup.sh");
  const std::string dir (root_core_dir);

  const std::string include_dir = dir + "/include";
  if (gSystem->AccessPathName (include_dir.c_str()) == 1)
    throw std::string ("failed to find " + include_dir);
  gSystem->AddIncludePath (("-I\"" + include_dir + "\"").c_str());

  const std::string lib_dir = dir + "/lib";
  if (gSystem->AccessPathName (lib_dir.c_str()) == 1)
    throw std::string ("failed to find " + lib_dir);

  {
    std::ifstream preload_file ((dir + "/preload").c_str());
    std::string preload;
    while (std::getline (preload_file, preload))
    {
      if (!preload.empty() && preload[0] != '#')
      {
	if (gSystem->Load (preload.c_str()) < 0)
	  throw std::string ("failed to load library " + preload);
      };
    };
  };

  {
    std::ifstream pkg_file ((dir + "/load").c_str());
    std::string name;
    while (std::getline (pkg_file, name))
    {
      if (!name.empty())
      {
	const std::string glob_lib = lib_dir + "/lib" + name + ".so";
	if (gSystem->Load (glob_lib.c_str()) < 0)
	  throw std::string ("failed to load library " + glob_lib);
      };
    };
  };
}
