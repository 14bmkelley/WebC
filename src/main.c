/*
 * main.c
 * The entry point of the web server.
 *
 * Author:   Brandon M. Kelley
 * Date:     May 8, 2016
 * Version:  1.0
 * License:  GNU GPL
 * Copyright (C) 2016 Brandon M. Kelley
 *
 * This file is a part of WebC
 *
 * WebC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WebC.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

/*
 * Runs the web server, prototype of function declared in core.c.
 * Returns:
 *    int result: Exit code of the server
 */
int run_server();

/*
 * Entry point of the program.
 * Params:
 *    int argc: Number of command line arguments
 *    char *argv[]: List of command line arguments
 * Returns:
 *    int result: Exit code
 */
int main(int argc, char *argv[]) {

   /* Run server */
   exit(run_server());

}
