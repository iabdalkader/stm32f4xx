#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libusb.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define USB_VID        (0x0483) /* vendor id    */
#define USB_PID        (0x5740) /* product id   */
#define EP_IN          (0x81)   /* IN endpoint  */
#define EP_OUT         (0x01)   /* OUT endpoint */
#define TIMEOUT        (500)      /* I/O transfer timeout */
#define DEBUG_LEVEL    (1)      /* libusb debug level   */

int stop = 0;
libusb_device_handle *dev = NULL;

static void sig_hdlr(int signum, siginfo_t *siginfo, void *user)
{
    switch (signum) {
        case SIGINT:
            stop =1;
    }
}

const char *err_str(int err)
{
    switch (err) {
        case 0: 
            return "Success";
        case LIBUSB_ERROR_TIMEOUT: 
            return "transfer timed out";
        case LIBUSB_ERROR_PIPE:
            return "endpoint halted";
        case LIBUSB_ERROR_OVERFLOW:
            return "overflow";
        case LIBUSB_ERROR_NO_DEVICE:
            return "device has been disconnected";
        case LIBUSB_ERROR_NOT_FOUND:
            return "the requested interface does not exist";
        default:
            return "other"; 
    };
}

void cleanup(void) 
{
    libusb_close(dev);
    libusb_exit(NULL);
}

int bulk_xfr(int ep, void *buf, int len)
{
    int ret;
    if ((ret = libusb_bulk_transfer(dev, ep, buf, len, &len, TIMEOUT)) != 0) {
        fprintf(stderr, "I/O error %s %d\n", err_str(ret), len);
    }
    return len;
}

int main (int argc, char **argv) 
{
    /*sigaction struct*/
    struct sigaction act = {
      .sa_sigaction = sig_hdlr,
      .sa_flags     = SA_SIGINFO|SA_NOCLDSTOP,
    };

    /*init libusb*/
    libusb_init(NULL);
    
    /*set cleanup to be called at exit*/
    atexit(cleanup);     

    /*install signal handlers*/
    sigaction(SIGINT, &act, NULL);

    libusb_set_debug(NULL, DEBUG_LEVEL);    /*set debugging level*/

    if ((dev = libusb_open_device_with_vid_pid(NULL, USB_VID, USB_PID)) == NULL){
        fprintf(stderr, "Device could not be found.\n");
        exit(1);
    }

    /* claim interace zero */
    if (libusb_claim_interface(dev, 0) != 0) {
        fprintf(stderr, "Failed to claim interface\n");
        exit(1);
    }

    char buf[64]="HelloWorld!";
    /* send data */
    bulk_xfr(EP_OUT, buf, strlen(buf+1));//send anything

    while (1) {
        /* receive data */
        if (bulk_xfr(EP_IN, buf, 64)) {
            printf("%s\n", buf);
        }
        if (stop) {
            break;
        }
        sleep(1);
    }
    return 0;
}


