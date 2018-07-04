#ifndef TJINFORMATION_H
#define TJINFORMATION_H 1

#include "TTree.h"
#include "TLorentzVector.h"

#include <vector>

struct TJCheatedObservables {

  float pair1_mass{};
  float pair2_mass{};

  void init(){
    pair1_mass = -10;
    pair2_mass = -10;
  }

};

class TJLevelInfo {

  public:

	float total_E;

	void init() {
		total_E = -10;
	}

};

class TJEventInfo {

  public:

	TJLevelInfo final_cn_system;
	TJLevelInfo initial_cn_system;
	TJLevelInfo reco_system;

  // This uses the jets associated to the initial colour neutrals
  // -> jet clustering and jet pairing are cheated
  TJCheatedObservables TJjets_from_initital_cn;

  // Use TJ jets but apply custom pairing
  // -> Cheat only clustering but not pairing
  TJCheatedObservables TJjets_custom_pairing;


	void init() {
		final_cn_system.init();
		initial_cn_system.init();
		reco_system.init();

    TJjets_from_initital_cn.init();
    TJjets_custom_pairing.init();
	}

};



#endif
