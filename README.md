# cache-simulator

## Introduction
This project explores cache coherence protocols in parallel architectures by simulating a 4-processor system. The simulator is designed to evaluate and compare the MESI and MOESI coherence protocols. I plan to extend this for other Bus and Directory-based protocols as well. The implementation tracks key statistics such as cache hits, misses, memory transactions, and coherence signals, providing insights into the performance and efficiency of these protocols.

## Objectives
- Develop a simulator to evaluate cache coherence protocols in a multiprocessor system.
- Analyze performance metrics including cache hits, misses, memory transactions, and invalidations.
- Compare the optimizations offered by MESI and MOESI protocols in reducing memory and bus transactions.

## Repository Structure
The project directory is organized as follows:
- `src/` - Contains the starter source code:
  - `cache.cc`: Implements basic cache behavior.
  - `cache.h`: Defines cache parameters and methods.
  - `main.cc`: Contains the main program logic.
  - `Makefile`: Compilation instructions.
- `trace/` - Memory access traces.
- `val/` - Validation outputs for provided traces.
- `outputs/` - Terminal outputs for better debugging.

## Implementation Details
The simulator models a system where each processor has an infinite-size L1 cache with a block size of 64 bytes. The provided starter code includes the following components:

### Cache Class
The `Cache` class simulates the behavior of a single cache. It implements basic cache methods and states. To support coherence, new methods such as `Access()` and `Snoop()` can be introduced or modified as required.

### Trace Format
Memory traces represent transactions by processors and include:
- Processor ID (0-3)
- Operation (`r` for read, `w` for write)
- Address (in hexadecimal)

Example:
```
5 w 0xabcd
```
This indicates processor 5 is writing to address `0xabcd`. The cache must handle this request and maintain coherence.

### Supported Traces
Two sample traces are provided:
1. `canneal.04t.debug` (10,000 trace)
2. `canneal.04t.longTrace` (500,000 trace)

## Performance Metrics
The simulator outputs the following statistics:
1. Number of reads
2. Number of read misses
3. Number of writes
4. Number of write misses
5. Number of write hits
6. Number of read hits
7. Total miss rate
8. Number of memory accesses (excluding writebacks)
9. Number of invalidations
10. Number of flushes
11. Total program execution time

You can edit the class structure and add support for more statistics as you please. Take a deeper look at `src/cache.h`

## Running the Simulator

### Compilation
Navigate to the `src/` directory and compile the code using the provided Makefile:
```bash
make clean  # Clean previous builds
make        # Compile the simulator
```

### Execution
Run the following commands to execute the simulator:
- `make test_mesi`: Run the MESI protocol simulation.
- `make test_moesi`: Run the MOESI protocol simulation.

Both commands are preconfigured for experimentation. Modify the traces for debugging or extended tests as needed. Also feel free to create your own traces. (highly recommended)

## Implementation Suggestions
1. Thoroughly study the starter code, especially `cache.cc` and `cache.h`.
2. Implement the MESI and MOESI protocols by overriding or extending the `Access()` method.
3. Use protocol-specific counters, states, and methods for tracking coherence.
4. Define an array of caches to represent the multi-processor system.
5. Optimize the implementation to reduce memory and bus transactions.
6. Validate results against the provided outputs in the `val/` directory using `make val`. (this hasn't been implemented yet. just manually compare outputs for now)
7. Depending on your implementation, the metrics can have different values. The best stats I could achieve are recorded in the `outputs/` folder. Do let me know if you manage to optimize the numbers further. 


This project provides a hands-on approach to understanding cache coherence protocols in parallel systems. By implementing and analyzing coherence protocols, the simulator highlights the trade-offs between memory and bus optimizations. The results offer valuable insights into designing efficient cache systems for modern parallel architectures.

---

## Repository Contents
- **Source Code**: `src/`
- **Traces**: Memory traces for debugging and testing.
- **Validation**: Pre-computed outputs for result comparison.
- **Outputs**: My personal best metrics achieved.

Feel free to contribute to this project or use it as a reference for exploring cache coherence protocols.

