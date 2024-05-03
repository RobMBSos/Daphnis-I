//
// Created by rovem on 5/2/2024.
//

#ifndef DAPHNIS_H
#define DAPHNIS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    AT_OK,
    AT_ERROR,
    AT_PARAM_ERROR,
    AT_BUSY_ERROR,
    AT_TEST_PARAM_OVERFLOW,
    AT_NO_NETWORK_JOINED,
    AT_RX_ERROR,
    AT_DUTYCYCLE_RESTRICTED,
    AT_CRYPTO_ERROR
} at_status_t;

typedef enum {
    MODULATION_FSK,
    MODULATION_LORA,
    MODULATION_BPSK,
    MODULATION_MSK
} modulation_t;

typedef struct {
    uint32_t frequency;
    uint8_t tx_power;
    uint32_t bandwidth;
    uint16_t data_rate;
    uint8_t coding_rate;
    uint8_t lna;
    uint8_t pa_boost;
    modulation_t modulation;
    uint8_t payload_length;
    uint32_t frequency_deviation;
    uint8_t low_dr_optimization;
    uint8_t gaussian_bt_product;
} test_config_t;

// General AT commands
at_status_t at_command(void);
at_status_t at_reset(void);
at_status_t at_get_local_time(char *time, size_t time_size, char *date, size_t date_size);
at_status_t at_get_battery_level(uint16_t *battery_level);
at_status_t at_sleep(void);

// General events
void handle_sysnotf_event(const char *event);
void handle_cs_event(const char *event);

// User Settings AT commands
at_status_t at_set_uart_baud_rate(uint8_t baud_rate);
at_status_t at_get_uart_baud_rate(uint8_t *baud_rate);
at_status_t at_restore_user_settings(void);

// Long-Range-WAN stack context AT commands
at_status_t at_store_context(void);
at_status_t at_erase_context(void);

// Long-Range-WAN keys, IDs, and EUIs AT commands
at_status_t at_set_appeui(const char *appeui);
at_status_t at_get_appeui(char *appeui, size_t appeui_size);
at_status_t at_set_appkey(const char *appkey);
at_status_t at_set_appskey(const char *appskey);
at_status_t at_set_nwkskey(const char *nwkskey);
at_status_t at_set_device_address(const char *daddr);
at_status_t at_get_device_address(char *daddr, size_t daddr_size);
at_status_t at_set_device_eui(const char *deui);
at_status_t at_get_device_eui(char *deui, size_t deui_size);


// Long-Range-WAN network AT commands
at_status_t at_set_network_id(uint8_t nwkid);
at_status_t at_get_network_id(uint8_t *nwkid);
at_status_t at_get_version(char *firmware_version, size_t firmware_version_size,
                           char *linklayer_version, size_t linklayer_version_size,
                           char *regional_params_version, size_t regional_params_version_size);
at_status_t at_set_adaptive_data_rate(uint8_t adr);
at_status_t at_get_adaptive_data_rate(uint8_t *adr);
at_status_t at_set_data_rate(uint8_t dr);
at_status_t at_get_data_rate(uint8_t *dr);
at_status_t at_set_duty_cycle_restriction(uint8_t dcs);
at_status_t at_get_duty_cycle_restriction(uint8_t *dcs);
at_status_t at_set_join_accept_delay1(uint32_t delay);
at_status_t at_get_join_accept_delay1(uint32_t *delay);
at_status_t at_set_join_accept_delay2(uint32_t delay);
at_status_t at_get_join_accept_delay2(uint32_t *delay);
at_status_t at_set_rx1_delay(uint32_t delay);
at_status_t at_get_rx1_delay(uint32_t *delay);
at_status_t at_set_rx2_delay(uint32_t delay);
at_status_t at_get_rx2_delay(uint32_t *delay);
at_status_t at_set_rx2_data_rate(uint8_t data_rate);
at_status_t at_get_rx2_data_rate(uint8_t *data_rate);
at_status_t at_set_rx2_frequency(uint32_t frequency);
at_status_t at_get_rx2_frequency(uint32_t *frequency);
at_status_t at_set_transmit_power(uint8_t power);
at_status_t at_get_transmit_power(uint8_t *power);
at_status_t at_set_ping_slot_periodicity(uint8_t periodicity_factor);
at_status_t at_get_ping_slot_periodicity(uint8_t *periodicity_factor);
at_status_t at_join_network(uint8_t join_mode);
at_status_t at_link_check_request(void);
at_status_t at_send_data(uint8_t frame_port, uint8_t confirmation, const char *payload);
at_status_t at_set_class(char device_class);
at_status_t at_get_class(char *device_class);

// Long-Range-WAN certification AT commands
at_status_t at_start_certification(uint8_t join_mode);

// Long-Range-WAN network events
void handle_join_event(const char *event);
void handle_rxdata_event(uint8_t port, uint8_t size, const char *payload);
void handle_rxinfo_event(const char *slot, uint8_t port, uint8_t data_rate, int8_t rssi, int8_t snr,
                         int8_t margin, uint8_t gateways);
void handle_txconf_event(void);
void handle_classupdate_event(char device_class);
void handle_beaconinfo_event(const char *window, uint8_t data_rate, int8_t rssi, int8_t snr,
                             uint32_t frequency, uint32_t timestamp, uint8_t info_descriptor,
                             const char *info_first, const char *info_second);
void handle_beaconlost_event(void);
void handle_beaconnotreceived_event(void);

// Radio test AT commands
at_status_t at_start_tone_test(void);
at_status_t at_start_rssi_test(int8_t *rssi);
at_status_t at_set_test_config(const test_config_t *config);
at_status_t at_get_test_config(test_config_t *config);
at_status_t at_start_tx_test(uint32_t num_packets, uint8_t *success_rate);
at_status_t at_start_rx_test(uint32_t num_packets, uint8_t *success_rate, int8_t *avg_rssi, int8_t *avg_snr, int8_t *avg_sensitivity);
at_status_t at_start_tx_hopping_test(uint32_t start_freq, uint32_t stop_freq, uint32_t freq_delta, uint32_t num_packets, uint8_t *success_rate);
at_status_t at_stop_tone_test(void);



#endif //DAPHNIS_H
