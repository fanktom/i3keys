#include <X11/keysym.h>

// A visual key represents the visual key for rendering of the keyboard layout
typedef struct {
  int row;      // row of key from the top
  float size;   // factor of the width of a key, where 1 is a square key
  int x;        // automatic
  int y;        // automatic
  int width;    // automatic
  int height;   // automatic
  char *text;
  int keycode;
  char *shift_text;
  int shift_keycode;
  int is_pressed;
} i3k_key;

int key_margin = 20;
int key_size = 80;
const char *key_font = "-misc-droid sans-medium-r-normal--0-0-0-0-p-0-iso10646-1";

static i3k_key keys[] = {
  { .row = 0, .size = 1, .text = "Esc", .keycode = XK_Escape },
  { .row = 0, .size = 1, .text = "`", .keycode = XK_quoteleft },
  { .row = 0, .size = 1, .text = "1", .keycode = XK_1 },
  { .row = 0, .size = 1, .text = "2", .keycode = XK_2 },
  { .row = 0, .size = 1, .text = "3", .keycode = XK_3 },
  { .row = 0, .size = 1, .text = "4", .keycode = XK_4 },
  { .row = 0, .size = 1, .text = "5", .keycode = XK_5 },
  { .row = 0, .size = 1, .text = "6", .keycode = XK_6 },
  { .row = 0, .size = 1, .text = "7", .keycode = XK_7 },
  { .row = 0, .size = 1, .text = "8", .keycode = XK_8 },
  { .row = 0, .size = 1, .text = "9", .keycode = XK_9 },
  { .row = 0, .size = 1, .text = "0", .keycode = XK_0 },
  { .row = 0, .size = 1, .text = "-", .keycode = XK_minus },
  { .row = 0, .size = 1, .text = "=", .keycode = XK_equal },
  { .row = 0, .size = 1.5, .text = "Backspace", .keycode = XK_BackSpace },
  
  { .row = 1, .size = 1.5, .text = "Tab",   .keycode = XK_Tab },
  { .row = 1, .size = 1,   .text = "Q",     .keycode = XK_q },
  { .row = 1, .size = 1,   .text = "W",     .keycode = XK_w },
  { .row = 1, .size = 1,   .text = "E",     .keycode = XK_e },
  { .row = 1, .size = 1,   .text = "R",     .keycode = XK_r },
  { .row = 1, .size = 1,   .text = "T",     .keycode = XK_t },
  { .row = 1, .size = 1,   .text = "Y",     .keycode = XK_y },
  { .row = 1, .size = 1,   .text = "U",     .keycode = XK_u },
  { .row = 1, .size = 1,   .text = "I",     .keycode = XK_i },
  { .row = 1, .size = 1,   .text = "O",     .keycode = XK_o },
  { .row = 1, .size = 1,   .text = "P",     .keycode = XK_p },
  { .row = 1, .size = 1,   .text = "[",     .keycode = XK_bracketleft },
  { .row = 1, .size = 1,   .text = "]",     .keycode = XK_bracketright },
  { .row = 1, .size = 2,   .text = "Return", .keycode = XK_Return },
  
  { .row = 2, .size = 1.8, .text = "Shift Lock", .keycode = XK_Shift_Lock },
  { .row = 2, .size = 1, .text = "A", .keycode = XK_a },
  { .row = 2, .size = 1, .text = "S", .keycode = XK_s },
  { .row = 2, .size = 1, .text = "D", .keycode = XK_d },
  { .row = 2, .size = 1, .text = "F", .keycode = XK_f },
  { .row = 2, .size = 1, .text = "G", .keycode = XK_g },
  { .row = 2, .size = 1, .text = "H", .keycode = XK_h },
  { .row = 2, .size = 1, .text = "J", .keycode = XK_j },
  { .row = 2, .size = 1, .text = "K", .keycode = XK_k },
  { .row = 2, .size = 1, .text = "L", .keycode = XK_l },
  { .row = 2, .size = 1, .text = ";", .keycode = XK_semicolon },
  { .row = 2, .size = 1, .text = "'", .keycode = XK_apostrophe },
  { .row = 2, .size = 1, .text = "\\", .keycode = XK_backslash },
  { .row = 2, .size = 1.7, .text = "Return", .keycode = XK_Return },
  
  { .row = 3, .size = 1.5, .text = "Shift", .keycode = XK_Shift_L },
  { .row = 3, .size = 1, .text = "<", .keycode = XK_less },
  { .row = 3, .size = 1, .text = "Z", .keycode = XK_z },
  { .row = 3, .size = 1, .text = "X", .keycode = XK_x },
  { .row = 3, .size = 1, .text = "C", .keycode = XK_c },
  { .row = 3, .size = 1, .text = "V", .keycode = XK_v },
  { .row = 3, .size = 1, .text = "B", .keycode = XK_b },
  { .row = 3, .size = 1, .text = "N", .keycode = XK_n },
  { .row = 3, .size = 1, .text = "M", .keycode = XK_m },
  { .row = 3, .size = 1, .text = ",", .keycode = XK_comma },
  { .row = 3, .size = 1, .text = ".", .keycode = XK_period },
  { .row = 3, .size = 1, .text = "/", .keycode = XK_slash },
  { .row = 3, .size = 1, .text = "Up", .keycode = XK_Up },
  { .row = 3, .size = 2, .text = "Shift", .keycode = XK_Shift_R },
  
  { .row = 4, .size = 1.5, .text = "Ctrl", .keycode = XK_Control_L },
  { .row = 4, .size = 1.5, .text = "Super", .keycode = XK_Super_L },
  { .row = 4, .size = 1.5, .text = "Alt", .keycode = XK_Alt_L },
  { .row = 4, .size = 7, .text = "Space", .keycode = XK_space },
  { .row = 4, .size = 1, .text = "Left", .keycode = XK_Left },
  { .row = 4, .size = 1, .text = "Down", .keycode = XK_Down },
  { .row = 4, .size = 1, .text = "Right", .keycode = XK_Right },
  { .row = 4, .size = 1, .text = "Meta", .keycode = XK_Meta_R },
};
