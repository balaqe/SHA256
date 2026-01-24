# SHA256
Case study of SHA256

# Usage
+ Import the library using the header file `sha256.h`
+ Allocate a result buffer
+ Call `sha256(char *input, char *res)` (the resulting hash will be found in the `res` buffer)

# Example program
For testing, compile the example program by running `make`

```
./testprog.out "Your input string here"
```

# Notes 
The implementation was designed for little endian systems with a built in conversion to big endian for initial encoding. If you are running a big endian systems, the code may need to be modified.