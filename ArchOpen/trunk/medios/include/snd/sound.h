/*
*   MediOS project
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/

#ifndef __SOUND_H
#define __SOUND_H

void sound_prevTrack();
void sound_nextTrack();

void sound_trackEnd();

void sound_init();
void sound_start(void);
void sound_stop(void);

void sound_playFile(char * fName);

#endif


