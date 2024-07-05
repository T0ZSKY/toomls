#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>


typedef enum {
    HELP,
    TIME,
    TIMER,
    OS,
    INFO,
    SOFTWARE,
    UNKNOWN
} command_t;

command_t get_command(const char *str) {
    if (strcmp(str, "help") == 0) return HELP;
    if (strcmp(str, "time") == 0) return TIME;
    if (strcmp(str, "timer") == 0) return TIMER;
    if (strcmp(str, "OS") == 0) return OS;
    if (strcmp(str, "info") == 0) return INFO;
    if (strcmp(str, "software") == 0) return SOFTWARE;
    return UNKNOWN;
}

int main(int argc, char *argv[]) {
    // verifiez s'il y a suffisamment d'arguments
    if (argc < 2) {
        printf("Usage: %s <command> [subcommand]\n", argv[0]);
        return 1;
    }

    command_t command = get_command(argv[1]);

    switch (command) {
        case HELP:
            printf("Help. \n");
            printf("time: Affiche l'heure actuelle\n");
            printf("timer <seconds>: Attendre pendant un certain nombre de secondes\n");
            printf("OS: Affiche le nom du système d'exploitation\n");
            printf("info: Affiche des informations sur le système\n");
            printf("software: Affiche la liste de tous les logiciels installés\n");

            break;




        case TIME:
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("Time: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900,
                   tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            break;




        case TIMER:
            if (argc < 3) {
                printf("Usage: %s sleep <seconds>\n", argv[0]);
                return 1;
            }
            int seconds = atoi(argv[2]);
            printf("Timer: %d seconds\n", seconds);
            sleep(seconds);
            break;



        case OS:
            #if defined(_WIN32) || defined(_WIN64)
                printf("OS: Windows\n");
            #elif defined(__linux__)
                printf("OS: Linux\n");
            #elif defined(__APPLE__) && defined(__MACH__)
                printf("OS: macOS\n");
            #elif defined(__unix__)
                printf("OS: Unix\n");
            #else
                printf("OS: Unknown\n");
            #endif
            break;


case INFO:
    printf("System Information:\n");

    // Afficher le nom d'utilisateur actuel
    printf("User: %s\n", getlogin());

    // Afficher la version de l'OS utilisé
    #if defined(_WIN32) || defined(_WIN64)
        printf("OS: Windows %s\n", _WIN32_WINNT);
    #elif defined(__linux__)
        {
            struct utsname uts;
            uname(&uts);
            printf("OS: Linux %s\n", uts.release);
        }
    #elif defined(__APPLE__) && defined(__MACH__)
        printf("OS: macOS %s\n", MAC_OS_X_VERSION_MAX_ALLOWED);
    #elif defined(__unix__)
        {
            struct utsname uts;
            uname(&uts);
            printf("OS: Unix %s\n", uts.sysname);
        }
    #else
        printf("OS: Unknown\n");
    #endif

    // Afficher si l'utilisateur actuel est administrateur ou pas
    if (getuid() == 0) {
        printf("User: Administrator\n");
    } else {
        printf("User: Standard user\n");
    }

    // Afficher les composants de l'ordinateur utilisé
    #if defined(_WIN32) || defined(_WIN64)
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        printf("CPU: %u bits\n", sysInfo.wProcessorArchitecture * 32);
        printf("RAM: %u MB\n", (sysInfo.lpMaximumApplicationAddress - sysInfo.lpMinimumApplicationAddress) / (1024 * 1024));
    #elif defined(__linux__) || defined(__unix__)
        FILE *fp;
        char buffer[256];
        long lSize;
        int cpu_bits = 0;
        int ram_mb = 0;

        // CPU bits
        fp = popen("getconf LONG_BIT", "r");
        fgets(buffer, 256, fp);
        lSize = strtol(buffer, NULL, 10);
        cpu_bits = lSize * 32;
        pclose(fp);

        // RAM
        fp = popen("free -m | grep Mem | awk '{print $2}'", "r");
        fgets(buffer, 256, fp);
        lSize = strtol(buffer, NULL, 10);
        ram_mb = lSize;
        pclose(fp);

        printf("CPU: %d bits\n", cpu_bits);
        printf("RAM: %d MB\n", ram_mb);
    #elif defined(__APPLE__) && defined(__MACH__)
        
    #endif

    break;
            


        
           case SOFTWARE:
    printf("Software:\n");
    {
        FILE *fp = popen("dpkg --get-selections", "r");
        if (fp != NULL) {
            char buffer[256];
            while (fgets(buffer, 256, fp) != NULL) {
                printf("  %s", buffer);
            }
            pclose(fp);
        }
    }
    break;


    


        case UNKNOWN:
            printf("Unknown command: %s type help for all command.\n", argv[1]);
            break;
    }
    return 0;
}