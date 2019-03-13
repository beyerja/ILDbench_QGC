#include "JakobsVBSProcessor.h"

void JakobsVBSProcessor::TJGetInitialCNRecoMasses(EventInfo &info) {

  std::vector<float> initial_cn_reco_masses;

  // Loop over all initial colour neutrals
  // If they come from quarks (only two should!!) write its mass to vector
  // afterwards write to output info.
  streamlog_out(DEBUG4) << "In TJGetInitialCNRecoMasses: Got " << nicn() << " total icns." << std::endl;
  for (int i=0; i<nicn(); i++) {
  	// Test if icn shows jet to be quark-like
  	bool quark_like = false;

    int initial_cn_parent_pdg = pdg_icn_parent(i);
    streamlog_out(DEBUG3) << "In TJGetInitialCNRecoMasses: Got icn with parent PDG " << initial_cn_parent_pdg << std::endl;
    if ( abs(initial_cn_parent_pdg) > 0 && abs(initial_cn_parent_pdg) <= 8 ) { 
      quark_like = true;
  	} else if ( abs(initial_cn_parent_pdg) > 22 ) {
  		// TJ could have identified it as coming from a Vector Boson (e.g. Z)
	    const IntVec& _pdg_icn_comps = pdg_icn_comps(i);
      streamlog_out(DEBUG2) << "  Got icn boson parent with components:" << std::endl << "    ";
  		for ( auto & icn_comp_pdg : _pdg_icn_comps ) {
        streamlog_out(DEBUG2) << icn_comp_pdg << " ";
  			if ( abs(icn_comp_pdg) == 0 || abs(icn_comp_pdg) > 8 ) {
  				quark_like = false;
          streamlog_out(DEBUG2) << " -> Not quark like!" << std::endl;
  				break;
  			}
  			quark_like = true;
  		}
      streamlog_out(DEBUG2) << std::endl;
  	}

  	if ( ! quark_like ) { continue; }

    const IntVec& jets_to_icn = jets_of_initial_cn(i);
    TLorentzVector icn_reco_tlv = TLorentzVector( 0, 0, 0, 0 );
    streamlog_out(DEBUG2) << "  Jets in icn: " << std::endl;
    for (int j=0; j<jets_to_icn.size(); j++) {
      int jet_i = jets_to_icn[j];
      
      // --- Check that no wrong jet is in used
      streamlog_out(DEBUG2) << "    Jet " << jet_i << ", Type: " << type_jet(jet_i) << std::endl;
      streamlog_out(DEBUG2) << "           Icn type: " << type_icn_parent(initial_cn(jet_i)) << std::endl;
      streamlog_out(DEBUG2) << "           Fcn type: " << type_fcn_parent(final_cn(jet_i))   << std::endl << std::endl;
      if ( type_fcn_parent(final_cn(jet_i)) > 3 || type_fcn_parent(final_cn(jet_i)) == 0 ) {
          streamlog_out(DEBUG2)  << " -> is non-hadronic jet!";
          continue; // For consistency, but likely unnecessary
      }
      // --- 
      
      TLorentzVector seen_jet_tlv = TLorentzVector( p4seen(jet_i)[1], p4seen(jet_i)[2], p4seen(jet_i)[3], p4seen(jet_i)[0]);
      icn_reco_tlv += seen_jet_tlv;
    }
    streamlog_out(DEBUG2) << std::endl;
    initial_cn_reco_masses.push_back( icn_reco_tlv.M() );
  }
  
  streamlog_out(DEBUG4) << "In TJGetInitialCNRecoMasses: Got " << initial_cn_reco_masses.size() << " icns with quark-like parents." << std::endl << std::endl;

  if (initial_cn_reco_masses.size() != 2) {
    streamlog_out(ERROR) << "ERROR when checking recos to initial cns: NOT 2 QUARK-LIKE ICNs!" << std::endl;
    return;
  }

  info.TJ_eventinfo.TJjets_from_initital_cn.pair1_mass = initial_cn_reco_masses[0];
  info.TJ_eventinfo.TJjets_from_initital_cn.pair2_mass = initial_cn_reco_masses[1];

}



void JakobsVBSProcessor::TJGetCustomPairingRecoMasses(EventInfo &info) {
 // TODO Must also take TrueJet_Parser class (tj)

// TODO Problem:  There can be easily more than 4 jets -> Figure out how to get one jet per quarks (approximation necessary)
  // Collect all jets belonging to each initial quark
  // Do this by looping over the jets (and not the initial quarks) to avoid double counting the jets 
  std::map<MCParticle*, std::vector<int>> TJindices_to_initial_quarks {}; 
  streamlog_out(DEBUG3) << "In TJGetCustomPairingRecoMasses: Got " << tj->njets() << " jets of types: ";
  for (int ijet=0 ; ijet<tj->njets(); ijet++ ) {
    if (  seen_partics(ijet).size() > 0 ) {
      // Jet types: 1 string, 2 lepton, 3 cluster, 4 ISR, 5 overlay.
      int jet_type =  type_jet(ijet);
      streamlog_out(DEBUG3) << jet_type;
      if ( ( jet_type % 100 == 1 ) || ( jet_type % 100 == 3 ) ) { // According to TrueJet marking of hadronic jets
        (TJindices_to_initial_quarks[initial_elementon(ijet)]).push_back(ijet);
        streamlog_out(DEBUG3) << "(hadronic)";
      }
      streamlog_out(DEBUG3) << " ";
    } else {
      streamlog_out(DEBUG3) << type_jet(ijet) << "(No PFOs) ";
    }
  }
  streamlog_out(DEBUG3) << std::endl;

  streamlog_out(DEBUG4) << "In TJGetCustomPairingRecoMasses: Found " << TJindices_to_initial_quarks.size() << " initial quark jets." << std::endl;
  if ( TJindices_to_initial_quarks.size() > 4 ) {
    // Less than 4 is okay if one did not produce particles detected by detector -> Just use as 0 four-momentum
    streamlog_out(WARNING) << "In TJGetCustomPairingRecoMasses: More than 4 initial quark vectors!" << std::endl;
    info.TJ_eventinfo.TJjets_custom_pairing.pair1_mass = 0;
    info.TJ_eventinfo.TJjets_custom_pairing.pair2_mass = 0;
    return;
  }

  // For each initial quark find the complete reconstructed four momentum (combined from all jets to the quark)
  // and create ReconstructedParticle from it to run through VBPairFinder 
  std::vector<ReconstructedParticle*> TJrecojets_to_initial_quarks {};
  for(auto const & indices_to_initial_quark : TJindices_to_initial_quarks) { 
    ReconstructedParticleImpl* jet_to_initial_quark = new ReconstructedParticleImpl;
    TLorentzVector tlv_to_initial_quark {};
    for (auto & ijet: indices_to_initial_quark.second) {
      tlv_to_initial_quark += TLorentzVector( pseen(ijet), Eseen(ijet) );
    } 
    double jet_momentum[3] = {tlv_to_initial_quark.Px(), tlv_to_initial_quark.Py(), tlv_to_initial_quark.Pz()};
    jet_to_initial_quark->setMomentum(jet_momentum);
    jet_to_initial_quark->setEnergy  (tlv_to_initial_quark.E());
    jet_to_initial_quark->setMass    (tlv_to_initial_quark.M());
    TJrecojets_to_initial_quarks.push_back(dynamic_cast<ReconstructedParticle*> (jet_to_initial_quark));
  }
  
  if ( TJrecojets_to_initial_quarks.size() < 4 ) {
    streamlog_out(DEBUG4) << "In TJGetCustomPairingRecoMasses: Adding " << 4-TJrecojets_to_initial_quarks.size() << " 0-energy jets to account for unobserved quarks." << std::endl;
    streamlog_out(DEBUG3) << "In TJGetCustomPairingRecoMasses: Adding empty jet: ";
    for ( int i_new=0; i_new<5-TJrecojets_to_initial_quarks.size(); i_new++ ){
      ReconstructedParticleImpl* empty_jet = new ReconstructedParticleImpl;
      double zero_momentum[3] = {0, 0, 0};
      empty_jet->setMomentum(zero_momentum); empty_jet->setEnergy(0); empty_jet->setMass(0);
      TJrecojets_to_initial_quarks.push_back(dynamic_cast<ReconstructedParticle*> (empty_jet));
      streamlog_out(DEBUG3) << i_new << " ";
    }
    streamlog_out(DEBUG3) << std::endl;
  }
  
  Observables initial_quark_jets_observables {};
  findVpair(TJrecojets_to_initial_quarks, initial_quark_jets_observables);
  info.TJ_eventinfo.TJjets_custom_pairing.pair1_mass = initial_quark_jets_observables.pair1_mass;
  info.TJ_eventinfo.TJjets_custom_pairing.pair2_mass = initial_quark_jets_observables.pair2_mass;

  for ( auto & new_jet_particle_pointer: TJrecojets_to_initial_quarks ) { delete new_jet_particle_pointer; } // Clean up new pointers to avoid memory leak
}
