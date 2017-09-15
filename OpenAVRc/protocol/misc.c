 /*
 **************************************************************************
 *                                                                        *
 *                 ____                ___ _   _____                      *
 *                / __ \___  ___ ___  / _ | | / / _ \____                 *
 *               / /_/ / _ \/ -_) _ \/ __ | |/ / , _/ __/                 *
 *               \____/ .__/\__/_//_/_/ |_|___/_/|_|\__/                  *
 *                   /_/                                                  *
 *                                                                        *
 *              This file is part of the OpenAVRc project.                *
 *                                                                        *
 *                         Based on code(s) named :                       *
 *             OpenTx - https://github.com/opentx/opentx                  *
 *             Deviation - https://www.deviationtx.com/                   *
 *                                                                        *
 *                Only AVR code here for visibility ;-)                   *
 *                                                                        *
 *   OpenAVRc is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by *
 *   the Free Software Foundation, either version 2 of the License, or    *
 *   (at your option) any later version.                                  *
 *                                                                        *
 *   OpenAVRc is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *   GNU General Public License for more details.                         *
 *                                                                        *
 *       License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html          *
 *                                                                        *
 **************************************************************************
*/


#include "../OpenAVRc.h"

extern volatile uint8_t g_sync_count;
static volatile uint32_t msecs = 0;

uint16_t (*timer_callback)(void);
const void * (*PROTO_Cmds)(enum ProtoCmds);


void inline PROTO_Change_Callback(uint16_t (*cb)(void))
{
  /*
   * Change the pointer to the callback function.
   * Should be called from within a protocol callback function.
   * e.g. To go from bind to normal mode.
   * skyartec_bind_cb() calls this ... Change_Callback(skyartec_norm_cb);
  */
  if(! cb) return;
  timer_callback = cb; // timer_callback = pointer to function.
}


void PROTO_Start_Callback(uint16_t half_us, uint16_t (*cb)(void))
{
  if(! cb) return;
  if(! half_us) return;
  timer_callback = cb; // timer_callback = pointer to function.

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
	OCR1A = TCNT1 + half_us;
  }

  TIFR1 |= 1<<OCF1A; // Reset Flag.
  TIMSK1 |= 1<<OCIE1A; // Enable Output Compare A interrupt.
}


void PROTO_Stop_Callback()
{
  TIMSK1 &= ~(1<<OCIE1A); // Disable Output Compare A interrupt.
  TIFR1 |= 1<<OCF1A; // Reset Flag.
  timer_callback = NULL;
}


uint32_t CLOCK_getms()
{
  uint32_t ms;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    ms = msecs;
  }
  return ms;
}


void CLOCK_delayms(uint32_t delay_ms)
{
  uint32_t start_ms;

  start_ms = msecs;
  while(msecs < (start_ms + delay_ms));
}


void PROTOCOL_SetBindState(uint32_t msec)
{
  uint32_t bind_time;

  if(msec) {
    SpiRFModule.mode = BIND_MODE;
    if (msec == 0xFFFFFFFF) bind_time = msec;
    else bind_time = CLOCK_getms() + msec;
  }
  else SpiRFModule.mode = NORMAL_MODE; // unimod rick added. Can't go from bind to range test.
}
