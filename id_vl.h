// ID_VL.H

// wolf compatability

void Quit (const char *error,...);

//===========================================================================

#define CHARWIDTH		2
#define TILEWIDTH		4

//===========================================================================

extern SDL_Surface *screen, *screenBuffer, *curSurface;

#ifdef _HARMATTAN

#define UPX 192
#define DOWNX 320
#define LEFTX 128
#define RIGHTX 256
#define OPENX 192
#define ESCAPEX 64
#define FIREX 128
#define STRAFEX 256
#define GODMODEX 384
#define CHEATX 448
#define YX 64
#define NX 0
#define PREVX 64
#define NEXTX 128
#define WEAPON1X 320
#define WEAPON2X 384
#define WEAPON3X 448
#define WEAPON4X 0
#define MOVELEFTX 128
#define MOVERIGHTX 256

#define UPY_R 0
#define DOWNY_R 0
#define LEFTY_R 0
#define RIGHTY_R 0
#define OPENY_R 128
#define FIREY_R 128
#define ESCAPEY_R 0
#define STRAFEY_R 128
#define GODMODEY_R 0
#define CHEATY_R 0
#define YY_R 128
#define NY_R 128
#define PREVY_R 256
#define NEXTY_R 256
#define WEAPON1Y_R 128
#define WEAPON2Y_R 128
#define WEAPON3Y_R 128
#define WEAPON4Y_R 256
#define MOVELEFTY_R 0
#define MOVERIGHTY_R 0

#define UPY_P 64
#define DOWNY_P 64
#define LEFTY_P 64
#define RIGHTY_P 64
#define OPENY_P 192
#define FIREY_P 192
#define ESCAPEY_P 64
#define STRAFEY_P 192
#define GODMODEY_P 64
#define CHEATY_P 64
#define YY_P 192
#define NY_P 192
#define PREVY_P 320
#define NEXTY_P 320
#define WEAPON1Y_P 192
#define WEAPON2Y_P 192
#define WEAPON3Y_P 192
#define WEAPON4Y_P 320
#define MOVELEFTY_P 64
#define MOVERIGHTY_P 64

typedef enum {
	YesFunction = 0,
	NoFunction,
	UpFunction,
	DownFunction,
	LeftFunction,
	RightFunction,
	FireFunction,
	EscapeFunction,
	OpenFunction,
	StrafeFunction,
	GodModeFunction,
	CheatFunction,
	/*
	PrevWeaponFunction,
	NextWeaponFunction,
	*/
	Weapon1Function,
	Weapon2Function,
	Weapon3Function,
	Weapon4Function,
	MoveLeftFunction,
	MoveRightFunction,
	TotalFunction
} ButtonFunction;

extern SDL_Surface *vb_surface;
extern SDL_Rect sprite_rects[TotalFunction];
extern SDL_Rect button_sdlrects[TotalFunction];
#ifdef _HARMATTAN
inline void DrawVirtualButtons(int type)
{
	//type == 0 -> all
	//type == 1 -> menu layout
	//type == 2 -> game layout
	ButtonFunction start, end;
	if(type == 1)
	{
		start = YesFunction;
		end = OpenFunction;
	}
	else if(type == 2)
	{
		start = UpFunction;
		end = TotalFunction;
	}
	else
	{
		start = YesFunction;
		end = TotalFunction;
	}
	for(int i = start; i < end; i++)
		SDL_BlitSurface(vb_surface, &sprite_rects[i], screen, &button_sdlrects[i]);
}
#endif

#endif

extern  boolean  fullscreen;
extern  unsigned screenWidth, screenHeight, screenBits, screenPitch, bufferPitch, curPitch;
extern  unsigned scaleFactor;

extern	boolean  screenfaded;
extern	unsigned bordercolor;

extern SDL_Color gamepal[256];

//===========================================================================

//
// VGA hardware routines
//

#define VL_WaitVBL(a) SDL_Delay((a)*8)

void VL_SetVGAPlaneMode (void);
void VL_SetTextMode (void);
void VL_Shutdown (void);

void VL_ConvertPalette(byte *srcpal, SDL_Color *destpal, int numColors);
void VL_FillPalette (int red, int green, int blue);
void VL_SetColor	(int color, int red, int green, int blue);
void VL_GetColor	(int color, int *red, int *green, int *blue);
void VL_SetPalette  (SDL_Color *palette);
void VL_GetPalette  (SDL_Color *palette);
void VL_FadeOut     (int start, int end, int red, int green, int blue, int steps);
void VL_FadeIn      (int start, int end, SDL_Color *palette, int steps);

byte *VL_LockSurface(SDL_Surface *surface);
void VL_UnlockSurface(SDL_Surface *surface);

#define LOCK() VL_LockSurface(curSurface)
#define UNLOCK() VL_UnlockSurface(curSurface)

byte VL_GetPixel        (int x, int y);
void VL_Plot            (int x, int y, int color);
void VL_Hlin            (unsigned x, unsigned y, unsigned width, int color);
void VL_Vlin            (int x, int y, int height, int color);
void VL_BarScaledCoord  (int scx, int scy, int scwidth, int scheight, int color);
void inline VL_Bar      (int x, int y, int width, int height, int color)
{
    VL_BarScaledCoord(scaleFactor*x, scaleFactor*y,
        scaleFactor*width, scaleFactor*height, color);
}
void inline VL_ClearScreen(int color)
{
    SDL_FillRect(curSurface, NULL, color);
}

void VL_MungePic                (byte *source, unsigned width, unsigned height);
void VL_DrawPicBare             (int x, int y, byte *pic, int width, int height);
void VL_MemToLatch              (byte *source, int width, int height,
                                    SDL_Surface *destSurface, int x, int y);
void VL_ScreenToScreen          (SDL_Surface *source, SDL_Surface *dest);
void VL_MemToScreenScaledCoord  (byte *source, int width, int height, int scx, int scy);
void VL_MemToScreenScaledCoord  (byte *source, int origwidth, int origheight, int srcx, int srcy,
                                    int destx, int desty, int width, int height);

void inline VL_MemToScreen (byte *source, int width, int height, int x, int y)
{
    VL_MemToScreenScaledCoord(source, width, height,
        scaleFactor*x, scaleFactor*y);
}

void VL_MaskedToScreen (byte *source, int width, int height, int x, int y);

void VL_LatchToScreenScaledCoord (SDL_Surface *source, int xsrc, int ysrc,
    int width, int height, int scxdest, int scydest);

void inline VL_LatchToScreen (SDL_Surface *source, int xsrc, int ysrc,
    int width, int height, int xdest, int ydest)
{
    VL_LatchToScreenScaledCoord(source,xsrc,ysrc,width,height,
        scaleFactor*xdest,scaleFactor*ydest);
}
void inline VL_LatchToScreenScaledCoord (SDL_Surface *source, int scx, int scy)
{
    VL_LatchToScreenScaledCoord(source,0,0,source->w,source->h,scx,scy);
}
void inline VL_LatchToScreen (SDL_Surface *source, int x, int y)
{
    VL_LatchToScreenScaledCoord(source,0,0,source->w,source->h,
        scaleFactor*x,scaleFactor*y);
}
