/*
*   kernel/target/arch_AV3XX/exit.c
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
#include <sys_def/font.h>
#include <sys_def/colordef.h>

#include <kernel/kernel.h>
#include <kernel/io.h>

#include <init/exit.h>

#include <driver/ata.h>
#include <driver/hardware.h>
#include <driver/lcd.h>

#include <fs/disk.h>

#include <gfx/graphics.h>
#include <gfx/screens.h>

#include <gui/splash.h>

void reset_device(void);

void arch_reload_firmware(void)
{
    printk("about to reboot\n");
    if(disk_rmAll()!=MED_OK)
        printk("WARN: was not able to umount\n");
    if(ata_StopHD(HD_DISK)!=MED_OK)
        printk("WARN: was not able to stop HDD\n");
    reset_device();
}

void arch_HaltMsg(void)
{
    splash_setString(getLangStr(STRLNG_SPLASH_SHUTDOWN));
    screens_show(SCREEN_SPLASH);    
}
