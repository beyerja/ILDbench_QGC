#ifndef EVENTSKIPPER_H
#define EVENTSKIPPER_H 1

#include "TBranch.h"
#include "TTree.h"

class EventSkipper {
  /** Class to that allows skipping specific events in a tree.
   **/
   
  float m_VV_mass {};
  
  bool m_low_mVV_only {false};
public:
  // Constructor
  EventSkipper(){}
  
  // Instructions on which events to use
  void UseLowMVVOnly() { m_low_mVV_only = true; }
  
  // Check if an event needs to be skipped
  bool should_skip_event(	TTree* tree, int event_number ) {
    bool skip = false;
    if (m_low_mVV_only) {
      tree->GetBranch("true_observables")->GetLeaf("VV_mass")->SetAddress(&m_VV_mass);
      tree->GetEntry(event_number);
      if (m_VV_mass > 500) {skip = true;}
    }
    return skip;
  }
};

#endif