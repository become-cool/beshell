#include "tml.h"


#if !defined(TML_MALLOC) || !defined(TML_FREE) || !defined(TML_REALLOC)
#  include <stdlib.h>
#  define TML_MALLOC  malloc
#  define TML_FREE    free
#  define TML_REALLOC realloc
#endif

#if !defined(TML_MEMCPY)
#  include <string.h>
#  define TML_MEMCPY  memcpy
#endif

#ifndef TML_NO_STDIO
#  include <stdio.h>
#endif

#define TML_NULL 0

////crash on errors and warnings to find broken midi files while debugging
//#define TML_ERROR(msg) *(int*)0 = 0xbad;
//#define TML_WARN(msg)  *(int*)0 = 0xf00d;

////print errors and warnings
//#define TML_ERROR(msg) printf("ERROR: %s\n", msg);
//#define TML_WARN(msg)  printf("WARNING: %s\n", msg);

#ifndef TML_ERROR
#define TML_ERROR(msg)
#endif

#ifndef TML_WARN
#define TML_WARN(msg)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TML_NO_STDIO
static int tml_stream_stdio_read(FILE* f, void* ptr, unsigned int size) { return (int)fread(ptr, 1, size, f); }
TMLDEF tml_message* tml_load_filename(const char* filename)
{
	struct tml_message* res;
	struct tml_stream stream = { TML_NULL, (int(*)(void*,void*,unsigned int))&tml_stream_stdio_read };
	#if __STDC_WANT_SECURE_LIB__
	FILE* f = TML_NULL; fopen_s(&f, filename, "rb");
	#else
	FILE* f = fopen(filename, "rb");
	#endif
	if (!f) { TML_ERROR("File not found"); return 0; }
	stream.data = f;
	res = tml_load(&stream);
	fclose(f);
	return res;
}
#endif

struct tml_stream_memory { const char* buffer; unsigned int total, pos; };
static int tml_stream_memory_read(struct tml_stream_memory* m, void* ptr, unsigned int size) { if (size > m->total - m->pos) size = m->total - m->pos; TML_MEMCPY(ptr, m->buffer+m->pos, size); m->pos += size; return size; }
TMLDEF struct tml_message* tml_load_memory(const void* buffer, int size)
{
	struct tml_stream stream = { TML_NULL, (int(*)(void*,void*,unsigned int))&tml_stream_memory_read };
	struct tml_stream_memory f = { 0, 0, 0 };
	f.buffer = (const char*)buffer;
	f.total = size;
	stream.data = &f;
	return tml_load(&stream);
}

struct tml_track
{
	unsigned int Idx, End, Ticks;
};

struct tml_tempomsg
{
	unsigned int time;
	unsigned char type, Tempo[3];
	tml_message* next;
};

struct tml_parser
{
	unsigned char *buf, *buf_end; 
	int last_status, message_array_size, message_count;
};

enum TMLSystemType
{
	TML_TEXT  = 0x01, TML_COPYRIGHT    = 0x02, TML_TRACK_NAME     = 0x03, TML_INST_NAME     = 0x04, TML_LYRIC           = 0x05, TML_MARKER       = 0x06, TML_CUE_POINT = 0x07,
	TML_EOT   = 0x2f, TML_SMPTE_OFFSET = 0x54, TML_TIME_SIGNATURE = 0x58, TML_KEY_SIGNATURE = 0x59, TML_SEQUENCER_EVENT = 0x7f,
	TML_SYSEX = 0xf0, TML_TIME_CODE    = 0xf1, TML_SONG_POSITION  = 0xf2, TML_SONG_SELECT   = 0xf3, TML_TUNE_REQUEST    = 0xf6, TML_EOX          = 0xf7, TML_SYNC      = 0xf8,
	TML_TICK  = 0xf9, TML_START        = 0xfa, TML_CONTINUE       = 0xfb, TML_STOP          = 0xfc, TML_ACTIVE_SENSING  = 0xfe, TML_SYSTEM_RESET = 0xff
};

static int tml_readbyte(struct tml_parser* p)
{
	return (p->buf == p->buf_end ? -1 : *(p->buf++));
}

static int tml_readvariablelength(struct tml_parser* p)
{
	unsigned int res = 0, i = 0;
	unsigned char c;
	for (; i != 4; i++)
	{
		if (p->buf == p->buf_end) { TML_WARN("Unexpected end of file"); return -1; }
		c = *(p->buf++);
		if (c & 0x80) res = ((res | (c & 0x7F)) << 7);
		else return (int)(res | c);
	}
	TML_WARN("Invalid variable length byte count"); return -1;
}

static int tml_parsemessage(tml_message** f, struct tml_parser* p)
{
	int deltatime = tml_readvariablelength(p), status = tml_readbyte(p);
	tml_message* evt;

	if (deltatime & 0xFFF00000) deltatime = 0; //throw away delays that are insanely high for malformatted midis
	if (status < 0) { TML_WARN("Unexpected end of file"); return -1; }
	if ((status & 0x80) == 0)
	{
		// Invalid, use same status as before
		if ((p->last_status & 0x80) == 0) { TML_WARN("Undefined status and invalid running status"); return -1; }
		p->buf--;
		status = p->last_status;
	}
	else p->last_status = status;

	if (p->message_array_size == p->message_count)
	{
		//start allocated memory size of message array at 64, double each time until 8192, then add 1024 entries until done
		p->message_array_size += (!p->message_array_size ? 64 : (p->message_array_size > 4096 ? 1024 : p->message_array_size));
		*f = (tml_message*)TML_REALLOC(*f, p->message_array_size * sizeof(tml_message));
		if (!*f) { TML_ERROR("Out of memory"); return -1; }
	}
	evt = *f + p->message_count;

	//check what message we have
	if ((status == TML_SYSEX) || (status == TML_EOX)) //sysex
	{
		//sysex messages are not handled
		p->buf += tml_readvariablelength(p);
		if (p->buf > p->buf_end) { TML_WARN("Unexpected end of file"); p->buf = p->buf_end; return -1; }
		evt->type = 0;
	}
	else if (status == 0xFF) //meta events
	{
		int meta_type = tml_readbyte(p), buflen = tml_readvariablelength(p);
		unsigned char* metadata = p->buf;
		if (meta_type < 0) { TML_WARN("Unexpected end of file"); return -1; }
		if (buflen > 0 && (p->buf += buflen) > p->buf_end) { TML_WARN("Unexpected end of file"); p->buf = p->buf_end; return -1; }

		switch (meta_type)
		{
			case TML_EOT:
				if (buflen != 0) { TML_WARN("Invalid length for EndOfTrack event"); return -1; }
				if (!deltatime) return TML_EOT; //no need to store this message
				evt->type = TML_EOT;
				break;

			case TML_SET_TEMPO:
				if (buflen != 3) { TML_WARN("Invalid length for SetTempo meta event"); return -1; }
				evt->type = TML_SET_TEMPO;
				((struct tml_tempomsg*)evt)->Tempo[0] = metadata[0];
				((struct tml_tempomsg*)evt)->Tempo[1] = metadata[1];
				((struct tml_tempomsg*)evt)->Tempo[2] = metadata[2];
				break;

			default:
				evt->type = 0;
		}
	}
	else //channel message
	{
		int param; 
		if ((param = tml_readbyte(p)) < 0) { TML_WARN("Unexpected end of file"); return -1; }
		evt->key = (param & 0x7f);
		evt->channel = (status & 0x0f);
		switch (evt->type = (status & 0xf0))
		{
			case TML_NOTE_OFF:
			case TML_NOTE_ON:
			case TML_KEY_PRESSURE:
			case TML_CONTROL_CHANGE:
				if ((param = tml_readbyte(p)) < 0) { TML_WARN("Unexpected end of file"); return -1; }
				evt->velocity = (param & 0x7f);
				break;

			case TML_PITCH_BEND:
				if ((param = tml_readbyte(p)) < 0) { TML_WARN("Unexpected end of file"); return -1; }
				evt->pitch_bend = ((param & 0x7f) << 7) | evt->key;
				break;

			case TML_PROGRAM_CHANGE:
			case TML_CHANNEL_PRESSURE:
				evt->velocity = 0;
				break;

			default: //ignore system/manufacture messages
				evt->type = 0;
				break;
		}
	}

	if (deltatime || evt->type)
	{
		evt->time = deltatime;
		p->message_count++;
	}
	return evt->type;
}

TMLDEF tml_message* tml_load(struct tml_stream* stream)
{
	int num_tracks, division, trackbufsize = 0;
	unsigned char midi_header[14], *trackbuf = TML_NULL;
	struct tml_message* messages = TML_NULL;
	struct tml_track *tracks, *t, *tracksEnd;
	struct tml_parser p = { TML_NULL, TML_NULL, 0, 0, 0 };

	// Parse MIDI header
	if (stream->read(stream->data, midi_header, 14) != 14) { TML_ERROR("Unexpected end of file"); return messages; }
	if (midi_header[0] != 'M' || midi_header[1] != 'T' || midi_header[2] != 'h' || midi_header[3] != 'd' ||
	    midi_header[7] != 6   || midi_header[9] >  2) { TML_ERROR("Doesn't look like a MIDI file: invalid MThd header"); return messages; }
	if (midi_header[12] & 0x80) { TML_ERROR("File uses unsupported SMPTE timing"); return messages; }
	num_tracks = (int)(midi_header[10] << 8) | midi_header[11];
	division = (int)(midi_header[12] << 8) | midi_header[13]; //division is ticks per beat (quarter-note)
	if (num_tracks <= 0 && division <= 0) { TML_ERROR("Doesn't look like a MIDI file: invalid track or division values"); return messages; }

	// Allocate temporary tracks array for parsing
	tracks = (struct tml_track*)TML_MALLOC(sizeof(struct tml_track) * num_tracks);
	tracksEnd = &tracks[num_tracks];
	for (t = tracks; t != tracksEnd; t++) t->Idx = t->End = t->Ticks = 0;

	// Read all messages for all tracks
	for (t = tracks; t != tracksEnd; t++)
	{
		unsigned char track_header[8];
		int track_length;
		if (stream->read(stream->data, track_header, 8) != 8) { TML_WARN("Unexpected end of file"); break; }
		if (track_header[0] != 'M' || track_header[1] != 'T' || track_header[2] != 'r' || track_header[3] != 'k')
			{ TML_WARN("Invalid MTrk header"); break; }

		// Get size of track data and read into buffer (allocate bigger buffer if needed)
		track_length = track_header[7] | (track_header[6] << 8) | (track_header[5] << 16) | (track_header[4] << 24);
		if (track_length < 0) { TML_WARN("Invalid MTrk header"); break; }
		if (trackbufsize < track_length) { TML_FREE(trackbuf); trackbuf = (unsigned char*)TML_MALLOC(trackbufsize = track_length); }
		if (stream->read(stream->data, trackbuf, track_length) != track_length) { TML_WARN("Unexpected end of file"); break; }

		t->Idx = p.message_count;
		for (p.buf_end = (p.buf = trackbuf) + track_length; p.buf != p.buf_end;)
		{
			int type = tml_parsemessage(&messages, &p);
			if (type == TML_EOT || type < 0) break; //file end or illegal data encountered
		}
		if (p.buf != p.buf_end) { TML_WARN( "Track length did not match data length"); }
		t->End = p.message_count;
	}
	TML_FREE(trackbuf);

	// Change message time signature from delta ticks to actual msec values and link messages ordered by time
	if (p.message_count)
	{
		tml_message *PrevMessage = TML_NULL, *Msg, *MsgEnd, Swap;
		unsigned int ticks = 0, tempo_ticks = 0; //tick counter and value at last tempo change
		int step_smallest, msec, tempo_msec = 0; //msec value at last tempo change
		double ticks2time = 500000 / (1000.0 * division); //milliseconds per tick

		// Loop through all messages over all tracks ordered by time
		for (step_smallest = 0; step_smallest != 0x7fffffff; ticks += step_smallest)
		{
			step_smallest = 0x7fffffff;
			msec = tempo_msec + (int)((ticks - tempo_ticks) * ticks2time);
			for (t = tracks; t != tracksEnd; t++)
			{
				if (t->Idx == t->End) continue;
				for (Msg = &messages[t->Idx], MsgEnd = &messages[t->End]; Msg != MsgEnd && t->Ticks + Msg->time == ticks; Msg++, t->Idx++)
				{
					t->Ticks += Msg->time;
					if (Msg->type == TML_SET_TEMPO)
					{
						unsigned char* Tempo = ((struct tml_tempomsg*)Msg)->Tempo;
						ticks2time = ((Tempo[0]<<16)|(Tempo[1]<<8)|Tempo[2])/(1000.0 * division);
						tempo_msec = msec;
						tempo_ticks = ticks;
					}
					if (Msg->type)
					{
						Msg->time = msec;
						if (PrevMessage) { PrevMessage->next = Msg; PrevMessage = Msg; }
						else { Swap = *Msg; *Msg = *messages; *messages = Swap; PrevMessage = messages; }
					}
				}
				if (Msg != MsgEnd && t->Ticks + Msg->time > ticks)
				{
					int step = (int)(t->Ticks + Msg->time - ticks);
					if (step < step_smallest) step_smallest = step;
				}
			}
		}
		if (PrevMessage) PrevMessage->next = TML_NULL;
		else p.message_count = 0;
	}
	TML_FREE(tracks);

	if (p.message_count == 0)
	{
		TML_FREE(messages);
		messages = TML_NULL;
	}

	return messages;
}

TMLDEF tml_message* tml_load_tsf_stream(struct tsf_stream* stream)
{
	return tml_load((struct tml_stream*)stream);
}

TMLDEF int tml_get_info(tml_message* Msg, int* out_used_channels, int* out_used_programs, int* out_total_notes, unsigned int* out_time_first_note, unsigned int* out_time_length)
{
	int used_programs = 0, used_channels = 0, total_notes = 0;
	unsigned int time_first_note = 0xffffffff, time_length = 0;
	unsigned char channels[16] = { 0 }, programs[128] = { 0 };
	for (;Msg; Msg = Msg->next)
	{
		time_length = Msg->time;
		if (Msg->type == TML_PROGRAM_CHANGE && !programs[(int)Msg->program]) { programs[(int)Msg->program] = 1; used_programs++; }
		if (Msg->type != TML_NOTE_ON) continue;
		if (time_first_note == 0xffffffff) time_first_note = time_length;
		if (!channels[Msg->channel]) { channels[Msg->channel] = 1; used_channels++; }
		total_notes++;
	}
	if (time_first_note == 0xffffffff) time_first_note = 0;
	if (out_used_channels  ) *out_used_channels   = used_channels;
	if (out_used_programs  ) *out_used_programs   = used_programs;
	if (out_total_notes    ) *out_total_notes     = total_notes;
	if (out_time_first_note) *out_time_first_note = time_first_note;
	if (out_time_length    ) *out_time_length     = time_length;
	return total_notes;
}

TMLDEF int tml_get_tempo_value(tml_message* msg)
{
	unsigned char* Tempo;
	if (!msg || msg->type != TML_SET_TEMPO) return 0;
	Tempo = ((struct tml_tempomsg*)msg)->Tempo;
	return ((Tempo[0]<<16)|(Tempo[1]<<8)|Tempo[2]);
}

TMLDEF void tml_free(tml_message* f)
{
	TML_FREE(f);
}

#ifdef __cplusplus
}
#endif