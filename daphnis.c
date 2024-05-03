//
// Created by RobertoMB on 5/2/2024.
// AT Commands for Daphnis-I
// https://www.we-online.com/components/products/manual/2618011181000_UM_Daphnis-I_2618011181000_V1_0_rev1.pdf
//

#include "daphnis.h"
#include <string.h>

/* AT commands in Daphnis-I are case sensitive.
* When an AT command has more than 1 parameter,
* then a comma (,) is used as delimiter to separate each parameter.
* */
at_status_t at_command(void) {
    return send_at_command("AT");
}

at_status_t at_reset(void) {
    return send_at_command("ATZ");
}
//The returned time is valid if you are in ClassB or have
// at least once switched to ClassB without resetting the module
at_status_t at_get_local_time(char *time, size_t time_size, char *date, size_t date_size) {
    at_status_t status = send_at_command("AT+LTIME=?");
    if (status == AT_OK) {
        char response[64];
        status = get_at_command_value("AT+LTIME=?", response, sizeof(response));
        if (status == AT_OK) {
            // Parse the response to extract time and date
            sscanf(response, "%[^:]:%[^,],%[^,]", time, date);
        }
    }
    return status;
}

at_status_t at_get_battery_level(uint16_t *battery_level) {
    at_status_t status = send_at_command("AT+BAT=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+BAT=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hu", battery_level);
        }
    }
    return status;
}

at_status_t at_sleep(void) {
    return send_at_command("AT+SLEEP");
}

void handle_sysnotf_event(const char *event) {
    if (strcmp(event, "READY") == 0) {
        // Handle the SYSNOTF event when the module is ready
        // ...
    }
}

void handle_cs_event(const char *event) {
    if (strcmp(event, "NVM_DATA_STORED") == 0) {
        // Handle the CS event when NVM data is stored
        // ...
    } else if (strcmp(event, "NVM_DATA_RESTORED") == 0) {
        // Handle the CS event when NVM data is restored
        // ...
    }
}

//The baud rate values are represented as integers (0, 1, 2)
// corresponding to the actual baud rates (9600, 57600, 115200).
at_status_t at_set_uart_baud_rate(uint8_t baud_rate) {
    char command[32];
    snprintf(command, sizeof(command), "AT+UARTBAUDRATE=%d", baud_rate);
    return send_at_command(command);
}

at_status_t at_get_uart_baud_rate(uint8_t *baud_rate) {
    at_status_t status = send_at_command("AT+UARTBAUDRATE=?");
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+UARTBAUDRATE=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", baud_rate);
        }
    }
    return status;
}
//The default UserSettings are only applied after the module has been reset.
at_status_t at_restore_user_settings(void) {
    return send_at_command("AT+RUS");
}

at_status_t at_store_context(void) {
    return send_at_command("AT+CS");
}
// This command restarts Daphnis-I after execution
at_status_t at_erase_context(void) {
    return send_at_command("AT+RFS");
}

at_status_t at_set_appeui(const char *appeui) {
    char command[64];
    snprintf(command, sizeof(command), "AT+APPEUI=%s", appeui);
    return send_at_command(command);
}

at_status_t at_get_appeui(char *appeui, size_t appeui_size) {
    at_status_t status = send_at_command("AT+APPEUI=?");
    if (status == AT_OK) {
        status = get_at_command_value("AT+APPEUI=?", appeui, appeui_size);
    }
    return status;
}
// This key can only be set but not read back due to security reasons
at_status_t at_set_appkey(const char *appkey) {
    char command[128];
    snprintf(command, sizeof(command), "AT+APPKEY=%s", appkey);
    return send_at_command(command);
}
// This key can only be set but not read back due to security reasons
at_status_t at_set_appskey(const char *appskey) {
    char command[128];
    snprintf(command, sizeof(command), "AT+APPSKEY=%s", appskey);
    return send_at_command(command);
}

at_status_t at_set_nwkskey(const char *nwkskey) {
    char command[128];
    snprintf(command, sizeof(command), "AT+NWKSKEY=%s", nwkskey);
    return send_at_command(command);
}

at_status_t at_set_device_address(const char *daddr) {
    char command[64];
    snprintf(command, sizeof(command), "AT+DADDR=%s", daddr);
    return send_at_command(command);
}

at_status_t at_get_device_address(char *daddr, size_t daddr_size) {
    at_status_t status = send_at_command("AT+DADDR=?");
    if (status == AT_OK) {
        status = get_at_command_value("AT+DADDR=?", daddr, daddr_size);
    }
    return status;
}

at_status_t at_set_device_eui(const char *deui) {
    char command[64];
    snprintf(command, sizeof(command), "AT+DEUI=%s", deui);
    return send_at_command(command);
}

at_status_t at_get_device_eui(char *deui, size_t deui_size) {
    at_status_t status = send_at_command("AT+DEUI=?");
    if (status == AT_OK) {
        status = get_at_command_value("AT+DEUI=?", deui, deui_size);
    }
    return status;
}

at_status_t at_set_network_id(uint8_t nwkid) {
    char command[32];
    snprintf(command, sizeof(command), "AT+NWKID=%d", nwkid);
    return send_at_command(command);
}

at_status_t at_get_network_id(uint8_t *nwkid) {
    at_status_t status = send_at_command("AT+NWKID=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+NWKID=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", nwkid);
        }
    }
    return status;
}

at_status_t at_get_version(char *firmware_version, size_t firmware_version_size,
                           char *linklayer_version, size_t linklayer_version_size,
                           char *regional_params_version, size_t regional_params_version_size) {
    at_status_t status = send_at_command("AT+VER=?");
    if (status == AT_OK) {
        char response[64];
        status = get_at_command_value("AT+VER=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%[^,],%[^,],%[^,]", firmware_version, linklayer_version, regional_params_version);
        }
    }
    return status;
}
// This command can be used only if adaptive data rate is off
at_status_t at_set_adaptive_data_rate(uint8_t adr) {
    char command[32];
    snprintf(command, sizeof(command), "AT+ADR=%d", adr);
    return send_at_command(command);
}

at_status_t at_get_adaptive_data_rate(uint8_t *adr) {
    at_status_t status = send_at_command("AT+ADR=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+ADR=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", adr);
        }
    }
    return status;
}

at_status_t at_set_data_rate(uint8_t dr) {
    char command[32];
    snprintf(command, sizeof(command), "AT+DR=%d", dr);
    return send_at_command(command);
}

at_status_t at_get_data_rate(uint8_t *dr) {
    at_status_t status = send_at_command("AT+DR=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+DR=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", dr);
        }
    }
    return status;
}
// Resetting the module or entering Sleep mode will cause the module to reset the calculated time on air
at_status_t at_set_duty_cycle_restriction(uint8_t dcs) {
    char command[32];
    snprintf(command, sizeof(command), "AT+DCS=%d", dcs);
    return send_at_command(command);
}

at_status_t at_get_duty_cycle_restriction(uint8_t *dcs) {
    at_status_t status = send_at_command("AT+DCS=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+DCS=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", dcs);
        }
    }
    return status;
}

at_status_t at_set_join_accept_delay1(uint32_t delay) {
    char command[32];
    snprintf(command, sizeof(command), "AT+JN1DL=%lu", delay);
    return send_at_command(command);
}

at_status_t at_get_join_accept_delay1(uint32_t *delay) {
    at_status_t status = send_at_command("AT+JN1DL=?");
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+JN1DL=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%lu", delay);
        }
    }
    return status;
}

at_status_t at_set_join_accept_delay2(uint32_t delay) {
    char command[32];
    snprintf(command, sizeof(command), "AT+JN2DL=%lu", delay);
    return send_at_command(command);
}

at_status_t at_get_join_accept_delay2(uint32_t *delay) {
    at_status_t status = send_at_command("AT+JN2DL=?");
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+JN2DL=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%lu", delay);
        }
    }
    return status;
}

at_status_t at_set_rx1_delay(uint32_t delay) {
    char command[32];
    snprintf(command, sizeof(command), "AT+RX1DL=%lu", delay);
    return send_at_command(command);
}

at_status_t at_get_rx1_delay(uint32_t *delay) {
    at_status_t status = send_at_command("AT+RX1DL=?");
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+RX1DL=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%lu", delay);
        }
    }
    return status;
}

at_status_t at_set_rx2_delay(uint32_t delay) {
    char command[32];
    snprintf(command, sizeof(command), "AT+RX2DL=%lu", delay);
    return send_at_command(command);
}

at_status_t at_get_rx2_delay(uint32_t *delay) {
    at_status_t status = send_at_command("AT+RX2DL=?");
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+RX2DL=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%lu", delay);
        }
    }
    return status;
}

at_status_t at_set_rx2_data_rate(uint8_t data_rate) {
    char command[32];
    snprintf(command, sizeof(command), "AT+RX2DR=%d", data_rate);
    return send_at_command(command);
}

at_status_t at_get_rx2_data_rate(uint8_t *data_rate) {
    at_status_t status = send_at_command("AT+RX2DR=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+RX2DR=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", data_rate);
        }
    }
    return status;
}

at_status_t at_set_rx2_frequency(uint32_t frequency) {
    char command[32];
    snprintf(command, sizeof(command), "AT+RX2FQ=%lu", frequency);
    return send_at_command(command);
}

at_status_t at_get_rx2_frequency(uint32_t *frequency) {
    at_status_t status = send_at_command("AT+RX2FQ=?");
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+RX2FQ=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%lu", frequency);
        }
    }
    return status;
}

at_status_t at_set_transmit_power(uint8_t power) {
    char command[32];
    snprintf(command, sizeof(command), "AT+TXP=%d", power);
    return send_at_command(command);
}

at_status_t at_get_transmit_power(uint8_t *power) {
    at_status_t status = send_at_command("AT+TXP=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+TXP=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", power);
        }
    }
    return status;
}
// Ping slot periodicity is calculated as Periodicity=2^<param1> seconds
at_status_t at_set_ping_slot_periodicity(uint8_t periodicity_factor) {
    if (periodicity_factor > 7) {
        return AT_PARAM_ERROR;
    }

    char command[32];
    snprintf(command, sizeof(command), "AT+PGSLOT=%d", periodicity_factor);
    return send_at_command(command);
}

at_status_t at_get_ping_slot_periodicity(uint8_t *periodicity_factor) {
    at_status_t status = send_at_command("AT+PGSLOT=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+PGSLOT=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", periodicity_factor);
        }
    }
    return status;
}
//This command triggers the +JOIN event
at_status_t at_join_network(uint8_t join_mode) {
    char command[32];
    snprintf(command, sizeof(command), "AT+JOIN=%d", join_mode);
    return send_at_command(command);
}
//This command affects the output of the +RXINFO event
at_status_t at_link_check_request(void) {
    return send_at_command("AT+LINKC");
}
//This command triggers the +TXCONF event
at_status_t at_send_data(uint8_t frame_port, uint8_t confirmation, const char *payload) {
    char command[256];
    snprintf(command, sizeof(command), "AT+SEND=%d:%d:%s", frame_port, confirmation, payload);
    return send_at_command(command);
}
// The class can be changed only after a network has been joined.
// In order to switch to Class B or Class C, the module must be in Class A first.
// This command triggers the +CLASSUPDATE event
at_status_t at_set_class(char device_class) {
    char command[32];
    snprintf(command, sizeof(command), "AT+CLASS=%c", device_class);
    return send_at_command(command);
}

at_status_t at_get_class(char *device_class) {
    at_status_t status = send_at_command("AT+CLASS=?");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+CLASS=?", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%c", device_class);
        }
    }
    return status;
}
//This command triggers +JOIN evant
at_status_t at_start_certification(uint8_t join_mode) {
    char command[32];
    snprintf(command, sizeof(command), "AT+CERTIF=%d", join_mode);
    return send_at_command(command);
}

void handle_join_event(const char *event) {
    if (strcmp(event, "JOINED") == 0) {
        // Successfully joined the network
        // ...
    } else if (strcmp(event, "JOIN_FAILED") == 0) {
        // Failed to join the network
        // ...
    }
}

void handle_rxdata_event(uint8_t port, uint8_t size, const char *payload) {
    // Handle received data event
    // ...
}

void handle_rxinfo_event(const char *slot, uint8_t port, uint8_t data_rate, int8_t rssi, int8_t snr,
                         int8_t margin, uint8_t gateways) {
    // Handle receive window slot information event
    // ...
}

void handle_txconf_event(void) {
    // Handle uplink confirmation event
    // ...
}

void handle_classupdate_event(char device_class) {
    // Handle successful class switch event
    // ...
}

void handle_beaconinfo_event(const char *window, uint8_t data_rate, int8_t rssi, int8_t snr,
                             uint32_t frequency, uint32_t timestamp, uint8_t info_descriptor,
                             const char *info_first, const char *info_second) {
    // Handle received beacon information event
    // ...
}

void handle_beaconlost_event(void) {
    // Handle no beacon received in the past 120 minutes event
    // ...
}

void handle_beaconnotreceived_event(void) {
    // Handle beacon not received within the beacon period event
    // ...
}

at_status_t at_start_tone_test(void) {
    return send_at_command("AT+TTONE");
}

at_status_t at_start_rssi_test(int8_t *rssi) {
    at_status_t status = send_at_command("AT+TRSSI");
    if (status == AT_OK) {
        char response[16];
        status = get_at_command_value("AT+TRSSI", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhd", rssi);
        }
    }
    return status;
}

at_status_t at_set_test_config(const test_config_t *config) {
    char command[256];
    const char *modulation_str;

    switch (config->modulation) {
        case MODULATION_FSK:
            modulation_str = "FSK";
            break;
        case MODULATION_LORA:
            modulation_str = "LORA";
            break;
        case MODULATION_BPSK:
            modulation_str = "BPSK";
            break;
        case MODULATION_MSK:
            modulation_str = "MSK";
            break;
        default:
            return AT_PARAM_ERROR;
    }

    snprintf(command, sizeof(command), "AT+TCONF=%lu:%d:%lu:%d:%d:%d:%d:%s:%d:%lu:%d:%d",
             config->frequency, config->tx_power, config->bandwidth, config->data_rate,
             config->coding_rate, config->lna, config->pa_boost, modulation_str,
             config->payload_length, config->frequency_deviation, config->low_dr_optimization,
             config->gaussian_bt_product);

    return send_at_command(command);
}
// When setting the test config, unused parameters still need to be defined
// as shown in PDF example
at_status_t at_get_test_config(test_config_t *config) {
    at_status_t status = send_at_command("AT+TCONF=?");
    if (status == AT_OK) {
        char response[256];
        status = get_at_command_value("AT+TCONF=?", response, sizeof(response));
        if (status == AT_OK) {
            char modulation_str[16];
            int coding_rate;
            sscanf(response, "%lu,%hhu,%lu,%hu,%d,%hhu,%hhu,%[^,],%hhu,%lu,%hhu,%hhu",
                   &config->frequency, &config->tx_power, &config->bandwidth, &config->data_rate,
                   &coding_rate, &config->lna, &config->pa_boost, modulation_str,
                   &config->payload_length, &config->frequency_deviation, &config->low_dr_optimization,
                   &config->gaussian_bt_product);

            config->coding_rate = coding_rate == 1 ? 1 : 0;

            if (strcmp(modulation_str, "FSK") == 0) {
                config->modulation = MODULATION_FSK;
            } else if (strcmp(modulation_str, "LORA") == 0) {
                config->modulation = MODULATION_LORA;
            } else if (strcmp(modulation_str, "BPSK") == 0) {
                config->modulation = MODULATION_BPSK;
            } else if (strcmp(modulation_str, "MSK") == 0) {
                config->modulation = MODULATION_MSK;
            } else {
                status = AT_PARAM_ERROR;
            }
        }
    }
    return status;
}
// This command will only respond after all packets have beeen sent
at_status_t at_start_tx_test(uint32_t num_packets, uint8_t *success_rate) {
    char command[32];
    snprintf(command, sizeof(command), "AT+TTX=%lu", num_packets);

    at_status_t status = send_at_command(command);
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+TTX", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", success_rate);
        }
    }
    return status;
}
// This command will only respond after all packets have been either received or timed out
at_status_t at_start_rx_test(uint32_t num_packets, uint8_t *success_rate, int8_t *avg_rssi, int8_t *avg_snr, int8_t *avg_sensitivity) {
    char command[32];
    snprintf(command, sizeof(command), "AT+TRX=%lu", num_packets);

    at_status_t status = send_at_command(command);
    if (status == AT_OK) {
        char response[64];
        status = get_at_command_value("AT+TRX", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu,%hhd,%hhd,%hhd", success_rate, avg_rssi, avg_snr, avg_sensitivity);
        }
    }
    return status;
}

at_status_t at_start_tx_hopping_test(uint32_t start_freq, uint32_t stop_freq, uint32_t freq_delta, uint32_t num_packets, uint8_t *success_rate) {
    char command[128];
    snprintf(command, sizeof(command), "AT+TTH=%lu,%lu,%lu,%lu", start_freq, stop_freq, freq_delta, num_packets);

    at_status_t status = send_at_command(command);
    if (status == AT_OK) {
        char response[32];
        status = get_at_command_value("AT+TTH", response, sizeof(response));
        if (status == AT_OK) {
            sscanf(response, "%hhu", success_rate);
        }
    }
    return status;
}
// Other radio test once started cannot be stopped using this command
at_status_t at_stop_tone_test(void) {
    return send_at_command("AT+TOFF");
}