#include "stdafx.hpp"

extern "C" {
	int __cdecl _purecall() {
		return 0;
	}

	void* memset(void* __restrict pDestination, int iValue, std::size_t iLength) {
		__asm volatile("cld;" "rep;" "stosb;"  : : "D" (pDestination), "a" (iValue), "c"(iLength) : "cc", "memory");
		return pDestination;
	}

	void* memcpy(void* __restrict pDestination, const void* __restrict pSource, std::size_t iLength) {
		__asm volatile("cld;" "rep;" "movsb;" : "+D" (pDestination), "+S"(pSource), "+c"(iLength) : : "memory");
		return pDestination;
	}

	int _fltused __attribute__((used)) = 0;
}