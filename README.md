# computer-network-hw
implement http server &amp; client with raw socket



## Dependencies

### Build

* gcc (11.2.0>=)
* meson (0.59.2 >=)
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

## Building The server &amp; client

``` bash
meson build
ninja -C build
```

## Run programs

```bash
(project root directory)/build/[server, client]
```

## Test library

```bash
ninja -C build
meson test -C build
```


## main logic

1. set server socket and select

2. parse path to PathTree and book handlers

3. run select (run main loop)

4. client request ~

    1. accept client

    2. receive client http request message

        1. parse request message to request instance

        2. make blank http response instance

        3. pass request & response instances to right handler

        4. call handler (modify blank response instance)

        5. stringfy response instance to http response message

        6. send http response message to client

    3. close client

## References

* meson sample project https://github.com/tiernemi/meson-sample-project

* unit test (unity) https://github.com/ThrowTheSwitch/Unity/tree/master/examples/example_4
