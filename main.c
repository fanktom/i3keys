#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

Display *display;
Window window;
int screen;
int height = 400;
int width;
Colormap colormap;
XColor color_background;
XColor color_focused;
XColor color_focused_text;
XColor color_unfocused;
XColor color_unfocused_text;

// Create an XColor from a hex code such as #1B1D1E
static XColor color_from_hex(const char *code) {
  XColor color;
  XParseColor(display, colormap, code, &color);
  XAllocColor(display, colormap, &color);

  return color;
}

// Expose is called to draw and redraw
static void expose(XEvent *event) {
  XSetForeground(display, DefaultGC(display, screen), color_unfocused.pixel);
  XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 100, 100);
}

// button press is called on touch or click down
static void button_press(XButtonEvent *event) {
  printf("Mouse down in %d, %d\n", event->x, event->y);
}

// button release is called on touch up or release
static void button_release(XButtonEvent *event) {
  printf("Mouse up in %d, %d\n", event->x, event->y);
}

// Main
int main() {

  // Open connection to X server
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  screen = DefaultScreen(display);
  colormap = DefaultColormap(display, 0);

  // Setup Colors
  color_background = color_from_hex("#1B1D1E");
  color_unfocused = color_from_hex("#303030");
  color_unfocused_text = color_from_hex("#ffffff");
  color_focused = color_from_hex("#4c7899");
  color_focused_text = color_from_hex("#4c7899");

  // Width is the full window width
  width = DisplayWidth(display, screen);

  // Create window
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, width, height, 0, None, color_background.pixel);

  // Set window type to DOCK
  Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
  XChangeProperty(display, window, type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&value, 1);

  // Select kind of events we want to receive
  XSelectInput(display, window, ExposureMask | ButtonPressMask | ButtonReleaseMask);

  // Show the window
  XMapWindow(display, window);
  XFlush(display);

  // Event loop
  XEvent event;
  while(1) {
    XNextEvent(display, &event);

    // Draw or redraw window
    if(event.type == Expose) {
      expose(&event);
      continue;
    }
    
    // Handle mouse button press
    if(event.type == ButtonPress) {
      printf("Button press\n");
      button_press((XButtonEvent*)&event);
      continue;
    }
    
    // Handle mouse button release
    if(event.type == ButtonRelease) {
      printf("Button release\n");
      button_release((XButtonEvent*)&event);
      continue;
    }
  }

  // Close connection to X server
  XCloseDisplay(display);

  return 0;
}
