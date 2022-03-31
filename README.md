# computer-network-hw
implement http server &amp; client with raw socket



## dependencies

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



## References

* meson sample project https://github.com/tiernemi/meson-sample-project

* unit test (unity) https://github.com/ThrowTheSwitch/Unity/tree/master/examples/example_4
