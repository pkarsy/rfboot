#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include  <signal.h>

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    //tty.c_cc[VMIN] = 1;
    //tty.c_cc[VTIME] = 1;

    /* okernel driver epistrefei sto read molis parei 1 byte
     * kanei block for ever. Koveteai me alarm  */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/* void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        // half second timer

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
} */

volatile bool alarm_expire = false;

void  ALARMhandler(int sig)
{
  //signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
  //printf("Hello");
  //signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
  alarm_expire = true;
  //printf("Al\n");

}

int readser(const int fd, unsigned char* buf, const int nbytes, const int timeout) {
	//
    // prepei se 100ms na exoume 3chars
    int idx = 0;
    siginterrupt(SIGALRM, 1);

    ualarm(timeout,0);
    alarm_expire = false;
    while (! alarm_expire) {

        //int rdlen;
        unsigned char inp;

        int rdlen = read(fd, &inp, 1);
        //if (alarm_expire) printf("Alarm\n");
        if (rdlen == 1) {

            buf[idx] = inp;
			idx++;
            if (idx==nbytes) break;

            // printf("Read %d: \"%s\"\n", rdlen, buf);

        } else if (rdlen < 0) {
            //printf("Error from read: %d: %s\n", rdlen, strerror(errno));
            //return rdlen;
            break;
        }
    }
    alarm(0);

    //buf[idx]=0;
	return idx;
}

int c_getchar(const int fd,  const int timeout) {

    siginterrupt(SIGALRM, 1);

    ualarm(timeout,0);
    alarm_expire = false;

    unsigned char inp;

    int rdlen = read(fd, &inp, 1);

    alarm(0);
    if (rdlen==1) return inp;
    else return -1;
}

void writeser(const int fd, char* buf, const int nbytes) {
	int n = write(fd,buf,nbytes);
//	//printf("write=%d\n",n);
}

int c_openport(char *portname)
{
    // = "/dev/ttyACM0";
    int fd;

	signal(SIGALRM, ALARMhandler);

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return fd;
    }
    // baudrate 38400, 8 bits, no parity, 1 stop bit
    // set_interface_attribs(fd, B38400);
    set_interface_attribs(fd, B57600);

    //wlen = write(fd, "Hello!\n", 7);
    //if (wlen != 7) {
    //    printf("Error from write: %d, %d\n", wlen, errno);
    //}
    //tcdrain(fd);    /* delay for output */


    // simple noncanonical input

    return fd;

}


//    unsigned char buf[4];
//	int b = readser(fd, buf, 3, 50000);
//   if (b>0) printf("Read %d: \"%s\"\n", b, buf);
