#!/bin/bash

signal_final_states=("vvxyyx" "xxxxvv" "yyyyvv")

for final_state in ${signal_final_states[@]}; do
  ./run_single_process.sh ${final_state} &
done
wait