void handleInterrupt21(int, int, int, int);
void printString(char*);
void readString(char*);
int mod(int,int);
int div(int,int);
void readSector(char* , int);

int main() {

  char line[80];
  makeInterrupt21();
  interrupt(0x21,1,line,0,0);
  interrupt(0x21,0,line,0,0);


/*
Task1
printString("Hello World\0");
*/
/*
Task 2
char line[80];
	printString("Enter a line: \0");
	readString(line);
	printString(line);

*/
/*
Task 3
char buffer[512];
readSector(buffer, 30);
printString(buffer);
*/
/* 
Task 4
makeInterrupt21();
interrupt(0x21,0,0,0,0);

*/
while(1);
return 0 ;
=======
  //while(1);
 // return 0;
}

void printString(char* x) {
  while(*x) {
    interrupt(0x10, 0xE*256+(*x), 0, 0, 0);
    x++;
  }
	return ;
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  if(ax == 0) {
    printString(bx);
  } else {
    if(ax == 1) {
      readString(bx);
    } else {
      if(ax == 2) {
        readSector(bx, cx);
      }
      else {
        printString("Error! No such function!\0");
      }
    }
  }
}

void readString(char* a) {
	char current = 0x0;
	int i = 0 ;
	while(current != 0xd) {
		current=interrupt(0x16,0,0,0,0);
		interrupt(0x10, 0xE*256+current, 0, 0, 0);

		if(current != 0x8) {
			*a= current;
			a++;
			i++;

		}
		else {
			interrupt(0x10, 0xE*256+' ', 0, 0, 0);
			interrupt(0x10,0xE*256+0x8,0,0,0);
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
void readSector(char* buffer, int sector){
  int relative = mod(sector,18)+1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13, 2*256+1, buffer,track*256+relative,head*256+0);
	return ;
}

int mod(int a, int b) {
   while(a >= b) {
        a =a-b;
    }
   return a;
}

int div(int a, int b) {
  int q = 0;
  while((q*b) <= a) {
    q++;
  }
  return q-1;
}
