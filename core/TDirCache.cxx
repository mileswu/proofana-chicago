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

#define TDirCache_cxx

#include "TDirCache.h"

TObject* TDirCache::Get(AnaKey namecycle)
{
	map<AnaKey,TObject*>::iterator iter = fObjMap.lower_bound(namecycle);
	if(iter!=fObjMap.end() && (namecycle()==iter->first())) return iter->second;
	else {
		TObject* obj = fDir->Get(namecycle.Data());
		if(obj) {
			fObjMap.insert(iter, map<AnaKey,TObject*>::value_type(namecycle,obj));
		}
		return obj;
	}
}
