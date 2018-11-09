#ifndef H_SELECTIONCUTTEMPLATES
#define H_SELECTIONCUTTEMPLATES 1

namespace Cuts {
  /** Here some simple templates are declared that create lambda versions of
      analysis cuts.
      These are needed to speed up RDataFrame.
  */
  template<class ValueType> function<bool (ValueType)> getMinCutLambda(ValueType min_limit);
  template<class ValueType> function<bool (ValueType)> getMaxCutLambda(ValueType max_limit);
  template<class ValueType> function<bool (ValueType)> getMinMaxCutLambda(ValueType min_limit, ValueType max_limit);
  function<bool (bool)>     getBoolCutLambda(bool desired_outcome);
  function<bool (float)>    getFloatCutLambda(float desired_outcome);
  function<bool (int)>      getIntCutLambda(int desired_outcome);
  function<bool (TString)>  getStringTestLambda(TString desired_outcome);
}

#endif