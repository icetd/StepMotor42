#include "common.h"
#include "configurations.h"
#include "Platform/Utils/st_hardware.h"
#include <tim.h>
#include <can.h>

/* Component Definitions -----------------------------------------------------*/
BoardConfig_t boardConfig;
Motor motor;
TB67H450 tb67H450;
MT6816 mt6816;
EncoderCalibrator encoderCalibrator(&motor);

Switch sw1(1);
Switch sw2(2);
Switch sw3(3);
Switch sw4(4);
Switch sw5(5);

/* Main Entry ----------------------------------------------------------------*/
extern "C" void Main()
{
    CanId_t can_id;
    can_id.sw.reserved = 0;
    can_id.sw.bit0 = sw1.ReadSwitchPinIO(1);
    can_id.sw.bit1 = sw2.ReadSwitchPinIO(2);
    can_id.sw.bit2 = sw3.ReadSwitchPinIO(3);
    can_id.sw.bit3 = sw4.ReadSwitchPinIO(4);
    uint16_t defaultNodeID = can_id.id;

    /*---------- Apply EEPROM Settings ----------*/
    // Setting priority is EEPROM > Motor.h
    EEPROM eeprom;
    eeprom.get(0, boardConfig);
    if (boardConfig.configStatus != CONFIG_OK) // use default settings
    {
        boardConfig = BoardConfig_t{
            .configStatus = CONFIG_OK,
            .canNodeId = defaultNodeID,
            .encoderHomeOffset = 0,
            .defaultMode = Motor::MODE_COMMAND_POSITION,
            .currentLimit = 2 * 1000,                                     // A
            .velocityLimit = 30 * motor.MOTOR_ONE_CIRCLE_SUBDIVIDE_STEPS, // r/s
            .velocityAcc = 100 * motor.MOTOR_ONE_CIRCLE_SUBDIVIDE_STEPS,  // r/s^2
            .calibrationCurrent = 2000,
            .dce_kp = 200,
            .dce_kv = 80,
            .dce_ki = 300,
            .dce_kd = 250,
            .enableMotorOnBoot = false,
            .enableStallProtect = false};
        eeprom.put(0, boardConfig);
    }
    boardConfig.canNodeId = defaultNodeID;
    motor.config.motionParams.encoderHomeOffset = boardConfig.encoderHomeOffset;
    motor.config.motionParams.ratedCurrent = 2 * 1000;
    motor.config.motionParams.ratedVelocity = boardConfig.velocityLimit;
    motor.config.motionParams.ratedVelocityAcc = boardConfig.velocityAcc;
    motor.motionPlanner.velocityTracker.SetVelocityAcc(boardConfig.velocityAcc);
    motor.motionPlanner.positionTracker.SetVelocityAcc(boardConfig.velocityAcc);
    motor.config.motionParams.caliCurrent = boardConfig.calibrationCurrent;
    motor.config.ctrlParams.dce.kp = boardConfig.dce_kp;
    motor.config.ctrlParams.dce.kv = boardConfig.dce_kv;
    motor.config.ctrlParams.dce.ki = boardConfig.dce_ki;
    motor.config.ctrlParams.dce.kd = boardConfig.dce_kd;
    motor.config.ctrlParams.stallProtectSwitch = boardConfig.enableStallProtect;

    /*---------------- Init Motor ----------------*/
    motor.AttachDriver(&tb67H450);
    motor.AttachEncoder(&mt6816);
    motor.controller->Init();
    motor.driver->Init();
    motor.encoder->Init();

    can_init(&hcan);

    /*------- Start Close-Loop Control Tick ------*/
    HAL_Delay(100);
    HAL_TIM_Base_Start_IT(&htim1); // 100Hz
    HAL_TIM_Base_Start_IT(&htim4); // 20kHz

    if (boardConfig.canNodeId == 0) {
        encoderCalibrator.isTriggered = true;
        printf("mode in CalibrationDataCheck\r\n");
    }

    for (;;) {
        encoderCalibrator.TickMainLoop();
        if (boardConfig.configStatus == CONFIG_COMMIT) {
            boardConfig.configStatus = CONFIG_OK;
            eeprom.put(0, boardConfig);
        } else if (boardConfig.configStatus == CONFIG_RESTORE) {
            eeprom.put(0, boardConfig);
            HAL_NVIC_SystemReset();
        }
    }
}

/* Event Callbacks -----------------------------------------------------------*/
extern "C" void Tim1Callback100Hz()
{
    __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
}

extern "C" void Tim4Callback20kHz()
{
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);

    if (encoderCalibrator.isTriggered)
        encoderCalibrator.Tick20kHz();
    else
        motor.Tick20kHz();
}