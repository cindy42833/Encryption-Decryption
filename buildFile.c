#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv) {
    FILE *fp;
    unsigned char *str = "Ci4Oi8t4wGN1KuKXYhtGn5TU4jhqsnUtm61WcCgQOZPp35tVDVCnt9k2pFCKHK7dn0hpyurQuQ2bry8BIHE515F02HNupGKJ83rLpt5LJJkz6LnybkLHmfmZRjKnfpshIBxhdIPvr6ra0OjdnkhTn7qTz3K2akLWdSL3go8R3dXrGvfVkX0BjHCQ5Zqob1q9v7Rh4yiZJKydylfqMQmHEogP7ub3NqoY5sC789gChNuaYxe8EkPpDMx0X2i1fEHmx2yMr8vjDeFh226IkIqNM8ntFd9a357FTeBvoz7Q9uvvjJgctv7T4KikAxNS9iwycxb4BEnS0L1gjYKdOcPNsasCiWylzApa3xYpm6qHWnfXHZuqzdRhbJ455gCmwELiSwTkMcNcnXEHt0O4f69WoDtMSI5STUxiPdLfhVJDsgPLNb8ULRddFvDGkte8jtCk8iqIrYWOXrP6YUMXHW78o1g3RtiVuHB5hckKCBoGxDrU3yNd9vAr88bwUE07IB4ScsDYQEw5i44wLJe0J2qhkwhX36JljkXa77GJoRKoKIxBgfHrPU0IjiuH6kUEBNA1FZ1yWvJSRMcSGEuu9V4sfEdjvWKq1LikqIHMlEouA3e0PpII7MjT3yeClkvvmdYofK7dGUe76ir47QmCMCZn1UL4xwg9aXIDq487uOM9NDShfLEx8xdWrDzF1NHsbdyHYL54FBpEnmcul4Y9rON6gVUrAXAUdfZrTKB6LnOl1dTB0r49atdR7pvkisxjQ2yOnNAKRbZRIX1VzSCykEoMlfFdLUIkFiZClbQZ9O5yDO8JERqU3EYDu7FlvxYUZD0Rb5M02OLh4TVf6SsbtWRVOKgPhFOUWdMixTofiGFK5L5fp3BvEfMm8LAhBoEEjFuCVh9cdmTW2BkGXrivA4WVRCrpfrxeBW8sEnuGOH3CPwxqpVeinNUbQMiMfki3l3IGJCA6DSu4Vn0tuTkQgq9Ylkl5oi1d4pbQ6sgTowDiFbjMAZTdWwmsWrv8uvKw2mR6";


    if(argc < 2) {
        fprintf(stderr, "./buildFile <fileSize_with_kb>\n");
        exit(0);
    }

    errno = 0;
    fp = fopen("input.txt", "w");
    if (errno != 0 ) {
        perror("Error occurred while opening file");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);

    int filesize_kb = atoi(argv[1]);
    for(int i=0; i<filesize_kb; i++) {
        fwrite(str, 1024, 1, fp);
    }

    fclose(fp);

    return 0;
}