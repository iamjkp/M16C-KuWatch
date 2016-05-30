#include "../sfr62p.h"
#include "LED.h"

void InitLED() {
	pd8_0 = 1;
	pd7_4 = 1;
	pd7_2 = 1;
	p8_0 = 1;
	p7_4 = 1;
	p7_2 = 1;
}

void OnAllLED() {
	p8_0 = 0;
	p7_4 = 0;
	p7_2 = 0;
}

void OffAllLED() {
	p8_0 = 1;
	p7_4 = 1;
	p7_2 = 1;
}