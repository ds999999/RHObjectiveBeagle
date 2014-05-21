//
//  RHObjectiveBeagle.h
//
//  Created by Richard Heard on 19/05/2014.
//  Copyright (c) 2014 Richard Heard. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//  3. The name of the author may not be used to endorse or promote products
//  derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
//  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Objective Beagle is an Obj-C debugging aid that sniffs out instances of classes on the heap.
//
//  eg: (lldb) po beagle(@"UIApplication")[0]
//      <UIApplication: 0x60000011cd40>
//
//  You can also load Beagle into your app at runtime using libBeagle.dylib
//  eg: (lldb) expr (void*)dlopen("/path/to/libBeagle.dylib", 0x2);



#ifndef RH_OBJECTIVE_BEAGLE_H
#define RH_OBJECTIVE_BEAGLE_H 1

#import <Foundation/Foundation.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------
// Objective Beagle


// instance search
extern NSArray * beagle(NSString *className);       // shorthand for beagle_getInstancesOfClass()
extern NSArray * beagle_exact(NSString *className); // shorthand for beagle_getInstancesOfExactClass()
extern        id beagle_first(NSString *className); // shorthand for beagle_getFirstInstanceOfClass()

// more verbose methods for those that enjoy typing
extern NSArray * beagle_getInstancesOfClass(Class aClass);
extern NSArray * beagle_getInstancesOfExactClass(Class aClass); // excludes subclasses from the result set
extern        id beagle_getFirstInstanceOfClass(Class aClass);  // returns the first matching object found



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



// ---------------------------------------------------------
// Objective Beagle - Implementation

// options for RHFindInstancesOfClassWithOptions
typedef NS_OPTIONS(NSUInteger, RHBeagleFindOptions) {
    RHBeagleFindOptionsDefault                    = 0UL,
    RHBeagleFindOptionFirstMatch                  = (1UL << 0), // return the first matching ([array count] == 1)
    RHBeagleFindOptionLastMatch                   = (1UL << 1), // return the last matching object ([array count] == 1)
    RHBeagleFindOptionExcludeSubclasses           = (1UL << 2), // prevent matching of subclasses
    RHBeagleFindOptionIncludeKnownUnsafeObjects   = (1UL << 3), // include known bad placeholder objects (ie __NSPlaceholderArray && __NSPlaceholderString) which raise exceptions
};

// search implementation
extern NSArray * RHBeagleFindInstancesOfClassWithOptions(Class aClass, RHBeagleFindOptions options);

// misc
extern NSArray * RHBeagleGetSubclassesOfClass(Class aClass);
extern NSArray * RHBeagleGetClassesWithNameAndOptions(NSString *partialName, NSStringCompareOptions options); //useful options include (NSCaseInsensitiveSearch, NSAnchoredSearch and NSBackwardsSearch)


// ---------------------------------------------------------
// Passthrough methods

@interface NSObject (RHBeaglePassthroughAdditions)

- (id)beagle_ivarDescription;
- (id)beagle_methodDescription;
- (id)beagle_shortMethodDescription;

@end

#ifdef __cplusplus
}
#endif

#endif //end RH_OBJECTIVE_BEAGLE_H

