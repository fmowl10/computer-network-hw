# computer-network-hw
implement http server &amp; client with raw socket

## Page_Index

* [Dependencies](#Dependencies)
* [Project Commands](#project-commands)
* [main logic](#main-logic)
* [References](#references)
___



## Dependencies

### Build

* gcc (>= 11.2.0)
* meson (>= 0.53.2 )
* ninja (1.10.2)
* build-enssential
* dot
* doxygen
* git
* graphviz
* python3
* curl
* wget
* cmake
* pdflatex

___

## Project Commands

### Building The server &amp; client

``` bash
meson builddir
ninja -C builddir
```

### Run programs

#### client

```bash
# full
(project root directory)/builddir/client -h [host] -m [method] -d [data]

# post
(project root directory)/builddir/client -h [host] -d [data]

# get
(project root directory)/builddir/client -h [host] 
```

### server
```bash
(project root directory)/builddir/server [portnumber]
```

### Test library

```bash
meson test -C builddir
```

### Valgrind

```sh
meson test -C build --wrap='valgrind --leak-check=full --error-limit=no --track-originas=yes' --versbose
```

### Create Document

(until meson 0.55.0)
```
cd builddir
ninja docs
ninja pdf
```

(since meson 0.55.0)
```sh
meson compile docs -C builddir
# create pdf 
# file located in builddir/docs/latex/refman.pdf
meson compile pdf -C builddir
```

___

## main logic

1. set server socket and select

2. parse path to PathTree and book handlers

3. run select (run main loop)

4. client request ~

    1. accept 
        1. accept client

    2. receive client http request message

        1. parse request message to request instance

        2. make blank http response instance

        3. pass request & response instances to right handler

        4. call handler (modify blank response instance)

        5. stringfy response instance to http response message

        6. send http response message to client

    3. close client
        1. close
___

## References

* meson sample project https://github.com/tiernemi/meson-sample-project

* unit test (unity) https://github.com/ThrowTheSwitch/Unity/tree/master/examples/example_4

* doxygen meson configuration https://gitlab.freedesktop.org/libinput/libinput/blob/a52f0db3c54b093a2c44dce37ea6dd5582a19c5a/doc/api/meson.build