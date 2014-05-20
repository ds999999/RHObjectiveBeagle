#Adding iOS dylib support to Xcode

* Navigate to  `/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/Library/Xcode/Specifications/`.
* Open `iPhoneOSPackageTypes.xcspec` in Xcode and add the contents of `iPhoneOSPackageTypes.plist` to the top level array.

* Open `iPhoneOSProductTypes.xcspec` and add the contents of `iPhoneOSProductTypes.plist` to the top level array.

* Do the same for iphonesimulator
* Restart Xcode.