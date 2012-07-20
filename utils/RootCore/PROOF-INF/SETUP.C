Int_t SETUP()
{
  std::string dir = gSystem->WorkingDirectory();
  dir += "/RootCore";
  gSystem->Setenv ("ROOTCOREDIR", dir.c_str());
  gSystem->Setenv ("ROOTCOREBIN", dir.c_str());

  try
  {
    gROOT->Macro ("RootCore/scripts/load_packages.C+");
  } catch (std::string& e)
  {
    std::cout << "error: " << e << std::endl;
    return -1;
  };
  return 0;
}
