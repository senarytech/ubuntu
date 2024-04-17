/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
#ifdef CONFIG_FASTLOGO

#include "io.h"
#include "debug.h"
#include "string.h"
#include "adc_driver.h"

///////////////////////////
//definition of draw struct and draw functions

typedef enum {
	UI_DIALOGUE = 0,
	UI_BUTTON,
	UI_TEXT
}UI_ELEMENT_TYPE;

typedef struct {
	UI_ELEMENT_TYPE type;
	void * element;
}ui_element;

typedef struct {
	int x;
	int y;
}ui_coordinate;  //relative coordinate if it's sub element

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}ui_color;

typedef struct {
	ui_coordinate lt;  //relative coordinate if it's sub element
	ui_color color;
	int bold;
	char * text;
}ui_text;

typedef struct {
	ui_coordinate lt;  //relative coordinate if it's sub element
	ui_coordinate rb;
	ui_color bg_color;
	ui_color border_color;
	ui_color border_focus_color;
	int border_width;
	int is_focus;
	ui_text text;
}ui_button;

typedef struct {
	ui_coordinate lt;//relative coordinate if it's sub element
	ui_coordinate rb;
	ui_color bg_color;
	ui_color border_color;
	int border_width;

	int num;
	ui_element * e;
}ui_dialogue;


extern void gr_buffer_init();
extern void gr_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
extern void gr_clear();
extern void gr_fill(int x1, int y1, int x2, int y2);
extern void gr_text(int x, int y, const char *s, int bold);
extern void gr_flip(void);

void ui_draw_element(ui_coordinate lt, ui_element e);

int ui_can_be_focus(ui_element e)
{
	switch(e.type) {
		case UI_DIALOGUE:
			return 0;
		case UI_BUTTON:
			return 1;
		case UI_TEXT:
			return 0;
		default:
			return 0;
			break;
	};
	return 0;
}

void ui_button_change_focus(ui_button * btn, int focus)
{
	btn->is_focus = focus;
}

void ui_change_focus(ui_element e, int focus)
{
	switch(e.type) {
		case UI_DIALOGUE:
			break;
		case UI_BUTTON:
			ui_button_change_focus((ui_button *)(e.element), focus);
			break;
		case UI_TEXT:
			break;
		default:
			break;
	};
}

void ui_draw_init(void)
{
	gr_buffer_init();
	gr_color(0, 0, 0, 255);
	gr_clear();
}

void ui_draw_text(ui_coordinate lt, ui_text * text) {
	gr_color(text->color.r, text->color.g, text->color.b, text->color.a);
	gr_text((lt.x + text->lt.x), (lt.y + text->lt.y), text->text, text->bold);
}

void ui_draw_button(ui_coordinate lt, ui_button * btn) {
	ui_coordinate next;
	if(btn->border_width > 0) {
		if(btn->is_focus) {
			gr_color(btn->border_focus_color.r, btn->border_focus_color.g,
				btn->border_focus_color.b, btn->border_focus_color.a);
		} else {
			gr_color(btn->border_color.r, btn->border_color.g,
				btn->border_color.b, btn->border_color.a);
		}
		gr_fill((lt.x + btn->lt.x), (lt.y + btn->lt.y), (lt.x + btn->rb.x), (lt.y + btn->rb.y));
	}
	

	gr_color(btn->bg_color.r, btn->bg_color.g, btn->bg_color.b, btn->bg_color.a);
	gr_fill((lt.x + btn->lt.x + btn->border_width), (lt.y + btn->lt.y + btn->border_width),
		(lt.x + btn->rb.x - btn->border_width), (lt.y + btn->rb.y - btn->border_width));

	next.x = lt.x + btn->lt.x;
	next.y = lt.y + btn->lt.y;
	ui_draw_text(next, &(btn->text));
}

void ui_draw_dialogue(ui_coordinate lt, ui_dialogue * d)
{
	int i = 0;
	ui_coordinate next;
	if(d->border_width > 0) {
		gr_color(d->border_color.r, d->border_color.g, d->border_color.b, d->border_color.a);
		gr_fill((lt.x + d->lt.x), (lt.y + d->lt.y), (lt.x + d->rb.x), (lt.y + d->rb.y));
	}

	gr_color(d->bg_color.r, d->bg_color.g, d->bg_color.b, d->bg_color.a);
	gr_fill((lt.x + d->lt.x + d->border_width), (lt.y + d->lt.y + d->border_width),
		(lt.x + d->rb.x - d->border_width), (lt.y + d->rb.y - d->border_width));

	next.x = lt.x + d->lt.x;
	next.y = lt.y + d->lt.y;
	for(i = 0; i < d->num; i++) {
		ui_draw_element(next, d->e[i]);
	}
}

void ui_draw_element(ui_coordinate lt, ui_element e)
{
	switch(e.type) {
		case UI_DIALOGUE:
			ui_draw_dialogue(lt, (ui_dialogue *)(e.element));
			break;
		case UI_BUTTON:
			ui_draw_button(lt, (ui_button *)(e.element));
			break;
		case UI_TEXT:
			ui_draw_text(lt, (ui_text *)(e.element));
			break;
		default:
			dbg_printf(PRN_ERR,"unsupport ui type %d\n", e.type);
			break;
	};
}

void ui_draw_done(void)
{
	gr_flip();
}

///////////////////////////////////////////////////////
//draw verified boot dialogue

#define R_DIALOGUE 240
#define G_DIALOGUE 240
#define B_DIALOGUE 240
#define A_DIALOGUE 255   //alpha


#define R_BUTTON_NORMAL 128	//230
#define G_BUTTON_NORMAL 128	//230
#define B_BUTTON_NORMAL 105	//230
#define A_BUTTON_NORMAL 128   //alpha


#define R_BUTTON_FOCUS 255
#define G_BUTTON_FOCUS 255
#define B_BUTTON_FOCUS 255
#define A_BUTTON_FOCUS 255   //alpha

//#define R_BUTTON_FOCUS 178
//#define G_BUTTON_FOCUS 211
//#define B_BUTTON_FOCUS 244
//#define A_BUTTON_FOCUS 255   //alpha

#define R_TEXT 0
#define G_TEXT 0
#define B_TEXT 0
#define A_TEXT 255   //alpha


typedef enum {
	MB_NULL = 0x00,     //only text
	MB_YESNO = 0x01,    //one "YES" button, on "NO" button
}MB_TYPE;

#define ID_YES 0x1
#define ID_NO 0x2

static ui_dialogue dialogue;
static ui_text message;
static ui_button confirm;
static ui_button cancle;
static int focus_item_num = 2;
static int focus_index = ID_YES;
static char msg[64];
static char msg_confirm[] = "YES";
static char msg_cancle[] = "NO";

int message_box_return(void)
{
	return focus_index;
}

ui_dialogue * message_box_init(MB_TYPE type, unsigned char state, char * text)
{
	static ui_element e[] = {
			{UI_TEXT, (void *)(&message)},
			{UI_BUTTON, (void *)(&confirm)},
			{UI_BUTTON, (void *)(&cancle)}
	};
	unsigned char r_dialogue = 240;
	unsigned char g_dialogue = 240;
	unsigned char b_dialogue = 240;

	if(type == MB_NULL) {
		dialogue.num = 1;
	} else if(type == MB_YESNO) {
		dialogue.num = 3;
	}
	dialogue.e = e;

	switch(state){
		case 0x0: //Yellow
			r_dialogue = 0;
			g_dialogue = 255;
			b_dialogue = 255;
			break;
		case 0x1: //Orange
			r_dialogue = 0;
			g_dialogue = 128;
			b_dialogue = 255;
			break;
		case 0x2: //Red
			r_dialogue = 0;
			g_dialogue = 0;
			b_dialogue = 255;
			break;
		case 0x3: //Green
		default:
			r_dialogue = 0;
			g_dialogue = 255;
			b_dialogue = 0;
			break;
	}

	//setup message
	message.lt.x = 24; //96;
	message.lt.y = 56;
	message.color.r = R_TEXT;
	message.color.g = G_TEXT;
	message.color.b = B_TEXT;
	message.color.a = A_TEXT;
	message.bold = 1;
	strcpy(msg, text);
	message.text = msg;

	if(type == MB_YESNO) {
		//setup button
		//1. confirm
		confirm.lt.x = 90;
		confirm.lt.y = 185;
		confirm.rb.x = 180;
		confirm.rb.y = 220;
		confirm.bg_color.r = r_dialogue;  //R_DIALOGUE;
		confirm.bg_color.g = g_dialogue;  //G_DIALOGUE;
		confirm.bg_color.b = b_dialogue;  //B_DIALOGUE;
		confirm.bg_color.a = A_DIALOGUE;
		confirm.border_width = 4;
		confirm.border_color.r = R_BUTTON_NORMAL;
		confirm.border_color.g = G_BUTTON_NORMAL;
		confirm.border_color.b = B_BUTTON_NORMAL;
		confirm.border_color.a = A_BUTTON_NORMAL;
		confirm.border_focus_color.r = R_BUTTON_FOCUS;
		confirm.border_focus_color.g = G_BUTTON_FOCUS;
		confirm.border_focus_color.b = B_BUTTON_FOCUS;
		confirm.border_focus_color.a = A_BUTTON_FOCUS;
		confirm.is_focus = 1;
		confirm.text.bold = 1;
		confirm.text.lt.x = 20;
		confirm.text.lt.y = 12;
		confirm.text.color.r = R_TEXT;
		confirm.text.color.g = G_TEXT;
		confirm.text.color.b = B_TEXT;
		confirm.text.color.a = A_TEXT;
		confirm.text.text= msg_confirm;
		//2. cancle
		cancle.lt.x = 320;
		cancle.lt.y = 185;
		cancle.rb.x = 410;
		cancle.rb.y = 220;
		cancle.bg_color.r = r_dialogue; //R_DIALOGUE;
		cancle.bg_color.g = g_dialogue; //G_DIALOGUE;
		cancle.bg_color.b = b_dialogue; //B_DIALOGUE;
		cancle.bg_color.a = A_DIALOGUE;
		cancle.border_width = 4;
		cancle.border_color.r = R_BUTTON_NORMAL;
		cancle.border_color.g = G_BUTTON_NORMAL;
		cancle.border_color.b = B_BUTTON_NORMAL;
		cancle.border_color.a = A_BUTTON_NORMAL;
		cancle.border_focus_color.r = R_BUTTON_FOCUS;
		cancle.border_focus_color.g = G_BUTTON_FOCUS;
		cancle.border_focus_color.b = B_BUTTON_FOCUS;
		cancle.border_focus_color.a = A_BUTTON_FOCUS;
		cancle.is_focus = 0;
		cancle.text.bold = 1;
		cancle.text.lt.x = 20;
		cancle.text.lt.y = 12;
		cancle.text.color.r = R_TEXT;
		cancle.text.color.g = G_TEXT;
		cancle.text.color.b = B_TEXT;
		cancle.text.color.a = A_TEXT;
		cancle.text.text = msg_cancle;
	}

	//setup dialogue
	dialogue.bg_color.r = r_dialogue; //R_DIALOGUE;
	dialogue.bg_color.g = g_dialogue; //G_DIALOGUE;
	dialogue.bg_color.b = b_dialogue; //B_DIALOGUE;
	dialogue.bg_color.a = A_DIALOGUE;
	dialogue.border_width = 1; //0: no border
	dialogue.lt.x = 0;
	dialogue.lt.y = 0;
	dialogue.rb.x = 500;
	dialogue.rb.y = 250;

	return &dialogue;
}

void message_box_draw(ui_dialogue *d, int focus_change)
{
	ui_coordinate lt;
	int i = 0, count = 0;

	//absolute coordinate in the screen
	lt.x = 390;
	lt.y = 150;
	if(focus_change) {
		focus_index++;
		if(focus_index > focus_item_num) {
			focus_index -= focus_item_num;
		}
		for(i = 0; i < d->num; i++) {
			if(ui_can_be_focus(d->e[i])) {
				count++;
				if(count == focus_index) {
					ui_change_focus(d->e[i], 1);//set to focus
				} else {
					ui_change_focus(d->e[i], 0);//set to unfocus
				}
			}
		}
	}

	ui_draw_init();
	ui_draw_dialogue(lt, d);
	ui_draw_done();
}

#if 1
// Exercises many of the gr_*() functions; useful for testing.


int message_box(MB_TYPE type, unsigned char state, char * text)
{
	int ret = ID_YES;

	ui_dialogue * d = message_box_init(type, state, text);

	message_box_draw(d, 0);

#ifdef CONFIG_ADC
	if(type == MB_YESNO) {
		adc_init();
		while(1) {
			ret = adc_key_detect(60*1000);	//Set the key press timeout duration to 60 seconds
			if(ret == 0x1) {
				dbg_printf(PRN_NOTICE,"verify_boot: short press\n");
			}
			if(ret == 0x2) {
				dbg_printf(PRN_NOTICE,"verify_boot: long press\n");
				ret = message_box_return();
				break;
			}
			if(ret == 0x3){
				dbg_printf(PRN_NOTICE,"verify_boot: timeout press,default to boot!\n");
				ret = ID_YES;
				break;
			}
			message_box_draw(d, 1);
		}
		return ret;
	}
#endif

	return ret;
}
#endif

#endif
