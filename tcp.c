#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <netdb.h>

typedef struct {
    int attempted;
    int connected;
    int failed;
    double min_time;
    double max_time;
    double total_time;
} ConnectionStats;

ConnectionStats stats;

void print_report() {
    double success_rate = (double)stats.connected / (double)stats.attempted * 100;
    printf("\nConnection statistics:\n");
    printf("  Attempted = %d, Connected = %d, Failed = %d (%.2f%%)\n",
           stats.attempted, stats.connected, stats.failed, success_rate);
    
    printf("Approximate connection times:\n");
    if (stats.connected > 0) {
        printf("  Minimum = %.2fms, Maximum = %.2fms, Average = %.2fms\n",
               stats.min_time, stats.max_time, stats.total_time / stats.connected);
    }
}

void handle_sigint(int sig) {
    print_report();
    exit(0);
}

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

void ping(const char *host, int port) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    int sock;
    struct sockaddr_in server_addr;
    struct hostent *server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        stats.failed++;
        stats.attempted++;
        return;
    }

    // Resolve host
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "Error resolving host\n");
        stats.failed++;
        stats.attempted++;
        close(sock);
        return;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port);

    // Attempt to connect
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("\033[31mConnection timed out\033[0m\n");
        stats.failed++;
    } else {
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        double duration = get_time_diff(start_time, end_time);

        printf("Connected to \033[32m%s\033[0m: time=\033[32m%.2fms\033[0m protocol=\033[32mTCP\033[0m port=\033[32m%d\033[0m\n",
               host, duration, port);

        stats.connected++;
        stats.total_time += duration;

        if (stats.min_time == 0 || duration < stats.min_time) {
            stats.min_time = duration;
        }
        if (duration > stats.max_time) {
            stats.max_time = duration;
        }
        close(sock);
    }

    stats.attempted++;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <host> <port>\n", argv[0]);
        return 1;
    }

    const char *host = argv[1];
    int port = atoi(argv[2]);

    signal(SIGINT, handle_sigint);

    stats.min_time = 0;
    stats.max_time = 0;
    stats.total_time = 0;
    stats.attempted = 0;
    stats.connected = 0;
    stats.failed = 0;

    while (1) {
        ping(host, port);
        sleep(1);  // Wait for 1 second between pings
    }

    return 0;
}
