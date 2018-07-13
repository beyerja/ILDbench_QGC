#ifndef JETCLUSTERINGTEMPLATES_h
#define JETCLUSTERINGTEMPLATES_h 1

template<class ParticleClass>
class JetClusterer {
  
  typedef std::vector <ParticleClass*> 	ParticleVec;

  public:  
    // These three lines avoid frequent compiler warnings when using -Weffc++
    JetClusterer<ParticleClass>() = default;
    JetClusterer<ParticleClass>( const JetClusterer<ParticleClass>& ) = delete;
    JetClusterer<ParticleClass>& operator=( const JetClusterer<ParticleClass>& ) = delete;
    
    // Member functions meant to be executed from outside 
    void setInputParticles( ParticleVec &input );
    void setClusteringAlgorithm( std::string algorithm );
    void setNFinalParticles( int N_final );
    void runClustering();
    ParticleVec getClusteredParticles();
    void Clean();
    
  protected:
    // Member functions
    ParticleClass* combineParticles( ParticleClass* particle1, ParticleClass* particle2 );
    
    ParticleVec ee_kt_algorithm_step( ParticleVec &particles );
    double ee_kt_distance( ParticleClass* particle1, ParticleClass* particle2  );
    
    // Member variables
    std::string   m_clustering_algorithm {};
    unsigned int  m_N_final {1};
    
    ParticleVec m_input_particles {};
    ParticleVec m_clustered_particles {};
    ParticleVec m_newly_created_particles {}; // MUST be cleaned up!
};

#include "JetClusteringTemplate.tpp"

#endif