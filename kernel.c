void printString(char*);

int main()
{
	printString("Yea! Science!\0");

	while(1);
	return 0;
}

void printString(char* x) {
	while(*x) {
		interrupt(0x10, 0xE*256+(*x), 0, 0, 0);
		x++;
	}
}
