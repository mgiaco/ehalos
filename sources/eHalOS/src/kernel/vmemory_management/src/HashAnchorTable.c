#include "HashAnchorTable.h"

// ---

HashAnchorTableEntry HashAnchorTable[MAXHASHANCHORTABLESIZE];

// ---

void initHashAnchorTable()
{
	int32_t i=0;
	for (i=0; i<MAXHASHANCHORTABLESIZE; i++)
		HashAnchorTable[i]=ILLEGALHATENTRY;
}

// hashes ASID and VPN to number of distinct buckets in IPT
int32_t getHashAnchorTableIndex(int32_t ASID, int32_t VPN)
{

	return ((ASID << ASIDSHIFT) OR VPN) % NROFBUCKETS;
}

HashAnchorTableEntry getHATEntry(int32_t hatIndex)
{
	return HashAnchorTable[hatIndex];
}

BOOL setHATEntry(int32_t hatIndex, HashAnchorTableEntry hatEntry)
{
	if (isIllegalHATIndex(hatIndex))
			return FALSE;
	if (isIllegalHATEntry(hatEntry))
		return FALSE;
	HashAnchorTable[hatIndex] = hatEntry;

	return TRUE;
}

BOOL setIllegalHATEntry(int32_t hatIndex)
{
	if (isIllegalHATIndex(hatIndex))
		return FALSE;
	HashAnchorTable[hatIndex] = ILLEGALHATENTRY;

	return TRUE;
}


HashAnchorTableEntry createHATEntry(int iptIndex)
{
	return (HashAnchorTableEntry)iptIndex;
}


BOOL isIllegalHATIndex(int32_t hatIndex)
{
	if (hatIndex == ILLEGALHATINDEX)
		return TRUE;
	return FALSE;
}

BOOL isIllegalHATEntry(HashAnchorTableEntry hatEntry){
	if (hatEntry == ILLEGALHATENTRY)
		return TRUE;
	return FALSE;
}
