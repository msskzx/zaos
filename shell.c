int main()
{
while(1)
{

char buffer [512];
char buffer1 [13312];
char filename [512];
char filename1 [512];
char content[13312];

int c = 0;
int c1 = 0;
int sector = 6;

char* pfile = filename;
char* pfile1 = filename;

char* pbuffer = buffer;
char* file_cur_char = content;


interrupt(0x21,0, "SHELL> \0", 0, 0);
interrupt(0x21,1, buffer, 0, 0);

if( *pbuffer=='v' && *(pbuffer +1)=='i' && *(pbuffer +2)=='e' && *(pbuffer +3)=='w' && *(pbuffer +4)==' ')
{
  // view file
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
  // excute program
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
    // delete file
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

    // TODO fix

    interrupt(0x21, 7, filename, 0, 0);
    interrupt(0x21, 0, "\n\0", 0, 0);
  }
  else
  {
      // create file
    if( *pbuffer=='c' && *(pbuffer + 1)=='r' && *(pbuffer + 2)=='e' && *(pbuffer + 3)=='a' && *(pbuffer + 4)=='t' && *(pbuffer + 5)=='e' && *(pbuffer + 6)==' ')
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

      while(1)
      {
          char line [13312];
          char* line_pointer = line;
          // read line
          interrupt(0x21,1 , line, 0, 0);

          while(*line_pointer)
          {
              *file_cur_char = *line_pointer;
              file_cur_char++;
              line_pointer++;
          }
          *line_pointer = '\0';

          // TODO delete
          interrupt(0x21, 0, "line: \0", 0, 0);
          interrupt(0x21, 0, line, 0, 0);

          // TODO fix
        //   if(*line_pointer == '\n')
              break;
      }

      // create the new file
      interrupt(0x21, 8, filename, content, sector);

      interrupt(0x21, 0, "\n\0", 0, 0);

    }
    else
    {
      if( *pbuffer=='d' && *(pbuffer + 1)=='i' && *(pbuffer + 2)=='r' && *(pbuffer + 3)==' ')
      {
        // dir
        // TODO
      }
      else
      {
        if( *pbuffer=='c' && *(pbuffer + 1)=='o' && *(pbuffer + 2)=='p' && *(pbuffer + 3)=='y' && *(pbuffer + 4)==' ')
        {
          // copy
          pbuffer+=5;
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

          // TODO read filename1

          // read the first file
          interrupt(0x21, 3, filename, content, 0);
          // create the new file
          interrupt(0x21, 8, filename1, content, sector);

          interrupt(0x21, 0, "\n\0", 0, 0);
        }
        else
        {
          interrupt(0x21, 0, "\nBad Command!\n\0", 0, 0);
        }
      }
    }
  }
}
}
}
