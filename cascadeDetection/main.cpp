#include "cascadeDetect.hpp"

using namespace std;
using namespace cv;

// LibVLC Media Player Variables:

struct VideoDataStruct{
	int param;
};

libvlc_media_player_t *mp;

int done = 0;
unsigned int videoBufferSize = 0;
uint8_t *videoBuffer = 0;

// LibVLC Media Player Functions:

void cbVideoPrerender(void *p_video_data, uint8_t **pp_pixel_buffer, int size){

	// Locking:

	if(size > videoBufferSize || !videoBuffer){

		printf("Reallocate raw video buffer.\n");

		free(videoBuffer);

		videoBuffer = (uint8_t *) malloc(size);

		videoBufferSize = size;		

	}

	*pp_pixel_buffer = videoBuffer;

}

void cbVideoPostrender(void *p_video_data, uint8_t *p_pixel_buffer, int width, int height, int pizel_pitch, int size, int64_t pts){

	// Unlocking:

	// CloseHandle(hMutex);

}

static void handleEvent(const libvlc_event_t* pEvt, void *pUserData){

	libvlc_time_t time;

	switch(pEvt->type){

		case libvlc_MediaPlayerTimeChanged:

			time = libvlc_media_player_get_time(mp);

			printf("MediaPlayerTimeChanged %11d ms\n", (long long)time);
		
			break;

		case libvlc_MediaPlayerEndReached:

			printf("MediaPlayerEndReached\n");

			done = 1;

			break;

		default:

			printf("%s\n", libvlc_event_type_name(pEvt->type));

	}
}

int main() {

	// Video sources:

	std::string aquapod = "rtsp://admin:ral1004@192.168.1.6:2020/videoinput_1/h264_1/media.stm";

	std::string minipod = "rtsp://something";

	const char* video_source = "file:///home/sealab/salmoncode/cascadeDetection/mostsat.avi";

	// VLC pointers:

	libvlc_instance_t *inst;
	libvlc_media_t *m;
	void *pUserData = 0;
	VideoDataStruct dataStruct;

	// VLC options:

	char smem_options[1000];

	// R24 codec:

	sprintf(smem_options
		, "#transcode{vcodec=R24}:smem{""video-prerender-callback=%11d,""video-postrender-callback=%11d,""video-data=%11d,""no-time-sync},"
		, (long long int)(intptr_t)(void*)&cbVideoPrerender
		, (long long int)(intptr_t)(void*)&cbVideoPostrender
		, (long long int)(intptr_t)(void*)&dataStruct
	); 

	const char * const vlc_args[] = {
		"-I", "dummy",
		"--ignore-config",
		"--extraintf=logger",
		"--verbose=1",
		"--sout", smem_options
		};

	// Launching VLC media player:

	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

	// Create a new media item:

	m = libvlc_media_new_location(inst, video_source);

	// Create a media player playing environment:

	mp = libvlc_media_player_new_from_media(m);

	libvlc_event_manager_t* eventManager = libvlc_media_player_event_manager(mp);

	libvlc_event_attach(eventManager, libvlc_MediaPlayerTimeChanged, handleEvent, pUserData);

	libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached, handleEvent, pUserData);

	libvlc_event_attach(eventManager, libvlc_MediaPlayerPositionChanged, handleEvent, pUserData);

	libvlc_video_set_format(mp, "RV24", 1280, 720, 1280*3);

	// Play the media player:

	libvlc_media_player_play(mp);

	while(1){

		// Check for valid frame:

		if(videoBuffer){

			Mat img = Mat(Size(1920, 1080), CV_8UC3, videoBuffer);

			namedWindow("Display", WINDOW_AUTOSIZE);

			imshow("Display", img);

			waitKey(15);

		}
	}

	libvlc_release(inst);

	// Detection using LBP Cascade Classifier:

	//cascadeDetect(video_source, "current_coidal.xml", "current_head.xml", "current_dorsal.xml");

	return 0;

}
