/*
*   gio_lcd.cpp
*
*   AV3XX emulator
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/
#include <stdlib.h>
#include <stdio.h>

#include <HW/gio_lcd.h>

GIO_LCD::GIO_LCD(void):gpio_port(GPIO_LCD,"LCD ON/OFF")
{
    state = 0;  /* default => btn not pressed */
}

void GIO_LCD::set_gpio(void)
{
    state = 1;
}

void GIO_LCD::clear_gpio(void)
{
    state = 0;
}

