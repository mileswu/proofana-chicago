/**************************************************************************
 **
 **   File:         Aux.h
 **
 **   Description:  Auxiliary functions that are problematic for CINT
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      5-23-12
 **   Modified:
 **
 **************************************************************************/

#ifndef Aux_cxx
#define Aux_cxx

#include "Aux.h"
#include "Error.h"
#include "TRegexp.h"
#include <map>
#include <set>
#include <iostream>

vector<TString> GetRegexp(MomentObj* obj, const char* regexp) {

	TRegexp re(regexp);

	set<TString> setds;
	vector<TString> ds;

	map<MomKey,MapObj*>::const_iterator iter = obj->Keys().begin();
	for(; iter!=obj->Keys().end();++iter) {
		TString key(iter->first.Data());
		if(key.Contains(re)) {
			if(iter->second->Type()!=STRING) {
				TString msg("Value for key ");
				msg.Append(iter->first);
				msg.Append(" not of type STRING, exiting.");
				Abort(msg);
			}
			setds.insert(iter->second->String());
		}
	}
	
	set<TString>::iterator setiter = setds.begin();
	for(; setiter!=setds.end(); ++setiter) ds.push_back(*setiter);
	return ds;
}

vector<TString> GetMoreOutputs(MomentObj* obj) {
	return GetRegexp(obj,"^output_.+$");
}

vector<TString> GetDatasets(MomentObj* obj) {
	return GetRegexp(obj,"^name_.+$");
}

#endif
