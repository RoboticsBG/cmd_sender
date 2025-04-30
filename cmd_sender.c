#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>          // for sleep()
#include <arpa/inet.h>
#include <netinet/in.h>

#define DEST_IP "10.10.50.142"     // Set this to the target machine's IP
#define PORT 8081
#define INTERVAL 1                 // seconds between messages

int main() {
    int sockfd;
    char buffer[128];
    struct sockaddr_in destaddr;

    // 1. Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set destination address
    memset(&destaddr, 0, sizeof(destaddr));
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, DEST_IP, &destaddr.sin_addr) <= 0) {
        perror("Invalid destination IP address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Cmd sender ready\n");

    // 3. Periodic send loop
    while (1) {

	fgets(buffer, sizeof(buffer), stdin);
         buffer[strcspn(buffer, "\n")] = 0;

        ssize_t sent = sendto(sockfd, buffer, strlen(buffer), 0,
                              (struct sockaddr *)&destaddr, sizeof(destaddr));
        if (sent < 0) {
            perror("sendto failed");
        } else {
            printf("[SENT] %s\n", buffer);
        }

        //sleep(INTERVAL);  // wait between messages
    }

    close(sockfd);
    return 0;
}
