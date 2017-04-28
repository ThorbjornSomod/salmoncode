#include "imageRetriever.hpp"

void *lock(void *data, void **p_pixels){

	struct ctx *ctx = (struct ctx*)data;

	ctx->imagemutex->lock();

	// Pixel will be stored on image pixel space:

	*p_pixels = ctx->pixels;
   
	return NULL;
}

void display(void *data, void *id){

	(void) data;

	assert(id == NULL);

}

void unlock(void *data, void *id, void *const *p_pixels){

	// Get back data structure:
 
	struct ctx *ctx = (struct ctx*)data;

	ctx->imagemutex->unlock();

}

void* updateMatrixData(void* voidContext){

	// VLC pointers:

	libvlc_instance_t* vlcInstance;
	libvlc_media_player_t* mp;
	libvlc_media_t* media;

	const char* const vlc_args[] = {

		"-I", "dummy", 			// Don't use any interface
		"--ignore-config", 		// Don't use VLC's config
		"--extraintf=logger", 		// Log anything
		"--verbose=0", 			// Be much more verbose then normal for debugging purpose
		"--avcodec-fast",		// Use fast avcodec
		"--live-caching==1000",		// Toggle live caching
		"--network-caching=1000",	// Hold data for 1 ms
		"--avcodec-hw=dxva2"		// Decode on GPU

	};

	// Create a VLC instance:

	vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

	// Read a distant video stream:

	media = libvlc_media_new_location(vlcInstance, "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm");
	//media = libvlc_media_new_path(vlcInstance, "file///home/sealab/salmoncode/cascadeDetection/shitfile.mp4");

	mp = libvlc_media_player_new_from_media(media);

	libvlc_media_release(media);

	//struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	struct ctx* context = static_cast<struct ctx*>(voidContext);

	context->imagemutex = new std::mutex();	
	context->image = new Mat(VIDEO_HEIGHT, VIDEO_WIDTH, CV_8UC3);
	context->pixels = (unsigned char*)context->image->data; 
  
	// Show blank image:

	imshow("Display", *context->image);

	libvlc_video_set_callbacks(mp, lock, unlock, display, context);
	libvlc_video_set_format(mp, "RV24", VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH * 24 / 8); // Pitch = Width * BitsPerPixel / 8

	int key = 0;

	libvlc_media_player_play(mp);	

	while(key != 27){
	
		sleep(100);
	
	}

	libvlc_media_player_stop(mp);
	libvlc_media_player_release(mp);
	libvlc_release(vlcInstance);

	return NULL;

}
