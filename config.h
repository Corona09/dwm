/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:style=medium:size=13", "monospace:size=13" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:style=medium:size=13";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_cyan6[]       = "#13c2c2";
static const char col_dust_red6[]   = "#f5222d";
static const char col_valcano6[]    = "#fa541c";
static const char col_daybreak8[]   = "#003eb3";
static const char col_magenta10[]   = "#520339";
static const char col_dracula[]     = "#6272a4";
static const char col_black[]       = "#000000";
static const char col1[]            = "#ffffff";
static const char col2[]            = "#ffffff";
static const char col3[]            = "#ffffff";
static const char col4[]            = "#ffffff";
static const char col5[]            = "#ffffff";
static const char col6[]            = "#ffffff";

enum { SchemeNorm, SchemeSel, SchemeTitle,
	   SchemeCol1, SchemeCol2, SchemeCol3,
	   SchemeCol4, SchemeCol5, SchemeCol6,  }; /* color schemes */

static const char *colors[][3]      = {
	/*                fg          bg          border   */
	[SchemeNorm]  = { col_gray3 , col_gray1 , col_gray2     } ,
	[SchemeSel]   = { col_gray4 , col_cyan6 , col_valcano6  } ,
	[SchemeTitle] = { col_gray3 , col_gray1 , col_gray2     } ,
	[SchemeCol1]  = { col1      , col_gray1 , col_gray2     } ,
	[SchemeCol2]  = { col2      , col_gray1 , col_gray2     } ,
	[SchemeCol3]  = { col3      , col_gray1 , col_gray2     } ,
	[SchemeCol4]  = { col4      , col_gray1 , col_gray2     } ,
	[SchemeCol5]  = { col5      , col_gray1 , col_gray2     } ,
	[SchemeCol6]  = { col6      , col_gray1 , col_gray2     } ,
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                   instance             title          tags mask     isfloating   monitor */
	{ "Gimp"                 , NULL               , NULL         , 0           , 1            , -1 } ,
	{ "fcitx5-config-qt"     , NULL               , NULL         , 0           , 1            , -1 } ,
	{ "Nm-connection-editor" , NULL               , NULL         , 0           , 1            , -1 } ,
	{ "QQ"                   , NULL               , "图片查看器" , 0           , 1            , -1 } ,
	{ "st-float"             , "st"               , NULL         , 0           , 1            , -1 } ,
	{ "Pavucontrol"          , NULL               , NULL         , 0           , 1            , -1 } ,
	{ "firefox"              , "Places"           , NULL         , 0           , 1            , -1 } ,
	{ "falkon"               , "falkon"           , NULL         , 0           , 1            , -1 } ,
	{ "Falkon"               , "Browsing Library" , NULL         , 0           , 1            , -1 } ,
	{ "Zotero"               , NULL               , NULL         , 1 << 5      , 0            , -1 } ,
	{ "ktorrent"             , NULL               , NULL         , 1 << 6      , 0            , -1 } ,
	{ "netease-cloud-music"  , NULL               , NULL         , 1 << 7      , 0            , -1 } ,
	{ "elisa"                , NULL               , NULL         , 1 << 7      , 0            , -1 } ,
	{ "cantata"              , NULL               , NULL         , 1 << 7      , 0            , -1 } ,
	{ "mpv"                  , NULL               , NULL         , 1 << 7      , 0            , -1 } ,
	{ "vlc"                  , NULL               , NULL         , 1 << 7      , 0            , -1 } ,
	{ "KeePassXC"            , NULL               , NULL         , 1 << 8      , 0            , -1 } ,
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "h[]",      htile },   /* horizontal tile */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "{=}",      magicgrid },

};

#define MODKEY Mod1Mask
#define AltMask Mod1Mask
#define CtrlMask ControlMask
#define SuperMask Mod4Mask
#define CAMask (ControlMask|Mod1Mask)
#define CSMask (ControlMask|Mod4Mask)
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, topbar ? NULL : "-b", NULL };
static const char *dwmlauncher[] = { "dwm-launcher", "-fn", dmenufont, topbar ? NULL : "-b", NULL };
static const char *dwmpowermenu[] = { "dwm-powermenu", "-fn", dmenufont, topbar ? NULL : "-b", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *termfloat[]  = { "st", "-c", "st-float", NULL };

static const Key keys[] = {
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termfloat } },
	{ CAMask,                       XK_t,      spawn,          SHCMD("konsole") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_a,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayoutall,   {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayoutall,   {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayoutall,   {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayoutall,   {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_g,      setlayoutall,   {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ AltMask,                      XK_space,  spawn,          {.v = dwmlauncher } },
	{ SuperMask,                    XK_space,  spawn,          {.v = dmenucmd } },
	{ AltMask|ShiftMask,            XK_s,      spawn,          SHCMD("dwm-screenshot fullscreen") },
	{ SuperMask|ShiftMask,          XK_s,      spawn,          SHCMD("dwm-screenshot select") },
	{ AltMask|ShiftMask,            XK_q,      spawn,          {.v = dwmpowermenu} },

	/* 一些功能键 */
	{ 0,       XF86XK_AudioMute,		 spawn,      SHCMD("dwm-volume t") },
	{ 0,       XF86XK_AudioRaiseVolume,  spawn,      SHCMD("dwm-volume u") },
	{ 0,       XF86XK_AudioLowerVolume,  spawn,      SHCMD("dwm-volume d") },
    { 0,       XF86XK_MonBrightnessUp,   spawn,      SHCMD("dwm-backlight u") },
    { 0,       XF86XK_MonBrightnessDown, spawn,      SHCMD("dwm-backlight d") },
	{ AltMask, XF86XK_MonBrightnessUp,   spawn,      SHCMD("dwm-backlight w") }, /* 切换暖色调 */
	{ AltMask, XF86XK_MonBrightnessDown, spawn,      SHCMD("dwm-backlight c") }, /* 切换冷色调 */

	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	// { MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	// { MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	// { MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	// { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
/* Button1 鼠标左键点击 */
/* Button2 鼠标中键点击 */
/* Button3 鼠标右键点击 */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkWinTitle,          0,              Button3,        spawn,          SHCMD("dwm-right-click-title-bar") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	if (!arg)
		return;
	if (!arg->ui) {
		tag(&((Arg) { .ui = ~0 }));
	} else if (arg->ui >=1 && arg->ui <= 9) {
		tag(&((Arg) { .ui = 1 << (arg->ui - 1) }));
	} else {
		return;
	}
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
};
