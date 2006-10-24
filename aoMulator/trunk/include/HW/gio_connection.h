/*
*   gio_lcd.h
*
*   AV3XX emulator
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/

#ifndef __GIO_CONNECTION_H
#define __GIO_CONNECTION_H

#include <emu.h>
#include <HW/HW_gpio.h>

class GIO_CONN;

#include <HW/HW_connection.h>

class GIO_CONN:public gpio_port {
    public:
        GIO_CONN(HW_connection * hwConn,int num,char * name);

        void set_gpio(void);
        void clear_gpio(void);
        bool is_set(void);

    private:
        HW_connection * hwConn;

};



#endif // __HW_ON_OFF_H
