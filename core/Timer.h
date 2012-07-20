/**************************************************************************
 **
 **   File:         Timer.h
 **
 **   Description:  Class for timing subroutines
 **
 **   Author:       B. Butler
 **   Created:      3-4-11
 **
 **************************************************************************/

#ifndef Timer_h
#define Timer_h

#include "TString.h"
#include "TROOT.h"
#include "TSystem.h"
#include <iostream>
#include <map>
#include "Key.h"

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::pair;

class Timer {
	public:
		Timer() {timerDir = 0; SetReference(); }
		virtual ~Timer() {if(timers.size()>0) cout << "Timers still running on destructor call!" << endl;}
		void Reset() {timers.clear();}

		void ShowRunning() { 
			//Reorder map with a scheme more intelligible to humans (but less efficient for searches)
			vector<pair<AnaKey, unsigned int> >::iterator q = timers.begin();
			map<TString, unsigned int> reordered_map;
			for(; q!=timers.end(); ++q) reordered_map[q->first.Data()] = q->second;
			ShowTimers(reordered_map,true); 
		}
		void ShowStopped() { 
			//Reorder map with a scheme more intelligible to humans (but less efficient for searches)
			map<AnaKey, unsigned int>::iterator q = stimers.begin();
			map<TString, unsigned int> reordered_map;
			for(; q!=stimers.end(); ++q) reordered_map[q->first.Data()] = q->second;
			ShowTimers(reordered_map,false); 
		}

		bool Start(const AnaKey& name);
		bool Stop(const AnaKey& name);
		bool Running(const AnaKey& name);

		void SetHistDir(TDirectory* dir) {timerDir = dir;}

		ClassDef(Timer,0);
	
	private:
		void SetReference() {ref = gSystem->Now();}
		unsigned int GetTime() { Long64_t now = gSystem->Now(); return (unsigned int)(now-ref);}
		void FillHistogram(TString name, unsigned int duration);
		void ShowTimers(map<TString, unsigned int>& t, bool isRunning);
	
		vector<pair<AnaKey, unsigned int> > timers; //running timers
		map<AnaKey, unsigned int> stimers; //stopped timers
		TDirectory* timerDir;
		Long64_t ref;
};


#endif  /* Timer_h */
