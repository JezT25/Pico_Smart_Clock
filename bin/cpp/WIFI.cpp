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
    const char* wifi_ssid = _set.SYS_WIFI_CON ? WIFI_SSID0 : WIFI_SSID1;
    const char* wifi_pass = _set.SYS_WIFI_CON ? WIFI_PASS0 : WIFI_PASS1;
    
    if (cyw43_arch_init())
    {   
        #if VERBOSE_WIFI
            printf("Wi-Fi init failed\n");
        #endif
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    #if VERBOSE_WIFI
        printf("Connecting to Wi-Fi...\n");
    #endif

    if (cyw43_arch_wifi_connect_timeout_ms(wifi_ssid, wifi_pass, CYW43_AUTH_WPA2_AES_PSK, CONNECT_TIMEOUT))
    {
        #if VERBOSE_WIFI
            printf("Failed to connect.\n");
        #endif
        return 1;
    }

    #if VERBOSE_WIFI
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    #endif

    return 0;
}

inline void WIFI_class::Poll()
{
    cyw43_arch_poll();
}

err_t WIFI_class::updateCloud(IDATA IData)
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

err_t WIFI_class::tcp_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (p == NULL) {
        #if VERBOSE_WIFI
            printf("Connection closed by server.\n");
        #endif
        tcp_close(tpcb);
        return ERR_OK;
    }
    #if VERBOSE_WIFI
        printf("Received response:\n%.*s\n", p->tot_len, (char *)p->payload);
    #endif
    pbuf_free(p);
    return ERR_OK;
}

err_t WIFI_class::tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    tcp_output(tpcb);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, OFF_SEGMENT);
    return ERR_OK;
}

err_t WIFI_class::tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    if (err != ERR_OK) {
        #if VERBOSE_WIFI
            printf("Failed to connect: %d\n", err);
        #endif
        return err;
    }
    #if VERBOSE_WIFI
        printf("Connected to server.\n");
        printf("Payload: %s\n", payload);
    #endif

    // Send HTTP GET request
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, ON_SEGMENT);

    err_t write_err = tcp_write(tpcb, payload, strlen(payload), TCP_WRITE_FLAG_COPY);
    if (write_err != ERR_OK) {
        #if VERBOSE_WIFI
            printf("TCP write error: %d\n", write_err);
        #endif
        return write_err;
    }

    tcp_sent(tpcb, tcp_sent_callback);
    tcp_recv(tpcb, tcp_recv_callback);
    return ERR_OK;
}

err_t WIFI_class::dns_callback(const char *hostname, const ip_addr_t *ipaddr, void *arg)
{
    if (!ipaddr) {
        #if VERBOSE_WIFI
            printf("DNS lookup failed.\n");
        #endif
        return ERR_ABRT;
    }

    #if VERBOSE_WIFI
        printf("Resolved IP: %s\n", ip4addr_ntoa(ipaddr));
    #endif

    // Create TCP connection
    client_pcb = tcp_new();
    return tcp_connect(client_pcb, ipaddr, HTTP_PORT, tcp_connected_callback);
}
