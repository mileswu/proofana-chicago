Int_t SETUP()
{
  gSystem->Setenv ("ROOTCOREDIR", "%ROOTCOREDIR%");
  gSystem->Setenv ("ROOTCOREBIN", "%ROOTCOREBIN%");

  try
  {
    gROOT->Macro ("$ROOTCOREDIR/scripts/load_packages.C+");
  } catch (std::string& e)
  {
    std::cout << "error: " << e << std::endl;
    return -1;
  };
  return 0;
}
