
import 'dart:async';

import 'package:flutter/services.dart';
import 'dart:ffi'; // For FFI
import 'dart:io'; // For Platform.isX

final DynamicLibrary nativeAddLib = Platform.isAndroid
    ? DynamicLibrary.open('libnative_hello.so')
    : DynamicLibrary.process();

final int Function() getVersionNative = nativeAddLib
    .lookup<NativeFunction<Int32 Function()>>('getVersionNative')
    .asFunction();

final int Function() bookmain = nativeAddLib
    .lookup<NativeFunction<Int32 Function()>>('bookmain')
    .asFunction();

class NativeHello {
  static const MethodChannel _channel = MethodChannel('native_hello');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
