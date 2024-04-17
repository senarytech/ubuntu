/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright � 2013-2018 Synaptics Incorporated. All rights reserved.
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
#ifndef __COMMON_IR_KEY_H__
#define __COMMON_IR_KEY_H__

/*
 * IR key definition
 */
typedef enum {
	MV_IR_KEY_NULL = 0, /* no key */

	MV_IR_KEY_POWER = 0x0200,
	MV_IR_KEY_OPENCLOSE,
	MV_IR_KEY_BEGIN_PAIRING,
	MV_IR_KEY_RECOVERY,				/* special reboot to the recovery mode */

	/* digital keys */
	MV_IR_KEY_DIGIT_1 = 0x1000,
	MV_IR_KEY_DIGIT_2,
	MV_IR_KEY_DIGIT_3,
	MV_IR_KEY_DIGIT_4,
	MV_IR_KEY_DIGIT_5,
	MV_IR_KEY_DIGIT_6,
	MV_IR_KEY_DIGIT_7,
	MV_IR_KEY_DIGIT_8,
	MV_IR_KEY_DIGIT_9,
	MV_IR_KEY_DIGIT_0,

	/* for BD */
	MV_IR_KEY_INFO = 0x2000,
	MV_IR_KEY_SETUPMENU,
	MV_IR_KEY_CANCEL, /* no key */

	MV_IR_KEY_DISCMENU,
	MV_IR_KEY_TITLEMENU,
	MV_IR_KEY_SUBTITLE,
	MV_IR_KEY_ANGLE,
	MV_IR_KEY_AUDIO,
	MV_IR_KEY_SEARCH,
	MV_IR_KEY_ZOOM,
	MV_IR_KEY_DISPLAY,

	MV_IR_KEY_REPEAT,
	MV_IR_KEY_REPEAT_AB,
	MV_IR_KEY_PIP,
	MV_IR_KEY_EXIT,
#define MV_IR_KEY_RED		MV_IR_KEY_A
#define MV_IR_KEY_GREEN		MV_IR_KEY_B
#define MV_IR_KEY_YELLOW	MV_IR_KEY_C
#define MV_IR_KEY_BLUE		MV_IR_KEY_D
	MV_IR_KEY_A,
	MV_IR_KEY_B,
	MV_IR_KEY_C,
	MV_IR_KEY_D,

	/* IR misc around ENTER */
	MV_IR_KEY_CLEAR = 0x3000,
	MV_IR_KEY_MEDIA_MODE,
	MV_IR_KEY_STEP,
	MV_IR_KEY_RETURN,
	MV_IR_KEY_MODE, /* for some vendor */

	/* up down left right enter */
	MV_IR_KEY_UP = 0x4000,
	MV_IR_KEY_DOWN,
	MV_IR_KEY_LEFT,
	MV_IR_KEY_RIGHT,
	MV_IR_KEY_ENTER,

	/* for BD */
	MV_IR_KEY_SLOW,
	MV_IR_KEY_PAUSE,
	MV_IR_KEY_PLAY,
	MV_IR_KEY_STOP,
	MV_IR_KEY_PLAY_PAUSE, /* no key */

	MV_IR_KEY_SKIP_BACKWARD,
	MV_IR_KEY_SKIP_FORWARD,
	MV_IR_KEY_SLOW_BACKWARD, /* no key */
	MV_IR_KEY_SLOW_FORWARD,  /* no key */
	MV_IR_KEY_FAST_BACKWARD,
	MV_IR_KEY_FAST_FORWARD,

	/* bottom keys */
	MV_IR_KEY_F1 = 0x5000,
	MV_IR_KEY_F2,
	MV_IR_KEY_F3,
	MV_IR_KEY_F4,
	MV_IR_KEY_F5,
	MV_IR_KEY_F6,
	MV_IR_KEY_F7,
	MV_IR_KEY_F8,

	/* for future */
	MV_IR_KEY_VOL_PLUS = 0x6000, /* no key */
	MV_IR_KEY_VOL_MINUS, /* no key */
	MV_IR_KEY_VOL_MUTE, /* no key */
	MV_IR_KEY_CHANNEL_PLUS, /* no key */
	MV_IR_KEY_CHANNEL_MINUS, /* no key */
	MV_IR_KEY_HOME, /* for some vendor */
	MV_IR_KEY_MUSIC_ID, /* for some vendor */
	MV_IR_KEY_HOMEPAGE, /* for some vendor */

	/* obsoleted keys */
	MV_IR_KEY_MENU,
	MV_IR_KEY_INPUTSEL,
	MV_IR_KEY_ANYNET,
	MV_IR_KEY_TELEVISION,
	MV_IR_KEY_CHANNEL_LIST,
	MV_IR_KEY_TVPOWER,
	MV_IR_KEY_MARKER,
	MV_IR_KEY_VIDEO_FORMAT,
	MV_IR_KEY_GUIDE,
	MV_IR_KEY_VIZIO,
	MV_IR_KEY_LAST,
	MV_IR_KEY_NUMBER_SWITCH,
	MV_IR_KEY_WIDE,
	MV_IR_KEY_RECODER,
	MV_IR_KEY_CLOSEDCAPTION,
	MV_IR_KEY_QUICK,
	MV_IR_KEY_INPUT,
	MV_IR_KEY_AMAZON,
	MV_IR_KEY_NETFLIX,
	MV_IR_KEY_VUDU,
	MV_IR_KEY_DASH,
	MV_IR_KEY_M_GO,
#define MV_IR_KEY_LIVE_TV	MV_IR_KEY_TELEVISION
#define MV_IR_KEY_VIA		MV_IR_KEY_HOME
	MV_IR_KEY_TV_INPUT,
	MV_IR_KEY_LIST,
	MV_IR_KEY_REC,
	MV_IR_KEY_BACK,

	/* qwerty keyboards */
	MV_IR_KB_ESC = 0x8000,
#define MV_IR_KB_1 		MV_IR_KEY_DIGIT_1
#define MV_IR_KB_2 		MV_IR_KEY_DIGIT_2
#define MV_IR_KB_3 		MV_IR_KEY_DIGIT_3
#define MV_IR_KB_4 		MV_IR_KEY_DIGIT_4
#define MV_IR_KB_5 		MV_IR_KEY_DIGIT_5
#define MV_IR_KB_6 		MV_IR_KEY_DIGIT_6
#define MV_IR_KB_7 		MV_IR_KEY_DIGIT_7
#define MV_IR_KB_8 		MV_IR_KEY_DIGIT_8
#define MV_IR_KB_9 		MV_IR_KEY_DIGIT_9
#define MV_IR_KB_0 		MV_IR_KEY_DIGIT_0
	MV_IR_KB_MINUS,
	MV_IR_KB_EQUAL,
	MV_IR_KB_BACKSPACE,
	MV_IR_KB_TAB,
	MV_IR_KB_Q,
	MV_IR_KB_W,
	MV_IR_KB_E,
	MV_IR_KB_R,
	MV_IR_KB_T,
	MV_IR_KB_Y,
	MV_IR_KB_U,
	MV_IR_KB_I,
	MV_IR_KB_O,
	MV_IR_KB_P,
	MV_IR_KB_LEFTBRACE,
	MV_IR_KB_RIGHTBRACE,
#define MV_IR_KB_ENTER		MV_IR_KEY_ENTER
	MV_IR_KB_LEFTCTRL,
	MV_IR_KB_A,
	MV_IR_KB_S,
	MV_IR_KB_D,
	MV_IR_KB_F,
	MV_IR_KB_G,
	MV_IR_KB_H,
	MV_IR_KB_J,
	MV_IR_KB_K,
	MV_IR_KB_L,
	MV_IR_KB_SEMICOLON,
	MV_IR_KB_APOSTROPHE,
	MV_IR_KB_GRAVE,
	MV_IR_KB_LEFTSHIFT,
	MV_IR_KB_BACKSLASH,
	MV_IR_KB_Z,
	MV_IR_KB_X,
	MV_IR_KB_C,
	MV_IR_KB_V,
	MV_IR_KB_B,
	MV_IR_KB_N,
	MV_IR_KB_M,
	MV_IR_KB_COMMA,
	MV_IR_KB_DOT,
	MV_IR_KB_SLASH,
	MV_IR_KB_RIGHTSHIFT,
	MV_IR_KB_LEFTALT,
	MV_IR_KB_SPACE,
	MV_IR_KB_CAPSLOCK,
	MV_IR_KB_F1,
	MV_IR_KB_F2,
	MV_IR_KB_F3,
	MV_IR_KB_F4,
	MV_IR_KB_F5,
	MV_IR_KB_F6,
	MV_IR_KB_F7,
	MV_IR_KB_F8,
	MV_IR_KB_F9,
	MV_IR_KB_F10,
	MV_IR_KB_NUMLOCK,
	MV_IR_KB_SCROLLLOCK,

	MV_IR_KB_ZENKAKUHANKAKU,
	MV_IR_KB_102ND,
	MV_IR_KB_F11,
	MV_IR_KB_F12,
	MV_IR_KB_RO,
	MV_IR_KB_KATAKANA,
	MV_IR_KB_HIRAGANA,
	MV_IR_KB_HENKAN,
	MV_IR_KB_KATAKANAHIRAGANA,
	MV_IR_KB_MUHENKAN,
	MV_IR_KB_RIGHTCTRL,
	MV_IR_KB_SYSRQ,
	MV_IR_KB_RIGHTALT,
	MV_IR_KB_LINEFEED,
#define MV_IR_KB_HOME		MV_IR_KEY_HOME
#define MV_IR_KB_UP		MV_IR_KEY_UP
	MV_IR_KB_PAGEUP,
#define MV_IR_KB_LEFT		MV_IR_KEY_LEFT
#define MV_IR_KB_RIGHT		MV_IR_KEY_RIGHT
	MV_IR_KB_END,
#define MV_IR_KB_DOWN		MV_IR_KEY_DOWN
	MV_IR_KB_PAGEDOWN,
	MV_IR_KB_INSERT,
	MV_IR_KB_DELETE,
	MV_IR_KB_MACRO,
#define MV_IR_KB_MUTE		MV_IR_KEY_MUTE
#define MV_IR_KB_VOLUMEDOWN	MV_IR_KEY_VOL_MINUS
#define MV_IR_KB_VOLUMEUP	MV_IR_KEY_VOL_PLuS
	MV_IR_KB_POWER,
	MV_IR_KB_PAUSE,
	MV_IR_KB_SCALE,

	MV_IR_KB_HANGEUL,
	MV_IR_KB_HANGUEL,
	MV_IR_KB_HANJA,
	MV_IR_KB_YEN,
	MV_IR_KB_LEFTMETA,
	MV_IR_KB_RIGHTMETA,
	MV_IR_KB_COMPOSE,

	/* Keypad keys */
	MV_IR_KB_KP0 = 0x8100,
	MV_IR_KB_KP1,
	MV_IR_KB_KP2,
	MV_IR_KB_KP3,
	MV_IR_KB_KP4,
	MV_IR_KB_KP5,
	MV_IR_KB_KP6,
	MV_IR_KB_KP7,
	MV_IR_KB_KP8,
	MV_IR_KB_KP9,
	MV_IR_KB_KPSLASH,
	MV_IR_KB_KPJPCOMMA,
	MV_IR_KB_KPCOMMA,
	MV_IR_KB_KPASTERISK,
	MV_IR_KB_KPMINUS,
	MV_IR_KB_KPPLUS,
	MV_IR_KB_KPPLUSMINUS,
	MV_IR_KB_KPENTER,
	MV_IR_KB_KPDOT,
	MV_IR_KB_KPEQUAL,
	MV_IR_KB_KPLEFTPAREN,
	MV_IR_KB_KPRIGHTPAREN,

	/* Game PAD */
	MV_IR_BTN_GAMEPAD = 0x8200,
	MV_IR_BTN_A,
	MV_IR_BTN_B,
	MV_IR_BTN_C,
	MV_IR_BTN_X,
	MV_IR_BTN_Y,
	MV_IR_BTN_Z,
	MV_IR_BTN_TL,
	MV_IR_BTN_TR,
	MV_IR_BTN_TL2,
	MV_IR_BTN_TR2,
	MV_IR_BTN_SELECT,
	MV_IR_BTN_START,
	MV_IR_BTN_MODE,
	MV_IR_BTN_THUMBL,
	MV_IR_BTN_THUMBR,

	/* Direction Pad (DPAD) */
	MV_IR_BTN_UP,
	MV_IR_BTN_DOWN,
	MV_IR_BTN_LEFT,
	MV_IR_BTN_RIGHT,

	/* Compose keys */
	MV_IR_KB_UPPER_A = 0x8300,
	MV_IR_KB_UPPER_B,
	MV_IR_KB_UPPER_C,
	MV_IR_KB_UPPER_D,
	MV_IR_KB_UPPER_E,
	MV_IR_KB_UPPER_F,
	MV_IR_KB_UPPER_G,
	MV_IR_KB_UPPER_H,
	MV_IR_KB_UPPER_I,
	MV_IR_KB_UPPER_J,
	MV_IR_KB_UPPER_K,
	MV_IR_KB_UPPER_L,
	MV_IR_KB_UPPER_M,
	MV_IR_KB_UPPER_N,
	MV_IR_KB_UPPER_O,
	MV_IR_KB_UPPER_P,
	MV_IR_KB_UPPER_Q,
	MV_IR_KB_UPPER_R,
	MV_IR_KB_UPPER_S,
	MV_IR_KB_UPPER_T,
	MV_IR_KB_UPPER_U,
	MV_IR_KB_UPPER_V,
	MV_IR_KB_UPPER_W,
	MV_IR_KB_UPPER_X,
	MV_IR_KB_UPPER_Y,
	MV_IR_KB_UPPER_Z,

	MV_IR_KB_TILDE,         // ~
	MV_IR_KB_FACTORIAL,     // !
	MV_IR_KB_AT,            // @
	MV_IR_KB_SHARP,         // #
	MV_IR_KB_DOLLAR,        // $
	MV_IR_KB_PERCENTAGE,    // %
	MV_IR_KB_CARET,         // ^
	MV_IR_KB_AND,           // &
	MV_IR_KB_ASTERISK,      // *
	MV_IR_KB_LEFTRBRACKET,  // (
	MV_IR_KB_RIGHTRBRACKET, // )
	MV_IR_KB_UNDERSCORE,    // _
	MV_IR_KB_PLUS,          // +
	MV_IR_KB_LEFTCBRACKET,  // {
	MV_IR_KB_RIGHTCBRACKET, // }
	MV_IR_KB_OR,            // |
	MV_IR_KB_COLON,         // :
	MV_IR_KB_QUOTATION,     // "
	MV_IR_KB_LEFTABRACKET,  // <
	MV_IR_KB_RIGHTABRACKET, // >
	MV_IR_KB_QUESTION,      // ?

	/* Cotehill-Specific keys */
	MV_IR_KEY_FACTORY_RESET = 0x8400,
	MV_IR_KEY_BLASTER_FACTORY_TEST,

	MV_HARD_KEY_FACTORY_RESET = 0x9400,

	/* Customization Keys */
	MV_IR_KEY_CUSTOM0 = 0xC000,
	MV_IR_KEY_CUSTOM1,
	MV_IR_KEY_CUSTOM2,
	MV_IR_KEY_CUSTOM3,
	MV_IR_KEY_CUSTOM4,
	MV_IR_KEY_CUSTOM5,
	MV_IR_KEY_CUSTOM6,
	MV_IR_KEY_CUSTOM7,
	MV_IR_KEY_CUSTOM8,
	MV_IR_KEY_CUSTOM9,
	MV_IR_KEY_CUSTOM10,
	MV_IR_KEY_CUSTOM11,
	MV_IR_KEY_CUSTOM12,
	MV_IR_KEY_CUSTOM13,
	MV_IR_KEY_CUSTOM14,
	MV_IR_KEY_CUSTOM15,
	MV_IR_KEY_CUSTOM16,
	MV_IR_KEY_CUSTOM17,
	MV_IR_KEY_CUSTOM18,
	MV_IR_KEY_CUSTOM19,
	MV_IR_KEY_CUSTOM20,
	MV_IR_KEY_CUSTOM21,
	MV_IR_KEY_CUSTOM22,
	MV_IR_KEY_CUSTOM23,
	MV_IR_KEY_CUSTOM24,
	MV_IR_KEY_CUSTOM25,
	MV_IR_KEY_CUSTOM26,
	MV_IR_KEY_CUSTOM27,
	MV_IR_KEY_CUSTOM28,
	MV_IR_KEY_CUSTOM29,
	MV_IR_KEY_CUSTOM30,
	MV_IR_KEY_CUSTOM31,

	MV_IR_KEY_SYNC = 0xD000,
	MV_IR_KEY_PICTURE_MODE,
	MV_IR_KEY_RESET,
	MV_IR_KEY_DEMO,

	MV_IR_KEY_MAX

} MV_IR_KEY_CODE_t;

#define MV_IR_HOLDKEY_FLAG		(1 << 31)
#define MV_IR_UPKEY_FLAG		(1 << 27)
#define MV_IR_KEY2UPKEY(key)	((key & (~MV_IR_HOLDKEY_FLAG)) | MV_IR_UPKEY_FLAG)
#define MV_IR_HOLDKEY_KEY(key)	(key | MV_IR_HOLDKEY_FLAG)
#define MV_IR_HOLDKEY2KEY(key)	(key & (~MV_IR_HOLDKEY_FLAG))

#endif
