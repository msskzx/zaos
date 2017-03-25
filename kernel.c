void handleInterrupt21(int, int, int, int);
void printString(char*);
void readString(char*);
int mod(int,int);
int div(int,int);
void readSector(char* , int);
void readFile(char* , char*);
void executeProgram(char*, int);
void terminate();

int main()
{


 makeInterrupt21();
 //interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
  //interrupt(0x21,5, 0, 0, 0);
  interrupt(0x21, 4, "shell\0", 0x2000, 0);
  //interrupt(0x21,5, 0, 0, 0);

//terminate();

}

void terminate()
{
  char shell [6];
  char* p = shell ;
  *p='s';
  *(p+1)='h';
  *(p+2)='e';
  *(p+3)='l';
  *(p+4)='l';
  *(p+5)='\0';
  //makeInterrupt21();

  interrupt(0x21, 4, shell, 0x2000, 0);

}

void printString(char* x) {
  while(*x && *x !='\0') {
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
            if(ax==5)
            {
             terminate();

            }
            else {
            printString("Error! No such function!\0");
            }
          }
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
  int c=0 ;
  int cc=0 ;

  // read all file names from the directory at sector 2
  readSector(buffer, 2);
  while(pbuffer<counter+512)
  {
    n = 0;
    match = 1;
    c =0;
    cc=0 ;

    // load the current file name
    while(*pbuffer && n<6 && *pbuffer != 0x00)
    {
      *psix =*pbuffer;
      n++;
      psix++;
      pbuffer++;
    }
    if(n != 6)
    {
      pbuffer =pbuffer+6-n ;
    }
    // point to the beginning
    psix-=n ;
    while(*x && *x != '\0' && *x !='\n')
    {
      c++;
      x++ ;
    }
    x = x-c ;

    if(c != n)
    {
      pbuffer+=26 ;
    }
else {


    while(cc<c)
    {
      if(*x != *psix)
      {
        match = 0;
      }

      psix++;
      x++;
      cc++;

    }
    // point to the beginning
    psix-=cc ;

    // is it a match
    if(match == 0)
    {
      pbuffer+=26;
      x-=cc;

    }
    else
    {
      x-=cc;
      out =1;
      break;
    }}
  }

  if(out == 0) return;
  while(sectors<26)
  {
    if(*pbuffer == 0x00){

break;}
    else
    {

      readSector(y , (int)*pbuffer);
      y+=512;
      pbuffer++;

    }
    sectors++;
  }

}


void executeProgram(char* name, int segment)
{
  char content [13312];
  char *pointer_content;
  int address = 0;
  // Loading the program into a buffer
  //printString(content);
  readFile(name, content);

  // Transferring the program into the bottom of the segment where you want it to run.
  //printString("salem");
  pointer_content = content;
  while(address<13312)
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
