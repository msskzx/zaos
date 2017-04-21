
void handleInterrupt21(int, int, int, int);
void printString(char*);
void readString(char*);
int mod(int,int);
int div(int,int);
void readSector(char*, int);
void readFile(char*, char*);
void executeProgram(char*);
void terminate();
void writeFile(char*, char*, int);
void writeSector(char*, int);
void deleteFile(char*);
void handleTimerInterrupt(int ,int);
void killProcess (int);
int processTableActive [8] ;
int processTableSP [8] ;
int currentProcess ;
int Quantum ;

int main()
{
        int i ;

        Quantum =0 ;
        for(i=0 ;i<8 ;i++)
        {
                processTableActive [i]=0 ;
                processTableSP [i]= 0xFF00 ;
        }
        currentProcess=0  ;
        makeTimerInterrupt();
        makeInterrupt21();

      interrupt(0x21, 4, "shell\0", 0, 0);
    // interrupt(0x21, 4, "hello1\0", 0, 0);
    //  interrupt(0x21, 4, "hello2\0", 0, 0);
        while(1);
    //  interrupt(0x21, 5, 0, 0, 0);
}
void killProcess(int process)
{
  setKernelDataSegment() ;
  processTableActive[process]=0 ;
  currentProcess = 0;
  while(1);
}

void handleTimerInterrupt(int segment, int sp)
{
        int i ;
        int nextProcess ;
        int newSegment ;
        int newSP ;
        int counter  ;

        // currentProcess = (segment-2)/0x1000 ;

        Quantum++ ;
        if(Quantum==100)
        {


          //  printString("salem\nsalem\nsalem\0");
                i=1 ;
                processTableSP[currentProcess-1]= sp ;

             while(i<8)
                {
                        nextProcess =mod((currentProcess+i-1),8) ;
                        if(processTableActive[nextProcess]==1)
                        {
                        currentProcess = nextProcess+1 ;
                        break  ;
                        }
                         i++ ;

                }


                Quantum =0 ;
                if(i ==8)
                {
                //    printString("mm\nmm\nmm\0");
                     returnFromTimer(segment,sp);

                }
                else {
                  //  printString("salem\0");
                        newSegment = (currentProcess+1)*0x1000 ;
                        newSP = processTableSP[currentProcess-1];
                returnFromTimer(newSegment, newSP);


                }



        }
        else {
        //  printString("salem\0");
        // newSegment = (currentProcess+2)*0x1000 ;
        //  newSP = processTableSP[currentProcess];

        returnFromTimer(segment,sp);
      //  printString("tic\0");
      }


}

void terminate()
{
        // char shell [6];
        // char* p = shell;
        // *p='s';
        // *(p+1)='h';
        // *(p+2)='e';
        // *(p+3)='l';
        // *(p+4)='l';
        // *(p+5)='\0';
        // //makeInterrupt21();

        // interrupt(0x21, 4, shell, 0x2000, 0);
        setKernelDataSegment() ;
        processTableActive[currentProcess-1]=0 ;
        while(1);

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
                                        //      printString(bx);
                                                executeProgram(bx);
                                        }
                                        else
                                        {
                                                if(ax==5)
                                                {
                                                        terminate();

                                                }
                                                else {
                                                        if(ax==8)
                                                        {
                                                                writeFile(bx,cx,dx);
                                                        }
                                                        else {
                                                                if(ax==6)
                                                                {
                                                                        writeSector(bx,cx);
                                                                }
                                                                else {
                                                                        if(ax==7)
                                                                        {
                                                                                deleteFile(bx);
                                                                        }
                                                                        else {
                                                                          if(ax==9)
                                                                          {
                                                                                  killProcess(bx);
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
                        interrupt(0x10,0xE*256+0x8,0,0,0);
                        if(i!=0) {
                                a--;
                                i--;
                        }
                }
        }

        *a = 0xa;
        interrupt(0x10, 0xE*256+0xa, 0, 0, 0);
        a++;
        *a =0x0;

        return;
}

// (output, sector_number)
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

void deleteFile(char* name)
{
        int out =0;
        char buffer [512];
        char map [512];
        char read [512];

        char six [6];
        char* psix = six;
        char* pbuffer = buffer;
        char* pmap = map;
        int n = 0;
        int sectors = 0;
        int match = 1;
        int counter = pbuffer;
        int c=0;
        int cc=0;

        readSector(map,1);
        readSector(buffer, 2);
        while(pbuffer<counter+512)
        {
                n = 0;
                match = 1;
                c =0;
                cc=0;

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
                        pbuffer =pbuffer+6-n;
                }
                // point to the beginning
                psix-=n;
                while(*name && *name != '\0' && *name !='\n')
                {
                        c++;
                        name++;
                }
                name = name-c;

                if(c != n)
                {
                        pbuffer+=26;
                }
                else {


                        while(cc<c)
                        {
                                if(*name != *psix)
                                {
                                        match = 0;
                                }

                                psix++;
                                name++;
                                cc++;

                        }
                        // point to the beginning
                        psix-=cc;

                        // is it a match
                        if(match == 0)
                        {
                                pbuffer+=26;
                                name-=cc;

                        }
                        else
                        {
                                name-=cc;
                                out =1;
                                break;
                        }
                }
        }

        if(out == 0) return;
        *(pbuffer-6)=0x00;
        while(sectors<26)
        {
                if(*pbuffer == 0x00) {

                        break;
                }
                else
                {

                        *(pmap + (int)*pbuffer +1) =0x00;
                        pbuffer++;

                }
                sectors++;
        }

        writeSector(map,1);
        writeSector(buffer,2);
}

// (filename, output)
void readFile(char* x, char* y )
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
        int c=0;
        int cc=0;


        // read all file names from the directory at sector 2
        readSector(buffer, 2);
        while(pbuffer<counter+512)
        {
                n = 0;
                match = 1;
                c =0;
                cc=0;

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
                        pbuffer =pbuffer+6-n;
                }
                // point to the beginning
                psix-=n;
                while(*x && *x != '\0' && *x !='\n')
                {
                        c++;
                        x++;
                }
                x = x-c;

                if(c != n)
                {
                        pbuffer+=26;
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
                        psix-=cc;

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
                        }
                }
        }

        if(out == 0) return;
        while(sectors<26)
        {
                if(*pbuffer == 0x00) {
                        break;
                }
                else
                {

                        readSector(y, (int)*pbuffer);

                        y+=512;
                        pbuffer++;

                }
                sectors++;
        }

}

void writeSector(char* buffer, int sector){
        int relative = mod(sector,18)+1;
        int head = mod(div(sector,18),2);
        int track = div(sector,36);
        interrupt(0x13, 3*256+1, buffer,track*256+relative,head*256+0);
        return;
}

void executeProgram(char* name )
{
        char content [13312];
        char *pointer_content;
        int address = 0;
        int segment =0 ;
        int i ;

        setKernelDataSegment();
        for( i=0 ; i<8 ;i++)
        {
                if(processTableActive[i]==0)
                {
                        processTableActive[i]=1 ;
                        segment= (i+2)*0x1000 ;

                        break ;
                }


        }

        restoreDataSegment() ;

        // Loading the program into a buffer
        //printString(content);
        if(segment ==0 )
        {
                printString("No Free Space \0");
        }
        else {
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
         initializeProgram(segment) ;

        // launchProgram(segment);

        }
}

void writeFile(char* name, char* buffer, int secNum)
{
        char map [512];
        char directory [512];
        char* p_directory = directory;
        char* p_map = map;
        int counter = 0;
        int counter2 = 1;
        int name_L =0;
        readSector(map, 1);
        readSector(directory, 2);
        while(counter < 512)
        {
                if(*p_directory==0x00)
                {
                        break;
                }
                else {

                        counter+=32;
                        p_directory+=32;
                }

        }
        if(counter>=512)
        {
                interrupt(0x21, 0, "no free space in directory \0", 0, 0);
                return;
        }

        while(name_L <6 && *name != "\0" && *name && *name !='\n')
        {
                *p_directory=*name;
                name++;
                p_directory++;
                name_L++;
        }

        while(name_L <6)
        {

                *p_directory=0x00;
                p_directory++;
                name_L++;
        }

        while(counter2<512 && secNum >0)
        {
                if( *p_map == 0x00)
                {
                        *p_map ==0xFF;
                        secNum--;

                        *p_directory= (char)counter2;
                        p_directory++;
                        p_map++;

                        writeSector(buffer,counter2);
                        counter2++;

                        buffer+=512;

                }
                else {
                        p_map++;
                        counter2++;
                }

        }
        if(secNum !=0 )
        {
                interrupt(0x21, 0, "no free space in map \0", 0, 0);
                return;
        }

        writeSector(directory,2);
        writeSector(map,1);

}
