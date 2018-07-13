#ifndef JETCLUSTERINGIMPL_h
#define JETCLUSTERINGIMPL_h 1

#include "JetClusteringTemplate.h"
#include <typeinfo> // For checking class of ParticleClass

//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
void JetClusterer<ParticleClass>::setInputParticles( ParticleVec &input ){
  m_input_particles = input;
}

template<class ParticleClass>
void JetClusterer<ParticleClass>::setClusteringAlgorithm( std::string algorithm ){
  m_clustering_algorithm = algorithm;
}

template<class ParticleClass>
void JetClusterer<ParticleClass>::setNFinalParticles( int N_final ){
  m_N_final = N_final;
}

//-------------------------------------------------------------------------------------------------
  
template<class ParticleClass>
std::vector<ParticleClass*> JetClusterer<ParticleClass>::getClusteredParticles(){
  return m_clustered_particles;
}

//-------------------------------------------------------------------------------------------------
template<class ParticleClass>
void JetClusterer<ParticleClass>::Clean(){
  for ( unsigned int i_new=0; i_new<m_newly_created_particles.size(); i_new++ ){
    delete m_newly_created_particles[i_new];
  }
}

//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
ParticleClass* JetClusterer<ParticleClass>::combineParticles( ParticleClass* particle1, ParticleClass* particle2 ){
  TLorentzVector tlv1 = TLorentzVector( particle1->getMomentum(), particle1->getEnergy() ); 
  TLorentzVector tlv2 = TLorentzVector( particle2->getMomentum(), particle2->getEnergy() ); 
  const TLorentzVector combined_tlv = tlv1 + tlv2;
  
  ParticleClass* new_particle{};
  
  std::string particle_class = typeid(ParticleClass).name();
  
  // For creating a new particle I must know the base class implementation -> Create Impl instance the cast to ParticleClass
  if ( particle_class == "N5EVENT10MCParticleE" ){
    MCParticleImpl* tmp= new MCParticleImpl;
    double combined_p[3] = {combined_tlv.Px(), combined_tlv.Py(), combined_tlv.Pz()} ;
    tmp->setMomentum( combined_p );
    tmp->setMass(     combined_tlv.M()  );
    new_particle = dynamic_cast<ParticleClass*>(tmp);
  }
  else {
    streamlog_out(ERROR) << "ERROR in combineParticles: Unknown particle type : " << particle_class << " ! " << std::endl;
  }
  
  // Register new particle for deletion
  m_newly_created_particles.push_back(new_particle);
  
  return new_particle;
}

//-------------------------------------------------------------------------------------------------
  
template<class ParticleClass>
double JetClusterer<ParticleClass>::ee_kt_distance( ParticleClass* particle1, ParticleClass* particle2 ){
  TLorentzVector tlv1 = TLorentzVector( particle1->getMomentum(), particle1->getEnergy() ); 
  TLorentzVector tlv2 = TLorentzVector( particle2->getMomentum(), particle2->getEnergy() ); 
  
  double Esquared_min = std::min( std::pow( tlv1.E(), 2 ), std::pow( tlv2.E(), 2 ) );
  double cosTheta_12  = TMath::Cos( tlv1.Angle(tlv2.Vect()) );
  
  return 2.0 * Esquared_min * ( 1.0 - cosTheta_12 ); 
}
  
template<class ParticleClass>
std::vector<ParticleClass*> JetClusterer<ParticleClass>::ee_kt_algorithm_step( ParticleVec &particles ){
  double minimal_distance = std::numeric_limits<double>::infinity();
  int minimum_index1 {};
  int minimum_index2 {};
  
  // Find the two particles with minimal distance
  for ( unsigned int i_p1=0; i_p1<particles.size()-1; i_p1++ ) {
    for ( unsigned int i_p2=i_p1+1; i_p2<particles.size(); i_p2++ ) {
      double distance_12 = ee_kt_distance( particles[i_p1], particles[i_p2] );
      if ( distance_12 < minimal_distance ) {
        minimal_distance = distance_12;
        minimum_index1 = i_p1;
        minimum_index2 = i_p2;
      }
    }
  }
  
  // Combine closest particle to new particle, then create new vector with particles replaced
  ParticleClass* new_particle = this->combineParticles(particles[minimum_index1], particles[minimum_index2]);
  ParticleVec new_clustered_particles (particles);
  new_clustered_particles.erase( std::remove(new_clustered_particles.begin(),new_clustered_particles.end(), particles[minimum_index1]), new_clustered_particles.end() );
  new_clustered_particles.erase( std::remove(new_clustered_particles.begin(),new_clustered_particles.end(), particles[minimum_index2]), new_clustered_particles.end() );
  new_clustered_particles.push_back(new_particle);
  
  return new_clustered_particles;
}

//-------------------------------------------------------------------------------------------------
  
template<class ParticleClass>
void JetClusterer<ParticleClass>::runClustering(){
  streamlog_out(DEBUG) << "In JetClusterer: Running " << m_clustering_algorithm << " clustering, "
    << "starting with " << m_input_particles.size() << " particles." << std::endl;
  
  m_clustered_particles = m_input_particles;
  
  // Keep clustering particles down until clustered vector no longer above requested size 
  while ( m_clustered_particles.size() > m_N_final ) {
    streamlog_out(DEBUG) << "In runClustering: Clustering step, current #particles: " << m_clustered_particles.size() << std::endl;
    if ( m_clustering_algorithm == "ee_kt_algorithm" ) {
      m_clustered_particles = this->ee_kt_algorithm_step( m_clustered_particles );
    }
    else {
      streamlog_out(ERROR) << "ERROR in JetClusterer: Unknown algorithm name: " << m_clustering_algorithm << std::endl;
    }
  }
  
  streamlog_out(DEBUG) << "In runClustering: Clustering done, final #particles: " << m_clustered_particles.size() << std::endl;
}

//-------------------------------------------------------------------------------------------------

#endif