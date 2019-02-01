#include "JakobsVBSProcessor.h"

void JakobsVBSProcessor::TJGetInitialCNRecoMasses(EventInfo &info) {

  std::vector<float> initial_cn_reco_masses;

  // Loop over all initial colour neutrals
  // If they come from quarks (only two should!!) write its mass to vector
  // afterwards write to output info.
  streamlog_out(DEBUG) << "In TJGetInitialCNRecoMasses: Got " << nicn() << " total icns." << std::endl;
  for (int i=0; i<nicn(); i++) {
  	// Test if icn shows jet to be quark-like
  	bool quark_like = false;

    int initial_cn_parent_pdg = pdg_icn_parent(i);
    streamlog_out(DEBUG) << "In TJGetInitialCNRecoMasses: Got icn with parent PDG " << initial_cn_parent_pdg << std::endl;
    if ( abs(initial_cn_parent_pdg) > 0 && abs(initial_cn_parent_pdg) <= 8 ) { 
      quark_like = true;
  	} else if ( abs(initial_cn_parent_pdg) > 22 ) {
  		// TJ could have identified it as coming from a Vector Boson (e.g. Z)
	    const IntVec& _pdg_icn_comps = pdg_icn_comps(i);
      streamlog_out(DEBUG) << "  Got icn boson parent with components:" << std::endl << "    ";
  		for ( auto & icn_comp_pdg : _pdg_icn_comps ) {
        streamlog_out(DEBUG) << icn_comp_pdg << " ";
  			if ( abs(icn_comp_pdg) == 0 || abs(icn_comp_pdg) > 8 ) {
  				quark_like = false;
          streamlog_out(DEBUG) << " -> Not quark like!" << std::endl;
  				break;
  			}
  			quark_like = true;
  		}
      streamlog_out(DEBUG) << std::endl;
  	}

  	if ( ! quark_like ) { continue; }

    const IntVec& jets_to_icn = jets_of_initial_cn(i);
    TLorentzVector icn_reco_tlv = TLorentzVector( 0, 0, 0, 0 );
    streamlog_out(DEBUG) << "  Jets in icn: " << std::endl;
    for (int j=0; j<jets_to_icn.size(); j++) {
      int jet_i = jets_to_icn[j];
      
      // --- Check that no wrong jet is in used
      streamlog_out(DEBUG) << "    Jet " << jet_i << ", Type: " << type_jet(jet_i) << std::endl;
      streamlog_out(DEBUG) << "           Icn type: " << type_icn_parent(initial_cn(jet_i)) << std::endl;
      streamlog_out(DEBUG) << "           Fcn type: " << type_fcn_parent(final_cn(jet_i))   << std::endl << std::endl;
      if ( type_fcn_parent(final_cn(jet_i)) > 3 || type_fcn_parent(final_cn(jet_i)) == 0 ) {
          streamlog_out(DEBUG)  << " -> is non-hadronic jet!";
          continue; // For consistency, but likely unnecessary
      }
      // --- 
      
      TLorentzVector seen_jet_tlv = TLorentzVector( p4seen(jet_i)[1], p4seen(jet_i)[2], p4seen(jet_i)[3], p4seen(jet_i)[0]);
      icn_reco_tlv += seen_jet_tlv;
    }
    streamlog_out(DEBUG) << std::endl;
    initial_cn_reco_masses.push_back( icn_reco_tlv.M() );
  }
  
  streamlog_out(DEBUG) << "In TJGetInitialCNRecoMasses: Got " << initial_cn_reco_masses.size() << " icns with quark-like parents." << std::endl << std::endl;

  if (initial_cn_reco_masses.size() != 2) {
    streamlog_out(ERROR) << "ERROR when checking recos to initial cns: NOT 2 QUARK-LIKE ICNs!" << std::endl;
    return;
  }

  info.TJ_eventinfo.TJjets_from_initital_cn.pair1_mass = initial_cn_reco_masses[0];
  info.TJ_eventinfo.TJjets_from_initital_cn.pair2_mass = initial_cn_reco_masses[1];

}
