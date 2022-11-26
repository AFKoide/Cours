#include <stdlib.h>

int main()
{
	LED_init();

	int value=256;
	int i=0;

	for(i=0; i<8; i=(i+1)%8)
	{
		Led_on(value<<i);
		wait(1000000);
	}

	return 0;

}