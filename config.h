/* Copyright (c) 2003, 2004, 2005 Jakub Wilk <ubanus@users.sf.net>
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "stdbool.h"

typedef struct
{
  bool color; // shall we use colors;
  bool html; // shall we print HTML instead of plain text?
  bool stats;
} Config;

extern Config config;

void parse_arguments(int, char**, char**);

#endif

/* vim:set ts=2 sw=2 et: */