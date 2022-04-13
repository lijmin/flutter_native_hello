# flutter_native_hello

## 前言
Flutter可直接调用android .so，ios .a库，有时候这个库需要通过c代码封装再提供给dart调用。
我们可通过dart:ffi来实现，在Flutter中创建插件调用第三方的c库。
github：<https://github.com/lijmin/flutter_native_hello>

## 准备
1. 创建android libhello.so库， 参考：https://lijmin.github.io/2022/03/20/15、android%20NDK引入第三方so库及头文件/
2. 创建ios libhello.a库，参考：https://lijmin.github.io/2022/03/24/16、iOS生成.a静态库并引用/
3. 分别添加getHelloWorld函数
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *getHelloWorld(char *name){
    char *hello = "Hello Android"; //iOS 
    char *helloName = (char *) malloc(strlen(hello) + strlen(name));
    strcpy(helloName, hello);
    strcat(helloName, name);
    return helloName;
}
```
<!--more-->
## 创建Flutter插件
`flutter create --platforms=android,ios -a java -i swift --template=plugin native_hello`
-a java 对应android开发语言，-i swift对应ios开发语言，尽量与项目保持一致，可减少项目配置。

## 添加c代码
由于iOS不能把文件路径放于podspec之上，而android CMakeLists.txt 可以指定到ios目录，所以我们把c代码放在`ios/Classes/`目录下
创建native_hello.c，将hello.h文件放在同级目录，调用getHelloWorld方法。
native_hello.c
```c
#include "hello.h"

char *getNativeTxt(char *name){
    return getHelloWorld(name);
}
```
### android 加入libhello.so
* 在android目录下创建libs文件夹，加入so库。
* 创建CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.4.1)

include_directories(../ios/Classes/hello.h) #包含头文件

add_library(hello SHARED IMPORTED)
set_target_properties(hello PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libhello.so)

add_library( native_hello
             SHARED
             ../ios/Classes/native_hello.c )
target_link_libraries(native_hello hello)
```
* build.gradle添加`externalNativeBuild`

```groovy
android {
  // ...
  externalNativeBuild {
    cmake {
      path "CMakeLists.txt"
    }
  }
  // ...
}
```
* 目录结构如下：

![](https://lijmin.github.io/00_blogImg/img_blog18_01.jpeg)
### iOS加入libhello.a
* 将libhello.a文件加入ios根目录
* 在根目录下的 .podspec配置 `s.vendored_libraries = 'libhello.a'`

## dart调用native_hello.c
* 根据平台动态加载本地代码

```dart
import 'dart:ffi'; // For FFI
import 'dart:io'; // For Platform.isX

final DynamicLibrary nativeLib = Platform.isAndroid
    ? DynamicLibrary.open('libnative_hello.so')
    : DynamicLibrary.process();
```
* 映射函数

```dart
final Pointer<Int8> Function(Pointer<Int8>) getNativeTxt = nativeLib
    .lookup<NativeFunction<Pointer<Int8> Function(Pointer<Int8>)>>('getNativeTxt')
    .asFunction();
```
* 类型映射表

![](https://lijmin.github.io/00_blogImg/img_blog18_02.jpeg)

* 在main.dart中调用函数getNativeTxt 

* 引入ffi插件，在pubspec.yaml中加入`ffi: ^1.1.2`，用于c字符串与dart字符串的映射

* 引入`package:ffi/ffi.dart`

```dart
    import 'package:ffi/ffi.dart';

    Pointer<Int8> dartString = " dart".toNativeUtf8().cast<Int8>();
    Pointer<Int8> res = getNativeTxt(dartString);
    String nativeTxt = res.cast<Utf8>().toDartString();
    print(nativeTxt);
```
* 最后记得内存释放

```dart
    malloc.free(dartString);
    malloc.free(res);
```
* 分别在ios、android上运行

![](https://lijmin.github.io/00_blogImg/img_blog18_03.png)
![](https://lijmin.github.io/00_blogImg/img_blog18_04.jpeg)

