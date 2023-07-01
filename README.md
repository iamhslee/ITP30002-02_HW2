## Operating Systems (ITP30002-02, Spring 2023)

### HW#2 : CIMIN (Crashing Input Minimizer)

Author:
* 21800030 구현우 <21800030@handong.ac.kr>
* 22100600 이현서 <hslee@handong.ac.kr>

---

### How to Build

You can build the project with `make` command.

```bash
$ make
```

To build the project with debug mode, you can use `make debug` command.

```bash
$ make debug
```

To clean the build files, you can use `make clean` command.

```bash
$ make clean
```

### How to Run

After building the project, you can run the program with `./cimin` command.

```bash
$ ./cimin [Options] [Executable file \"Options\"]
```

Here is the list of options. (Required options are marked with `*`)

* `  -h` : Print the help message.
* `* -i <file>` : file path of the crashing input.
* `* -m <"string">` : a string whose appearance in standard error determines whether the expected crash occurs or not.
* `* -o <file>` : a new file path to store the reduced crashing input.
* `* Executable file "Options"` : the executable file and its options.

Here is an example of running the program with jsondump.

```bash
$ ./cimin -i example/jsmn/testcases/crash.json -m "AddressSanitizer: heap-buffer-overflow" -o output/reduced.json ./example/jsmn/jsondump
```