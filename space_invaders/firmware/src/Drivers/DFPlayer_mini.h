#ifndef DFPLAYERMINI_H
#define DFPLAYERMINI_H

typedef enum DFPM_cmd
{
	DFP_CMD_NEXT	=1,
	DFP_CMD_PREVIOUS,
	DFP_CMD_TRACKNB,
	DFP_CMD_INCVOL,
	DFP_CMD_DECVOL,
	DFP_CMD_SETVOL,
	DFP_CMD_SETEQ,
	DFP_CMD_PLAYMODE,
	DFP_CMD_PLAYSOURCE,
	DFP_CMD_LOWPOWER,
	DFP_CMD_NORMALOP,
	DFP_CMD_RESET,
	DFP_CMD_PLAYBACK,
	DFP_CMD_PAUSE,
	DFP_CMD_SETFOLDER
}DFPM_cmd;

int DFPM_get_checksum(int buff[]);
void DFPM_send_cmd(int8 cmd, int16 arg, int1 feedback);

void DFPM_set_device(int16 device);
void DFPM_set_EQ(int16 eq);
void DFPM_set_volume(int16 volume);

void DFPM_play_track(int16 track_num);

void DFPM_stop();

#endif