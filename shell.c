int main()
{
        char buffer [512];
        char buffer1 [13312];
        char filename [512];
        char filename1 [512];
        char content[13312];
        char directory[512];
        char cur_file_name[7];

        int c = 0;
        int c1 = 0;
        int sector = 6;
        int i=0;
        int counter =0;
        int counter2 =0;
        int num_of_sectors = 0;
        int j = 0;

        char* pfile = filename;
        char* pfile1 = filename1;
        char* pbuffer = buffer;
        char* file_cur_char = content;
        char line [13312];
        char* line_pointer = line;
        char* directory_pointer = directory;
        char* base_address = directory;
        char* cur_file_name_pointer = cur_file_name;
        char* filename_pointer = filename;
        char* filename1_pointer = filename1;

        while(1)
        {
                for( i=0; i<13312; i++)
                {
                        if(i<7)
                        {
                                cur_file_name[i] = '\0';
                        }
                        if(i<512)
                        {
                                buffer[i]='\0';
                                filename[i]='\0';
                                filename1[i]='\0';
                                directory[i] = '\0';
                        }
                        buffer1[i]='\0';
                        content[i]='\0';
                }
                c=0;
                c1 =0;
                sector =6;
                i=0;
                counter =0;
                counter2 =0;
                num_of_sectors = 0;
                j = 0;

                pfile = filename;
                pfile1 = filename1;
                pbuffer = buffer;
                file_cur_char = content;
                line_pointer = line;
                directory_pointer = directory;
                base_address = directory;
                cur_file_name_pointer = cur_file_name;
                filename_pointer = filename;
                filename1_pointer = filename1;

                interrupt(0x21,0, "SHELL> \0", 0, 0);
                interrupt(0x21,1, buffer, 0, 0);

                if( *pbuffer=='v' && *(pbuffer +1)=='i' && *(pbuffer +2)=='e' && *(pbuffer +3)=='w' && *(pbuffer +4)==' ')
                {
                        // view file
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
                        //pfile++;
                        //*pfile ='\0';
                        pfile = pfile-c+2;
                        pbuffer = pbuffer-c;
                        //  interrupt(0x21,0,buffer,0,0);
                        //  interrupt(0x21,0,filename,0,0);
                        interrupt(0x21,3,filename,buffer1,0);
                        interrupt(0x21,0,buffer1,0,0);
                        interrupt(0x21,0,"\n\0", 0, 0);
                        pbuffer-=5;



                }
                else if( *pbuffer=='e' && *(pbuffer +1)=='x' && *(pbuffer +2)=='e' && *(pbuffer +3)=='c' && *(pbuffer +4)=='u'&& *(pbuffer +5)=='t'&& *(pbuffer +6)=='e'&& *(pbuffer +7)==' ')
                {
                        // excute program
                        pbuffer+=8;

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
                        pbuffer=pbuffer-c;
                        interrupt(0x21,4,filename,0x2000,0);
                        interrupt(0x21,0,"\n\0", 0, 0);
                        pbuffer-=8;
                }
                else {
                        if( *pbuffer=='d' && *(pbuffer + 1)=='e' && *(pbuffer + 2)=='l' && *(pbuffer + 3)=='e' && *(pbuffer + 4)=='t'&& *(pbuffer + 5)=='e' && *(pbuffer + 6)==' ')
                        {
                                // delete file
                                pbuffer+=7;
                                while(*pbuffer && c<512)
                                {
                                        *pfile1 = *pbuffer;
                                        pbuffer++;
                                        pfile1++;
                                        c++;
                                }

                                pfile1--;
                                *pfile1 ='\0';
                                pfile1--;
                                *pfile1 ='\0';
                                pfile1 = pfile1-c+2;
                                pbuffer = pbuffer-c;

                                // TODO fix
                                interrupt(0x21, 7, filename1, 0, 0);
                                interrupt(0x21, 0, "\n\0", 0, 0);
                                pbuffer-=7;
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
                                        counter2=0;
                                        while(1)
                                        {
                                                counter =0;

                                                for( i=0; i<13312; i++)
                                                {
                                                        line[i]='\0';
                                                }
                                                // read line
                                                interrupt(0x21,1, line, 0, 0);

                                                while(*line_pointer != '\0' &&  *line_pointer !='\n' && counter < 13312)
                                                {
                                                        *file_cur_char = *line_pointer;
                                                        file_cur_char++;
                                                        line_pointer++;
                                                        counter++;
                                                        counter2++;
                                                }

                                                *line_pointer = '\0';
                                                *file_cur_char='\n';
                                                file_cur_char++;
                                                line_pointer-=counter;
                                                counter2++;
                                                if(line[0] == '\n' || line[1]=='\0')
                                                        break;
                                        }

                                        // create the new file
                                        file_cur_char-=counter2;

                                        interrupt(0x21, 8, filename, content, sector);

                                        interrupt(0x21, 0, "\n\0", 0, 0);
                                        pbuffer-=7;

                                }
                                else
                                {
                                        // TODO fix
                                        if( *pbuffer=='d' && *(pbuffer + 1)=='i' && *(pbuffer + 2)=='r' && *(pbuffer + 3) == 'r')
                                        {
                                                // dir
                                                pbuffer+=4;
                                                interrupt(0x21, 0, "Name        \0", 0, 0);
                                                interrupt(0x21, 0, "Size\n\0", 0, 0);

                                                interrupt(0x21, 2, directory, 2, 0);

                                                while(directory_pointer < base_address+512)
                                                {
                                                        j = 0;
                                                        num_of_sectors = 0;
                                                        cur_file_name_pointer = cur_file_name;

                                                        while(j<7)
                                                        {
                                                                cur_file_name[j] = '\0';
                                                                j++;
                                                        }
                                                        j = 0;

                                                        // load the current file name
                                                        while(*directory_pointer && j<6 && *directory_pointer != 0x00)
                                                        {
                                                                *cur_file_name_pointer = *directory_pointer;
                                                                cur_file_name_pointer++;
                                                                j++;
                                                                directory_pointer++;
                                                        }

                                                        *cur_file_name_pointer = '\0';

                                                        directory_pointer = directory_pointer+6-j;

                                                        if(j!=0)
                                                        {
                                                                j = 6;
                                                                // print file name
                                                                interrupt(0x21, 0, cur_file_name, 0, 0);
                                                                interrupt(0x21, 0, "    \0", 0, 0);

                                                                // number of sectors
                                                                while(*directory_pointer && j<32 && *directory_pointer != 0x00)
                                                                {
                                                                        num_of_sectors++;
                                                                        j++;
                                                                        directory_pointer++;
                                                                }

                                                                // print file size
                                                                // TODO fix
                                                                interrupt(0x21, 0, (char)num_of_sectors, 0, 0);

                                                                interrupt(0x21, 0, "\n\0", 0, 0);
                                                        }
                                                        else
                                                        {
                                                            directory_pointer+=32;
                                                        }
                                                }
                                        }
                                        else
                                        {
                                                if( *pbuffer=='c' && *(pbuffer + 1)=='o' && *(pbuffer + 2)=='p' && *(pbuffer + 3)=='y' && *(pbuffer + 4)==' ')
                                                {
                                                        // copy
                                                        pbuffer+=5;

                                                        // read filename
                                                        while(*pbuffer && j<512 && *pbuffer != ' ')
                                                        {
                                                                *filename_pointer = *pbuffer;
                                                                pbuffer++;
                                                                filename_pointer++;
                                                                j++;
                                                        }
                                                        pbuffer++;

                                                        *filename_pointer = '\0';

                                                        j = 0;

                                                        // read filename1
                                                        while(*pbuffer && j<512 && *pbuffer != ' ')
                                                        {
                                                                *filename1_pointer = *pbuffer;
                                                                pbuffer++;
                                                                filename1_pointer++;
                                                                j++;
                                                        }
                                                        pbuffer++;

                                                        *filename1_pointer = '\0';

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
