#ifndef CSOUND
#define CSOUND

#include <dmusici.h>
#include <clist.h>

class Sound : public Object
{
private:
	int muted;
	IDirectMusicSegment8 *segment;
	IDirectMusicSegmentState *segmentState;
	IDirectMusicSegmentState8 *segmentState8;
	MUSIC_TIME length;
	static IDirectMusic *music;
	static IDirectMusic8 *music8;
	static IDirectMusicLoader8 *loader;
	static IDirectMusicPerformance8 *performance;
	static IDirectMusicAudioPath8 *audioPath;
public:
	Sound();
	~Sound();
	void destroy();
	static void destroyPlayer();
	void fadeOut(int duration);
	bool finished();
	MUSIC_TIME getLength() {return length;}
	int getMuted() {return muted;}
	DWORD getRepeats();
	MUSIC_TIME getSeek();
	static Sound* getSoundElement(int index, List *pList) {return dynamic_cast<Sound*>(pList->GetElement(index));}
	static void initPlayer(HWND hWnd, DWORD path, DWORD channel, DWORD features);
	bool isPlaying() {return performance->IsPlaying(segment, NULL) == S_OK;}
	void load(Str fileName, bool midi=false);
	void mute();
	void play(DWORD flag, bool useAudioPath=true);
	void setMuted(int muted) {this->muted = muted;}
	void setRepeats(DWORD n) {segment->SetRepeats(n);}
	void stop();
	static void stop(DWORD flag);
	static void stopAll() {performance->Stop(NULL, NULL, 0, 0);}
	void unload();
	void unmute();
};

#endif