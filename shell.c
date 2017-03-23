
int main()
{

char buffer [13312];
interrupt(0x21,0, "SHELL> : \0", 0, 0);
interrupt(0x21,1, buffer, 0, 0);
interrupt(0x21,0, "\n Bad Command! \0", 0, 0);
interrupt(0x21,5, 0, 0, 0);

}
