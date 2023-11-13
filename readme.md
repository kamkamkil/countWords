# WordCounter 

Program designed to count how many different words are in given file.

Input: name of file (it is assumed that file consist only of letter a-z and A-Z)

Output: amount of different words 

## usage 

` wordCounter [FILE PATH] option  ` 

### options: 

    -?, -h, --help                  display usage information
    -t, --threads                   set how many threads will be used
    -q, --quiet                     disable printing to standard output

### Compilation

Project uses Cmake to help with compilation example using cmake

```
mkdir build 
cd build 
cmake ../
cmake --build .
or 
cmake --build . -t wordCounter  //to just build app

```

## Tests and benchmarks 

### Running test 

To build just test use `cmake --build . -t test` You can run test using `ctest`. To run Heavy tests or benchmark use binary tests and using appropriate tags : `test.exe [!benchmark]`

### Running benchmarks

To just build benchmark use `cmake --build . -t benchmarks` by default only fast benchmarks will run (1Mb file), if you wish to run longer benchmark use:

```
[medium] for 254mb file 
[heavy] for 1Gb files
```
by default every benchmark will run 100 times for bigger files that make take to long you can decrease it by using `--benchmark-samples` flag

You can use flag `-N` if you wish not to delate test files after testing / benchmarking 