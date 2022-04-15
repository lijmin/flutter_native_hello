#import "NativeHelloPlugin.h"
#if __has_include(<native_hello/native_hello-Swift.h>)
#import <native_hello/native_hello-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "native_hello-Swift.h"
#endif

@implementation NativeHelloPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftNativeHelloPlugin registerWithRegistrar:registrar];
}
@end
