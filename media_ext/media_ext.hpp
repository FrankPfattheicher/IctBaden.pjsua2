
#ifndef __PJSUA2_MEDIA_EXT_HPP__
#define __PJSUA2_MEDIA_EXT_HPP__

#include <mutex>
#include <list>

/** PJSUA2 API is inside pj namespace */
namespace pj
{

	class AudioMediaCapture : public AudioMedia 
	{
		public:
			AudioMediaCapture();
			~AudioMediaCapture();
			
			pj_status_t createMediaCapture(pjsua_call_id);
			void stopMediaCapture();
			
			unsigned getFrameSize();
			unsigned getFramesCaptured();
			void *getFrameBuffer();
			
			virtual void onNewFrame() = 0;
			
		private:
			pj_pool_t *pool;
			pjmedia_port *capture_port;
			std::mutex frames_mtx;
			
			unsigned frame_size;
			unsigned received_frames;
			void *frame_buffer;

			static void processFrame(pjmedia_port *, void *);
	};
	
	class AudioMediaPlayback : public AudioMedia 
	{
		public:
			AudioMediaPlayback();
			pj_status_t createMediaPlayback(pjsua_call_id);
			virtual ~AudioMediaPlayback();
			static void processFrames(pjmedia_port *, void *);

			unsigned getFrameSize();
			void stopPlayback();

			void putFrame(void *frameData, size_t datasize);
			bool isPaying();
		    virtual void onPlaybackDone() = 0;
			
		private:
			pj_pool_t *pool;
			pjmedia_port *playback_port;
			void *frame_buffer;
			unsigned frame_size;
			std::list<string> frames;
			std::mutex frames_mtx;
	};

} // namespace pj

#endif  /* __PJSUA2_MEDIA_EXT_HPP__ */

