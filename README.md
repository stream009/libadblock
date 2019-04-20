# libadblock
- ADBlock engine library implemented with C++
- Adblock Plus (https://adblockplus.org/) compatible
- build as shared library that expose C API

## motivation
Adblock is a rather complicated component in the web browser and 
naive implementation will incur startup delay and high runtime 
memory usage. I wanted more efficient one for my own use.
I originally wrote it as building block of [Qupzilla](https://www.qupzilla.com/)'s
additional plugin. Eventually I graduate Qupzilla and it become component of my
own QtWebEngine base browser. Currently it is used as an component of
[adblockd](https://gitlab.com/stream9/adblockd).

## functionality
### provided
- parsing filter file
- build database
- respond to the query from client

### NOT provided (should be done by the client)
- actual blocking request/content
- updating filter file from the Internet
- editing custom filter list
- UI

## API
see [api.h](https://gitlab.com/stream9/libadblock/blob/master/src/api.h)

## build 
### requirement
- C++17 compiler
- boost
- cmake

### typical instruction
```shell
$ mkdir release
$ cd release
$ cmake -DBUILD_TYPE=Release ../src
$ make -j4
```
