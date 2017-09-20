/***************************************************************************
 *                                                                         *
 *          ###########   ###########   ##########    ##########           *
 *         ############  ############  ############  ############          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ###########   ####  ######  ##   ##   ##  ##    ######          *
 *          ###########  ####  #       ##   ##   ##  ##    #    #          *
 *                   ##  ##    ######  ##   ##   ##  ##    #    #          *
 *                   ##  ##    #       ##   ##   ##  ##    #    #          *
 *         ############  ##### ######  ##   ##   ##  ##### ######          *
 *         ###########    ###########  ##   ##   ##   ##########           *
 *                                                                         *
 *            S E C U R E   M O B I L E   N E T W O R K I N G              *
 *                                                                         *
 * This file is part of NexMon.                                            *
 *                                                                         *
 * Copyright (c) 2016 NexMon Team                                          *
 *                                                                         *
 * NexMon is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * NexMon is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with NexMon. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 **************************************************************************/

__attribute__((naked))
unsigned int
get_SP(void)
{
    asm(
        "mov r0, sp\n"
        "j [blink]\n"
    );
}

__attribute__((naked,noinline))
unsigned int
get_IDENTITY(void)
{
    asm(
        "lr r0, [identity]\n"
        "j [blink]\n"
    );
}

__attribute__((naked,noinline))
unsigned int
get_DEBUG(void)
{
    asm(
        "lr r0, [debug]\n"
        "j [blink]\n"
    );
}

__attribute__((naked,noinline))
unsigned int
get_PC(void)
{
    asm(
        "lr r0, [pc]\n"
        "j [blink]\n"
    );
}

__attribute__((naked,noinline))
unsigned int
get_STATUS32(void)
{
    asm(
        "lr r0, [status32]\n"
        "j [blink]\n"
    );
}
