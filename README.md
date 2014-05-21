# Objective Beagle
Beagle is an Objective C debugging aid that sniffs out instances of a specified class. 
It does this by hunting through the heap, looking for things that smell like object instances.
 For more info see [How Beagle Works](#how).



## What can Beagle do for me?
* Get all instances of a class and it's subclasses.
* Get all instances of a specified class.
* Get the first instance of a specific class.
* Lookup and list all the subclasses of a specific class.



## <a name="how"></a>How does Beagle work?
* First, Beagle iterates through all "pointer in use" malloc zone ranges in the process (aka the heap).
* For each range, Beagle then grabs the first 4 or 8 bytes (arch pointer size) of the range and casts it to a pointer.
* If the platform uses tagged isa pointers, this pointer is then massaged to remove any possible isa taggedness.
* Beagle then compares this pointer to a list of known class isa pointers.
* If the pointer matches a known class, we then grab the matching classes required instance size and then round it up to the nearest malloc allocation size.
* Finally, we compare this rounded size to the size of the malloc zone range.
* If they match, Beagle treats it as a valid instance and it's added to the result set.
* Woof!



## Beagle is really easy to use
If you just read the above block, you might be a little worried that Beagle is going to be hard to use.
Don't worry, it's super easy and has even has shorthand to save you from having to type.

### beagle (AKA beagle_getInstancesOfClass)
`beagle()` is the main interface to Objective Beagle, and it also exposes some useful class search helpers.
If you can only remember a single function, make it this one! 

```
(lldb) po beagle(@"UISwitch")
<__NSCFArray 0x8f2e6c0>(
<UISwitch: 0x8f73aa0; frame = (93 226; 51 31); opaque = NO; autoresize = RM+BM; layer = <CALayer: 0x8f73bd0>>,
<UISwitch: 0x8e6fa50; frame = (171 226; 51 31); opaque = NO; autoresize = RM+BM; layer = <CALayer: 0x8c6a760>>
)

(lldb) po beagle(@"UISwitch")[0]
<UISwitch: 0x8f73aa0; frame = (93 226; 51 31); opaque = NO; autoresize = RM+BM; layer = <CALayer: 0x8f73bd0>>

(lldb) po beagle(@"UISwi")
[RHBeagle] Error: Unknown class 'UISwi'. Perhaps you want one of these:
	UISwipeGestureRecognizer
	_UISwitchInternalView
	_UISwitchInfo
	_UISwitchInternalViewNeueStyle1
	UISwitch

(lldb) 
```

### beagle_exact (AKA beagle_getInstancesOfExactClass)
`beagle_exact()` operates in exactly the same way as `beagle()` except that it doesn't include subclasses when searching through the heap for instances. This is more useful than you might think when you are working with things like `UIViews` etc.

```
(lldb) po beagle(@"UIView")
<__NSCFArray 0x8d040c0>(
<UITextFieldLabel: 0x8d5c940; frame = (7 1; 83 27); text = 'Text'; opaque = NO; userInteractionEnabled = NO; layer = <CALayer: 0x8d9ca80>>,
<UIStatusBarBackgroundView: 0x8d64dd0; frame = (0 0; 320 20); autoresize = W+H; layer = <CALayer: 0x8d64ee0>>
...

(lldb) po beagle_exact(@"UIView")
<__NSCFArray 0x8cd27a0>(
<UIView: 0x8f722b0; frame = (0 0; 320 480); autoresize = RM+BM; layer = <CALayer: 0x8f71ca0>>,
<UIView: 0x8f74180; frame = (-35.5 0; 51 31); layer = <CALayer: 0x8f741e0>>,
<UIView: 0x8f74210; frame = (0 0; 51 31); layer = <CALayer: 0x8f74270>>,
...

```

### beagle_first (AKA beagle_getFirstInstanceOfClass)
`beagle_first()` returns the first instance that matches the specified class or nil if none found. Quick and dirty.

```
(lldb) po beagle_first(@"UIApplication")
<UIApplication: 0x8d61360>

(lldb) po beagle_first(@"MKMapView")
nil

```

### beagle_classes (AKA beagle_getClassesWithPrefix)
You can use `beagle_classes()` to quickly locate interesting classes with a given name prefix. If you want to be more specific with search patterns, check out `RHBeagleGetClassesWithNameAndOptions()`.

```
(lldb) po beagle_classes(@"OB")
<__NSCFArray 0xe095e00>(
OBViewController,
OBAppDelegate
)

(lldb) po beagle_classes(@"NS")
<__NSCFArray 0xaac0000>(
NSKeyValueSlowGetter,
NSConstantValueExpression,
...

```


### beagle_subclasses (AKA beagle_getSubclassesOfClass)
`beagle_subclasses()` lists all subclasses of a particular class. Again useful for introspection.

```
(lldb) po beagle_subclasses(@"UIButton")
<__NSCFArray 0x8c50890>(
_UIActivityGroupListViewDimControl,
UIActivityGroupCancelButton,
UIKeyboardDictationStarkDoneButton,
_UIStepperButton,
UIRoundedRectButton,
UITexturedButton,
UITableViewCellDeleteConfirmationButton,
...

```

### NSObject category
Beagle also adds some methods onto NSObjects, all prefixed with `beagle_`.
These operate in the same way as the above method defs.

```
@interface NSObject (RHBeagleAdditions)

+ (NSArray *)beagle_instances;
+ (NSArray *)beagle_exactInstances;
+ (id)beagle_firstInstance;
+ (NSArray *)beagle_subclasses;
@end

```


## Adding Beagle to your project.
The easiest way to add Beagle to your project is going to be adding `RHObjectiveBeagle.h` and `RHObjectiveBeagle.m` to your project. You can also find it on CocoaPods.


## Use it for debugging purposes only
Beagle is debugging tool and therefore is only meant to be used for debugging.  
If you need to debug a production build, you can always [load](#load) libBeagle.dylib into the process at runtime.

* It's likely not a good idea to ship it in your production builds.
* Please don't write code that uses it explicitly.
* **Whatever you do, NEVER use it in place of a singleton!**



## Objective Beagle's Interface

```

// Objective Beagle

// object instance search
extern NSArray * beagle(NSString *className);       // shorthand for beagle_getInstancesOfClass()
extern NSArray * beagle_exact(NSString *className); // shorthand for beagle_getInstancesOfExactClass()
extern id beagle_first(NSString *className); // shorthand for beagle_getFirstInstanceOfClass()

// more verbose methods for those that enjoy typing
extern NSArray * beagle_getInstancesOfClass(Class aClass);
extern NSArray * beagle_getInstancesOfExactClass(Class aClass); // excludes subclasses from the result set
extern id beagle_getFirstInstanceOfClass(Class aClass);  // returns the first matching object found



// class lookup
extern NSArray * beagle_classes(NSString *partialName);     // shorthand for beagle_getClassesWithPrefix()
extern NSArray * beagle_subclasses(NSString *className);    // shorthand for beagle_getSubclassesOfClass()

// again, more verbose methods...
extern NSArray * beagle_getClassesWithPrefix(NSString *prefix); // fetch classes that contain partialName in their name
extern NSArray * beagle_getSubclassesOfClass(Class aClass);     // fetch a classes subclasses (Class objects, not instances)



// category additions
@interface NSObject (RHBeagleAdditions)

+ (NSArray *)beagle_instances;
+ (NSArray *)beagle_exactInstances;
+ (id)beagle_firstInstance;

// misc
+ (NSArray *)beagle_subclasses; // returns (Class) subclasses of the current class

@end

```



## <a name="load"></a>Loading libBeagle.dylib into an existing process
Also known as, you're debugging a production build and you want to use Beagle to find instances etc.
Well the news is good. It's easy to do and should only a few steps!
First you need to locate the apropriate libBeagle.dylib. (For iOS targets, you will need `libBeagle-ios.dylib`)

You can likely just use the prebuild dylib files that are checked into this repo in the `libBeagle` folder.

```
(lldb) expr (void*)dlopen("/path/to/libBeagle.dylib", 0x2);
(void *) $12 = 0x005202a5

```

#### Not working? Check for errors
if dlopen returns 0x00000000, an error occured. You can print a human readable error using the below command.

```
(lldb) p (char*)dlerror()
(char *) $13 = 0x005203b5 "dlopen(/path/to/libBeagle.dylib, 2): no suitable image found. Did find: /path/to/libBeagle.dylib: open() failed with errno=1"

```

#### Sandboxed app considerations
Be aware that if your application is sandboxed, the dylib must be inside your apps sandbox. 

The error above was caused by open() failing, due to sandbox restrictions.
For OS X apps, the easiest way around the sandbox is to place a copy of libBeagle.dylib inside your apps sandbox container. The same should be true on iOS using your apps documents directory exposed by iTunes.

```
cp libBeagle.dylib ~/Library/Containers/<app.bundle.id>/Data/
```

## Building libBeagle.dylib && libBeagle-ios.dylib
### libBeagle-ios.dylib specific considerations
Xcode by default will refuse to build libBeagle-ios.dylib with the below error.

```
Target specifies product type 'com.apple.product-type.library.dynamic', but there's no such product type for the 'iphoneos' platform

```
To fix this error, we have to manually edit some Xcode files to add the dynamic library product type. See [here](http://stackoverflow.com/questions/12549633/xcode-4-5-no-com-apple-product-type-application-product-type-for-iphoneos), [here](http://realmacsoftware.com/blog/dynamic-linking) or the `libBeagle` folder for more info.

Also, note that libBeagle-ios is actualy lipo'd together from -iphoneos and -iphonesimulator targets.

## Licence
Released under the Modified BSD License. 
(Attribution Required)
<pre>
RHObjectiveBeagle

Copyright (c) 2014 Richard Heard. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
</pre>



### Version Support
Beagle has been tested on iOS 7 and OS X 10.9 and it supports all current system archetectures including arm64. It should work fine on older systems too. 



### Things that might be nice to have in the future (AKA todo)
* Better support for partial class name matches.
* The ability to exclude all known system classes.
* Filtering instances, based on various properties.


## Who created Beagle?
Beagle is the creation of Richard Heard, a Mac + iOS developer based in San Francisco.

I'm currently working at [Atlas Cove](http://) and i've previously worked at [Apple](http://apple.com), [DailyBooth](http://en.wikipedia.org/wiki/DailyBooth), [Batch](http://techcrunch.com/2012/07/24/airbnb-brian-pokorny-batch-dailybooth/) & [AirBnB](http://airbnb.com). 

From time to time i take on contract work and would be happy to talk about my availability / project requirements etc. 


Finally, you can find me on [Twitter](http://twitter.com/heardrwt)!



