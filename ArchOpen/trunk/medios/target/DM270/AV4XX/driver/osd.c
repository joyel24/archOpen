/*
*   kernel/target/arch_AV4XX/osd.c
*
*   MediOS project
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/

#include <sys_def/stddef.h>

#include <kernel/io.h>
#include <kernel/kernel.h>

#include <driver/hardware.h>
#include <driver/osd.h>
#include <driver/lcd.h>

void arch_osd_init(void)
{
    osd_setMainConfig(0x0000);
	osd_setMainShift(SCREEN_LCD_SHIFT_X, SCREEN_LCD_SHIFT_Y);

// 	outw(9,VIDEO_BASE+0x2C); // LCD VSync
}
