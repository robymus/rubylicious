# Demo

Demo is a singleton class (class with class level methods) defined in the Ruby VM.

This is the main entry point for the demo engine, all functions are accessed through here.

## Setup and teardown

### Demo.messageBox(title, message, type)

Shows a modal message box (mainly for error messages). This can be used before any initialization also (or when initialization fails).

Type must be "error", "warning" or "information", everything else defaults to information.

### Demo.initialize

Initialize engine and SDL. May throw *RuntimeError* exception with error message. 

### Demo.shutdown

Shutdown engine and SDL.

### Demo.createWindow(title, width, height, fullScreen = false, display = 0)

Creates the main demo window, with width and height specified. If *fullscreen* is *true*, starts in fullscreen.

The *display* parameter is the number of the display - it is used only in fullscreen mode.  

### Demo.closeWindow

Closes the main demo window. Not necessary to call if shutdown is called, it might be useful for reopening an other window, maybe. 

### Demo.setGLAttribute(attr, value)

Exposes OpenGL attributes as here: https://wiki.libsdl.org/SDL_GL_SetAttribute

Attribute has to be passed as string, without the SDL_GL_ prefix, and in any case, eg. "red_size" words for SDL_GL_RED_SIZE.

Value is parsed as integer, except for CONTEXT_PROFILE_MASK, where "CORE", "COMPATIBILITY" and "ES" are allowed.

May throw *RuntimeError* exception with error message.

## Information query

### Demo.width, Demo.height

Returns the dimensions of the active window (0 if window is not initialized)

## Events

### Demo.pollEvents { |evt| ... }

Polls SDL events, all events are passed to the block for processing. Event objects are simple hashes with symbols as keys.

The following event types are mapped currently, with associated event hash format.
 
**keyboard**
- Scancodes are converted to integer, details [from SDL source](https://hg.libsdl.org/SDL/file/498a7f27758e/include/SDL_scancode.h#l43)  
- Keycodes: are converted to integer, details [from SDL source](https://hg.libsdl.org/SDL/file/498a7f27758e/include/SDL_keycode.h#l50)
- Modifiers is not mapped at all, use the binary values [from SDL source](https://hg.libsdl.org/SDL/file/498a7f27758e/include/SDL_keycode.h#l325)  
```
[
    source: 'keyboard',
    type: 'KEYUP' | 'KEYDOWN',
    scancode: 0,
    key: 0,
    modifiers: 0    
]
```

**quit**
```
[
    source: 'quit',
    type: 'QUIT',
]
```

Note: mouse and other events are currently not handled.
