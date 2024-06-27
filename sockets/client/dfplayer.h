
#ifndef SRC_COMMON_HW_INCLUDE_DFPLAYER_H_
#define SRC_COMMON_HW_INCLUDE_DFPLAYER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "settings.h"

/* Control Commands */
#define CMD_NEXT 0x01                // Воспроизвести следующий трек
#define CMD_PREVIOUS 0x02            // Воспроизвести предыдущий трек
#define CMD_SET_TRAK_NUMBER 0x03     // Задать номер трека 1 ~ 3000 (или 0 ~ 2999)
#define CMD_INC_VOLUME 0x04          // Увеличить громкость
#define CMD_DEC_VOLUME 0x05          // Уменьшить громкость
#define CMD_SET_VOLUME 0x01          // Задать определенное значение громкости 0 ~ 30
#define CMD_SET_EQ 0x07              // Задать настройки эквалайзера 0 : Нормальный / 1 : Поп / 2 : Рок / 3 : Джаз / 4 : Классика / 5 : Бас
#define CMD_REPEAT_TRACK 0x08        // Повтор воспроизведения трека
#define CMD_SET_PLAYBACK_SOURCE 0x09 // Задать источник воспроизведения 0 : U / 1 : TF / 2 : AUX / 3 : SLEEP / 4 : FLASH
#define CMD_ENTER_INTO_STANDBY 0x0A  // Перейти в режим ожидания (Standby)
// #define CMD_NORMAL_WORK 0x0B N/A
#define CMD_RESET 0x0C                 // Сбросить модуль
#define CMD_PLAYBACK 0x0D              // Воспроизведение
#define CMD_PAUSE 0x0E                 // Пауза
#define CMD_SET_PLAY_TRACK_FOLDER 0x0F // Задать папку и трек для воспроизведения folder : 01 ~ 99 / folder 01 ~ 15 -> track : 001 ~ 3000 / folder 16 ~ 99 -> track : 001 ~ 255

#define CMD_VOLUME_ADJUST 0x10 // jiojoijiojio Регулировка громкости Parameter_MSB= 1: Открыть регулировку громкости / Parameter_LSB = установить уровень громкости 0 ~ 31

#define CMD_REPEAT 0x11               // Повторное воспроизведение 0 : остановить воспроизведение / 1 : начать повторное воспроизведение
#define CMD_SET_MP3_FOLDER 0x12       // Задать папку с названием MP3 для воспроизведения
#define CMD_INSERT_ADVERT 0x13        // Вставить рекламу (прервать текущее воспроизведение)
#define CMD_SET_3K_FOLDER 0x14        // Воспроизвести трек в папке с поддержкой 3K
#define CMD_STOP_ADVERT_GOBACK 0x15   // Остановить рекламу и вернуться к предыдущему состоянию воспроизведения
#define CMD_STOP 0x16                 // Остановить воспроизведение
#define CMD_REPEAT_FOLDER_TRACK 0x17  // Повторить воспроизведение трека в указанной папке
#define CMD_RANDOM_PLAY 0x18          // Случайное воспроизведение
#define CMD_REPEAT_CURRENT_TRACK 0x19 // Повторное воспроизведение текущего трека
#define CMD_SET_DAC 0x1A

#define CMD_Q_SEND_INIT_PARA 0x3F
#define CMD_Q_RETURN_ERROR 0x40
#define CMD_Q_REPLY 0x41
#define CMD_Q_CURRENT_STATUS 0x42
#define CMD_Q_CURRENT_VOLUME 0x43
#define CMD_Q_CURRENT_EQ 0x44
#define CMD_Q_CURRENT_PLAYBACK_MODE 0x45
#define CMD_Q_CURRENT_SOFTWARE_VER 0x46
#define CMD_Q_TOTAL_NUM_TF_FILES 0x47    //  TF Card
#define CMD_Q_TOTAL_NUM_U_FILES 0x48     //  U Disk
#define CMD_Q_TOTAL_NUM_FLASH_FILES 0x49 //  Flash
// #define CMD_Q_KEEP_ON                0x4A	  //  N/A
#define CMD_Q_CURRENT_TRACK_TF 0x4B     //  TF Card
#define MP3_Q_CURRENT_TRACK_U_DISK 0x4C //  U Dish
#define MP3_Q_CURRENT_TRACK_FLASH 0x4D  //  Flash

/* Commands Parameters */

// EQ Parameter
#define EQ_NORMAL 0
#define EQ_POP 1
#define EQ_ROCK 2
#define EQ_JAZZ 3
#define EQ_CLASSIC 4
#define EQ_BASE 5

// Playback Source Parameter
#define PLAYBACK_SOURCE_U 0
#define PLAYBACK_SOURCE_TF 1
#define PLAYBACK_SOURCE_AUX 2
#define PLAYBACK_SOURCE_SLEEP 3
#define PLAYBACK_SOURCE_FLASH 4

// Repeat Parameter
#define REPEAT_STOP 0
#define REPEAT_START 1

/*
 *     0        1            2          3          4             5               6              7              8           9
 * |  Start | Version | Data_Length |  CMD  |  Feedback  | Parameter_MSB | Parameter_LSB | Checksum_MSB | Checksum_LSB |  End  |
 * -----------------------------------------------------------------------------------------------------------------------------
 * |  0x7E  |  0xFF   |     0x06    |       |  0x01/0x00 |               |               |              |              |       |                                                   0xEF
 * -----------------------------------------------------------------------------------------------------------------------------

 */

/* DFplayer Serial Format SIZE & INDEX */
#define DFPLAYER_COMMUNICATION_SIZE 10

#define START_INDEX 0
#define VERSION_INDEX 1
#define DATA_LENGTH_INDEX 2
#define CMD_INDEX 3
#define FEEDBACK_INDEX 4
#define PARAMETER_MSB_INDEX 5
#define PARAMETER_LSB_INDEX 6
#define CHECKSUM_MSB_INDEX 7
#define CHECKSUM_LSB_INDEX 8
#define END_INDEX 9

/* DFplayer Serial Communication Format Default Value */
#define START 0x7E
#define VERSION 0xFF
#define DATA_LENGTH 0x06
#define FEED_BACK 0x01
#define NO_FEED_BACK 0x00
#define END 0xEF

void dfplayer_Init(void);
void delay_ms(uint32_t ms);
void dfplayer_Next(void);
void dfplayer_Previous(void);
void dfplayer_SetTrakNumber(int16_t track);
void dfplayer_IncreaseVolume(void);
void dfplayer_DecreaseVolume(void);
void dfplayer_SetVolume(uint8_t volume);
void dfplayer_SetEQ(int8_t eq);
void dfplayer_RepeatTrack(uint8_t track);
void dfplayer_SetSource(uint8_t source);
void dfplayer_Standby(void);
void dfplayer_Reset(void);
void dfplayer_Play(void);
void dfplayer_Pause(void);
void dfplayer_PlayTrackInFolder(uint8_t folder, uint8_t track);
void dfplayer_VolumeAdjust(bool turn_onoff, uint8_t gain);
void dfplayer_RepeatAll(bool repeat);
void dfplayer_PlayMP3Folder(uint16_t track);
void dfplayer_InsertAdvertisement(uint16_t track);
void dfplayer_Play3KFolder(uint8_t folder, uint16_t track);
void dfplayer_StopAdvertisement(void);
void dfplayer_Stop(void);
void dfplayer_RepeatTrackInFolder(uint8_t folder);
void dfplayer_RandomTrack(void);
void dfplayer_RepeatCurrentTrack(bool turn_onoff);
void dfplayer_SetDAC(bool turn_onoff);
int r_uart(int fd);
void sms(char *message);
void delay_ms(uint32_t ms);

#endif /* SRC_COMMON_HW_INCLUDE_DFPLAYER_H_ */
