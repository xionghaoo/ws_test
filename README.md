## uwebsockets安装：

1. 安装vcpkg
   https://vcpkg.io/en/getting-started

2. 利用vcpkg安装uwebsockets，默认是安装32位，64位需要指定

```shell
vcpkg install uwebsockets:x64-windows
```

安装完成后，执行

```shell
vcpkg integrate install
```

完成后，uwebsockets的头文件会自动包含在vs的工程中。
如果没有，可以手动包含，头文件路径
`vcpkg\installed\x64-windows\include`
然后工程中就可以使用头文件啦

```c++
#include <uwebsockets/App.h>
```

## Visual Studio设置

uwebsockets用的是c++17的语言标准，但是实际测试要调到20才行。

```
C++语言标准：ISO C++20
C语言标准：ISO C17
```



## 其他

vcpkg命令
https://learn.microsoft.com/en-us/vcpkg/commands/integrate
生成nuget配置文件

```vcpkg integrate project```