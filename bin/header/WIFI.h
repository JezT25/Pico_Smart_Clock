/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#ifndef WIFI_h
#define WIFI_h

#include "../setup.hpp"

#define WIFI_SSID       "Vodafone-7zgp"
#define WIFI_PASS       "yrdCy2x4"
#define API_KEY         "7YY865RJIK8NIQJ2"
#define HTTP_SERVER     "api.thingspeak.com"
#define HTTP_PORT       80
#define PAYLOAD_SIZE    1024
#define CONNECT_REFRESH 20000
#define CONNECT_TIMEOUT 30000

class WIFI_class {
    private:
        ip_addr_t resolved_ip;
        uint32_t updatewifi_lpt;
        static char payload[PAYLOAD_SIZE];
        static struct tcp_pcb *client_pcb;
        
        void setPayload(IDATA IData);
        static err_t tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
        static err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
        static bool dns_callback(const char *hostname, const ip_addr_t *ipaddr, void *arg);

    public:
        bool Initialize();
        bool updateCloud(IDATA IData);
};

#endif
