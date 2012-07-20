/**************************************************************************
 **
 **   File:         TDirCache.h
 **
 **   Description:  TDirectory key-caching system to avoid string lookups
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      6-3-12
 **   Modified:
 **
 **************************************************************************/

#ifndef TDirCache_h
#define TDirCache_h

#include "Key.h"
#include "TFile.h"

class TDirCache {
public :

	TDirCache(TDirectory* dir) : fDir(dir) { }
	virtual 			~TDirCache() { }
    TObject*			Get(AnaKey namecycle);
    TFile*				GetFile() const { return fDir->GetFile(); }
    TList*				GetList() const { return fDir->GetList(); }
    Bool_t				cd(const char* path = 0) const { return fDir->cd(path); }
	TDirectory*			Dir() const { return fDir; }
	
	ClassDef(TDirCache,0);
	
private :

	TDirectory* fDir;
	map<AnaKey,TObject*> fObjMap;

};

#endif
