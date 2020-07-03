#!/bin/bash

source /software/anaconda2/bin/activate zhijieli_stxflow3.0
export MKL_NUM_THREADS=10
result_file=benchmark_result.txt

function run_benchmark()
{
    echo "+++++++++ Start benchmark, K=$1, method=$2"
    start_time=$(date +%s)

    python ../examples.py \
    --out-A-optimal bench_A_optimal \
    --num-times 1 \
    -K $1 \
    --method $2

    end_time=$(date +%s)
    cost_time=$[ $end_time-$start_time ]
    # echo $(date) >> $result_file
    echo $(date)" K=$1 method=$2 elapse time $(($cost_time/60))min $(($cost_time%60))s" >> $result_file
    echo "K=$1 method=$2 elapse time $(($cost_time/60))min $(($cost_time%60))s"
}

function run_profile()
{
    echo "+++++++++ Start profile, K=$1, method=$2"
    start_time=$(date +%s)

    python -m cProfile -o profile.out ../examples.py \
    --out-A-optimal bench_A_optimal \
    --num-times 1 \
    -K $1 \
    --method $2 \
    1>>profile.log

    end_time=$(date +%s)
    cost_time=$[ $end_time-$start_time ]
    echo "K=$1 method=$2 elapse time $(($cost_time/60))min $(($cost_time%60))s"
    python -c "import pstats; p=pstats.Stats('profile.out'); p.sort_stats('cumtime').print_stats()" > profile.txt
}

# run_profile 50 conelp
run_benchmark 100 conelp
# run_benchmark 20 conelp
# run_benchmark 30 conelp
# run_benchmark 40 conelp
# run_benchmark 50 conelp
# run_benchmark 60 conelp
# run_benchmark 70 conelp
# run_benchmark 80 conelp
# run_benchmark 90 conelp
# run_benchmark 100 conelp
