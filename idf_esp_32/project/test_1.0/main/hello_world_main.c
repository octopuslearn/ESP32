
#include <stdio.h>
#include "esp_system.h"

#include "esp_chip_info.h"
#include "esp_flash_spi_init.h"
#include "esp_log.h"

#include "esp_mac.h"

const char* hw = "硬件";
const char* sw = "软件";
void app_main(void)
{  
    ESP_LOGE(sw,"版本号： %s",esp_get_idf_version());//Miscellaneous system api


// typedef struct {
//     esp_chip_model_t model;  //!< chip model, one of esp_chip_model_t
//     uint32_t features;       //!< bit mask of CHIP_FEATURE_x feature flags
//     uint16_t revision;       //!< chip revision number (in format MXX; where M - wafer major version, XX - wafer minor version)
//     uint8_t cores;           //!< number of CPU cores
// } esp_chip_info_t;

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    ESP_LOGE(hw,"cpu核心数: %d",chip_info.cores);
    ESP_LOGE(hw,"cpu版本号:  %d",chip_info.revision);


//     /* Chip feature flags, used in esp_chip_info_t */
// #define CHIP_FEATURE_EMB_FLASH      BIT(0)      //!< Chip has embedded flash memory
// #define CHIP_FEATURE_WIFI_BGN       BIT(1)      //!< Chip has 2.4GHz WiFi
// #define CHIP_FEATURE_BLE            BIT(4)      //!< Chip has Bluetooth LE
// #define CHIP_FEATURE_BT             BIT(5)      //!< Chip has Bluetooth Classic
// #define CHIP_FEATURE_IEEE802154     BIT(6)      //!< Chip has IEEE 802.15.4
// #define CHIP_FEATURE_EMB_PSRAM      BIT(7)      //!< Chip has embedded psram

// /**

// ESP_LOGE(hw,"特征-CHIP_FEATURE_x 功能标志的位掩码:  %d",chip_info.features);//此处报错？？？

ESP_LOGE(hw,"Embedded Flash:  %s",chip_info.features & CHIP_FEATURE_EMB_FLASH ?"YES":"NO");//--->E (343) 硬件: Embedded Flash:  NO无外置Flash
ESP_LOGE(hw,"Embedded WIFI_BGN:  %s",chip_info.features & CHIP_FEATURE_WIFI_BGN ?"YES":"NO");//--->E (343) 硬件: Embedded Flash:  NO无外置Flash
ESP_LOGE(hw,"Embedded BLE:  %s",chip_info.features & CHIP_FEATURE_BLE ?"YES":"NO");//--->E (343) 硬件: Embedded Flash:  NO无外置Flash
ESP_LOGE(hw,"Embedded BT:  %s",chip_info.features & CHIP_FEATURE_BT ?"YES":"NO");//--->E (343) 硬件: Embedded Flash:  NO无外置Flash
ESP_LOGE(hw,"Embedded IEEE802154:  %s",chip_info.features & CHIP_FEATURE_IEEE802154 ?"YES":"NO");//--->E (343) 硬件: Embedded Flash:  NO无外置Flash
ESP_LOGE(hw,"Embedded PSRAM:  %s",chip_info.features & CHIP_FEATURE_EMB_PSRAM ?"YES":"NO");//--->E (343) 硬件: Embedded Flash:  NO无外置Flash
    // CHIP_FEATURE_WIFI_BGN

    // typedef enum {
    //     CHIP_ESP32  = 1, //!< ESP32
    //     CHIP_ESP32S2 = 2, //!< ESP32-S2
    //     CHIP_ESP32S3 = 9, //!< ESP32-S3
    //     CHIP_ESP32C3 = 5, //!< ESP32-C3
    //     CHIP_ESP32C2 = 12, //!< ESP32-C2
    //     CHIP_ESP32C6 = 13, //!< ESP32-C6
    //     CHIP_ESP32H2 = 16, //!< ESP32-H2
    //     CHIP_POSIX_LINUX = 999, //!< The code is running on POSIX/Linux simulator
    // } esp_chip_model_t;

    char* esp_type;
    switch(chip_info.model)
    {
        case CHIP_ESP32:
            esp_type = "CHIP_ESP32";
            break;

        case CHIP_ESP32S2:
            esp_type = "CHIP_ESP32S2";
            break;

        case CHIP_ESP32S3:
            esp_type = "CHIP_ESP32S3";
            break;

        case CHIP_ESP32C3:
            esp_type = "CHIP_ESP32C3";
            break;

        case CHIP_ESP32C2:
            esp_type = "CHIP_ESP32C2";
            break;

        case CHIP_ESP32C6:
            esp_type = "CHIP_ESP32C6";
            break;
        case CHIP_ESP32H2:
            esp_type = "CHIP_ESP32H2";
            break;
        
        default:
            esp_type = "UNKNOWN";
            break;
    }
    ESP_LOGE(hw,"ESP型号:  %s",esp_type);


    // ESP_LOGE(hw,"Flash 容量:  %d MBytes", spi_flash_get_chip_size()/(1024*1024));//为啥报错
    // ESP_LOGE(hw,"Flash 容量:   %d MBits", spi_flash_get_chip_size()*8/(1024*1024));//为啥报错
    
    

    
/*#############  MAC法1  #############*/
    // uint8_t mac_address;
    // esp_efuse_mac_get_default(&mac_address);//查看MAC地址，所谓MAC地址即联网设备物理地址，共计6个字节
    // ESP_LOGE(hw,"MAC adreess:   %x", mac_address);
    // ESP_LOGE(hw,"MAC adreess:   %x", *(&mac_address+1));
    // ESP_LOGE(hw,"MAC adreess:   %x", *(&mac_address+2));
    // ESP_LOGE(hw,"MAC adreess:   %x", *(&mac_address+3));
    // ESP_LOGE(hw,"MAC adreess:   %x", *(&mac_address+4));
    // ESP_LOGE(hw,"MAC adreess:   %x", *(&mac_address+5));
/*此方法，很危险，不建议使用。应该使用数组的方法*/
// E (385) 硬件: MAC adreess:   34
// E (389) 硬件: MAC adreess:   86
// E (393) 硬件: MAC adreess:   5d
// E (397) 硬件: MAC adreess:   ff
// E (401) 硬件: MAC adreess:   ac
// E (405) 硬件: MAC adreess:   48


/*#############  MAC法2  #############*/
uint8_t mac_address[6];
esp_efuse_mac_get_default(&mac_address);//查看MAC地址，所谓MAC地址即联网设备物理地址，共计6个字节
ESP_LOGE(hw, "mac adreess: %02x-%02x-%02x-%02x-%02x-%02x", mac_address[0],mac_address[1],mac_address[2],mac_address[3],mac_address[4],mac_address[5]);
/*此方法可行*/
//E (376) 硬件: mac adreess: 34-86-5d-ff-1d-b0
}   
