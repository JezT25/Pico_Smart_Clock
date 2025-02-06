/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef WIFI_h
#define WIFI_h

#include "setup.hpp"

#define WIFI_SSID0      "Vodafone-7zgp"
#define WIFI_PASS0      "yrdCy2x4"
#define WIFI_SSID1      "JezT 15 Pro"
#define WIFI_PASS1      "lil6ix9ineZ"

#define API_KEY         "DF9P5Y9XLY8AQ73L"
#define HTTP_SERVER     "api.thingspeak.com"
#define HTTP_PORT       80
#define PAYLOAD_SIZE    1024
#define CONNECT_TIMEOUT 30000

class WIFI_class {
    private:
        ip_addr_t resolved_ip;
        static char payload[PAYLOAD_SIZE];
        static struct tcp_pcb *client_pcb;
        
        void setPayload(IDATA IData);
        static err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
        static err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len);
        static err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
        static err_t dns_callback(const char *hostname, const ip_addr_t *ipaddr, void *arg);

    public:
        bool Initialize();
        inline void Poll();
        err_t updateCloud(IDATA IData);
};

#endif
