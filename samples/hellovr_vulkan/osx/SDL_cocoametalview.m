/*
 Simple DirectMedia Layer
 Copyright (C) 1997-2017 Sam Lantinga <slouken@libsdl.org>
 
 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

/*
 * @author Mark Callow, www.edgewise-consulting.com.
 *
 * Thanks to Alex Szpakowski, @slime73 on GitHub, for his gist showing
 * how to add a CAMetalLayer backed view.
 */

#import "SDL_cocoametalview.h"

#include "SDL_assert.h"
#include "SDL_loadso.h"
#include <SDL_syswm.h>
#include <dlfcn.h>

@implementation SDL_cocoametalview

/* The synthesized getter should be called by super's viewWithTag. */
@synthesize tag = _tag;

/* Return a Metal-compatible layer. */
+ (Class)layerClass
{
  return NSClassFromString(@"CAMetalLayer");
}

/* Indicate the view wants to draw using a backing layer instead of drawRect. */
-(BOOL) wantsUpdateLayer { return YES; }

/* When the wantsLayer property is set to YES, this method will be invoked to
 * return a layer instance.
 */
-(CALayer*) makeBackingLayer { return [self.class.layerClass layer]; }

- (instancetype)initWithFrame:(NSRect)frame
                   useHighDPI:(bool)useHighDPI
{
  if ((self = [super initWithFrame:frame])) {
    
    /* Allow resize. */
    self.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    _tag = METALVIEW_TAG;
      
    _useHighDPI = useHighDPI;
    [self updateDrawableSize];
  }
  
  return self;
}

/* Set the size of the metal drawables when the view is resized. */
- (void)resizeSubviewsWithOldSize:(NSSize)oldSize {
    [super resizeSubviewsWithOldSize:oldSize];
    [self updateDrawableSize];
}

- (void)updateDrawableSize
{
    if (_useHighDPI) {
        NSSize size = [self convertRectToBacking:[self bounds]].size;
         /* Isn't there a better way to convert from NSSize to CGSize? */
        CGSize cgsize = *(CGSize*)&size;
        ((CAMetalLayer *) self.layer).drawableSize = cgsize;
    }
}

@end

SDL_cocoametalview*
Cocoa_Mtl_AddMetalView(SDL_Window* window)
{
    SDL_SysWMinfo wmInfo;
    SDL_VERSION( &wmInfo.version );
    SDL_GetWindowWMInfo( window, &wmInfo );
    NSView *view = wmInfo.info.cocoa.window.contentView;

    Uint32 flags = SDL_GetWindowFlags(window);

    SDL_cocoametalview *metalview
        = [[SDL_cocoametalview alloc] initWithFrame:view.frame
                       useHighDPI:(flags & SDL_WINDOW_ALLOW_HIGHDPI)];
    // Instantiate the CAMetalLayer
    metalview.wantsLayer = YES;
    [view addSubview:metalview];
    return metalview;
}

void
Cocoa_Mtl_GetDrawableSize(SDL_Window * window, int * w, int * h)
{
    SDL_SysWMinfo wmInfo;
    SDL_VERSION( &wmInfo.version );
    SDL_GetWindowWMInfo( window, &wmInfo );
    NSView *view = wmInfo.info.cocoa.window.contentView;

    SDL_cocoametalview* metalview = [view viewWithTag:METALVIEW_TAG];
    if (metalview) {
        CAMetalLayer *layer = (CAMetalLayer*)metalview.layer;
        assert(layer != NULL);
        if (w)
            *w = layer.drawableSize.width;
        if (h)
            *h = layer.drawableSize.height;
    }
}

/* vi: set ts=4 sw=4 expandtab: */
