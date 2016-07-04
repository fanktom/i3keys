#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

Display *display;
Window window;
int screen;
int height = 400;
Colormap colormap;
XColor color_background;
XColor color_foreground;
XColor color_focused;
XColor color_text;

// Create an XColor from a hex code such as #1B1D1E
static XColor color_from_hex(const char *code) {
  XColor color;
  XParseColor(display, colormap, code, &color);
  XAllocColor(display, colormap, &color);

  return color;
}

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
  color_foreground = color_from_hex("#303030");
  color_focused = color_from_hex("#4c7899");
  color_text = color_from_hex("#ffffff");

  // Create window
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, DisplayWidth(display, screen), height, 0, None, color_background.pixel);

  // Set window type to DOCK
  Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
  XChangeProperty(display, window, type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&value, 1);

  // Select kind of events we want to receive
  XSelectInput(display, window, ExposureMask | KeyPressMask);

  // Show the window
  XMapWindow(display, window);
  XFlush(display);

  // Event loop
  XEvent event;
  while(1) {
    XNextEvent(display, &event);

    // Draw or redraw window
    if(event.type == Expose) {
      XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 10, 10);
    }

    if(event.type == KeyPress) {
      break;
    }
  }

  // Close connection to X server
  XCloseDisplay(display);

  return 0;
}
