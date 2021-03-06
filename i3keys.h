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
  char *shift_text;
  int keycode;
  int is_pressed;
  int modifier_mask;
} i3k_key;

int key_margin = 20;
int key_size = 80;
const char *key_font = "-misc-droid sans-medium-r-normal--0-0-0-0-p-0-iso10646-1";

static i3k_key keys[] = {
  { .row = 0, .size = 1, .text = "Esc", .shift_text = "Esc", .keycode = XK_Escape },
  { .row = 0, .size = 1, .text = "`", .shift_text = "~", .keycode = XK_quoteleft },
  { .row = 0, .size = 1, .text = "1", .shift_text = "!", .keycode = XK_1 },
  { .row = 0, .size = 1, .text = "2", .shift_text = "@", .keycode = XK_2 },
  { .row = 0, .size = 1, .text = "3", .shift_text = "#", .keycode = XK_3 },
  { .row = 0, .size = 1, .text = "4", .shift_text = "$", .keycode = XK_4 },
  { .row = 0, .size = 1, .text = "5", .shift_text = "%", .keycode = XK_5 },
  { .row = 0, .size = 1, .text = "6", .shift_text = "^", .keycode = XK_6 },
  { .row = 0, .size = 1, .text = "7", .shift_text = "&", .keycode = XK_7 },
  { .row = 0, .size = 1, .text = "8", .shift_text = "*", .keycode = XK_8 },
  { .row = 0, .size = 1, .text = "9", .shift_text = "(", .keycode = XK_9 },
  { .row = 0, .size = 1, .text = "0", .shift_text = ")", .keycode = XK_0 },
  { .row = 0, .size = 1, .text = "-", .shift_text = "_", .keycode = XK_minus },
  { .row = 0, .size = 1, .text = "=", .shift_text = "+", .keycode = XK_equal },
  { .row = 0, .size = 1.5, .text = "Backspace", .shift_text = "Backspace", .keycode = XK_BackSpace },
  
  { .row = 1, .size = 1.5, .text = "Tab",   .shift_text = "Tab", .keycode = XK_Tab },
  { .row = 1, .size = 1,   .text = "q",     .shift_text = "Q", .keycode = XK_q },
  { .row = 1, .size = 1,   .text = "w",     .shift_text = "W", .keycode = XK_w },
  { .row = 1, .size = 1,   .text = "e",     .shift_text = "E", .keycode = XK_e },
  { .row = 1, .size = 1,   .text = "r",     .shift_text = "R", .keycode = XK_r },
  { .row = 1, .size = 1,   .text = "t",     .shift_text = "T", .keycode = XK_t },
  { .row = 1, .size = 1,   .text = "y",     .shift_text = "Y", .keycode = XK_y },
  { .row = 1, .size = 1,   .text = "u",     .shift_text = "U", .keycode = XK_u },
  { .row = 1, .size = 1,   .text = "i",     .shift_text = "I", .keycode = XK_i },
  { .row = 1, .size = 1,   .text = "o",     .shift_text = "O", .keycode = XK_o },
  { .row = 1, .size = 1,   .text = "p",     .shift_text = "P", .keycode = XK_p },
  { .row = 1, .size = 1,   .text = "[",     .shift_text = "{", .keycode = XK_bracketleft },
  { .row = 1, .size = 1,   .text = "]",     .shift_text = "}", .keycode = XK_bracketright },
  { .row = 1, .size = 2,   .text = "Close", .shift_text = "Close", .keycode = XK_Return },
  
  { .row = 2, .size = 1.8, .text = "Shift Lock", .shift_text = "Shift Lock", .keycode = XK_Shift_Lock, .modifier_mask = ShiftMask },
  { .row = 2, .size = 1, .text = "a", .shift_text = "A", .keycode = XK_a },
  { .row = 2, .size = 1, .text = "s", .shift_text = "S", .keycode = XK_s },
  { .row = 2, .size = 1, .text = "d", .shift_text = "D", .keycode = XK_d },
  { .row = 2, .size = 1, .text = "f", .shift_text = "F", .keycode = XK_f },
  { .row = 2, .size = 1, .text = "g", .shift_text = "G", .keycode = XK_g },
  { .row = 2, .size = 1, .text = "h", .shift_text = "H", .keycode = XK_h },
  { .row = 2, .size = 1, .text = "j", .shift_text = "J", .keycode = XK_j },
  { .row = 2, .size = 1, .text = "k", .shift_text = "K", .keycode = XK_k },
  { .row = 2, .size = 1, .text = "l", .shift_text = "L", .keycode = XK_l },
  { .row = 2, .size = 1, .text = ";", .shift_text = ":", .keycode = XK_semicolon },
  { .row = 2, .size = 1, .text = "'", .shift_text = "\"", .keycode = XK_apostrophe },
  { .row = 2, .size = 1, .text = "\\", .shift_text = "|", .keycode = XK_backslash },
  { .row = 2, .size = 1.7, .text = "Return", .shift_text = "Return", .keycode = XK_Return },
  
  { .row = 3, .size = 1.5, .text = "Shift", .shift_text = "Shift", .keycode = XK_Shift_L, .modifier_mask = ShiftMask },
  { .row = 3, .size = 1, .text = "<", .shift_text = ">", .keycode = XK_less },
  { .row = 3, .size = 1, .text = "z", .shift_text = "Z", .keycode = XK_z },
  { .row = 3, .size = 1, .text = "x", .shift_text = "X", .keycode = XK_x },
  { .row = 3, .size = 1, .text = "c", .shift_text = "C", .keycode = XK_c },
  { .row = 3, .size = 1, .text = "v", .shift_text = "V", .keycode = XK_v },
  { .row = 3, .size = 1, .text = "b", .shift_text = "B", .keycode = XK_b },
  { .row = 3, .size = 1, .text = "n", .shift_text = "N", .keycode = XK_n },
  { .row = 3, .size = 1, .text = "m", .shift_text = "M", .keycode = XK_m },
  { .row = 3, .size = 1, .text = ",", .shift_text = "<", .keycode = XK_comma },
  { .row = 3, .size = 1, .text = ".", .shift_text = ">", .keycode = XK_period },
  { .row = 3, .size = 1, .text = "/", .shift_text = "?", .keycode = XK_slash },
  { .row = 3, .size = 1, .text = "XX", .shift_text = "XX", .keycode = XK_Shift_R, .modifier_mask = ShiftMask },
  { .row = 3, .size = 1, .text = "Up", .shift_text = "Up", .keycode = XK_Up },
  { .row = 3, .size = 1, .text = "Shift", .shift_text = "Shift", .keycode = XK_Shift_R, .modifier_mask = ShiftMask },
  
  { .row = 4, .size = 1.5, .text = "Ctrl", .shift_text = "Ctrl", .keycode = XK_Control_L, .modifier_mask = ControlMask },
  { .row = 4, .size = 1.5, .text = "Super", .shift_text = "Super", .keycode = XK_Super_L, .modifier_mask = Mod4Mask },
  { .row = 4, .size = 1.5, .text = "Alt",   .shift_text = "Alt", .keycode = XK_Alt_L, .modifier_mask = Mod1Mask },
  { .row = 4, .size = 7, .text = "Space",   .shift_text = "Space", .keycode = XK_space },
  { .row = 4, .size = 1, .text = "Alt",     .shift_text = "Alt", .keycode = XK_Alt_R, .modifier_mask = Mod1Mask },
  { .row = 4, .size = 1, .text = "Left",    .shift_text = "Left", .keycode = XK_Left },
  { .row = 4, .size = 1, .text = "Down",    .shift_text = "Down", .keycode = XK_Down },
  { .row = 4, .size = 1, .text = "Right",   .shift_text = "Right", .keycode = XK_Right },
};
