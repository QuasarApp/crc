# QuasarApp crc library
suppot:  crc16/crc32/crc64

## Build

``` bash
git clone https://github.com/gityf/crc.git
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=1 -DCRC_TESTS=1
make 
```

## Testing

``` bash
./crc_test 
[ RUN      ] ==== Test CRC8Test.BasicTest
[ RUN      ] ==== Test CRC16Test.BasicTest
[ RUN      ] ==== Test CRC32Test.BasicTest
[ RUN      ] ==== Test CRC64Test.BasicTest
[ RUN      ] ==== Test CRC8PolyTest.BasicTest
[ RUN      ] ==== Test CRC16PolyTest.BasicTest
[ RUN      ] ==== Test CRC32PolyTest.BasicTest
[ PASS     ] ==== PASSED 7 tests
[ NOPASS   ] ==== ERROR 0 tests

```

## Include 

### Cmake build system 

``` cmake
add_subdirectory(crc)

target_link_libraries(${PROJECT_NAME} PUBLIC crc)

```


## usage

``` cpp
#include <crc/crchash.h>

int main() {
    std::string data = "This is data for hash";
    
    auto hash16 = qa_common::hash16(data)
    auto hash32 = qa_common::hash32(data)
    auto hash64 = qa_common::hash64(data)
    
    return 0;
}


```
