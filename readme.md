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

### running test 

You can run test using `ctest`. To run Heavy tests or benchmark use binary tests and using appropriate tags : `test.exe [!benchmark]` 