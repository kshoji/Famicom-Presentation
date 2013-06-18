// TODO
// state pattern (normal_mode, select_mode, midi_mode, etc.)
// remove up/down key action in normal state
// voice
// 
// -------------------------------------------

//this example shows how to poll the gamepad
//and how to use nametable update system that allows to modify nametable
//while rendering is enabled

#include "neslib.h"
#include "slide.h"

//these macro are needed to simplify defining update list constants
#define NTADR(x,y)	((0x2000|((y)<<5)|x))

#define SET_RESISTER(x,y) *((unsigned char*)(x)) = (y)

//variables
#define INPUT_FIRST_FRAME (0xff)
static unsigned char current_slide = 0;
static unsigned char last_input = INPUT_FIRST_FRAME;
static unsigned char input = INPUT_FIRST_FRAME;

#define FADE_SPEED (2)
void fadeout() {
	ppu_waitnmi();//wait for next TV frame
	pal_bright(3);
	delay(FADE_SPEED);
	pal_bright(2);
	delay(FADE_SPEED);
	pal_bright(1);
	delay(FADE_SPEED);
	pal_bright(0);
}

void fadein() {
	ppu_waitnmi();//wait for next TV frame
	pal_bright(1);
	delay(FADE_SPEED);
	pal_bright(2);
	delay(FADE_SPEED);
	pal_bright(3);
	delay(FADE_SPEED);
	pal_bright(4);
}

void clear_screen() {
	int a;
	ppu_off();
	vram_adr(NTADR(0, 0));
	for (a = 0; a < 960; a++) {
		vram_put(0);
	}
	ppu_on_all();
}

void beep() {
	int a;
	SET_RESISTER(0x4010, 0x8f);

	for (a = 0; a < 0x4ff; a++) {
		SET_RESISTER(0x4011, a<<4);
	}
}

void set_palette() {
	int a;

	// header
	for (a = 0x23c0; a < 0x23c8; a++) {
		vram_adr(a);
		vram_put(0xaa); // 0x10101010
	}

	// body
	for (a = 0x23c8; a < 0x23f0; a++) {
		vram_adr(a);
		vram_put(0x00);
	}

	// footer
	for (a = 0x23f0; a < 0x2400; a++) {
		vram_adr(a);
		vram_put(0xa0);
	}
}

// for mmc1
void select_chrrom_bank(unsigned char bank) {
	// bank change
	SET_RESISTER(0xA000, 0); // ignored
	SET_RESISTER(0xA000, bank & 0x1);
	SET_RESISTER(0xA000, (bank >> 1) & 0x1);
	SET_RESISTER(0xA000, (bank >> 2) & 0x1);
	SET_RESISTER(0xA000, (bank >> 3) & 0x1);
}

static unsigned char vram_buffer[896]; // 896:32*28

void extract_slide_data(unsigned char slide_index) {
	int i;
	unsigned char slide_num = 0;
	int slide_ptr = 0;
	unsigned char rle_length;
	unsigned char rle_data;
	unsigned char rle_count;

	while(1) {
		if (slide_num == slide_index) {
			break;
		}
		// 0xff: end of slide data
		if (slides[slide_ptr++] == 0xff) {
			slide_num++;
		}
	}

	// fill with 0
	memfill(vram_buffer, 0, 896); // 896:32*28

	for (i = 0; i < 896; i++, slide_ptr++) { // 896:32*28
		// 0xff: end of slide data
		if (slides[slide_ptr] == 0xff) {
			break;
		}

		// 0xfe: rle, next byte: fill length, next2 byte: data
		if (slides[slide_ptr] == 0xfe) {
			slide_ptr++;
			rle_length = slides[slide_ptr];
			slide_ptr++;
			rle_data = slides[slide_ptr];

			for (rle_count = 0; rle_count < rle_length; i++, rle_count++) {
				vram_buffer[i] = rle_data;
			}
			i--;
		} else {
			vram_buffer[i] = slides[slide_ptr];
		}
	}
}

void main() {
	// select bank
	bank_bg(0);

	ppu_on_all();//enable rendering

	// send low bit first
	// vrom mode setup
	// http://wiki.nesdev.com/w/index.php/INES_Mapper_001
	SET_RESISTER(0x8000, 0);// mirror : 0b00 -> 1ScA
	SET_RESISTER(0x8000, 0);
	SET_RESISTER(0x8000, 0);
	SET_RESISTER(0x8000, 0);
	SET_RESISTER(0x8000, 0);// chrrom mode 1 -> 8kb mode (load both of bank0/1)

	// palette for text
	pal_col(1,0x00);
	pal_col(2,0x10);
	pal_col(3,0x20);
	pal_col(4,0x20);
	pal_col(8,0x10);
	pal_col(9,0x10);
	pal_col(10,0x10);
	pal_col(11,0x10);

	//now the main loop
	while (1) {
		//poll the pad and change coordinates according to pressed buttons
		ppu_waitnmi();//wait for next TV frame

		last_input = input;
		input = pad_poll(0);

		if (last_input == INPUT_FIRST_FRAME || ((input != last_input) && input)) {
			if (input & PAD_LEFT ) {
				current_slide --;
			} else if (input & PAD_RIGHT) {
				current_slide ++;
			} else if (input) {
				// ignore other input
				continue;
			}

			// changed key state
			current_slide += SLIDES_MAX;
			current_slide %= SLIDES_MAX;

			beep();

			fadeout();

			ppu_off();

			// palette for background
			pal_col(0, current_slide % 0x0e);

			set_palette();

			// chrrom bank change
			// NOTE: write chrrom data include to crt0.s
		 	select_chrrom_bank(bank_for_slide[current_slide]);
		 	bank_bg(page_for_slide[current_slide]);

			// draw buffers
			extract_slide_data(current_slide);
			vram_write((unsigned char *)vram_buffer, NTADR(0, 1), 32 * SLIDES_HEIGHT);

			ppu_on_all();

			fadein();
		}
	}
}