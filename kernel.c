
void readString(char*);
void printString(char*);
int main()
{


	char line[80];
	printString("Enter a line: \0");
	readString(line);
	printString(line);

	while(1);
	return 0 ;

}
void printString(char* x) {
  	while(*x) {
  		interrupt(0x10, 0xE*256+(*x), 0, 0, 0);
 		
 		x++;
  	}
  }
void readString(char* a)
{

	char current = 0x0;
	int i=0 ;
	while(current != 0xd)
	{
		current=interrupt(0x16,0,0,0,0);

		interrupt(0x10, 0xE*256+current, 0, 0, 0);
		
		
		
		if(current != 0x8) {
			
			*a= current;
			a++;
			i++;

		}
		else {
			interrupt(0x10, 0xE*256+'-', 0, 0, 0);
			if(i!=0)
				{ a-- ;
				i-- ;
				
				}
			
		}
		


	}

	*a = 0xa;
	interrupt(0x10, 0xE*256+0xa, 0, 0, 0);
	a++;
	*a =0x0 ;

	return ;
}
