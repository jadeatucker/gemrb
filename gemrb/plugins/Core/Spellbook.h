/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /data/gemrb/cvs2svn/gemrb/gemrb/gemrb/plugins/Core/Spellbook.h,v 1.9 2004/12/07 22:51:07 avenger_teambg Exp $
 *
 */

/* Class implementing creature's spellbook and (maybe) spell management */

#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include <vector>
#include "../../includes/win32def.h"
#include "../../includes/ie_types.h"

#ifdef WIN32

#ifdef GEM_BUILD_DLL
#define GEM_EXPORT __declspec(dllexport)
#else
#define GEM_EXPORT __declspec(dllimport)
#endif

#else
#define GEM_EXPORT
#endif

//LearnSpell flags
#define LS_ADDXP  1
#define LS_LEARN  2
#define LS_STATS  4

//LearnSpell return values
#define LSR_OK      0
#define LSR_KNOWN   1  //already knows
#define LSR_INVALID 2  //invalid resref
#define LSR_FAILED  3  //failed stat roll
#define LSR_STAT    4  //insufficient stat (can't learn the spell due to low stat)
#define LSR_LEVEL   5  //insufficient level (low mage, etc level)
#define LSR_FULL    6  //can't learn more spells of this level (due to level)

const int NUM_SPELL_TYPES = 3;

// !!! Keep these synchronized with GUIDefines.py !!!
typedef enum ieSpellType {
	IE_SPELL_TYPE_PRIEST = 0,
	IE_SPELL_TYPE_WIZARD = 1,
	IE_SPELL_TYPE_INNATE = 2
} ieSpellType;

typedef struct CREKnownSpell {
	ieResRef SpellResRef;
	ieWord Level;
	ieWord Type;
} CREKnownSpell;

typedef struct CREMemorizedSpell {
	ieResRef SpellResRef;
	ieDword Flags;
} CREMemorizedSpell;

typedef struct CRECastSpell {    // IWD2 only
	ieDword Type;
	ieDword TotalCount;
	ieDword RemainingCount;
	ieDword Unknown0B;
} CRECastSpell;

typedef struct CRESpellMemorization {
	ieWord  Level;
	ieWord  Number;
	ieWord  Number2;
	ieWord  Type;
	ieDword MemorizedIndex;
	ieDword MemorizedCount;

	std::vector<CREKnownSpell*> known_spells;
	std::vector<CREMemorizedSpell*> memorized_spells;
} CRESpellMemorization;


class GEM_EXPORT Spellbook {
private:
	std::vector<CRESpellMemorization*> spells[3];

public: 
	Spellbook();
	virtual ~Spellbook();

	void FreeSpellPage(CRESpellMemorization* sm);
	bool HaveSpell(const char *resref, ieDword flags);
	bool HaveSpell(int spellid, ieDword flags);

	bool AddSpellMemorization(CRESpellMemorization* sm);
	//int AddMemorizedSpell(CREMemorizedSpell* spell);

	unsigned int GetKnownSpellsCount(int type, unsigned int level);
	bool AddKnownSpell(int type, unsigned int level, CREKnownSpell *spl);
	CREKnownSpell* GetKnownSpell(int type, unsigned int level, unsigned int index);
	unsigned int GetMemorizedSpellsCount(int type, unsigned int level);
	CREMemorizedSpell* GetMemorizedSpell(int type, unsigned int level, unsigned int index);

	int GetMemorizableSpellsCount(ieSpellType type, unsigned int level, bool bonus);
	void SetMemorizableSpellsCount(int Value, ieSpellType type, unsigned int level, bool bonus);

	/** Adds spell from known to memorized */
	bool MemorizeSpell(CREKnownSpell* spl, bool usable);

	/** Removes memorized spell */
	bool UnmemorizeSpell(CREMemorizedSpell* spl);

	/** Sets index'th spell from memorized as 'not-yet-cast' */
	bool ChargeSpell(CREMemorizedSpell* spl);

	/** Sets index'th spell from memorized as 'already-cast' */
	bool DepleteSpell(CREMemorizedSpell* spl);

	void ChargeAllSpells();

	/** Dumps spellbook to stdout */
	void dump();
};

#endif
