#include <csound.h>
#include <cmsgreporter.h>
#include <dxerror.h>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

IDirectMusic *Sound::music = NULL;
IDirectMusic8 *Sound::music8 = NULL;
IDirectMusicLoader8 *Sound::loader = NULL;
IDirectMusicPerformance8 *Sound::performance = NULL;
IDirectMusicAudioPath8 *Sound::audioPath = NULL;

Sound::Sound()
{
	length = muted = 0;

	segment = NULL;
	segmentState = NULL;
	segmentState8 = NULL;

	CoCreateInstance(CLSID_DirectMusicSegment, NULL, CLSCTX_INPROC, IID_IDirectMusicSegment8, (void**)&segment);
	CoCreateInstance(CLSID_DirectMusicSegmentState, NULL, CLSCTX_INPROC, IID_IDirectMusicSegmentState, (void**)&segmentState);
	CoCreateInstance(CLSID_DirectMusicSegmentState, NULL, CLSCTX_INPROC, IID_IDirectMusicSegmentState8, (void**)&segmentState8);
}

Sound::~Sound()
{
}

void Sound::destroy()
{
	if (segmentState)
		segmentState8->Release();

	if (segment)
		segment->Release();
}

void Sound::destroyPlayer()
{
	performance->CloseDown();
	audioPath->Release();
	performance->Release();
	loader->Release();

	CoUninitialize();
}

void Sound::fadeOut(int duration)
{
	DMUS_CURVE_PMSG *pCurveMsg;

	ReportDxError(performance->AllocPMsg(sizeof(DMUS_CURVE_PMSG), (DMUS_PMSG**) &pCurveMsg), DIRECTMUSIC, ERR_DM_ANY);

	ZeroMemory(pCurveMsg, sizeof(DMUS_CURVE_PMSG));
	pCurveMsg->dwSize = sizeof(DMUS_CURVE_PMSG);
	pCurveMsg->rtTime = 0;
	pCurveMsg->dwFlags = DMUS_PMSGF_DX8 | DMUS_PMSGF_REFTIME | DMUS_PMSGF_LOCKTOREFTIME;
	pCurveMsg->dwPChannel = DMUS_PCHANNEL_BROADCAST_PERFORMANCE;
	pCurveMsg->dwType = DMUS_PMSGT_CURVE;
	pCurveMsg->dwGroupID = 0xFFFFFFF;
	pCurveMsg->mtDuration = duration;
	pCurveMsg->nEndValue = 0;
	pCurveMsg->bCurveShape = DMUS_CURVES_LINEAR;
	pCurveMsg->bCCData = 7;
	pCurveMsg->bFlags = DMUS_CURVE_RESET | DMUS_CURVE_START_FROM_CURRENT;
	pCurveMsg->bType = DMUS_CURVET_CCCURVE ;
	pCurveMsg->mtResetDuration = 0;
	pCurveMsg->nResetValue = 127;

	ReportDxError(performance->SendPMsg((DMUS_PMSG*) pCurveMsg), DIRECTMUSIC, ERR_DM_ANY);
}

bool Sound::finished()
{
	MUSIC_TIME seek;

	segmentState8->GetSeek(&seek);

	if (!seek)
		return true;

	if (seek >= length)
		return true;

	return false;
}

DWORD Sound::getRepeats()
{
	DWORD repeats;

	segment->GetRepeats(&repeats);

	return repeats;
}

MUSIC_TIME Sound::getSeek()
{
	MUSIC_TIME seek;

	segmentState8->GetSeek(&seek);

	return seek;
}

void Sound::initPlayer(HWND hWnd, DWORD path, DWORD channel, DWORD features)
{
	BOOL fAuto = TRUE;

	CoInitialize(NULL);

	CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&loader);
	CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**)&performance);
	
	ReportDxError(performance->InitAudio(&music, NULL, hWnd, path, channel, features, NULL), DIRECTMUSIC, ERR_DM_ANY); //Fait foirer sous Win98
	ReportDxError(performance->CreateStandardAudioPath(path, channel, TRUE, &audioPath), DIRECTMUSIC, ERR_DM_ANY);
	ReportDxError(music->QueryInterface(IID_IDirectMusic8, (LPVOID*)&music8), DIRECTMUSIC, ERR_DM_ANY);
	ReportDxError(performance->SetGlobalParam(GUID_PerfAutoDownload, &fAuto, sizeof(BOOL)), DIRECTMUSIC, ERR_DM_ANY);

	music->Release();
}

void Sound::load(Str fileName, bool midi)
{
	ReportDxError(loader->LoadObjectFromFile(CLSID_DirectMusicSegment, IID_IDirectMusicSegment8, fileName, (LPVOID*)&segment), DIRECTMUSIC, ERR_DM_ANY);

	if (midi)
		ReportDxError(segment->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL), DIRECTMUSIC, ERR_DM_ANY);

	ReportDxError(segment->GetLength(&length), DIRECTMUSIC, ERR_DM_ANY);
}

void Sound::mute()
{
	if (!muted)
		muted = 1;

	if (isPlaying())
		stop();
}

void Sound::play(DWORD flag, bool useAudioPath)
{
	if (!muted)
	{
		if (useAudioPath)
			ReportDxError(performance->PlaySegmentEx(segment, NULL, NULL, flag, 0, &segmentState, NULL, audioPath), DIRECTMUSIC, ERR_DM_ANY);
		else
			ReportDxError(performance->PlaySegment(segment, flag, 0, &segmentState), DIRECTMUSIC, ERR_DM_ANY);
		ReportDxError(segmentState->QueryInterface(IID_IDirectMusicSegmentState8, (LPVOID*)&segmentState8), DIRECTMUSIC, ERR_DM_ANY);
		segmentState->Release();
	}
}

void Sound::stop()
{
	performance->Stop(segment, segmentState8, 0, 0);
}

void Sound::stop(DWORD flag)
{
	// Stop only sounds playing on corresponding segment flag
	performance->StopEx(NULL, 0, flag);
}

void Sound::unload()
{
	if (segmentState)
		SAFE_RELEASE(segmentState);

	if (segment)
	{
		ReportDxError(loader->ReleaseObjectByUnknown(segment), DIRECTMUSIC, ERR_DM_ANY);
		ReportDxError(segment->Unload(performance), DIRECTMUSIC, ERR_DM_ANY);
		SAFE_RELEASE(segment);
	}

	loader->CollectGarbage();
}

void Sound::unmute()
{
	if (muted == 1)
		muted = 0;
}
