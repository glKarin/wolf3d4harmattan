//
//	ID Engine
//	ID_IN.c - Input Manager
//	v1.0d1
//	By Jason Blochowiak
//

//
//	This module handles dealing with the various input devices
//
//	Depends on: Memory Mgr (for demo recording), Sound Mgr (for timing stuff),
//				User Mgr (for command line parms)
//
//	Globals:
//		LastScan - The keyboard scan code of the last key pressed
//		LastASCII - The ASCII value of the last key pressed
//	DEBUG - there are more globals
//

#include "wl_def.h"


/*
=============================================================================

					GLOBAL VARIABLES

=============================================================================
*/


//
// configuration variables
//
#ifdef _HARMATTAN

typedef struct{
	Rect rect;
	ButtonFunction function;
	int pressedY;
	int releasedY;
	int index;
	int index2;
	int index3;
} Button_rect;

static Button_rect button_rects[TotalFunction] = {
	{
		{
			{N950_W - VB_W - VB_S,
				N950_H - VB_S * 3 - VB_W * 3},
			{N950_W,
				N950_H - VB_S * 3 - VB_W * 2},
		},
		YesFunction,
		YY_P,
		YY_R,
		sc_Y, 0, 0
	},

	{
		{
			{N950_W - VB_W - VB_S,
				N950_H - VB_S - VB_W},
			{N950_W,
				N950_H - VB_S},
		},
		NoFunction,
		NY_P,
		NY_R,
		sc_N, 0, 0
	},

	{
		{
			{VB_S,
				N950_H - VB_S * 3 - VB_W * 3},
			{VB_S * 3 + VB_W * 3,
				N950_H - VB_S * 3 - VB_W * 2},
		},
		UpFunction,
		UPY_P,
		UPY_R,
		di_north, 0, 0//bt_moveforward
	},

	{
		{
			{VB_S,
				N950_H - VB_S - VB_W},
			{VB_S * 3 + VB_W * 3,
				N950_H - VB_S},
		},
		DownFunction,
		DOWNY_P,
		DOWNY_R,
		di_south, 0, 0//bt_movebackward
	},

	{
		{
			{VB_S,
				N950_H - VB_S * 3 - VB_W * 3},
			{VB_S + VB_W,
				N950_H - VB_S},
		},
		LeftFunction,
		LEFTY_P,
		LEFTY_R,
		di_west, 0, 0//bt_turnleft
	},

	{
		{
			{VB_S * 3 + VB_W * 2,
				N950_H - VB_S * 3 - VB_W * 3},
			{VB_S * 3 + VB_W * 3,
				N950_H - VB_S},
		},
		RightFunction,
		RIGHTY_P,
		RIGHTY_R,
		di_east, 0, 0//bt_turnright
	},

	{
		{
			{N950_W - VB_S * 2 - VB_W * 2,
				N950_H - VB_S * 2 - VB_W * 2},
			{N950_W - VB_S * 2 - VB_W,
				N950_H - VB_S * 2 - VB_W},
		},
		FireFunction,
		FIREY_P,
		FIREY_R,
		bt_attack, 0, 0
	},

	{
		{
			{N950_W - VB_W,
				0},
			{N950_W,
				VB_W},
		},
		EscapeFunction,
		ESCAPEY_P,
		ESCAPEY_R,
		sc_Escape, 0, 0
	},

	{
		{
			{N950_W - VB_S * 2 - VB_W * 2,
				N950_H - VB_S * 3 - VB_W * 3},
			{N950_W - VB_S * 2 - VB_W,
				N950_H - VB_S * 3 - VB_W * 2},
		},
		OpenFunction,
		OPENY_P,
		OPENY_R,
		bt_use, 0, 0
	},

	{
		{
			{N950_W - VB_S * 2 - VB_W * 2,
				N950_H - VB_S - VB_W},
			{N950_W - VB_S * 2 - VB_W,
				N950_H - VB_S},
		},
		StrafeFunction,
		STRAFEY_P,
		STRAFEY_R,
		bt_run, 0, 0
	},

	{
		{
			{N950_W / 2 - VB_S - VB_W,
				0},
			{N950_W / 2 - VB_S,
				VB_W},
		},
		GodModeFunction,
		GODMODEY_P,
		GODMODEY_R,
		sc_Tab, sc_G, sc_F10
	},

	{
		{
			{N950_W / 2 + VB_S,
				0},
			{N950_W / 2 + VB_S + VB_W,
				VB_W},
		},
		CheatFunction,
		CHEATY_P,
		CHEATY_R,
		sc_M, sc_L, sc_I
	},

	/*
	{
		{
			{N950_W / 2 - VB_S - VB_W,
				N950_H - VB_S * 2 - VB_W * 2},
			{N950_W / 2 - VB_S,
				N950_H - VB_S - VB_W},
		},
		PrevWeaponFunction,
		PREVY_P,
		PREVY_R,
		bt_prevweapon, 0, 0
	},

	{
		{
			{N950_W / 2 + VB_S,
				N950_H - VB_S * 2 - VB_W * 2},
			{N950_W / 2 + VB_S + VB_W,
				N950_H - VB_S - VB_W},
		},
		NextWeaponFunction,
		NEXTY_P,
		NEXTY_R,
		bt_nextweapon, 0, 0
	},
	*/

	{
		{
			{N950_W / 2 - VB_S / 2 * 3 - VB_W * 2,
				N950_H - VB_S * 2 - VB_W},
			{N950_W / 2 - VB_S / 2 * 3 - VB_W,
				N950_H - VB_S * 2},
		},
		Weapon1Function,
		WEAPON1Y_P,
		WEAPON1Y_R,
		bt_readyknife, 0, 0
	},

	{
		{
			{N950_W / 2 - VB_S / 2 - VB_W,
				N950_H - VB_S * 2 - VB_W},
			{N950_W / 2 - VB_S / 2,
				N950_H - VB_S * 2},
		},
		Weapon2Function,
		WEAPON2Y_P,
		WEAPON2Y_R,
		bt_readypistol, 0, 0
	},

	{
		{
			{N950_W / 2 + VB_S / 2,
				N950_H - VB_S * 2 - VB_W},
			{N950_W / 2 + VB_S / 2 + VB_W,
				N950_H - VB_S * 2},
		},
		Weapon3Function,
		WEAPON3Y_P,
		WEAPON3Y_R,
		bt_readymachinegun, 0, 0
	},

	{
		{
			{N950_W / 2 + VB_S / 2 * 3 + VB_W,
				N950_H - VB_S * 2 - VB_W},
			{N950_W / 2 + VB_S / 2 * 3 + VB_W * 2,
				N950_H - VB_S * 2},
		},
		Weapon4Function,
		WEAPON4Y_P,
		WEAPON4Y_R,
		bt_readychaingun, 0, 0
	},

	{
		{
			{N950_W - VB_S * 3 - VB_W * 3,
				N950_H - VB_S * 2 - VB_W * 2},
			{N950_W - VB_S * 3 - VB_W * 2,
				N950_H - VB_S * 2 - VB_W},
		},
		MoveLeftFunction,
		MOVELEFTY_P,
		MOVELEFTY_R,
		bt_strafe, di_west, 0
	},

	{
		{
			{N950_W - VB_S - VB_W,
				N950_H - VB_S * 2 - VB_W * 2},
			{N950_W - VB_S,
				N950_H - VB_S * 2 - VB_W},
		},
		MoveRightFunction,
		MOVERIGHTY_P,
		MOVERIGHTY_R,
		bt_strafe, di_east, 0
	}

};

static inline void VB_EmulateButtonScan(int x, int y, bool pressed, const Button_rect *rect)
{
	if((x >= rect -> rect.ul.x && x <= rect -> rect.lr.x)
			&& (y >= rect -> rect.ul.y && y <= rect -> rect.lr.y))
	{
		sprite_rects[rect -> function].y = pressed ? rect -> pressedY : rect -> releasedY;
		Keyboard[buttonscan[rect -> index]] = pressed;
	}
}

static inline void VB_EmulateDirScan(int x, int y, bool pressed, const Button_rect *rect)
{
	if((x >= rect -> rect.ul.x && x <= rect -> rect.lr.x)
			&& (y >= rect -> rect.ul.y && y <= rect -> rect.lr.y))
	{
		sprite_rects[rect -> function].y = pressed ? rect -> pressedY : rect -> releasedY;
		Keyboard[dirscan[rect -> index]] = pressed;
	}
}

static inline void VB_EmulateScan(int x, int y, bool pressed, const Button_rect *rect)
{
	if((x >= rect -> rect.ul.x && x <= rect -> rect.lr.x)
			&& (y >= rect -> rect.ul.y && y <= rect -> rect.lr.y))
	{
		sprite_rects[rect -> function].y = pressed ? rect -> pressedY : rect -> releasedY;
		if(pressed)
			LastScan = rect -> index;
		else
			LastScan = sc_None;
		Keyboard[rect -> index] = pressed;
	}
}

static inline void VB_EmulateMultiScan(int x, int y, bool pressed, const Button_rect *rect)
{
	if((x >= rect -> rect.ul.x && x <= rect -> rect.lr.x)
			&& (y >= rect -> rect.ul.y && y <= rect -> rect.lr.y))
	{
		sprite_rects[rect -> function].y = pressed ? rect -> pressedY : rect -> releasedY;
		Keyboard[rect -> index] = pressed;
		Keyboard[rect -> index2] = pressed;
		Keyboard[rect -> index3] = pressed;
	}
}

static inline void VB_EmulateModScan(int x, int y, bool pressed, const Button_rect *rect)
{
	if((x >= rect -> rect.ul.x && x <= rect -> rect.lr.x)
			&& (y >= rect -> rect.ul.y && y <= rect -> rect.lr.y))
	{
		sprite_rects[rect -> function].y = pressed ? rect -> pressedY : rect -> releasedY;
		Keyboard[buttonscan[rect -> index]] = pressed;
		Keyboard[dirscan[rect -> index2]] = pressed;
	}
}

static inline void VB_EmulateButtonScanMove(int ox, int oy, int nx, int ny, const Button_rect *rect)
{
	bool oldState = ((ox >= rect -> rect.ul.x && ox <= rect -> rect.lr.x)
			&& (oy >= rect -> rect.ul.y && oy <= rect -> rect.lr.y));
	bool newState = ((nx >= rect -> rect.ul.x && nx <= rect -> rect.lr.x)
			&& (ny >= rect -> rect.ul.y && ny <= rect -> rect.lr.y));
	if(oldState && !newState)
	{
		sprite_rects[rect -> function].y = rect -> releasedY;
		Keyboard[buttonscan[rect -> index]] = false;
	}
	else if(newState && !oldState)
	{
		sprite_rects[rect -> function].y = rect -> pressedY;
		Keyboard[buttonscan[rect -> index]] = true;
	}
}

static inline void VB_EmulateDirScanMove(int ox, int oy, int nx, int ny, const Button_rect *rect)
{
	bool oldState = ((ox >= rect -> rect.ul.x && ox <= rect -> rect.lr.x)
			&& (oy >= rect -> rect.ul.y && oy <= rect -> rect.lr.y));
	bool newState = ((nx >= rect -> rect.ul.x && nx <= rect -> rect.lr.x)
			&& (ny >= rect -> rect.ul.y && ny <= rect -> rect.lr.y));
	if(oldState && !newState)
	{
		sprite_rects[rect -> function].y = rect -> releasedY;
		Keyboard[dirscan[rect -> index]] = false;
	}
	else if(newState && !oldState)
	{
		sprite_rects[rect -> function].y = rect -> pressedY;
		Keyboard[dirscan[rect -> index]] = true;
	}
}

static inline void VB_EmulateScanMove(int ox, int oy, int nx, int ny, const Button_rect *rect)
{
	bool oldState = ((ox >= rect -> rect.ul.x && ox <= rect -> rect.lr.x)
			&& (oy >= rect -> rect.ul.y && oy <= rect -> rect.lr.y));
	bool newState = ((nx >= rect -> rect.ul.x && nx <= rect -> rect.lr.x)
			&& (ny >= rect -> rect.ul.y && ny <= rect -> rect.lr.y));
	if(oldState && !newState)
	{
		sprite_rects[rect -> function].y = rect -> releasedY;
		LastScan = sc_None;
		Keyboard[rect -> index] = false;
	}
	else if(newState && !oldState)
	{
		sprite_rects[rect -> function].y = rect -> pressedY;
		LastScan = rect -> index;
		Keyboard[rect -> index] = true;
	}
}

static inline void VB_EmulateMultiScanMove(int ox, int oy, int nx, int ny, const Button_rect *rect)
{
	bool oldState = ((ox >= rect -> rect.ul.x && ox <= rect -> rect.lr.x)
			&& (oy >= rect -> rect.ul.y && oy <= rect -> rect.lr.y));
	bool newState = ((nx >= rect -> rect.ul.x && nx <= rect -> rect.lr.x)
			&& (ny >= rect -> rect.ul.y && ny <= rect -> rect.lr.y));
	if(oldState && !newState)
	{
		sprite_rects[rect -> function].y = rect -> releasedY;
		Keyboard[rect -> index] = false;
		Keyboard[rect -> index2] = false;
		Keyboard[rect -> index3] = false;
	}
	else if(newState && !oldState)
	{
		sprite_rects[rect -> function].y = rect -> pressedY;
		Keyboard[rect -> index] = true;
		Keyboard[rect -> index2] = true;
		Keyboard[rect -> index3] = true;
	}
}

static inline void VB_EmulateModScanMove(int ox, int oy, int nx, int ny, const Button_rect *rect)
{
	bool oldState = ((ox >= rect -> rect.ul.x && ox <= rect -> rect.lr.x)
			&& (oy >= rect -> rect.ul.y && oy <= rect -> rect.lr.y));
	bool newState = ((nx >= rect -> rect.ul.x && nx <= rect -> rect.lr.x)
			&& (ny >= rect -> rect.ul.y && ny <= rect -> rect.lr.y));
	if(oldState && !newState)
	{
		sprite_rects[rect -> function].y = rect -> releasedY;
		Keyboard[buttonscan[rect -> index]] = false;
		Keyboard[dirscan[rect -> index2]] = false;
	}
	else if(newState && !oldState)
	{
		sprite_rects[rect -> function].y = rect -> pressedY;
		Keyboard[buttonscan[rect -> index]] = true;
		Keyboard[dirscan[rect -> index2]] = true;
	}
}

#endif
boolean MousePresent;


// 	Global variables
volatile boolean    Keyboard[SDLK_LAST];
volatile boolean	Paused;
volatile char		LastASCII;
volatile ScanCode	LastScan;

//KeyboardDef	KbdDefs = {0x1d,0x38,0x47,0x48,0x49,0x4b,0x4d,0x4f,0x50,0x51};
static KeyboardDef KbdDefs = {
    sc_Control,             // button0
    sc_Alt,                 // button1
    sc_Home,                // upleft
    sc_UpArrow,             // up
    sc_PgUp,                // upright
    sc_LeftArrow,           // left
    sc_RightArrow,          // right
    sc_End,                 // downleft
    sc_DownArrow,           // down
    sc_PgDn                 // downright
};

static SDL_Joystick *Joystick;
int JoyNumButtons;
static int JoyNumHats;

static bool GrabInput = false;

/*
=============================================================================

					LOCAL VARIABLES

=============================================================================
*/
byte        ASCIINames[] =		// Unshifted ASCII for scan codes       // TODO: keypad
{
//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,8  ,9  ,0  ,0  ,0  ,13 ,0  ,0  ,	// 0
    0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,27 ,0  ,0  ,0  ,	// 1
	' ',0  ,0  ,0  ,0  ,0  ,0  ,39 ,0  ,0  ,'*','+',',','-','.','/',	// 2
	'0','1','2','3','4','5','6','7','8','9',0  ,';',0  ,'=',0  ,0  ,	// 3
	'`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',	// 4
	'p','q','r','s','t','u','v','w','x','y','z','[',92 ,']',0  ,0  ,	// 5
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 6
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0		// 7
};
byte ShiftNames[] =		// Shifted ASCII for scan codes
{
//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,8  ,9  ,0  ,0  ,0  ,13 ,0  ,0  ,	// 0
    0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,27 ,0  ,0  ,0  ,	// 1
	' ',0  ,0  ,0  ,0  ,0  ,0  ,34 ,0  ,0  ,'*','+','<','_','>','?',	// 2
	')','!','@','#','$','%','^','&','*','(',0  ,':',0  ,'+',0  ,0  ,	// 3
	'~','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',	// 4
	'P','Q','R','S','T','U','V','W','X','Y','Z','{','|','}',0  ,0  ,	// 5
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 6
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0		// 7
};
byte SpecialNames[] =	// ASCII for 0xe0 prefixed codes
{
//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 0
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,13 ,0  ,0  ,0  ,	// 1
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 2
	0  ,0  ,0  ,0  ,0  ,'/',0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 3
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 4
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 5
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,	// 6
	0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0   	// 7
};


static	boolean		IN_Started;

static	Direction	DirTable[] =		// Quick lookup for total direction
{
    dir_NorthWest,	dir_North,	dir_NorthEast,
    dir_West,		dir_None,	dir_East,
    dir_SouthWest,	dir_South,	dir_SouthEast
};


///////////////////////////////////////////////////////////////////////////
//
//	INL_GetMouseButtons() - Gets the status of the mouse buttons from the
//		mouse driver
//
///////////////////////////////////////////////////////////////////////////
static int
INL_GetMouseButtons(void)
{
    int buttons = SDL_GetMouseState(NULL, NULL);
    int middlePressed = buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    int rightPressed = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    buttons &= ~(SDL_BUTTON(SDL_BUTTON_MIDDLE) | SDL_BUTTON(SDL_BUTTON_RIGHT));
    if(middlePressed) buttons |= 1 << 2;
    if(rightPressed) buttons |= 1 << 1;

    return buttons;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_GetJoyDelta() - Returns the relative movement of the specified
//		joystick (from +/-127)
//
///////////////////////////////////////////////////////////////////////////
void IN_GetJoyDelta(int *dx,int *dy)
{
    if(!Joystick)
    {
        *dx = *dy = 0;
        return;
    }

    SDL_JoystickUpdate();
#ifdef _arch_dreamcast
    int x = 0;
    int y = 0;
#else
    int x = SDL_JoystickGetAxis(Joystick, 0) >> 8;
    int y = SDL_JoystickGetAxis(Joystick, 1) >> 8;
#endif

    if(param_joystickhat != -1)
    {
        uint8_t hatState = SDL_JoystickGetHat(Joystick, param_joystickhat);
        if(hatState & SDL_HAT_RIGHT)
            x += 127;
        else if(hatState & SDL_HAT_LEFT)
            x -= 127;
        if(hatState & SDL_HAT_DOWN)
            y += 127;
        else if(hatState & SDL_HAT_UP)
            y -= 127;

        if(x < -128) x = -128;
        else if(x > 127) x = 127;

        if(y < -128) y = -128;
        else if(y > 127) y = 127;
    }

    *dx = x;
    *dy = y;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_GetJoyFineDelta() - Returns the relative movement of the specified
//		joystick without dividing the results by 256 (from +/-127)
//
///////////////////////////////////////////////////////////////////////////
void IN_GetJoyFineDelta(int *dx, int *dy)
{
    if(!Joystick)
    {
        *dx = 0;
        *dy = 0;
        return;
    }

    SDL_JoystickUpdate();
    int x = SDL_JoystickGetAxis(Joystick, 0);
    int y = SDL_JoystickGetAxis(Joystick, 1);

    if(x < -128) x = -128;
    else if(x > 127) x = 127;

    if(y < -128) y = -128;
    else if(y > 127) y = 127;

    *dx = x;
    *dy = y;
}

/*
===================
=
= IN_JoyButtons
=
===================
*/

int IN_JoyButtons()
{
    if(!Joystick) return 0;

    SDL_JoystickUpdate();

    int res = 0;
    for(int i = 0; i < JoyNumButtons && i < 32; i++)
        res |= SDL_JoystickGetButton(Joystick, i) << i;
    return res;
}

boolean IN_JoyPresent()
{
    return Joystick != NULL;
}

static void processEvent(SDL_Event *event)
{
#ifdef _HARMATTAN
	int xoffset = 0;
	int yoffset = 0;
#endif
    switch (event->type)
    {
        // exit if the window is closed
        case SDL_QUIT:
            Quit(NULL);

        // check for keypresses
        case SDL_KEYDOWN:
        {
            if(event->key.keysym.sym==SDLK_SCROLLOCK || event->key.keysym.sym==SDLK_F12)
            {
                GrabInput = !GrabInput;
                SDL_WM_GrabInput(GrabInput ? SDL_GRAB_ON : SDL_GRAB_OFF);
                return;
            }

            LastScan = event->key.keysym.sym;
            SDLMod mod = SDL_GetModState();
            if(Keyboard[sc_Alt])
            {
                if(LastScan==SDLK_F4)
                    Quit(NULL);
            }

            if(LastScan == SDLK_KP_ENTER) LastScan = SDLK_RETURN;
            else if(LastScan == SDLK_RSHIFT) LastScan = SDLK_LSHIFT;
            else if(LastScan == SDLK_RALT) LastScan = SDLK_LALT;
            else if(LastScan == SDLK_RCTRL) LastScan = SDLK_LCTRL;
            else if(LastScan == SDLK_COMMA) LastScan = SDLK_ESCAPE;
            else
            {
                if((mod & KMOD_NUM) == 0)
                {
                    switch(LastScan)
                    {
                        case SDLK_KP2: LastScan = SDLK_DOWN; break;
                        case SDLK_KP4: LastScan = SDLK_LEFT; break;
                        case SDLK_KP6: LastScan = SDLK_RIGHT; break;
                        case SDLK_KP8: LastScan = SDLK_UP; break;
                    }
                }
            }

            int sym = LastScan;
            if(sym >= 'a' && sym <= 'z')
                sym -= 32;  // convert to uppercase

            if(mod & (KMOD_SHIFT | KMOD_CAPS))
            {
                if(sym < lengthof(ShiftNames) && ShiftNames[sym])
                    LastASCII = ShiftNames[sym];
            }
            else
            {
                if(sym < lengthof(ASCIINames) && ASCIINames[sym])
                    LastASCII = ASCIINames[sym];
            }
            if(LastScan<SDLK_LAST)
                Keyboard[LastScan] = 1;
            if(LastScan == SDLK_PAUSE)
                Paused = true;
            break;
		}

        case SDL_KEYUP:
        {
            int key = event->key.keysym.sym;
            if(key == SDLK_KP_ENTER) key = SDLK_RETURN;
            else if(key == SDLK_RSHIFT) key = SDLK_LSHIFT;
            else if(key == SDLK_RALT) key = SDLK_LALT;
            else if(key == SDLK_RCTRL) key = SDLK_LCTRL;
            else
            {
                if((SDL_GetModState() & KMOD_NUM) == 0)
                {
                    switch(key)
                    {
                        case SDLK_KP2: key = SDLK_DOWN; break;
                        case SDLK_KP4: key = SDLK_LEFT; break;
                        case SDLK_KP6: key = SDLK_RIGHT; break;
                        case SDLK_KP8: key = SDLK_UP; break;
                    }
                }
            }

            if(key<SDLK_LAST)
                Keyboard[key] = 0;
            break;
        }

#ifdef GP2X
        case SDL_JOYBUTTONDOWN:
            GP2X_ButtonDown(event->jbutton.button);
            break;

        case SDL_JOYBUTTONUP:
            GP2X_ButtonUp(event->jbutton.button);
            break;
#endif

#ifdef _HARMATTAN
				case SDL_MOUSEBUTTONUP:
						VB_EmulateScan(event -> button.x, event -> button.y, false, &button_rects[YesFunction]);
						VB_EmulateScan(event -> button.x, event -> button.y, false, &button_rects[NoFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, false, &button_rects[UpFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, false, &button_rects[DownFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, false, &button_rects[LeftFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, false, &button_rects[RightFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[OpenFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[StrafeFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[FireFunction]);
						VB_EmulateScan(event -> button.x, event -> button.y, false, &button_rects[EscapeFunction]);
						VB_EmulateMultiScan(event -> button.x, event -> button.y, false, &button_rects[GodModeFunction]);
						VB_EmulateMultiScan(event -> button.x, event -> button.y, false, &button_rects[CheatFunction]);
						/*
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[PrevWeaponFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[NextWeaponFunction]);
						*/
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[Weapon1Function]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[Weapon2Function]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[Weapon3Function]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, false, &button_rects[Weapon4Function]);
						VB_EmulateModScan(event -> button.x, event -> button.y, false, &button_rects[MoveLeftFunction]);
						VB_EmulateModScan(event -> button.x, event -> button.y, false, &button_rects[MoveRightFunction]);
						break;
				case SDL_MOUSEBUTTONDOWN:
						VB_EmulateScan(event -> button.x, event -> button.y, true, &button_rects[YesFunction]);
						VB_EmulateScan(event -> button.x, event -> button.y, true, &button_rects[NoFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, true, &button_rects[UpFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, true, &button_rects[DownFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, true, &button_rects[LeftFunction]);
						VB_EmulateDirScan(event -> button.x, event -> button.y, true, &button_rects[RightFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[OpenFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[StrafeFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[FireFunction]);
						VB_EmulateScan(event -> button.x, event -> button.y, true, &button_rects[EscapeFunction]);
						VB_EmulateMultiScan(event -> button.x, event -> button.y, true, &button_rects[GodModeFunction]);
						VB_EmulateMultiScan(event -> button.x, event -> button.y, true, &button_rects[CheatFunction]);
						/*
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[PrevWeaponFunction]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[NextWeaponFunction]);
						*/
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[Weapon1Function]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[Weapon2Function]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[Weapon3Function]);
						VB_EmulateButtonScan(event -> button.x, event -> button.y, true, &button_rects[Weapon4Function]);
						VB_EmulateModScan(event -> button.x, event -> button.y, true, &button_rects[MoveLeftFunction]);
						VB_EmulateModScan(event -> button.x, event -> button.y, true, &button_rects[MoveRightFunction]);
						break;
				case SDL_MOUSEMOTION:
						if(event -> motion.state == SDL_PRESSED)
						{
							xoffset = event -> motion.x - event -> motion.xrel;
							yoffset = event -> motion.y - event -> motion.yrel;
							VB_EmulateScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[YesFunction]);
							VB_EmulateScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[NoFunction]);
							VB_EmulateScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[NoFunction]);
							VB_EmulateDirScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[UpFunction]);
							VB_EmulateDirScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[DownFunction]);
							VB_EmulateDirScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[LeftFunction]);
							VB_EmulateDirScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[RightFunction]);
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[OpenFunction]);
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[StrafeFunction]);
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[FireFunction]);
							VB_EmulateScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[EscapeFunction]);
							VB_EmulateMultiScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[GodModeFunction]);
							VB_EmulateMultiScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[CheatFunction]);
							/*
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[PrevWeaponFunction]);
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[NextWeaponFunction]);
							*/
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[Weapon1Function]);
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[Weapon2Function]);
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[Weapon3Function]);
							VB_EmulateButtonScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[Weapon4Function]);
							VB_EmulateModScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[MoveLeftFunction]);
							VB_EmulateModScanMove(xoffset, yoffset, event -> motion.x, event -> motion.y, &button_rects[MoveRightFunction]);
						}
						break;
#endif
		}
}

void IN_WaitAndProcessEvents()
{
	SDL_Event event;
	if(!SDL_WaitEvent(&event)) return;
	do
	{
		processEvent(&event);
	}
	while(SDL_PollEvent(&event));
}

void IN_ProcessEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		processEvent(&event);
	}
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_Startup() - Starts up the Input Mgr
//
///////////////////////////////////////////////////////////////////////////
	void
IN_Startup(void)
{
	if (IN_Started)
		return;

	IN_ClearKeysDown();

	if(param_joystickindex >= 0 && param_joystickindex < SDL_NumJoysticks())
	{
		Joystick = SDL_JoystickOpen(param_joystickindex);
		if(Joystick)
		{
			JoyNumButtons = SDL_JoystickNumButtons(Joystick);
			if(JoyNumButtons > 32) JoyNumButtons = 32;      // only up to 32 buttons are supported
			JoyNumHats = SDL_JoystickNumHats(Joystick);
			if(param_joystickhat < -1 || param_joystickhat >= JoyNumHats)
				Quit("The joystickhat param must be between 0 and %i!", JoyNumHats - 1);
		}
	}

#ifndef _HARMATTAN
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
#endif

	if(fullscreen)
	{
		GrabInput = true;
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}

	// I didn't find a way to ask libSDL whether a mouse is present, yet...
#if defined(GP2X)
	MousePresent = false;
#elif defined(_arch_dreamcast)
	MousePresent = DC_MousePresent();
#else
	MousePresent = true;
#endif

	IN_Started = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_Shutdown() - Shuts down the Input Mgr
//
///////////////////////////////////////////////////////////////////////////
	void
IN_Shutdown(void)
{
	if (!IN_Started)
		return;

	if(Joystick)
		SDL_JoystickClose(Joystick);

	IN_Started = false;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_ClearKeysDown() - Clears the keyboard array
//
///////////////////////////////////////////////////////////////////////////
	void
IN_ClearKeysDown(void)
{
	LastScan = sc_None;
	LastASCII = key_None;
	memset ((void *) Keyboard,0,sizeof(Keyboard));
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_ReadControl() - Reads the device associated with the specified
//		player and fills in the control info struct
//
///////////////////////////////////////////////////////////////////////////
	void
IN_ReadControl(int player,ControlInfo *info)
{
	word		buttons;
	int			dx,dy;
	Motion		mx,my;

	dx = dy = 0;
	mx = my = motion_None;
	buttons = 0;

	IN_ProcessEvents();

	if (Keyboard[KbdDefs.upleft])
		mx = motion_Left,my = motion_Up;
	else if (Keyboard[KbdDefs.upright])
		mx = motion_Right,my = motion_Up;
	else if (Keyboard[KbdDefs.downleft])
		mx = motion_Left,my = motion_Down;
	else if (Keyboard[KbdDefs.downright])
		mx = motion_Right,my = motion_Down;

	if (Keyboard[KbdDefs.up])
		my = motion_Up;
	else if (Keyboard[KbdDefs.down])
		my = motion_Down;

	if (Keyboard[KbdDefs.left])
		mx = motion_Left;
	else if (Keyboard[KbdDefs.right])
		mx = motion_Right;

	if (Keyboard[KbdDefs.button0])
		buttons += 1 << 0;
	if (Keyboard[KbdDefs.button1])
		buttons += 1 << 1;

	dx = mx * 127;
	dy = my * 127;

	info->x = dx;
	info->xaxis = mx;
	info->y = dy;
	info->yaxis = my;
	info->button0 = (buttons & (1 << 0)) != 0;
	info->button1 = (buttons & (1 << 1)) != 0;
	info->button2 = (buttons & (1 << 2)) != 0;
	info->button3 = (buttons & (1 << 3)) != 0;
	info->dir = DirTable[((my + 1) * 3) + (mx + 1)];
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_WaitForKey() - Waits for a scan code, then clears LastScan and
//		returns the scan code
//
///////////////////////////////////////////////////////////////////////////
	ScanCode
IN_WaitForKey(void)
{
	ScanCode	result;

	while ((result = LastScan)==0)
		IN_WaitAndProcessEvents();
	LastScan = 0;
	return(result);
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_WaitForASCII() - Waits for an ASCII char, then clears LastASCII and
//		returns the ASCII value
//
///////////////////////////////////////////////////////////////////////////
	char
IN_WaitForASCII(void)
{
	char		result;

	while ((result = LastASCII)==0)
		IN_WaitAndProcessEvents();
	LastASCII = '\0';
	return(result);
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_Ack() - waits for a button or key press.  If a button is down, upon
// calling, it must be released for it to be recognized
//
///////////////////////////////////////////////////////////////////////////

boolean	btnstate[NUMBUTTONS];

void IN_StartAck(void)
{
	IN_ProcessEvents();
	//
	// get initial state of everything
	//
	IN_ClearKeysDown();
	memset(btnstate, 0, sizeof(btnstate));

	int buttons = IN_JoyButtons() << 4;

	if(MousePresent)
		buttons |= IN_MouseButtons();

	for(int i = 0; i < NUMBUTTONS; i++, buttons >>= 1)
		if(buttons & 1)
			btnstate[i] = true;
}


boolean IN_CheckAck (void)
{
	IN_ProcessEvents();
	//
	// see if something has been pressed
	//
	if(LastScan)
		return true;

	int buttons = IN_JoyButtons() << 4;

	if(MousePresent)
		buttons |= IN_MouseButtons();

	for(int i = 0; i < NUMBUTTONS; i++, buttons >>= 1)
	{
		if(buttons & 1)
		{
			if(!btnstate[i])
			{
				// Wait until button has been released
				do
				{
					IN_WaitAndProcessEvents();
					buttons = IN_JoyButtons() << 4;

					if(MousePresent)
						buttons |= IN_MouseButtons();
				}
				while(buttons & (1 << i));

				return true;
			}
		}
		else
			btnstate[i] = false;
	}

	return false;
}


void IN_Ack (void)
{
	IN_StartAck ();

	do
	{
		IN_WaitAndProcessEvents();
	}
	while(!IN_CheckAck ());
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_UserInput() - Waits for the specified delay time (in ticks) or the
//		user pressing a key or a mouse button. If the clear flag is set, it
//		then either clears the key or waits for the user to let the mouse
//		button up.
//
///////////////////////////////////////////////////////////////////////////
boolean IN_UserInput(longword delay)
{
	longword	lasttime;

	lasttime = GetTimeCount();
	IN_StartAck ();
	do
	{
		IN_ProcessEvents();
		if (IN_CheckAck())
			return true;
		SDL_Delay(5);
	} while (GetTimeCount() - lasttime < delay);
	return(false);
}

//===========================================================================

/*
	 ===================
	 =
	 = IN_MouseButtons
	 =
	 ===================
	 */
int IN_MouseButtons (void)
{
	if (MousePresent)
		return INL_GetMouseButtons();
	else
		return 0;
}

bool IN_IsInputGrabbed()
{
	return GrabInput;
}

void IN_CenterMouse()
{
	SDL_WarpMouse(screenWidth / 2, screenHeight / 2);
}
