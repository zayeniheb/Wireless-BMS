#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_wifi_types_generic.h"
#include "nvs_flash.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "driver/adc.h"
#include <math.h>
#include "ads1115.h"
#include "ads1115.c" 
#include "driver/i2c.h"
#include "lwip/ip4_addr.h"
#include "driver/gpio.h"
#define PORT 1234
#define UART_NUM UART_NUM_0
#define BUF_SIZE (1024)
#define NTC_PIN 34
#define ACS_PIN 36
#define LOAD_PIN 17
#define CHARGE_PIN 23
#define CELL1_PIN 32
#define CELL2_PIN 33
#define CELL3_PIN 3
#define SERIES_RESISTOR 10000  
#define NOMINAL_RESISTANCE 10000 
#define NOMINAL_TEMPERATURE 25.0 
#define BETA_COEFFICIENT 2600 
#define ADC_MAX 4095 
#define VREF 3.3 
 float current;
int n;
char name[30]="ESP32_AP2";
float Vmin=2.5;
float Vmax=4.2;
float Vnominal=3.6;
float CEEL1_TABLE[5];
float CEEL2_TABLE[5];
float offset=2.33;
float voltage1; 
float voltage2; 
float voltage3; 
float voltage4; 
float SOC1;
float SOC2;
float SOH1;
float SOH2;
float SOH3;
float SOC3;
 int Server_sock;
float temperature;
float vol;
float vol_current;
int a=2;
int adc_value;
int adc_value_current;
char rx_buffer[128];
uint8_t mac[6];
char Tx_buffer[2000];
char Tx_msg1[200];
char Tx_msg11[200];
char Tx_msg2[200];
char Tx_msg22[200];
float sensetivity=0.066;
int i=0;
int j=0;

void gpio_configuration(){
	
	gpio_config_t IO={
		  .pin_bit_mask = (1ULL << LOAD_PIN)|(1ULL << CHARGE_PIN)|(1ULL << CELL1_PIN)|(1ULL << CELL2_PIN)|(1ULL << CELL3_PIN),    // Which pin(s)
        .mode = GPIO_MODE_OUTPUT,               // Output mode
        .pull_up_en = GPIO_PULLUP_ENABLE,      // No pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // No pull-down
        .intr_type = GPIO_INTR_DISABLE 
		
		
	
};
gpio_config(&IO);
}

static int create_server_socket() {
    struct sockaddr_in server_addr;
    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listen_sock < 0) {
       printf("Unable to create socket\n");
       return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        printf("Socket unable to bind\n");
        close(listen_sock);
        return -1;
    }

    if (listen(listen_sock, 1) != 0) {
        printf("Error during listen\n");
        close(listen_sock);
        return -1;
    }

    return listen_sock;
}

static int accept_client_connection(int listen_sock) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sock < 0) {
        printf("Unable to accept connection\n");
    } else {
        printf("ESP_B connected!\n");
    }
    return client_sock;
}
static void send_task(void *pvParameters){
	  ads1115_t ads = ads1115_config(I2C_NUM_0, 0x48);
	  while (1) {
	 /////////////////IP AND MAC ////////////////////////////////////////////
esp_netif_ip_info_t ip;
esp_netif_t* network_interface = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
esp_netif_get_ip_info(network_interface, &ip);
esp_wifi_get_mac(WIFI_IF_AP, mac); 
          
           
          ////////////////// //NTC CALCULATION/////////////////////////////////////////
   	adc1_config_width(ADC_WIDTH_BIT_12); // 12-bit resolution (0-4095)
   	adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_12);
adc_value = adc1_get_raw(ADC1_CHANNEL_6); 
 vol= adc_value * (VREF / ADC_MAX);
temperature = fabs((1.0 / ((log(SERIES_RESISTOR * (VREF / vol - 1) / NOMINAL_RESISTANCE) / BETA_COEFFICIENT) + (1.0 / (NOMINAL_TEMPERATURE + 273.15)))) - 273.15)/1.5;		

/////////////////////////////VOLTAGE CALCULATIO//////////////////////////////////
 

  ads.config.bit.MUX = ADS1115_MUX_0_1;
   ads.config.bit.OS = 1;
   ads.changed = 1;
    	int16_t raw_value = ads1115_get_raw(&ads);
   vTaskDelay(pdMS_TO_TICKS(10));
   voltage1 = fabs(ads1115_get_voltage(&ads));
  /*ads.config.bit.MUX = ADS1115_MUX_0_3;
   ads.config.bit.OS = 1;
   ads.changed = 1;
   raw_value = ads1115_get_raw(&ads);
 vTaskDelay(pdMS_TO_TICKS(10));
   voltage4= fabs(ads1115_get_voltage(&ads));
     //ads.config.bit.MUX = ADS1115_MUX_2_3;
  //ads.config.bit.OS = 1;
   //ads.changed = 1;
     //raw_value = ads1115_get_raw(&ads);
     //vTaskDelay(pdMS_TO_TICKS(10));
    //voltage3 = fabs(ads1115_get_voltage(&ads));
  //raw_value = ads1115_get_raw(&ads);
   ads.config.bit.MUX = ADS1115_MUX_1_3;
   ads.config.bit.OS = 1;
   ads.changed = 1;
 raw_value = ads1115_get_raw(&ads);
  vTaskDelay(pdMS_TO_TICKS(10));
voltage2 = fabs(ads1115_get_voltage(&ads));
  //printf("voltage1:%f\n",voltage1);
    // printf("voltage2:%f\n",voltage2);
     // printf("voltage3:%f\n",voltage3);*/
   
		 CEEL1_TABLE[i]=voltage1;
		 CEEL2_TABLE[i]=voltage2;
		 i++;
		 //if (i>=5){i=0;}
	 if (voltage1<1.5 && voltage2<1.5){
		 n=0;
		voltage1=0;
		 voltage1=0;
	 }
	 
	 else if (voltage1 >1.5 && voltage2<1.5){
		 n=1;
		 voltage2=0;
	 }
	 else if(voltage1 <1.5 && voltage2>1.5) {
		 n=1;
		 voltage1=0;
	 }
	 else if ((voltage2-voltage1)<=0.1){
		 voltage1=0;
		 n=1;
	 }
	 else {
		 n=2;
	 }
	SOC1=(voltage1-Vmin )/(Vmax-Vnominal);
	SOC2=(voltage2-Vmin )/(Vmax-Vnominal);
	SOH1=voltage1/Vnominal;
	SOH2=voltage1/Vnominal;
     /////////////////////////current calculation //////////////////////
       
       	adc1_config_width(ADC_WIDTH_BIT_12); // 12-bit resolution (0-4095)
       		adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
adc_value_current = adc1_get_raw(ADC1_CHANNEL_0); 
 vol_current= adc_value_current * (VREF / ADC_MAX);
  current = (vol_current-offset) / sensetivity;
 	         if (current < 0) current = 0;
 
     ////////////////////////////////WARNINGS!!!!!!!!!!!!!//////////////////////////////////////
       if (voltage1<1){
		  strcpy(Tx_msg1," not connected" ) ;
		  }
		else if (voltage1 >=4.2) {
			
		     strcpy(Tx_msg1," over voltage" ) ;
		     
		} 
		  
		else if (voltage1 <=2.17) {
			
		     strcpy(Tx_msg1," under voltage" ) ;
		     
		} 
	
	   else { strcpy(Tx_msg1," connected and stable" );
	   
	   }
	   
	   
	    
}else{strcpy(Tx_msg11," " ) ;

voltage4=voltage1;
	////////////////////////////////SEND_MESSAGE/////////////////////////////////////////
	//voltage3=0;
	//SOH3=voltage3/Vnominal;
	//SOC3=(voltage3-Vmin )/(Vmax-Vnominal);
	//float voltage_all=fabs(voltage4+voltage3);
   sprintf( Tx_buffer,"%s, %d.%d.%d.%d , %02X:%02X:%02X:%02X:%02X:%02X ,%d,%.2f  ,%f %f , %f ,%f ,%f,%f,%f,%f\n",
     name,IP2STR(&ip.ip), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],n ,temperature, current,voltage4,voltage1,SOC1,SOH1,voltage2,SOC2,SOH2);
    printf("voltage1:%f\n",voltage1);
     printf("voltage2:%f\n", voltage2);
     
      printf("current:%f\n", current);
       printf("voltage_current:%f\n",vol_current);
       
      printf("voltage_ntc:%f\n",vol);
            printf("tempreture"":%f\n",temperature);
      int err = send(Server_sock,  Tx_buffer, strlen( Tx_buffer), 0);
      
  
        if (err < 0) { printf("Client disconnected or send failed, closing socket.\n");
       // Exit task
       break;
       
        }
        vTaskDelay(1000/ portTICK_PERIOD_MS);
       }

//vTaskDelay(1000/ portTICK_PERIOD_MS);

close(Server_sock);
          vTaskDelete(NULL); 
        
}

static void receive_task(void*pvParameters){
	//while (1) {
/////////////////////////////// //RECEIVE_MESSAGE///      ////////////////////////////////
            int len = recv(Server_sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
    if (len > 0) {
        rx_buffer[len] = 0;
        printf(" %s\n\r", rx_buffer);
        if (strcmp(rx_buffer,"LOAD_ON")==0){
			  gpio_set_level(LOAD_PIN, 0);
			  // vTaskDelay(1000);
			// gpio_set_level(LOAD_PIN, 0);
		}else if (strcmp(rx_buffer,"LOAD_OFF")==0){
			 gpio_set_level(LOAD_PIN, 1);
		}//else if (strcmp(rx_buffer,"LOAD_ON")==0){
			// gpio_set_level(LOAD_PIN, 0);
		//}
		else if (strcmp(rx_buffer,"CHARGER_OFF")==0){
			 gpio_set_level(CHARGE_PIN, 1);
		}else if (strcmp(rx_buffer,"CHARGER_ON")==0){
			 gpio_set_level(CHARGE_PIN, 0);
			 // vTaskDelay(1000);
			  //gpio_set_level(CHARGE_PIN, 0);
		}else if (strcmp(rx_buffer,"CELL1_OFF")==0){
			 gpio_set_level(CELL1_PIN, 1);	
			 }else if (strcmp(rx_buffer,"CELL1_ON")==0){
			 gpio_set_level(CELL1_PIN, 0);}
			 else if (strcmp(rx_buffer,"CELL2_OFF")==0){
			 gpio_set_level(CELL2_PIN, 0);	
			 }else if (strcmp(rx_buffer,"CELL2_ON")==0){
			 gpio_set_level(CELL2_PIN, 1);}
			 else if (strcmp(rx_buffer,"CELL3_OFF")==0){
			 gpio_set_level(CELL3_PIN, 0);	
			 }else if (strcmp(rx_buffer,"CELL3_ON")==0){
			 gpio_set_level(CELL3_PIN, 1); 
    }
    } else if (len == 0){
        printf("wait for data\n");
             
    }else { printf("no data\n");
    //break;
    }
    
vTaskDelay(1000 / portTICK_PERIOD_MS);
   
//}
close(Server_sock);
       vTaskDelete(NULL);

	}

static void server_task(void *pvParameters) {
	    int listen_sock = create_server_socket();
    if (listen_sock < 0) {
       vTaskDelete(NULL);  
    }
    while (1) {

			// gpio_set_level(	CHARGE_PIN, 1);
		//vTaskDelay(1000 / portTICK_PERIOD_MS);
		 //printf("chargeur on\n");
			 //gpio_set_level(CHARGE_PIN, 0);

    printf("Server listening on port %d\n", PORT);
   
        Server_sock = accept_client_connection(listen_sock);
        if (Server_sock >= 0) {
		 xTaskCreatePinnedToCore(send_task,"send_task",4096,NULL,5,NULL,1);
		xTaskCreatePinnedToCore(receive_task,"receive_task",4096,NULL,5,NULL,1);
		//close(Server_sock);
//break;


    }

    //close(listen_sock);
}
}


void wifi_init_ap(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .ap = {
        .ssid = "ESP32_AP2",
        .password = "12345678",
        .max_connection = 4,
        .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

   xTaskCreatePinnedToCore(server_task, "server_task", 4096, NULL, 5, NULL,0);
}

void app_main(void) {
	i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = GPIO_NUM_21,
    .scl_io_num = GPIO_NUM_22,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 100000,
};
i2c_param_config(I2C_NUM_0, &conf);
i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
gpio_configuration();
	  gpio_install_isr_service(0);
	 
    wifi_init_ap();
}