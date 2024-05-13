#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>

#define    SOFT_WDT_DEV    "/dev/soft_wdt"

int feed_dog_cnt;
int feed_dog_gap;

int main(int argc, char *argv[])
{
    int i;
    int  timeout;
    struct watchdog_info ident;
    
    int fd;

    if (argc<3)
    {
        printf("usage:\n %s  <feed_gap(in seconds)>  <feed_cnt>\n", argv[0]);
        return 0;
    }
    
    fd=open("/dev/soft_wdt", O_WRONLY);

    if (fd < 0)
    {
        printf("open %s failed\n", SOFT_WDT_DEV);
        exit(1);
    }


    printf("open %s succeed\n", SOFT_WDT_DEV);
    
    timeout = 7;
    printf("set timeout to %d\n", timeout);
    ioctl(fd, WDIOC_SETTIMEOUT, &timeout);

    timeout = 0;
    ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
    printf("get timeout returns %d\n", timeout);

    ioctl(fd, WDIOC_GETSUPPORT, &ident);
    printf("dog name is %s\n", ident.identity);

    printf("make the dog closable\n");
    write(fd, "V", 1);

    feed_dog_gap = atoi(argv[1]);
    feed_dog_cnt = atoi(argv[2]);
    for (i=0; i<feed_dog_cnt; i++)
    {
        printf("feed dog\n");
        write(fd, "1234", 4);
        usleep(feed_dog_gap*1000000);
    }

    printf("stop feeding dog\n");
    while (1)
    {
        usleep(1000000);
    }
    
    return 0;
}
