#!/bin/bash

source /software/anaconda2/bin/activate netbfe

function do_benchmark()
{
    echo "++++++++++++++++++++ Start benchmark, K="$1
    start_time=$(date +%s)

    python -m cProfile -o profile.out ../examples.py \
    --out-update-A-optimal bench_update_A_optimal \
    --num-times 1 \
    -K $1

    end_time=$(date +%s)
    cost_time=$[ $end_time-$start_time ]
    echo "K=$1, time is $(($cost_time/60))min $(($cost_time%60))s"
}

function do_benchmark_sparse()
{
    echo "++++++++++++++++++++ Start benchmark sparse, K="$1
    start_time=$(date +%s)

    python examples.py \
    --out-sparse-A-optimal-network bench_update_A_optimal_sparse \
    --num-times 1 \
    -K $1 \
    --connectivity $2

    end_time=$(date +%s)
    cost_time=$[ $end_time-$start_time ]
    echo "K=$1, connectivity=$2, time is $(($cost_time/60))min $(($cost_time%60))s" >> sparse_result.txt
}

# do_benchmark 10
# do_benchmark 20
# do_benchmark 30
# do_benchmark 40
do_benchmark 30
# do_benchmark 60
# do_benchmark 70
# do_benchmark 80
# do_benchmark 90
# do_benchmark 100

#do_benchmark_sparse 10 3
#do_benchmark_sparse 20 3
#do_benchmark_sparse 30 3
#do_benchmark_sparse 40 3
#do_benchmark_sparse 50 3
#do_benchmark_sparse 60 3
#do_benchmark_sparse 70 3
#do_benchmark_sparse 80 3
#do_benchmark_sparse 90 3
#do_benchmark_sparse 100 3

python -c "import pstats; p=pstats.Stats('profile.out'); p.sort_stats('cumtime').print_stats()" > profile.txt
