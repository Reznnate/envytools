/*
 * Copyright (C) 2009-2011 Marcin Kościelnicki <koriakin@0x04.net>
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "dis.h"
#include "dis-intern.h"

int op8len[] = { 1 };
int op16len[] = { 2 };
int op24len[] = { 3 };
int op32len[] = { 4 };
int op40len[] = { 5 };
int op64len[] = { 8 };

static const struct {
	const char *name;
	struct disisa *isa;
} isas[] = {
	"nv50", &nv50_isa_s,
	"nvc0", &nvc0_isa_s,
	"ctx", &ctx_isa_s,
	"fuc", &fuc_isa_s,
	"fµc", &fuc_isa_s,
	"hwsq", &hwsq_isa_s,
	"vp2", &vp2_isa_s,
	"vuc", &vuc_isa_s,
	"vµc", &vuc_isa_s,
	"macro", &macro_isa_s,
	"vp1", &vp1_isa_s,
};

const struct disisa *ed_getisa(const char *name) {
	int i;
	for (i = 0; i < sizeof isas / sizeof *isas; i++)
		if (!strcmp(name, isas[i].name)) {
			struct disisa *isa = isas[i].isa;
			if (!isa->prepdone) {
				isas[i].isa->ed2 = ed2i_read_isa(isas[i].isa->ed2name);
				if (!isas[i].isa->ed2)
					return 0;
				isas[i].isa->vardata = isas[i].isa->ed2->vardata;
				if (isa->prep)
					isa->prep(isa);
				if (!isa->vardata) {
					isa->vardata = vardata_new("empty");
					vardata_validate(isa->vardata);
				}
				isa->prepdone = 1;
			}
			return isa;
		}
	return 0;
};
