#ifndef H_VARIABLECOMBINATIONTEMPLATES
#define H_VARIABLECOMBINATIONTEMPLATES 1

namespace VarComb {
  /** Here some simple templates are declared that create lambda versions of
      analysis cuts.
      These are needed to speed up RDataFrame.
  */
  
  
  template<class ValueType> function<ValueType (ValueType, ValueType)> getPlusLambda();
  template<class ValueType> function<ValueType (ValueType, ValueType)> getMeanLambda();
}

#endif