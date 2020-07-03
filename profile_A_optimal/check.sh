#!/bin/bash

source /software/anaconda2/bin/activate zhijieli_stxflow3.0
export MKL_NUM_THREADS=10

function run_check()
{
    echo "+++++++++ Start check, K=$1"
    start_time=$(date +%s)

    python ../examples.py \
    --check-A-optimal check_A_optimal \
    --num-times 1 \
    -K $1 \
    1>>check.log

    end_time=$(date +%s)
    cost_time=$[ $end_time-$start_time ]

    echo $(date)" K=$1 elapse time $(($cost_time/60))min $(($cost_time%60))s"
    echo "K=$1 elapse time $(($cost_time/60))min $(($cost_time%60))s"
}

run_check 10
run_check 20
run_check 30
#run_check 40
#run_check 50
#run_check 60
