#ifndef VBPAIRFINDING_IMPL_h
#define VBPAIRFINDING_IMPL_h 1

#include "VBPairFindingTemplates.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// General functions

//-------------------------------------------------------------------------------------------------
template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::setMinimizationCriterium( std::string criterium ) {
	m_minimization_criterium = criterium;
}

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::setParticleVector( const ParticleVec &particle_vector ) {
	m_particle_vector = particle_vector;
}

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::setVBPairCandidateVector( const VBPairVec &VBpair_candidates ){
	m_VBpair_candidates = VBpair_candidates;
}

//-------------------------------------------------------------------------------------------------

template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson1Particle1() const { return m_boson1_p1; }
template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson1Particle2() const { return m_boson1_p2; }
template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson2Particle1() const { return m_boson2_p1; }
template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson2Particle2() const { return m_boson2_p2; }

template<class ParticleClass> int VectorBosonPairFinder<ParticleClass>::getVBPairType() const { return m_VBpair_type; }

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Functions for best pair finding

//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::getAllVBPairCandidates() {
	/**	Take a collection of particles and find all possible
	 		VB pair candidates (consisting of 2x2 particles)
	*/

	int n_particles = m_particle_vector.size();

	// This loop combination should give you all possible 
	// (particle particle) (particle particle) pair combinations exactly once.
	// ( If you don't believe me test it on paper :P )
	for(int i=0;i<n_particles-3;i++){
		for(int j=i+1;j<n_particles;j++){ 
			for(int k=i+1;k<n_particles-1;k++){
				for(int l=k+1;l<n_particles;l++){
					// Skip combinations that take same quark twice.
					// Duplicate within pair already excluded by starting second loop with i+1 and fourth with k+1.
					if (j==k || j==l) { continue; }

					VB pair1 ( m_particle_vector[i], m_particle_vector[j] );
					VB pair2 ( m_particle_vector[k], m_particle_vector[l] );
					VBPair VBpair_candidate ( pair1, pair2 );

					m_VBpair_candidates.push_back( VBpair_candidate );
				}
			}
		}
	}

}

//-------------------------------------------------------------------------------------------------
template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::getMassesToVBPairCandidates() {
	/** Fill vector with mass pairs to VB pair candidates
			-> Indices of both vectors should be parallel  (alternative for later: Separate class)
	*/

	// for every VB pair candidate
	for (unsigned int i_pair=0; i_pair<m_VBpair_candidates.size(); i_pair++) {
		VBPair VBpair_candidate = m_VBpair_candidates[i_pair];
		VB VB1_candidate = VBpair_candidate.first;
		VB VB2_candidate = VBpair_candidate.second;

		// Get 4 momenta of all particles in the VB pair candidate
		TLorentzVector VB1_p1_tlv ( VB1_candidate.first->getMomentum(),		VB1_candidate.first->getEnergy() );
		TLorentzVector VB1_p2_tlv ( VB1_candidate.second->getMomentum(), 	VB1_candidate.second->getEnergy() );
		TLorentzVector VB2_p1_tlv ( VB2_candidate.first->getMomentum(), 	VB2_candidate.first->getEnergy() );
		TLorentzVector VB2_p2_tlv ( VB2_candidate.second->getMomentum(), 	VB2_candidate.second->getEnergy() );

		// Get masses of the VB candidates
		float VB1_mass = ( VB1_p1_tlv + VB1_p2_tlv ).M();
		float VB2_mass = ( VB2_p1_tlv + VB2_p2_tlv ).M();

		MassPair VBpair_candidate_masses ( VB1_mass, VB2_mass );

		// Push into member vector for use later
		m_VBpair_candidate_masses.push_back( VBpair_candidate_masses );
  }
}

//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::minimizeMassDifference() {
	/** Find index of best candidate according to minimal difference between
			masses of the two vector bosons.
	*/
	FloatVec mass_differences {};

  for (unsigned int i_pair=0; i_pair<m_VBpair_candidate_masses.size(); i_pair++) {
    mass_differences.push_back( fabs( m_VBpair_candidate_masses[i_pair].first - m_VBpair_candidate_masses[i_pair].second ) );
  }

	// Find minimal mass difference
	m_best_pair_index = std::distance( 	
												std::begin(mass_differences), 
												std::min_element( std::begin(mass_differences), std::end(mass_differences) ) 
											);
}


//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::findSignalMCVBPair() {

		IntVec ZZ_candidates{};
		IntVec WW_candidates{};
		
		// Find possible WW or ZZ candidates
		for (unsigned int i_pair=0; i_pair<m_VBpair_candidates.size(); i_pair++) {
			// float m_VB1 = m_VBpair_candidate_masses[i_pair].first;
			// float m_VB2 = m_VBpair_candidate_masses[i_pair].second;
			VBPair VBpair_candidate = m_VBpair_candidates[i_pair];
			
			int VB1_p1_pdgID,VB1_p2_pdgID, VB2_p1_pdgID, VB2_p2_pdgID;
			try {
				VB1_p1_pdgID = dynamic_cast<MCParticle*>(VBpair_candidate.first.first)->getPDG();
				VB1_p2_pdgID = dynamic_cast<MCParticle*>(VBpair_candidate.first.second)->getPDG();
				VB2_p1_pdgID = dynamic_cast<MCParticle*>(VBpair_candidate.second.first)->getPDG();
				VB2_p2_pdgID = dynamic_cast<MCParticle*>(VBpair_candidate.second.second)->getPDG();
				streamlog_out(DEBUG) << "In findMCVBPair: ParticleIDs of candidates: " 
								<< VB1_p1_pdgID << " " << VB1_p2_pdgID << " " << VB2_p1_pdgID<< " " << VB2_p2_pdgID  << " "<< std::endl; 
			} catch( ... ) {
				streamlog_out(ERROR) << "ERROR in findMCVBPair: Tried to cast non-MC into MC" << std::endl; 
				return;
			}
			
			// Check that V masses are not more than 20GeV apart
			// if ( fabs( m_VB1 - m_VB2 ) > 20 ) { 
			// 	streamlog_out(DEBUG) << "In findMCVBPair: Failed mass difference cut" << std::endl; 
			// 	continue;
			// }
			
			// Is ZZ candidate if each pair consists of q and qbar of same flavour
			if ( PDGIDChecks::isZZDecayLike( VB1_p1_pdgID, VB1_p2_pdgID, VB2_p1_pdgID, VB2_p2_pdgID ) ) {
		  	// Check if V within Z mass window
		    // if ( ( m_VB1 + m_VB2 > 171.0 ) && ( m_VB1 + m_VB2 < 195 ) ) { 
				streamlog_out(DEBUG) << "In findMCVBPair: Found as ZZ!" << std::endl; 
				ZZ_candidates.push_back( i_pair );  
	      // }
	    }
		
		  // Is WW candidate if pair consists of (q anti-q) (q anti-q) and in each 
		  // (q anti-q) pair one is up-type and one down-type
		  if ( PDGIDChecks::isWWDecayLike( VB1_p1_pdgID, VB1_p2_pdgID, VB2_p1_pdgID, VB2_p2_pdgID ) ) {
	      // // Check if V within W mass window
	      // if ( ( m_VB1 + m_VB2 > 147.0 ) && ( m_VB1 + m_VB2 < 171.0 ) ) {
				streamlog_out(DEBUG) << "In findMCVBPair: Found as WW!" << std::endl; 
	      WW_candidates.push_back( i_pair );
	      // }
		  }
		}

		// Check if any candidates were found 
	  if ( ( ZZ_candidates.size() == 0) && ( WW_candidates.size() == 0) ) {
	    // Does not fulfill signal requirements for WW or ZZ
			streamlog_out(DEBUG) << "In findMCVBPair: No WW/ZZ candidates found." << std::endl; 
	  }
		else if ( ( ZZ_candidates.size() != 0) && ( WW_candidates.size() != 0) ) { 
			streamlog_out(DEBUG) << "In findMCVBPair: Found both WW and ZZ candidates on MC level! Taking one with smallest VB mass difference." << std::endl; 
			VBPairVec tmp_VBpair_candidates = {};
			for(auto const& i_ZZ: ZZ_candidates ) { tmp_VBpair_candidates.push_back( m_VBpair_candidates[i_ZZ] ); }
			for(auto const& i_WW: WW_candidates ) { tmp_VBpair_candidates.push_back( m_VBpair_candidates[i_WW] ); }
			m_VBpair_candidates = tmp_VBpair_candidates;
			this->minimizeMassDifference();
		}
		else if ( ( ZZ_candidates.size() > 1) || ( WW_candidates.size() > 1) ) { 
			streamlog_out(DEBUG) << "In findMCVBPair: Found more than one possible WW (ZZ) candidate on MC level! Taking one with smallest VB mass difference." << std::endl; 
			VBPairVec tmp_VBpair_candidates = {};
			for(auto const& i_ZZ: ZZ_candidates ) { tmp_VBpair_candidates.push_back( m_VBpair_candidates[i_ZZ] ); }
			for(auto const& i_WW: WW_candidates ) { tmp_VBpair_candidates.push_back( m_VBpair_candidates[i_WW] ); }
			m_VBpair_candidates = tmp_VBpair_candidates;
			this->minimizeMassDifference();
		}
		else if ( ( ZZ_candidates.size() == 1) && ( WW_candidates.size() == 1) ) { 
			streamlog_out(DEBUG) << "In findMCVBPair: Could be either." << std::endl;
			m_VBpair_type = 47; // = 23+24 , could be WW or ZZ depending on definition
		}
	  else if ( ZZ_candidates.size() == 1 ) {
			streamlog_out(DEBUG) << "In findMCVBPair: ZZ candidate found." << std::endl; 
	    m_VBpair_type = 23; // is ZZ
			m_best_pair_index = ZZ_candidates[0];
	  }
	  else if ( WW_candidates.size() == 1 ) {
			streamlog_out(DEBUG) << "In findMCVBPair: WW candidate found." << std::endl; 
	    m_VBpair_type = 24; // is ZZ
			m_best_pair_index = WW_candidates[1];
	  }

}

//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::findBestCandidate() {
	/** First find all VB pair candidates unless they were explicitly set.
			Then choose function for finding best pair according to given
			minimization criterium.
			The choosen function will then set the index of the best pair, and then
			we set the VB pair particles accordingly here.
	*/

	if ( m_VBpair_candidates.empty() ) {

		if (	m_particle_vector.empty() ) {
			streamlog_out(ERROR) << "ERROR in VBPairFinder: No particles were given for pair finding!" << std::endl;
			return;
		}

		this->getAllVBPairCandidates();
		this->getMassesToVBPairCandidates();
		streamlog_out(DEBUG) << "In VBPairFinder: Number of found possible candidates to scan: " << m_VBpair_candidates.size() << std::endl;
	}

	if (m_minimization_criterium == "mass_difference"){
		this->minimizeMassDifference();
	}
	else if (m_minimization_criterium == "MC_VV_likeness"){
		this->findSignalMCVBPair();
	}
	else {
		streamlog_out(ERROR) << "ERROR in VBPairFinder: Unknown m_minimization_criterium: " << m_minimization_criterium << std::endl;
		return;
	}

	m_boson1_p1 = m_VBpair_candidates[m_best_pair_index].first.first;
	m_boson1_p2 = m_VBpair_candidates[m_best_pair_index].first.second;
	m_boson2_p1 = m_VBpair_candidates[m_best_pair_index].second.first;
	m_boson2_p2 = m_VBpair_candidates[m_best_pair_index].second.second;
}

//-------------------------------------------------------------------------------------------------


#endif