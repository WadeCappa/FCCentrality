g++ -Os source/run_sequential_timings.cpp source/adjacency_builder/*.cpp source/flow_cost_centrality/*.cpp

./a.out networks/synthetic/network_v32.txt    128
./a.out networks/synthetic/network_v64.txt    128
./a.out networks/synthetic/network_v128.txt   128
./a.out networks/synthetic/network_v256.txt   128
./a.out networks/synthetic/network_v512.txt   128
./a.out networks/synthetic/network_v1024.txt  128
