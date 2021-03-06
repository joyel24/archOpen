/*
* kernel/target/gfx/gui/main-menu.c
*
* MediOS project
* Copyright (c) 2005 by Christophe THOMAS
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*
*/

#include <sys_def/colordef.h>
#include <sys_def/font.h>
#include <sys_def/stddef.h>

#include <driver/lcd.h>

#include <gui/status_line.h>

struct statusLine_data lineData = {
    x:0,y:0,
    w:SCREEN_REAL_WIDTH,
    bg_color:COLOR_LIGHT_BLUE,
    bat_x:410,
    bat_y:2,
    clk_x:440,
    clk_y:3,
    clk_color:COLOR_BLACK,
    has_date:0,
    logo_x:2,
    logo_y:2,
    ver_x:54,
    ver_y:5,
    ver_color:COLOR_DARK_BLUE,
    module_x:338,
    module_y:4,
    pwr_x:371,
    pwr_y:4,
    usb_x:390,
    usb_y:4,
    intHP_x:358,
    intHP_y:2
};
