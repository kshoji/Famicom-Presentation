#define SLIDES_HEIGHT (28)
#define SLIDES_MAX (12)
const unsigned char slides[] = {
#include "map_table.h"
};

const unsigned char bank_for_slide[] = { 0,0,0,0,0,0,0,0,1,1,1,1 };
const unsigned char page_for_slide[] = { 0,0,0,1,1,1,1,1,0,0,0,1 }; 
