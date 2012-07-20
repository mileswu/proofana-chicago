/**************************************************************************
 **
 **   File:         Key.cxx
 **
 **   Description:  String-based key class to avoid string copies and comparisons
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      12-29-11
 **   Modified:
 **
 **************************************************************************/

#ifndef Key_cxx
#define Key_cxx

#include "Key.h"
#include "TBuffer.h"
#include <iostream>
#include <cstdlib>

namespace std
{

	template<>
	void swap(Key<0>& lhs, Key<0>& rhs) { lhs.swap(rhs); }

	template<>
	void swap(Key<1>& lhs, Key<1>& rhs) { lhs.swap(rhs); }
	
	template<>
	void swap(Key<2>& lhs, Key<2>& rhs) { lhs.swap(rhs); }

	template<>	
	void swap(InternalKey& lhs, InternalKey& rhs) { lhs.swap(rhs); }
}


InternalKey::InternalKey(const InternalKey& key) { 

	//Makes persistent
	//m_vec inside Key classes should hold the only persistent copies of this class
	
	Init(key.m_str,key.m_len);
}

void InternalKey::swap(InternalKey& key) {
	const char* str = m_str;
	unsigned int len = m_len;
	bool init = m_init;

	m_str = key.m_str;
	m_len = key.m_len;
	m_init = key.m_init;

	key.m_str = str;
	key.m_len = len;
	key.m_init = init;
}

template<unsigned int N>
void Key<N>::swap(Key& key) {

	InternalKey* backupkey = m_key;
	m_key = key.m_key;
	key.m_key = backupkey;
}

template<unsigned int N>
void Key<N>::Streamer(TBuffer &b) {
   
	Int_t   nbig;
	UChar_t nwh;
	if (b.IsReading()) {
		b >> nwh;
      	if (nwh == 0) {
      		InternalKey key("",0);
      		m_key = Init(key);
      	}
      	else {
         	if (nwh == 255) b >> nbig;
         	else nbig = nwh;

			char data[nbig+1];
   			data[nbig] = 0;  // terminating null
					
         	b.ReadFastArray(data,nbig);
			InternalKey key(data,nbig);
			m_key = Init(key);
      	}
   	} 
   	else {
		nbig = m_key->Length();
     	if (nbig > 254) {
       		nwh = 255;
    		b << nwh;
        	b << nbig;
      	} else {
    		nwh = UChar_t(nbig);
    		b << nwh;
    	}
      	b.WriteFastArray(m_key->Data(),nbig);
   	}
} 

//static member initialization
template<unsigned int N> vector<InternalKey*> Key<N>::m_vec;
template<unsigned int N> map<InternalKey*,map<InternalKey*, InternalKey*> > Key<N>::m_append;

//explicit instantiations must be at the end
template class Key<0>;
template class Key<1>;
template class Key<2>;

template class KeyObj<0>;
template class KeyObj<1>;
template class KeyObj<2>;

#endif
