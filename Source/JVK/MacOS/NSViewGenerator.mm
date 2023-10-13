/*
  ==============================================================================

    NSViewGenerator.mm
    Created: 9 Oct 2023 4:29:14pm
    Author:  Gavin

  ==============================================================================
*/

#include <AppKit/AppKit.h>
#import <QuartzCore/QuartzCore.h>
#include "NSViewGenerator.h"

namespace jvk
{

NSViewGenerator::NSViewGenerator()
{
    // Constructor implementation if needed
}

NSViewGenerator::~NSViewGenerator()
{
    release();
}

bool NSViewGenerator::isValid()
{
    NSView* view = (NSView*)ptr;
    return view;
}

void* NSViewGenerator::create()
{
    NSView* view = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 800, 600)];
    if (!view.layer || ![view.layer isKindOfClass:[CAMetalLayer class]])
    {
        view.wantsLayer = YES;
        view.layer = [CAMetalLayer layer];
    }
    [view retain];
    ptr = (void*)view;
    return (void*)view;
}

void NSViewGenerator::release()
{
    NSView* view = (NSView*)ptr;
    if (view)
    {
        [view removeFromSuperview];  // Remove from its parent view or window.
        [view release];
        ptr = nullptr;
    }
}

} // jvk
