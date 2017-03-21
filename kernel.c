void handleInterrupt21(int, int, int, int);
void printString(char*);
void readString(char*);
int mod(int,int);
int div(int,int);
void readSector(char* , int);
void readFile(char* , char*);

int main() {

  char buffer[13312] ;/*this is the maximum size of a file*/
  makeInterrupt21();
interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
while(1); /*hang up*/

return 0 ;
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
        if(ax==3)
        {
          readFile(bx,cx);
        }
        else {
        printString("Error! No such function!\0");
      }
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
			if(i!=0) {
				a-- ;
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

void readFile(char* x , char* y ){

  int out =0;
  char buffer [512];
  char read [512];

  char six [6];
  char* psix = six;
  char* pbuffer = buffer;
  int n =0;
  int nn = 0;
  int n1 =0;
  int counter = pbuffer;
  readSector(buffer,2);
//  printString(buffer);
  while(pbuffer<counter+512)
  {
    n =0;
    n1 =0;
    while(*pbuffer && n<6)
    {
      *psix =*pbuffer;
      n= n+1;
      psix = psix+1;
      pbuffer++;
    }

    // check
    psix = psix-6 ;

    //printString(six);
    while(*psix && *x)
    {
      if(*x != *psix)
      {
        n1 = 1;

      }
      psix = psix +1;
      x = x+1;

    }
    psix = psix-6 ;

    if(n1 == 1)
    {
      pbuffer+=26;
      x = x-6;
    }
    else
    {
        x = x-6;
        out =1;
        break;
    }
    // printString(six);
  }

  if(out ==0) return;

  nn = 0;
  while(nn<26)
  {
    if(*pbuffer ==0){break;}
    else
    {
      readSector(y , (int)*pbuffer);
    //  printString(buffer)
      y = y+512;
      pbuffer++;
    }
    nn+=1;
  }
  //printString(y);
}
