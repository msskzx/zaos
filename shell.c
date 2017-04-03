
int main()
{

while(1){
char buffer [512];
char buffer1 [13312];
int c=0 ;
char filename [512];
char* pfile = filename ;
char* pbuffer = buffer;


interrupt(0x21,0, "SHELL> \0", 0, 0);
interrupt(0x21,1, buffer, 0, 0);

if( *pbuffer=='v' && *(pbuffer +1)=='i' && *(pbuffer +2)=='e' && *(pbuffer +3)=='w' && *(pbuffer +4)==' ')
{

  pbuffer+=5 ;

  while(*pbuffer && c<512)
  {


    *pfile = *pbuffer ;
    pbuffer++;
    pfile++;
    c++;

  }
  pfile--;
  *pfile ='\0';
  pfile--;
  *pfile ='\0';
  //pfile++;
  //*pfile ='\0';
  pfile = pfile-c+2;
  pbuffer = pbuffer-c ;
//  interrupt(0x21,0,buffer,0,0);
//  interrupt(0x21,0,filename,0,0);
  interrupt(0x21,3,filename,buffer1,0);
  interrupt(0x21,0,buffer1,0,0);
  interrupt(0x21,0,"\n\0" , 0, 0);



}
else if( *pbuffer=='e' && *(pbuffer +1)=='x' && *(pbuffer +2)=='e' && *(pbuffer +3)=='c' && *(pbuffer +4)=='u'&& *(pbuffer +5)=='t'&& *(pbuffer +6)=='e'&& *(pbuffer +7)==' ')
{

  pbuffer+=8 ;

  while(*pbuffer && c<512)
  {


    *pfile = *pbuffer ;
    pbuffer++;
    pfile++;
    c++;

  }

  pfile--;
  *pfile ='\0';

  pfile--;
  *pfile ='\0';
  pfile = pfile-c+2;
  pbuffer=pbuffer-c ;
  interrupt(0x21,4,filename,0x2000,0);
  interrupt(0x21,0,"\n\0" , 0, 0);



}
else {
  if( *pbuffer=='d' && *(pbuffer + 1)=='e' && *(pbuffer + 2)=='l' && *(pbuffer + 3)=='e' && *(pbuffer + 4)=='t'&& *(pbuffer + 5)=='e' && *(pbuffer + 6)==' ')
  {
    pbuffer+=7;
    while(*pbuffer && c<512)
    {
      *pfile = *pbuffer;
      pbuffer++;
      pfile++;
      c++;
    }

    pfile--;
    *pfile ='\0';
    pfile--;
    *pfile ='\0';
    pfile = pfile-c+2;
    pbuffer = pbuffer-c;

    interrupt(0x21, 7, filename, 0, 0);
    interrupt(0x21, 0, "\n\0", 0, 0);
  }
  else
  {
    interrupt(0x21, 0, "\n Bad Command!\n\0", 0, 0);
  }
}
}
}
