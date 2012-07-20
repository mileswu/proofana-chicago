/**************************************************************************
 **
 **   File:         AnaConfig.h
 **
 **   Description:  Generic persistable configuration class
 **
 **   Author:       B. Butler
 **   Created:      2-24-11
 **
 **************************************************************************/

#ifndef AnaConfig_h
#define AnaConfig_h

#include "TString.h"
#include "MomentObj.h"
#include "TNamed.h"

class Config : public MomentObj {
	public:
		Config() { }	
		Config(const TString& name) {SetName(name);}	
		Config(const TString& name, const TString& cfgfile) {SetName(name); ReadFile(cfgfile);}  
		bool ReadFile(const TString& cfgfile);

		ClassDef(Config,1);
	
	private:

};

#endif  /* Config_h */
