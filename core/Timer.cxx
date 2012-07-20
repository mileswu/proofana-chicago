/**************************************************************************
 **
 **   File:         Timer.cxx
 **
 **   Description:  Class for timing subroutines
 **
 **   Author:       B. Butler
 **   Created:      3-4-11
 **
 **************************************************************************/

#define Timer_cxx

#include "Timer.h"
#include <TH1F.h>

bool Timer::Start(const AnaKey& name) {
	if(timers.rbegin()!=timers.rend() && (timers.rbegin()->first()==name())) {
		cout << "Timer: ERROR a timer named \"" << name.Data() << "\" is already running!" << endl;
		return false;
	}
	else {
		pair<AnaKey, int> timer(name,GetTime());
		timers.push_back(timer);
		return true;
	}
}

bool Timer::Stop(const AnaKey& name) {
	if(timers.rbegin()!=timers.rend() && (timers.rbegin()->first()!=name())) {
		cout << "Timer: ERROR most recent timer is named \"" << timers.rbegin()->first.Data() << "\", not \"" << name.Data() << "\"!" << endl;
		return false;
	}
	unsigned int duration  = GetTime()-timers.rbegin()->second;
	timers.pop_back();
	stimers[name] += duration; //timers with the same name get summed
	
	if(timerDir) FillHistogram(name.Data(),duration);
	return true;
}

bool Timer::Running(const AnaKey& name) { //inefficient, but typically only used for debugging purposes
	vector<pair<AnaKey, unsigned int> >::reverse_iterator iter = timers.rbegin();
	for(; iter!=timers.rend(); ++iter) {
		if(iter->first()==name()) return true;
 	}
 	return false;
}

void Timer::FillHistogram(TString name, unsigned int duration) {

	if(!timerDir) {
		cout << "Timer: ERROR no TDirectory for storing timer histograms, fix your code!" << endl;
		return;	
	}
	
	name.ReplaceAll(":","_");
	
	TH1F* histo = (TH1F*)timerDir->Get(name);
	if(histo) {
		histo->Fill(duration);
		return;
	}

	// Save current directory
	TDirectory* savdir = gDirectory;

	float dur = (float)duration;

	timerDir->cd();	
	histo = new TH1F(name,name,100,0.,2000.);
	histo->GetXaxis()->SetTitle("Duration (ms)");
	histo->Fill(dur);

	//Restore previous directory
	savdir->cd();
}

void Timer::ShowTimers(map<TString, unsigned int>& t, bool isRunning) {
	
	cout << "*******************************************************************" << endl;
	cout << "* Timer Name                                       | Duration     *" << endl;	
	cout << "*-----------------------------------------------------------------*" << endl;
	map<TString, unsigned int>::iterator p = t.begin();
	for(; p!=t.end(); ++p) {
		TString first(p->first);

		float seconds;		
		if(isRunning) seconds = (GetTime()-p->second)/1000.;
		else seconds = p->second/1000.;
		int minutes = 0;
		int hours = 0;
		if(seconds>=60.) {
			minutes = seconds/60.;
			seconds = seconds-60.*minutes;
		}
		if(minutes>=60) {
			hours = minutes/60;
			minutes = minutes%60;
		}

		TString second;
		if(hours) second = TString::Format("%ih %im %.0fs",hours,minutes,seconds);
		else if (minutes) second = TString::Format("%im %.0fs",minutes,seconds);
		else if(seconds>10.) second = TString::Format("%.0fs",seconds);
		else second = TString::Format("%.1fs",seconds);
		
		while(first.Length()<48) {first.Append(" ");}
		while(second.Length()<12) {second.Append(" ");}	
		cout << "* " << first << " | " << second << " *"<< endl;
	}
	cout << "*******************************************************************" << endl;
}
