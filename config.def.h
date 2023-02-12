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
	{ "wemeetapp"            , "wemeetapp"        , NULL         , 1 << 4      , 1            , -1 } ,
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
#define PATH(name) "<path to the folder containing block scripts>/"name

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, topbar ? NULL : "-b", NULL };

static const Key keys[] = {
	/* modifier             key            function         argument */
	{ MODKEY              , XK_Tab       , view           , {0}                 } ,
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
	{ ClkWinTitle,          0,              Button3,        spawn,          SHCMD(PATH("dwm-right-click-title-bar")) },
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
setlayoutallex(const Arg *arg)
{
	setlayoutall(&((Arg) { .v = &layouts[arg->i] }));
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

void tview(const Arg *arg) {
	view(&((Arg) { 0 }));
}

void setgapsex(const Arg *arg) {
	setgaps(&((Arg) { .i = arg->i }));
}

void resetgapex(const Arg *arg) {
	setgaps(&((Arg) { .i = GAP_RESET }));
}

void togglegapex(const Arg *arg) {
	setgaps(&((Arg) { .i = GAP_TOGGLE }));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack"     , focusstack     } , /* 切换聚焦的窗口                */
	{ "setmfact"       , setmfact       } , /* 调整 master 区域比例          */
	{ "togglebar"      , togglebar      } , /* 切换显示/隐藏 bar             */
	{ "incnmaster"     , incnmaster     } , /* 调整 master 窗口数量          */
	{ "togglefloating" , togglefloating } , /* 切换 floating                 */
	{ "zoom"           , zoom           } , /* 切换主从窗口                  */
	{ "viewall"        , viewall        } , /* 显示所有工作区的窗口          */
	{ "viewex"         , viewex         } , /* 切换工作区                    */
	{ "toggleviewex"   , toggleviewex   } , /* 切换显示/隐藏某一工作区       */
	{ "tagex"          , tagex          } , /* 将当前窗口移动到某一工作区    */
	{ "toggletagex"    , toggletagex    } , /* 在某工作区下显示/隐藏当前窗口 */
	{ "killclient"     , killclient     } , /* 关闭窗口                      */
	{ "setlayoutex"    , setlayoutex    } , /* 设置布局                      */
	{ "setlayoutallex" , setlayoutallex } , /* 为所有工作区设置同一布局      */
	{ "view"           , tview          } , /* 切换当前工作区与上一个工作区  */
	{ "setgapsex"      , setgapsex      } , /* 设置 gap                      */
	{ "resetgapex"     , resetgapex     } , /* 重设 gap 值                   */
	{ "togglegapex"    , togglegapex    } , /* 切换 gap                      */

	// { "setlayout"      , setlayout      } ,
	// { "quit"           , quit           } ,
	// { "focusmon"       , focusmon       } ,
	// { "tagmon"         , tagmon         } ,

};
