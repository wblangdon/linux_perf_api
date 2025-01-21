# linux_perf
The Linux perf utility is available via the command line.
see man perf.

Here we show an example of calling perf's API in demo_perf.c

Examples:
```
  gcc demo_perf.c
  ./a.out ; echo "status $status"
  Linux perf can measure many things
  Here we ask it how instruction run_me() consumed
  Notice this will depend on how run_me() is compiled
  For example try without -O3 and then with -O3
  
  run_me() took 40027 instructions
  status 0

  gcc -O3 demo_perf.c
  ./a.out 
  Linux perf can measure many things. 
  Here we ask it how instruction run_me() consumed. 
  Notice this will depend on how run_me() is compiled. 
  For example try without -O3 and then with -O3 
  
  run_me() took 11 instructions
```

Examples from "Population Diversity, Information Theory and Genetic Improvement"
William B. Langdon and David Clark. In EuroGP-2025, Bing Xue, Luca Manzoni, Illya Bakurov _Eds._,
Trieste.
23-25 April 2025.
Springer.

triangle.c source code and tests cases in https://github.com/wblangdon/triangle
