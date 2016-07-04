// A visual key represents the visual key for rendering of the keyboard layout
typedef struct {
  int id;       // unique key number used for assigning logical key layout
  int row;      // row of key from the top
  int span;     // span of keys, e.g. space bar
  int x;        // automatic
  int y;        // automatic
  int width;    // automatic
  int height;   // automatic
} i3k_key;

#define MARGIN 20
#define KEYSIZE 80

static i3k_key keys[] = {
  { .id = 1, .row = 0, .span = 1 },
  { .id = 2, .row = 0, .span = 1 },
  { .id = 3, .row = 0, .span = 1 },
  { .id = 4, .row = 0, .span = 1 },
  { .id = 5, .row = 0, .span = 1 },
  { .id = 6, .row = 0, .span = 1 },
  { .id = 7, .row = 1, .span = 1 },
  { .id = 8, .row = 1, .span = 1 },
  { .id = 9, .row = 1, .span = 2 },
  { .id = 10, .row = 1, .span = 1 },
  { .id = 11, .row = 1, .span = 1 },
};
