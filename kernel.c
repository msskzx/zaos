void handleInterrupt21(int, int, int, int);
void printString(char*);
void readString(char*);
int mod(int, int);
int div(int, int);
void readSector(char*, int);
void readFile(char*, char*);
void executeProgram(char*, int);

int main()
{

  makeInterrupt21();
  interrupt(0x21, 4, "tstprg\0", 0x2000, 0);

  while(1);
  return 0;
}

void printString(char* x) {
  while(*x && *x != 0) {
    interrupt(0x10, 0xE*256+(*x), 0, 0, 0);
    x++;
  }
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  if(ax == 0)
  {
    printString(bx);
  } else
  {
    if(ax == 1)
    {
      readString(bx);
    } else
    {
      if(ax == 2)
      {
        readSector(bx, cx);
      }
      else
      {
        if(ax==3)
        {
          readFile(bx,cx);
        }
        else
        {
          if(ax == 4)
          {
            executeProgram(bx, cx);
          }
          else
          {
            printString("Error! No such function!\0");
          }
        }
      }
    }
  }
}

void readString(char* a) {
	char current = 0x0;
	int i = 0;
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
			interrupt(0x10, 0xE*256+0x8, 0, 0, 0);
			if(i!= 0)
      {
				a--;
				i--;
			}
		}
	}

	*a = 0xa;
	interrupt(0x10, 0xE*256+0xa, 0, 0, 0);
	a++;
	*a =0x0;
}

void readSector(char* buffer, int sector){
  int relative = mod(sector,18)+1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13, 2*256+1, buffer,track*256+relative,head*256+0);
	return;
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

void readFile(char* x , char* y )
{
  int out =0;
  char buffer [512];
  char read [512];

  char six [6];
  char* psix = six;
  char* pbuffer = buffer;
  int n = 0;
  int sectors = 0;
  int match = 1;
  int counter = pbuffer;

  // read all file names from the directory at sector 2
  readSector(buffer, 2);

  while(pbuffer<counter+512)
  {
    n = 0;
    match = 1;

    // load the current file name
    while(*pbuffer && n<6)
    {
      *psix =*pbuffer;
      n++;
      psix++;
      pbuffer++;
    }

    // point to the beginning
    psix-=6 ;

    // compare
    while(*psix && *x)
    {
      if(*x != *psix)
      {
        match = 0;
      }
      psix++;
      x++;

    }
    // point to the beginning
    psix-=6 ;

    // is it a match
    if(match == 0)
    {
      pbuffer+=26;
      x-=6;
    }
    else
    {
      x-=6;
      out =1;
      break;
    }
  }

  printString("ZERO\0");
  if(out == 0) return;

  printString("ONE\0");
  while(sectors<26)
  {
    printString("TWO\0");
    if(*pbuffer == 0){break;}
    else
    {
      printString("THREE\0");
      readSector(y , (int)*pbuffer);
      printString("FOUR\0");
      y+=512;
      pbuffer++;
    }
    sectors++;
  }

  printString("FIVE\0");

}


void executeProgram(char* name, int segment)
{
  char* content;
  char* pointer_content;
  int address = 0;

  // Loading the program into a buffer
  readFile(name, content);

  // Transferring the program into the bottom of the segment where you want it to run.
  pointer_content = content;
  while(*pointer_content)
  {
    // put current character in memory
    putInMemory(segment, address, *pointer_content);
    pointer_content++;
    address++;
  }

  // Setting the segment registers to that segment and setting the stack pointer
  // to the program’s stack and jumping to the program.
  launchProgram(segment);
}
