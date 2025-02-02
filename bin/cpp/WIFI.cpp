/************************
	    Jezreel Tan
	    jvt6@hi.is
	    820 7803
************************/

#include "setup.hpp"

char WIFI_class::payload[PAYLOAD_SIZE] = {};
struct tcp_pcb *WIFI_class::client_pcb = nullptr;

bool WIFI_class::Initialize()
{
    if (cyw43_arch_init())
    {   
        #if VERBOSE
            printf("Wi-Fi init failed\n");
        #endif
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    #if VERBOSE
        printf("Connecting to Wi-Fi...\n");
    #endif

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, CONNECT_TIMEOUT))
    {
        #if VERBOSE
            printf("Failed to connect.\n");
        #endif
        return 1;
    }

    return 0;
}

inline void WIFI_class::Poll()
{
    cyw43_arch_poll();
}


bool WIFI_class::updateCloud(IDATA IData)
{
    setPayload(IData);
    dns_gethostbyname(HTTP_SERVER, &resolved_ip, NULL, NULL);
    return dns_callback(HTTP_SERVER, &resolved_ip, NULL);
}

void WIFI_class::setPayload(IDATA IData)
{
    snprintf(payload, sizeof(payload), 
        "GET /update?api_key=%s"
        "&field1=%f"
        "&field2=%f"
        "&field3=%f"
        " HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n\r\n", 
    API_KEY, IData.SENSOR_TEMP, IData.SENSOR_HUMIDITY, IData.SENSOR_PRESSURE, HTTP_SERVER);
}

err_t WIFI_class::tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        #if VERBOSE
            printf("Connection closed by server.\n");
        #endif
        tcp_close(tpcb);
        return ERR_OK;
    }
    #if VERBOSE
        printf("Received response:\n%.*s\n", p->tot_len, (char *)p->payload);
    #endif
    pbuf_free(p);
    return ERR_OK;
}

err_t WIFI_class::tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    if (err != ERR_OK) {
        #if VERBOSE
            printf("Failed to connect: %d\n", err);
        #endif
        return err;
    }
    #if VERBOSE
        printf("Connected to server.\n");
        printf("Payload: %s\n", payload);
    #endif

    // Send HTTP GET request
    tcp_write(tpcb, payload, strlen(payload), TCP_WRITE_FLAG_COPY);
    sleep_ms(500);
    tcp_output(tpcb);

    // Set receive callback
    tcp_recv(tpcb, tcp_recv_callback);
    return ERR_OK;
}

bool WIFI_class::dns_callback(const char *hostname, const ip_addr_t *ipaddr, void *arg)
{
    if (!ipaddr) {
        #if VERBOSE
            printf("DNS lookup failed.\n");
        #endif
        return 1;
    }

    #if VERBOSE
        printf("Resolved IP: %s\n", ip4addr_ntoa(ipaddr));
    #endif

    // Create TCP connection
    client_pcb = tcp_new();
    tcp_connect(client_pcb, ipaddr, HTTP_PORT, tcp_connected_callback);

    return 0;
}
