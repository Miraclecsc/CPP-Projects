#include <iostream>
#include <iomanip>
#include <cstring>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>

using namespace std::chrono;

// 计算校验和
unsigned short checksum(void *b, int len) {    
    unsigned short *buf = (unsigned short *)b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

// 创建 ICMP Echo 请求数据包
void create_packet(struct icmphdr *icmp_hdr, int seq) {
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.sequence = seq;
    icmp_hdr->un.echo.id = getpid();
    icmp_hdr->checksum = 0;
    icmp_hdr->checksum = checksum(icmp_hdr, sizeof(*icmp_hdr));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <destination IP>" << std::endl;
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        std::cerr << "Socket error" << std::endl;
        return 1;
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &dest_addr.sin_addr);

    const int max_ttl = 64;
    const int timeout = 1;  // seconds

    char des[500];
    inet_ntop(AF_INET, &dest_addr.sin_addr, des, INET_ADDRSTRLEN);
    std::cout << "traceroute to " << des << " (" << des << "), " << max_ttl <<" hops max\n";

    for (int ttl = 1; ttl <= max_ttl; ++ttl) 
    {
        bool is_final = false;
        for(int _ = 0; _ < 3; _++) {
            setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
            
            struct icmphdr icmp_hdr;
            create_packet(&icmp_hdr, ttl);

            auto start = high_resolution_clock::now();

            if (sendto(sockfd, &icmp_hdr, sizeof(icmp_hdr), 0, 
                (struct sockaddr*)&dest_addr, sizeof(dest_addr)) <= 0) {
                std::cerr << "Send error" << std::endl;
                return 1;
            }

            struct sockaddr_in response_addr;
            socklen_t addr_len = sizeof(response_addr);
            char buffer[512];
            struct timeval tv;
            tv.tv_sec = timeout;
            tv.tv_usec = 0;
            setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

            int recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&response_addr, &addr_len);
            if (recv_len > 0) {
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<std::chrono::duration<double, std::milli>>(end - start).count();

                struct iphdr *ip_hdr = (struct iphdr *)buffer;
                int ip_header_len = ip_hdr->ihl * 4;
                struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + ip_header_len);

                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &response_addr.sin_addr, ip, INET_ADDRSTRLEN);

                if (icmp_hdr->type == ICMP_ECHOREPLY) {
                    is_final = true;
                    if (!_)
                        std::cout << "  " << ttl << "   " << ip << "  " << std::fixed << std::setprecision(3) << duration << "ms";
                    else
                        std::cout << "  " << std::fixed << std::setprecision(3) << duration << "ms";
                    if (_ == 2)
                        std::cout << "\n";
                } else if (icmp_hdr->type == ICMP_TIME_EXCEEDED) {
                    if (!_)
                        std::cout << "  " << ttl << "   " << ip << "  " << std::fixed << std::setprecision(3) << duration << "ms";
                    else
                        std::cout << "  " << std::fixed << std::setprecision(3) << duration << "ms";
                    if (_ == 2)
                        std::cout << "\n";
                }
            } else {
                if (!_)
                    std::cout << "  " << ttl << "   *";  // 没有接收到回复时，输出星号
                else
                    std::cout << "  *";
                if (_ == 2)
                    std::cout << "\n";
            }
        }
        if(is_final) {
            break;  // 当is_final为true时，跳出循环      
        }
    }

    close(sockfd);
    return 0;
}
