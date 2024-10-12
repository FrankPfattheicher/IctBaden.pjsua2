
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

			static pj_status_t processFrame(pjmedia_port *, void *);
	};

} // namespace pj

#endif  /* __PJSUA2_MEDIA_EXT_HPP__ */

