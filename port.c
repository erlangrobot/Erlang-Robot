/* demo_server.c */
#include <stdio.h>
#include <string.h>
#include<sys/io.h>
#define f_left !(i>>6 & 1)
#define f_right !(i>>5 & 1)
#define back !(i>>4 & 1)
#define left (i>>3 & 1)
#define right (i>>7 & 1)
#define b 0x0a
#define f 0x11
#define r 0x09
#define l 0x12

unsigned char read_ir(){
	int i;
	iopl(3);
	i= inb(0x379);
	if( f_left &&  f_right ){
		if(!right)
			return 'a';
		else if(!left)
			return 'b';
		if( ! back  )
			return 'c';
		else
			return 'd';
	} else if( f_right ) {
		if( !left )
			return 'b';
		else if( !right )
			return 'a';
		else if(! back)
			return 'c';
		else 
			return 'd';
	} else if( f_left ){
		if( !right)
			return 'a';
		else if(!left)
			return 'b';
		else if(!back)
			return 'c';
		else
			return 'd';
	}
	else
		return 'e';
}

void move_forward(char k)
{
	outb(k,0x378);
	usleep(50);
	outb(0x00,0x378);
	usleep(3500);
}


/* Message data are all unsigned bytes */
typedef unsigned char byte;
main(argc, argv)
int argc;
char **argv;
{
	iopl(3);
    int len;
    char c;
    int i;
    char *progname;
    byte buf[1000];
//    progname = argv[0];         /* Save start name of program */
//    fprintf(stderr, "demo_server in C Starting \n");
    while ((len = read_cmd(buf)) > 0) {
	    if(buf[0] == '1') {
		    move_forward(buf[1]);
	    }
	    if(buf[0] == '2') {
		    	buf[0] = read_ir();
		    	write_cmd(buf,1);
		        usleep(2550);
	    }

    }
  
}
/* Read the 2 length bytes (MSB first), then the data. */
read_cmd(buf)
byte *buf;
{
    int len;
    if (read_exact(buf, 2) != 2)
         return(-1);
    len = (buf[0] << 8) | buf[1];
    return read_exact(buf, len);
}
/* Pack the 2 bytes length (MSB first) and send it */
write_cmd(buf, len)
byte *buf;
int len;
{
    byte str[2];
    put_int16(len, str);
    if (write_exact(str, 2) != 2)
         return(-1);
    return write_exact(buf, len);
}
/* [read|write]_exact are used since they may return
 * BEFORE all bytes have been transmitted
 */
read_exact(buf, len)
byte *buf;
int len;
{
    int i, got = 0;
    do {
         if ((i = read(0, buf+got, len-got)) <= 0)
           return (i);
         got += i;
    } while (got < len);
    return (len);
}

write_exact(buf, len)
byte *buf;
int len;
{
    int i, wrote = 0;
    do {
         if ((i = write(1, buf+wrote, len-wrote)) <= 0)
           return (i);
         wrote += i;
    } while (wrote < len);
    return (len);
}

put_int16(i, s)
byte *s;
{
    *s = (i >> 8) & 0xff;
    s[1] = i & 0xff;
}
