#include "dfplayer.h"


static void 	dfplayer_SendControlMessage(uint8_t cmd, uint8_t para_msb, uint8_t para_lsb);
static uint16_t dfplayer_Checksum(void);
static uint8_t tx_buf[DFPLAYER_COMMUNICATION_SIZE];





/************************************************************************/
void delay_ms(uint32_t ms)
{
        delay(ms);
}



void dfplayer_SendControlMessage( uint8_t cmd, uint8_t para_msb, uint8_t para_lsb)
{
    tx_buf[START_INDEX] 		= START;
    tx_buf[VERSION_INDEX]	 	= VERSION;
    tx_buf[DATA_LENGTH_INDEX] 	= DATA_LENGTH;
    tx_buf[CMD_INDEX] 			= cmd;
    tx_buf[FEEDBACK_INDEX] 		= NO_FEED_BACK;
    tx_buf[PARAMETER_MSB_INDEX] = para_msb;
    tx_buf[PARAMETER_LSB_INDEX] = para_lsb;

    uint16_t checksum = dfplayer_Checksum();

    tx_buf[CHECKSUM_MSB_INDEX]  = (uint8_t)(checksum >> 8);
    tx_buf[CHECKSUM_LSB_INDEX]  = (uint8_t)(checksum & 0xFF);
    tx_buf[END_INDEX] 			= END;

    int fq ;

    if ((fq = serialOpen ("/dev/ttyS1", 9600)) < 0)
    {
     fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    }
    delay (100);
    for (uint8_t i=0;i<10;i++)
    {
      serialPutchar(fq,tx_buf[i]);
    }
    serialClose(fq);
}




uint16_t dfplayer_Checksum(void)
{
    uint16_t checksum = 0;

    for (int i = VERSION_INDEX; i <= PARAMETER_LSB_INDEX; i ++)
    {
        checksum += tx_buf[i];
    }

    checksum = 0xFFFF - checksum + 1;

    return checksum;
}



void dfplayer_Init(void)
{
    delay_ms(1500);
    dfplayer_SetSource(PLAYBACK_SOURCE_FLASH);
    delay_ms(100);
    dfplayer_SetVolume(VOLUME_VALUE);
    delay_ms(100);
    dfplayer_SetVolume(VOLUME_VALUE);
    delay_ms(100);
    dfplayer_VolumeAdjust(0,1);
    delay_ms(100);
    dfplayer_PlayMP3Folder(1);
    delay_ms(100);
    dfplayer_SetEQ(4);
}




void dfplayer_Next(void)
{
    dfplayer_SendControlMessage(CMD_NEXT, 0, 0);
}



void dfplayer_Previous(void)
{
    dfplayer_SendControlMessage(CMD_PREVIOUS, 0, 0);
}


void dfplayer_SetTrakNumber(int16_t track)
{
    dfplayer_SendControlMessage(CMD_SET_TRAK_NUMBER, (uint8_t)(track >> 8), (uint8_t)track & 0xFF);
}



void dfplayer_IncreaseVolume(void)
{
    dfplayer_SendControlMessage(CMD_INC_VOLUME, 0, 0);
}



void dfplayer_DecreaseVolume(void)
{
    dfplayer_SendControlMessage(CMD_DEC_VOLUME, 0, 0);
}




void dfplayer_SetVolume(uint8_t volume)
{
    if (volume > 30)
    {
        volume = 30;
    }

    dfplayer_SendControlMessage(CMD_SET_VOLUME, 0, volume);

}




void dfplayer_SetEQ(int8_t eq)
{
    if (eq > EQ_BASE)
    {
        eq = EQ_NORMAL ;
    }

    dfplayer_SendControlMessage(CMD_SET_EQ, 0, eq);
}



void dfplayer_RepeatTrack(uint8_t track)
{

    dfplayer_SendControlMessage(CMD_REPEAT_TRACK, (uint8_t)(track & 0xFF00) >> 8, (uint8_t)track & 0xFF);
}



/*
 * 							PLAYBACK_SOURCE_U
 *			  			    PLAYBACK_SOURCE_TF
 *			  			    PLAYBACK_SOURCE_AUX
 *			  			    PLAYBACK_SOURCE_SLEEP
 *			  			    PLAYBACK_SOURCE_FLASH
 */
void dfplayer_SetSource(uint8_t source)
{

    if (source > PLAYBACK_SOURCE_FLASH)
    {
        return ;
    }

    dfplayer_SendControlMessage(CMD_SET_PLAYBACK_SOURCE, 0, source);

}



void dfplayer_Standby(void)
{
    dfplayer_SendControlMessage(CMD_ENTER_INTO_STANDBY, 0, 0);
}



void dfplayer_Reset(void)
{
    dfplayer_SendControlMessage(CMD_RESET, 0, 0);
}




void dfplayer_Play(void)
{
    dfplayer_SendControlMessage(CMD_PLAYBACK, 0, 0);
}



void dfplayer_Pause(void)
{
    dfplayer_SendControlMessage(CMD_PAUSE, 0, 0);
}



void dfplayer_PlayTrackInFolder(uint8_t folder, uint8_t track) //TODO ongoing - 4
{
    if (folder > 99)
    {
        folder = 99;
    }

    if (track > 255)
    {
        track = 255;
    }

    dfplayer_SendControlMessage(CMD_SET_PLAY_TRACK_FOLDER, folder, track);
}



void dfplayer_VolumeAdjust(bool turn_onoff, uint8_t gain)
{

    if (gain > 31)
    {
        gain = 31;
    }

    dfplayer_SendControlMessage(CMD_VOLUME_ADJUST, turn_onoff, gain);

}


void dfplayer_RepeatAll(bool repeat)
{
    dfplayer_SendControlMessage(CMD_REPEAT, 0, repeat);

}


void dfplayer_PlayMP3Folder(uint16_t track)
{
    dfplayer_SendControlMessage(CMD_SET_MP3_FOLDER, (uint8_t)(track >> 8), (uint8_t)(track & 0xFF));
}




void dfplayer_InsertAdvertisement(uint16_t track)
{
    dfplayer_SendControlMessage(CMD_INSERT_ADVERT,(uint8_t)(track >> 8), (uint8_t)(track & 0xFF));
}




void dfplayer_Play3KFolder(uint8_t folder, uint16_t track) //
{
    if (folder > 15)
    {
        folder = 15;
    }

    if (track > 3000)
    {
        track = 3000;
    }

    dfplayer_SendControlMessage(CMD_SET_3K_FOLDER, (folder << 4) | (uint8_t)(track & 0xF00) >> 8, (uint8_t)(track & 0xFF));
}



void dfplayer_StopAdvertisement(void)
{

    dfplayer_SendControlMessage(CMD_STOP_ADVERT_GOBACK, 0, 0);
}





void dfplayer_Stop(void)
{

    dfplayer_SendControlMessage(CMD_STOP, 0, 0);
}




void dfplayer_RepeatTrackInFolder(uint8_t folder) //TODO ongoing - 2
{
    if (folder > 99)
    {
        folder = 99;
    }

    dfplayer_SendControlMessage(CMD_REPEAT_FOLDER_TRACK, 0, folder);
}




void dfplayer_RandomTrack(void)
{
    dfplayer_SendControlMessage(CMD_RANDOM_PLAY, 0, 0);
}


void dfplayer_RepeatCurrentTrack(bool turn_onoff)
{
    dfplayer_SendControlMessage(CMD_RANDOM_PLAY, 0, !turn_onoff);
}


void dfplayer_SetDAC(bool turn_onoff)
{
    dfplayer_SendControlMessage(CMD_SET_DAC, 0, !turn_onoff);
}
