#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

Display *display;
Window window;
int screen;

int main() {

  // Open connection to X server
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  screen = DefaultScreen(display);

  // Create window
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 800, 200, 0, None, WhitePixel(display, screen));

  // Set window type to DOCK
  Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
  XChangeProperty(display, window, type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&value, 1);

  // Select kind of events we want to receive
  XSelectInput(display, window, ExposureMask | KeyPressMask);

  // Show the window
  XMapWindow(display, window);

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
