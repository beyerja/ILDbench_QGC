#!/bin/bash

# 6f_xxWW
background_final_states=("xxveyx" "xxvlyx" "xxxyev" "xxxylv")
# 6f_eeWW, 6f_llWW
background_final_states+=("eexyyx" "llxyyx" )
# 6f_xxxxZ
background_final_states+=("xxxxee" "xxxxll" )
# 6f_yyyyZ
background_final_states+=("yyyyee" "yyyyll" )


for final_state in ${background_final_states[@]}; do
  ./run_single_process.sh ${final_state} &
done
wait