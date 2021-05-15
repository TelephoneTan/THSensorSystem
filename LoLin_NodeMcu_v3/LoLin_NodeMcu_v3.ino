#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <stdint.h>
#include "ESPAsyncTCP.h"
#include "SyncClient.h"
#include "httpparser.h"
#include "vutils.h"

#define GET_BAIDU "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n"
#define LOGIN_MSG_HEAD "GET /eportal/portal/login\?"
#define LOGIN_MSG_TAIL " HTTP/1.0\r\n"
#define DEFAULT_QUERY "callback=dr1003&login_method=1&wlan_user_ipv6=&jsVersion=4.1&terminal_type=1&lang=zh-cn&v=2624&lang=zh"
#define LOGIN_AC_NAME "wlan_ac_name=HJ-BRAS-NE20E-S8"
#define DEFAULT_LOGIN_HOST "10.0.1.5"
#define DEFAULT_LOGIN_PORT 801

#define NETWORK_LOGIN_ID "1800301129"
#define NETWORK_LOGIN_PWD "Abc965827"

#define SerialLog Serial1
#define SerialData Serial

const char* ssid[] = {
  "Redmi10X",
  "GUET-Teacher",
  "GUET-Student"
};
const char* password[] = {
  "ilovezhcn",
  NULL,
  NULL
};

String local_ip;
String mac_address;
String mac_address_no_delimiter;
String gateway_ip;

void hw_wdt_disable(){
  *((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF
}

void hw_wdt_enable(){
  *((volatile uint32_t*) 0x60000900) |= 1; // Hardware WDT ON
}

void ensure_wifi(){
    int wifi_nums = sizeof(ssid) / sizeof(const char *);
    while(WiFi.status() != WL_CONNECTED){
        SerialLog.printf("WiFi Not Connected, loop...\n");
        for(int i = 0; i < wifi_nums; i++){
            WiFi.begin(ssid[i], password[i]);
            // feed hardware WDT
            ESP.wdtFeed();
            // connect timeout for each try: 5s
            int8_t cnt_res = WiFi.waitForConnectResult(5000);
            // feed hardware WDT
            ESP.wdtFeed();
            if (cnt_res == WL_CONNECTED) {
                SerialLog.printf("WiFi Connected to: %s\n", ssid[i]);
                break;
            }
        }
    }
    local_ip = WiFi.localIP().toString();
    mac_address = WiFi.macAddress();
    mac_address_no_delimiter = WiFi.macAddress();
    mac_address_no_delimiter.replace(":", "");
    gateway_ip = WiFi.gatewayIP().toString();
    SerialLog.printf("WiFi local ip: ");
    SerialLog.println(local_ip);
    SerialLog.printf("WiFi mac address: ");
    SerialLog.println(mac_address);
    SerialLog.printf("WiFi mac address(no delimiter): ");
    SerialLog.println(mac_address_no_delimiter);
    SerialLog.printf("WiFi gateway ip: ");
    SerialLog.println(gateway_ip);
}

typedef struct generator_params {
  char* buf;
  int read_size;
  int buf_size;
} generator_params;

char generator(void* params_p, int* continue_flag_p) {
  generator_params* gpp = (generator_params*)params_p;
  *continue_flag_p = (gpp->buf_size - gpp->read_size > 0);
  return *continue_flag_p ? gpp->buf[gpp->read_size++] : '\?';
}

int printHttpHeader(vlist this_vlist, long i, void* extra) {
  const HttpHeader* header = (const HttpHeader*)(this_vlist->get_const(this_vlist, i));
  SerialLog.printf("%s: %s\n", header->field, header->value);
  return 0; // go on
}

void login(const char *host, int port, const char *query_string, int no_replace){
    SyncClient client_login;
    for(int i = 0; i < 3 && !(client_login.connected()); i++){
        client_login.connect(host, port);
    }
    if(!(client_login.connected())){
        SerialLog.printf("Failed to connect to network login host[%s]:port[%d]\n", host, port);
        return;
    }
    SerialLog.printf("Connected to network login host[%s]:port[%d]\n", host, port);
    SerialLog.printf("@@@@@@@@@@@@@@@@@@@@ Login Get Sending... @@@@@@@@@@@@@@@@@@@@\n");
    /* Be careful of the URL encoding! */
    String query_ip(query_string ? query_string : "");
    if(!no_replace){
        query_ip.replace("wlanuserip", "wlan_user_ip");
        query_ip.replace("wlanacname", "wlan_ac_name");
        query_ip.replace("wlanacip", "wlan_ac_ip");
        query_ip.replace("wlanusermac", "wlan_user_mac");
        query_ip.replace("-", "");
        query_ip.replace(":", "%3A");
        query_ip.replace("/", "%2F");
    }
    String login_msg = String(LOGIN_MSG_HEAD) + String(DEFAULT_QUERY)
        + String("&") + String(LOGIN_AC_NAME)
        + String("&") + query_ip
        + String("&") + String(LOGIN_AC_NAME)
        + String("&") + String("user_account=%2C0%2C") + String(NETWORK_LOGIN_ID)
        + String("&") + String("user_password=") + String(NETWORK_LOGIN_PWD)
        + String(LOGIN_MSG_TAIL)
        + String("Host: ") + String(host) + String(":") + String(port) + String("\r\n")
        + String("User-Agent: insomnia%2F2021.3.0") + String("\r\n")
        + String("\r\n")
        ;
    SerialLog.println(login_msg);
    if(client_login.write((const uint8_t*)(login_msg.c_str()), strlen(login_msg.c_str())) > 0){
        SerialLog.printf("@@@@@@@@@@@@@@@@@@@@@@ Login Get Sent @@@@@@@@@@@@@@@@@@@@@@\n");
        /* Debug Code */
        long wait_times = 0;
        while(client_login.connected() && client_login.available() == 0 && wait_times++ < 300){
            // feed hardware WDT
            ESP.wdtFeed();
            delay(1);
        }
        while(client_login.available()){
            SerialLog.write(client_login.read());
        }
        SerialLog.printf("\n");
        SerialLog.printf("VVVVVVVVVVVVVVVVVVVV Login Get Response End VVVVVVVVVVVVVVVVVVVV\n");
        /* Debug Code */
    }else{
        SerialLog.printf("XXXXXXXXXXXXXXXXXXXXXX Login Get Unsent XXXXXXXXXXXXXXXXXXXXXX\n");
    }
}

String generate_default_query_string(){
    return 
    String("wlan_user_ip=") + local_ip 
    + String("&wlan_ac_ip=") + gateway_ip
    + String("&wlan_user_mac=") + mac_address_no_delimiter
    ;
}

int is_network_ok()
{
  int res = 0;
  // this pre-login may help
  login(DEFAULT_LOGIN_HOST, DEFAULT_LOGIN_PORT, generate_default_query_string().c_str(), 1);
  SyncClient client;
  if(!client.connect("www.baidu.com", 80)){
    SerialLog.printf("Connecting Baidu Failed\n");
    return res;
  }
  SerialLog.printf("Connected to Baidu\n");
  client.setTimeout(2);
  SerialLog.printf("test_client uses 2s timeout\n");
  if(client.write((const uint8_t*)GET_BAIDU, strlen(GET_BAIDU)) <= 0){
    SerialLog.printf("Failed to send GET_BAIDU\n");
    return res;
  }
  SerialLog.printf("GET_BAIDU sent, waiting for response...\n");
  long wait_times = 0;
  while(client.connected() && !client.available() && wait_times++ < 300){
    // feed hardware WDT
    ESP.wdtFeed();
    SerialLog.printf("waiting for response...\n");
    delay(1); // this delay may be important, it is from the example
  }
  char temp[2048] = {0};
  int len = 0;
  read_again:
  for(int i = 0; i < 300; i++){
    // feed hardware WDT
    ESP.wdtFeed();
    if(client.available() && len < sizeof(temp) - 1){
      temp[len++] = client.read();
      goto read_again;
    }
  }
  if(!len){
    SerialLog.printf("Failed to get response of GET_BAIDU\n");
    return res;
  }
  char* message = NULL;
  HttpMethod method = INVALID_METHOD;
  generator_params gp = {
    .buf = temp,
    .read_size = 0,
    .buf_size = len
  };
  int nres = next_http_message(&method, &message, generator, &gp, 1);
  if(nres < 0){
    SerialLog.printf("next_http_message() = %d\n", nres);
    SerialLog.printf("#### RAW Response from Baidu: ####\n");
    SerialLog.printf("%s\n", temp);
    SerialLog.printf("##################################\n");
    if(
      strstr(temp, "HTTP/1.1 200") ||
      strstr(temp, "http/1.1 200") ||
      strstr(temp, "Http/1.1 200")
    ){
      handle_200:
      SerialLog.printf("[ 200 OK ]\n");
      res = 1;
      return res;
    }else if(
      strstr(temp, "HTTP/1.1 302") ||
      strstr(temp, "http/1.1 302") ||
      strstr(temp, "Http/1.1 302")
    ){
      unhandled_302:
      SerialLog.printf("[ 302 Unhandled ]\n");
      return res;
    }else{
      unknown_status_code_from_baidu:
      SerialLog.printf("[ Status Unknown ]\n");
      return res;
    }
  }else{
    SerialLog.printf("#### RAW Response from Baidu: ####\n");
    SerialLog.printf("%s\n", temp);
    SerialLog.printf("##################################\n");
    SerialLog.printf("---- HTTP Response from Baidu: ----\n");
    SerialLog.printf("%s\n", message);
    SerialLog.printf("-----------------------------------\n");
    if(
      strstr(message, "HTTP/1.1 200") ||
      strstr(message, "http/1.1 200") ||
      strstr(message, "Http/1.1 200")
    ){
      free(message);
      goto handle_200;
    }
    HttpMessage hmsg = parse_http_message(message, 1);
    free(message);
    if(!(hmsg.malloc_success)){
      SerialLog.printf("[ Parsed HTTP Message ] <Malloc Fail>\n");
      freeHttpMessage(&hmsg);
      return res;
    }else if(!(hmsg.success)){
      SerialLog.printf("[ Parsed HTTP Message ] <Parse Fail> <%s><%s>\n", hmsg.error_name, hmsg.error_reason);
      freeHttpMessage(&hmsg);
      return res;
    }
    SerialLog.printf("[ Parsed HTTP Message ] HTTP/%d.%d %ld content_length=%ld\n", 
      hmsg.http_major, 
      hmsg.http_minor,
      hmsg.status_code,
      hmsg.content_length
    );
    if(hmsg.http_headers){
      hmsg.http_headers->foreach(hmsg.http_headers, printHttpHeader, NULL);
    }
    if(hmsg.status_code == 302){
      if(!(hmsg.location)){
        freeHttpMessage(&hmsg);
        goto unhandled_302;
      }
      SerialLog.printf("[ 302 Location ] %s\n", hmsg.location);
      UrlMeta um = parse_url(hmsg.location);
      if(!(um.valid)){
        freeHttpMessage(&hmsg);
        goto unhandled_302;
      }
      SerialLog.printf("******[ 302 Detail ]******"
      "\nValid=%d"
      "\nProtocol=%s"
      "\nHost=%s"
      "\nPort=%ld"
      "\nPath=%s"
      "\nQuery=%s"
      "\nFragment=%s"
      "\n***********************\n",
      um.valid,
      um.protocol,
      um.host,
      um.port,
      um.path_start ? um.path_start : "/",
      um.query_start ? um.query_start : "",
      um.fragment_start ? um.fragment_start : ""
      );
      // the port of 302 location is wrong, use default port instead
      login(um.host, DEFAULT_LOGIN_PORT, um.query_start, 0);
      freeHttpMessage(&hmsg);
      return res;
    }else if(hmsg.status_code == 200){
      freeHttpMessage(&hmsg);
      goto handle_200;
    }else{
      freeHttpMessage(&hmsg);
      goto unknown_status_code_from_baidu;
    }
  }
}

void ensure_network(){
  ensure_wifi();
  while(!is_network_ok()){
    SerialLog.printf("Network Not OK, loop...\n");
  }
  SerialLog.printf("Network OK >>>>>>>>>>>>\n");
}

void reboot_machine(){
    SerialLog.printf(">>>>>>>>> ** Reboot Machine ** <<<<<<<<<\n");
    ESP.restart();
}

void reboot_if_wifi_connected_else_ensure_wifi(){
    if(WiFi.status() == WL_CONNECTED){
        SerialLog.printf("<<<<<<<<<<<<<< WiFi Connected, reboot...\n");
        reboot_machine();
    }else{
        SerialLog.printf(">>>>>>>>>>>>>> WiFi Not Connected, ensure_wifi...\n");
        ensure_wifi();
    }
}

void setup(){
    // SerialLog could transmit log from functions to PC
    SerialLog.setDebugOutput(true);
    // SerialData is used to receive data from STM32
    SerialData.begin(115200);
    // SerialLog is used to send log to PC via CP2102
    SerialLog.begin(115200);

    // disable software watchdog
    ESP.wdtDisable();

    // print empty line
    SerialLog.printf("\n\n");

    // ensure_network() should only be called from setup() due to its expensive time cost
    ensure_network();
}

void loop(){
    // feed hardware WDT
    ESP.wdtFeed();
}
