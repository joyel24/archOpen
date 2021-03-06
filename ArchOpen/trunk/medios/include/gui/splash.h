/*
*   include/gui/splash.h
*
*   MediOS project
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/

#ifndef __SPLASH_H
#define __SPLASH_H

#include <kernel/lang.h>

void splash_init(void);
void splash_show(void);
void splash_setString(char * str);

#endif
