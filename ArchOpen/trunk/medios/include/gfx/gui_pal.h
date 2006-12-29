int gui_pal[256][3] = {
	/* 16 EGA colors, arranged for direct predefined palette indexing*/
	{ 0  , 0  , 0   },	/* black*/
	{ 0  , 0  , 128 },	/* blue*/
	{ 0  , 128, 0   },	/* green*/
	{ 255, 255, 255 },	/* white used for trsp */
	{ 128, 0  , 0   },	/* red*/  /* COLOR_ACTIVECAPTION A*/
	{ 128, 0  , 128 },	/* magenta*/ /* COLOR_ACTIVECAPTION B*/
	{ 128, 64 , 0   },	/* adjusted brown*/
	{ 192, 192, 192 },	/* ltgray*/
	{ 128, 128, 128 },	/* gray*/
	{ 0  , 0  , 255 },	/* ltblue*/
	{ 0  , 255, 0   },	/* ltgreen*/
	{ 0  , 255, 255 },	/* ltcyan*/
	{ 255, 0  , 0   },	/* ltred*/
	{ 255, 0  , 255 },	/* ltmagenta*/
	{ 255, 255, 0   },	/* yellow*/
	{ 255, 255, 255 },	/* white*/ /* 32 basic windows colors (first 8 are most important)*/	
	{ 32 , 32 , 32  },	/* DKGRAY_BRUSH*/
	{ 128, 128, 0   },	/* non-adjusted brown*/
        { 223, 223, 223 },	/* COLOR_3DLIGHT B*/
	{ 160, 160, 160 }, 	/* COLOR_3DLIGHT C*/
	{ 234, 230, 221 },	/* COLOR_BTNHIGHLIGHT A*/
	{ 213, 204, 187 },	/* COLOR_BTNFACE A*/
	{ 162, 141, 104 },	/* COLOR_BTNSHADOW A*/
	{ 0  , 64 , 128 },	/* COLOR_INACTIVECAPTION C*/
	{ 0  , 128, 255 },
	{ 0  , 255, 128 },
	{ 64 , 0  , 0   },
	{ 64 , 0  , 64  },
	{ 64 , 0  , 128 },
	{ 64 , 128, 128 },
	{ 128, 0  , 64  },
	{ 128, 0  , 255 },
	{ 128, 64 , 64  },
	{ 128, 128, 64  },
	{ 128, 128, 192 },
	{ 128, 128, 255 },
	{ 128, 255, 0   },
	{ 128, 255, 255 },
	{ 164, 200, 240 },
	{ 192, 220, 192 },
	{ 255, 0  , 128 },
	{ 255, 128, 0   },
	{ 255, 128, 192 },
	{ 255, 128, 255 },
	{ 255, 128, 128 },
	{ 255, 255, 128 },
	{ 255, 251, 240 },
	{ 255, 255, 232 }, /* 216 colors spread uniformly across rgb spectrum 8 colors removed that are duplicated above*/
	{ 0x00, 0x00, 0x33 },
	{ 0x00, 0x00, 0x66 },
	{ 0x00, 0x00, 0x99 },
	{ 0x00, 0x00, 0xcc },
	{ 0x33, 0x00, 0x00 },
	{ 0x33, 0x00, 0x33 },
	{ 0x33, 0x00, 0x66 },
	{ 0x33, 0x00, 0x99 },
	{ 0x33, 0x00, 0xcc },
	{ 0x33, 0x00, 0xff },
	{ 0x66, 0x00, 0x00 },
	{ 0x66, 0x00, 0x33 },
	{ 0x66, 0x00, 0x66 },
	{ 0x66, 0x00, 0x99 },
	{ 0x66, 0x00, 0xcc },
	{ 0x66, 0x00, 0xff },
	{ 0x99, 0x00, 0x00 },
	{ 0x99, 0x00, 0x33 },
	{ 0x99, 0x00, 0x66 },
	{ 0x99, 0x00, 0x99 },
	{ 0x99, 0x00, 0xcc },
	{ 0x99, 0x00, 0xff },
	{ 0xcc, 0x00, 0x00 },
	{ 0xcc, 0x00, 0x33 },
	{ 0xcc, 0x00, 0x66 },
	{ 0xcc, 0x00, 0x99 },
	{ 0xcc, 0x00, 0xcc },
	{ 0xcc, 0x00, 0xff },
	{ 0xff, 0x00, 0x33 },
	{ 0xff, 0x00, 0x66 },
	{ 0xff, 0x00, 0x99 },
	{ 0xff, 0x00, 0xcc },
	{ 0x00, 0x33, 0x00 },
	{ 0x00, 0x33, 0x33 },
	{ 0x00, 0x33, 0x66 },
	{ 0x00, 0x33, 0x99 },
	{ 0x00, 0x33, 0xcc },
	{ 0x00, 0x33, 0xff },
	{ 0x33, 0x33, 0x00 },
	{ 0x33, 0x33, 0x33 },
	{ 0x33, 0x33, 0x66 },
	{ 0x33, 0x33, 0x99 },
	{ 0x33, 0x33, 0xcc },
	{ 0x33, 0x33, 0xff },
	{ 0x66, 0x33, 0x00 },
	{ 0x66, 0x33, 0x33 },
	{ 0x66, 0x33, 0x66 },
	{ 0x66, 0x33, 0x99 },
	{ 0x66, 0x33, 0xcc },
	{ 0x66, 0x33, 0xff },
	{ 0x99, 0x33, 0x00 },
	{ 0x99, 0x33, 0x33 },
	{ 0x99, 0x33, 0x66 },
	{ 0x99, 0x33, 0x99 },
	{ 0x99, 0x33, 0xcc },
	{ 0x99, 0x33, 0xff },
	{ 0xcc, 0x33, 0x00 },
	{ 0xcc, 0x33, 0x33 },
	{ 0xcc, 0x33, 0x66 },
	{ 0xcc, 0x33, 0x99 },
	{ 0xcc, 0x33, 0xcc },
	{ 0xcc, 0x33, 0xff },
	{ 0xff, 0x33, 0x00 },
	{ 0xff, 0x33, 0x33 },
	{ 0xff, 0x33, 0x66 },
	{ 0xff, 0x33, 0x99 },
	{ 0xff, 0x33, 0xcc },
	{ 0xff, 0x33, 0xff },
	{ 0x00, 0x66, 0x00 },
	{ 0x00, 0x66, 0x33 },
	{ 0x00, 0x66, 0x66 },
	{ 0x00, 0x66, 0x99 },
	{ 0x00, 0x66, 0xcc },
	{ 0x00, 0x66, 0xff },
	{ 0x33, 0x66, 0x00 },
	{ 0x33, 0x66, 0x33 },
	{ 0x33, 0x66, 0x66 },
	{ 0x33, 0x66, 0x99 },
	{ 0x33, 0x66, 0xcc },
	{ 0x33, 0x66, 0xff },
	{ 0x66, 0x66, 0x00 },
	{ 0x66, 0x66, 0x33 },
	{ 0x66, 0x66, 0x66 },
	{ 0x66, 0x66, 0x99 },
	{ 0x66, 0x66, 0xcc },
	{ 0x66, 0x66, 0xff },
	{ 0x99, 0x66, 0x00 },
	{ 0x99, 0x66, 0x33 },
	{ 0x99, 0x66, 0x66 },
	{ 0x99, 0x66, 0x99 },
	{ 0x99, 0x66, 0xcc },
	{ 0x99, 0x66, 0xff },
	{ 0xcc, 0x66, 0x00 },
	{ 0xcc, 0x66, 0x33 },
	{ 0xcc, 0x66, 0x66 },
	{ 0xcc, 0x66, 0x99 },
	{ 0xcc, 0x66, 0xcc },
	{ 0xcc, 0x66, 0xff },
	{ 0xff, 0x66, 0x00 },
	{ 0xff, 0x66, 0x33 },
	{ 0xff, 0x66, 0x66 },
	{ 0xff, 0x66, 0x99 },
	{ 0xff, 0x66, 0xcc },
	{ 0xff, 0x66, 0xff },
	{ 0x00, 0x99, 0x00 },
	{ 0x00, 0x99, 0x33 },
	{ 0x00, 0x99, 0x66 },
	{ 0x00, 0x99, 0x99 },
	{ 0x00, 0x99, 0xcc },
	{ 0x00, 0x99, 0xff },
	{ 0x33, 0x99, 0x00 },
	{ 0x33, 0x99, 0x33 },
	{ 0x33, 0x99, 0x66 },
	{ 0x33, 0x99, 0x99 },
	{ 0x33, 0x99, 0xcc },
	{ 0x33, 0x99, 0xff },
	{ 0x66, 0x99, 0x00 },
	{ 0x66, 0x99, 0x33 },
	{ 0x66, 0x99, 0x66 },
	{ 0x66, 0x99, 0x99 },
	{ 0x66, 0x99, 0xcc },
	{ 0x66, 0x99, 0xff },
	{ 0x99, 0x99, 0x00 },
	{ 0x99, 0x99, 0x33 },
	{ 0x99, 0x99, 0x66 },
	{ 0x99, 0x99, 0x99 },
	{ 0x99, 0x99, 0xcc },
	{ 0x99, 0x99, 0xff },
	{ 0xcc, 0x99, 0x00 },
	{ 0xcc, 0x99, 0x33 },
	{ 0xcc, 0x99, 0x66 },
	{ 0xcc, 0x99, 0x99 },
	{ 0xcc, 0x99, 0xcc },
	{ 0xcc, 0x99, 0xff },
	{ 0xff, 0x99, 0x00 },
	{ 0xff, 0x99, 0x33 },
	{ 0xff, 0x99, 0x66 },
	{ 0xff, 0x99, 0x99 },
	{ 0xff, 0x99, 0xcc },
	{ 0xff, 0x99, 0xff },
	{ 0x00, 0xcc, 0x00 },
	{ 0x00, 0xcc, 0x33 },
	{ 0x00, 0xcc, 0x66 },
	{ 0x00, 0xcc, 0x99 },
	{ 0x00, 0xcc, 0xcc },
	{ 0x00, 0xcc, 0xff },
	{ 0x33, 0xcc, 0x00 },
	{ 0x33, 0xcc, 0x33 },
	{ 0x33, 0xcc, 0x66 },
	{ 0x33, 0xcc, 0x99 },
	{ 0x33, 0xcc, 0xcc },
	{ 0x33, 0xcc, 0xff },
	{ 0x66, 0xcc, 0x00 },
	{ 0x66, 0xcc, 0x33 },
	{ 0x66, 0xcc, 0x66 },
	{ 0x66, 0xcc, 0x99 },
	{ 0x66, 0xcc, 0xcc },
	{ 0x66, 0xcc, 0xff },
	{ 0x99, 0xcc, 0x00 },
	{ 0x99, 0xcc, 0x33 },
	{ 0x99, 0xcc, 0x66 },
	{ 0x99, 0xcc, 0x99 },
	{ 0x99, 0xcc, 0xcc },
	{ 0x99, 0xcc, 0xff },
	{ 0xcc, 0xcc, 0x00 },
	{ 0xcc, 0xcc, 0x33 },
	{ 0xcc, 0xcc, 0x66 },
	{ 0xcc, 0xcc, 0x99 },
	{ 0xcc, 0xcc, 0xcc },
	{ 0xcc, 0xcc, 0xff },
	{ 0xff, 0xcc, 0x00 },
	{ 0xff, 0xcc, 0x33 },
	{ 0xff, 0xcc, 0x66 },
	{ 0xff, 0xcc, 0x99 },
	{ 0xff, 0xcc, 0xcc },
	{ 0xff, 0xcc, 0xff },
	{ 0x00, 0xff, 0x33 },
	{ 0x00, 0xff, 0x66 },
	{ 0x00, 0xff, 0x99 },
	{ 0x00, 0xff, 0xcc },
	{ 0x33, 0xff, 0x00 },
	{ 0x33, 0xff, 0x33 },
	{ 0x33, 0xff, 0x66 },
	{ 0x33, 0xff, 0x99 },
	{ 0x33, 0xff, 0xcc },
	{ 0x33, 0xff, 0xff },
	{ 0x66, 0xff, 0x00 },
	{ 0x66, 0xff, 0x33 },
	{ 0x66, 0xff, 0x66 },
	{ 0x66, 0xff, 0x99 },
	{ 0x66, 0xff, 0xcc },
	{ 0x66, 0xff, 0xff },
	{ 0x99, 0xff, 0x00 },
	{ 0x99, 0xff, 0x33 },
	{ 0x99, 0xff, 0x66 },
	{ 0x99, 0xff, 0x99 },
	{ 0x99, 0xff, 0xcc },
	{ 0x99, 0xff, 0xff },
	{ 0xcc, 0xff, 0x00 },
	{ 0xcc, 0xff, 0x33 },
	{ 0xcc, 0xff, 0x66 },
	{ 0xcc, 0xff, 0x99 },
	{ 0xcc, 0xff, 0xcc },
	{ 0xcc, 0xff, 0xff },
	{ 0xff, 0xff, 0x33 },
	{ 0xff, 0xff, 0x66 },
	{ 0xff, 0xff, 0x99 },
	{ 0xff, 0xff, 0xcc },
	/*{ 0xff, 0xff, 0xff )*/
};