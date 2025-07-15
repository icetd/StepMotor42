#include "can.h"
#include "log.h"
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

// === 通用发送封装 ===
void send_float(Can* can, uint8_t node_id, uint8_t cmd, float val, bool ack = false) {
    struct can_frame frame;
    frame.can_id = (node_id << 7) | cmd;
    frame.can_dlc = 8;
    memset(frame.data, 0, 8);
    memcpy(&frame.data[0], &val, sizeof(float));
    frame.data[4] = ack ? 1 : 0;
    can->transmit(&frame);
}

void send_int(Can* can, uint8_t node_id, uint8_t cmd, uint32_t val, bool ack = false) {
    struct can_frame frame;
    frame.can_id = (node_id << 7) | cmd;
    frame.can_dlc = 8;
    memset(frame.data, 0, 8);
    memcpy(&frame.data[0], &val, sizeof(uint32_t));
    frame.data[4] = ack ? 1 : 0;
    can->transmit(&frame);
}

void send_cmd(Can* can, uint8_t node_id, uint8_t cmd) {
    struct can_frame frame;
    frame.can_id = (node_id << 7) | cmd;
    frame.can_dlc = 1;
    frame.data[0] = 0;
    can->transmit(&frame);
}

// === 0x00~0x0F 无存储命令 ===
void enableMotor(Can* can, uint8_t node_id, bool enable) {
    send_int(can, node_id, 0x01, enable ? 1 : 0);
}

void calibrate(Can* can, uint8_t node_id) {
    send_cmd(can, node_id, 0x02);
}

void setCurrent(Can* can, uint8_t node_id, float current_a) {
    send_float(can, node_id, 0x03, current_a);
}

void setSpeed(Can* can, uint8_t node_id, float speed_rps) {
    send_float(can, node_id, 0x04, speed_rps);
}

void setPosition(Can* can, uint8_t node_id, float position_rad, bool ack = true) {
    send_float(can, node_id, 0x05, position_rad, ack);
}

void setPositionWithTime(Can* can, uint8_t node_id, float position_rad, float time_sec, bool ack = true) {
    struct can_frame frame;
    frame.can_id = (node_id << 7) | 0x06;
    frame.can_dlc = 8;
    memset(frame.data, 0, 8);
    memcpy(&frame.data[0], &position_rad, sizeof(float));
    memcpy(&frame.data[4], &time_sec, sizeof(float));
    can->transmit(&frame);
}

void setPositionWithVelLimit(Can* can, uint8_t node_id, float position_rad, float vel_limit_rps) {
    struct can_frame frame;
    frame.can_id = (node_id << 7) | 0x07;
    frame.can_dlc = 8;
    memset(frame.data, 0, 8);
    memcpy(&frame.data[0], &position_rad, sizeof(float));
    memcpy(&frame.data[4], &vel_limit_rps, sizeof(float));
    can->transmit(&frame);
}

// === 0x20~0x2F 查询命令 ===
void getCurrent(Can* can, uint8_t node_id) {
    send_cmd(can, node_id, 0x21);
}

void getSpeed(Can* can, uint8_t node_id) {
    send_cmd(can, node_id, 0x22);
}

void getPosition(Can* can, uint8_t node_id) {
    send_cmd(can, node_id, 0x23);
}

void getHomeOffet(Can* can, uint8_t node_id) {
    send_cmd(can, node_id, 0x24);
}

// === 其他命令 ===
void eraseConfigs(Can* can, uint8_t node_id) {
    send_cmd(can, node_id, 0x7E);
}

void reboot(Can* can, uint8_t node_id) {
    send_cmd(can, node_id, 0x7F);
}

// === 接收打印回调 ===
void rx_callback(struct can_frame rx_frame) {
    uint8_t cmd = rx_frame.can_id & 0x7F;
    uint8_t node_id = rx_frame.can_id >> 7;

    float fval = 0.0f;
    memcpy(&fval, rx_frame.data, sizeof(float));
    printf("<< [Node %d | CMD 0x%02X] float=%.3f, raw=[", node_id, cmd, fval);
    for (int i = 0; i < rx_frame.can_dlc; i++)
        printf("%02X ", rx_frame.data[i]);
    printf("]\n");
}

// === 主函数入口 ===
int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s [node_id] [command] [value(s)]\n", argv[0]);
        printf("Commands:\n");
        printf("  enable [0|1]\n");
        printf("  calibrate\n");
        printf("  set_current [float]\n");
        printf("  set_vel [float]\n");
        printf("  set_pos [float]\n");
        printf("  set_pos_time [pos(float)] [time(float)]\n");
        printf("  set_pos_vel_limit [pos(float)] [vel_limit(float)]\n");
        printf("  get_current\n");
        printf("  get_vel\n");
        printf("  get_pos\n");
        printf("  get_home_offset\n");
        printf("  erase_configs\n");
        printf("  reboot\n");
        return 0;
    }

    uint8_t node_id = atoi(argv[1]);
    std::string cmd = argv[2];

    initLogger(INFO);
    Can* can = new Can((char*)"can0");
    can->init();
    can->setOnCanReceiveDataCallback(rx_callback);
    can->startAutoRead();

    if (cmd == "enable" && argc >= 4) {
        enableMotor(can, node_id, atoi(argv[3]) != 0);
    } else if (cmd == "calibrate") {
        calibrate(can, node_id);
    } else if (cmd == "set_current" && argc >= 4) {
        setCurrent(can, node_id, atof(argv[3]));
    } else if (cmd == "set_vel" && argc >= 4) {
        setSpeed(can, node_id, atof(argv[3]));
    } else if (cmd == "set_pos" && argc >= 4) {
        setPosition(can, node_id, atof(argv[3]));
    } else if (cmd == "set_pos_time" && argc >= 5) {
        setPositionWithTime(can, node_id, atof(argv[3]), atof(argv[4]));
    } else if (cmd == "set_pos_vel_limit" && argc >= 5) {
        setPositionWithVelLimit(can, node_id, atof(argv[3]), atof(argv[4]));
    } else if (cmd == "get_current") {
        getCurrent(can, node_id);
    } else if (cmd == "get_vel") {
        getSpeed(can, node_id);
    } else if (cmd == "get_pos") {
        getPosition(can, node_id);
    } else if (cmd == "get_home_offset") {
        getHomeOffet(can, node_id);
    } else if (cmd == "erase_configs") {
        eraseConfigs(can, node_id);
    } else if (cmd == "reboot") {
        reboot(can, node_id);
    } else {
        printf("Unknown command or wrong parameters\n");
 
        printf("Usage: %s [node_id] [command] [value(s)]\n", argv[0]);
        printf("Commands:\n");
        printf("  enable [0|1]\n");
        printf("  calibrate\n");
        printf("  set_current [float]\n");
        printf("  set_vel [float]\n");
        printf("  set_pos [float]\n");
        printf("  set_pos_time [pos(float)] [time(float)]\n");
        printf("  set_pos_vel_limit [pos(float)] [vel_limit(float)]\n");
        printf("  get_current\n");
        printf("  get_vel\n");
        printf("  get_pos\n");
        printf("  get_home_offset\n");
        printf("  erase_configs\n");
        printf("  reboot\n");
    }

    usleep(100000); // 等待回包
    delete can;
    return 0;
}

