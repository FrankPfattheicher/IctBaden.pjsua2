
#ifndef __PJSUA2_MEDIA_EXT_HPP__
#define __PJSUA2_MEDIA_EXT_HPP__

#include <mutex>
#include <list>

/** PJSUA2 API is inside pj namespace */
namespace pj
{
// pj_status_t incomingFrameHandler(pjmedia_port *, void *);

class AudioMediaCapture : public AudioMedia {
public:
    AudioMediaCapture();
    ~AudioMediaCapture();
    void createMediaCapture(pjsua_call_id);
    void getFrames(char **data, size_t *datasize);
    string getFramesAsString();
    // static pj_status_t processFrames(pjmedia_port *, void *);
    static void processFrames(pjmedia_port *, void *);
private:
    pj_pool_t *pool;
    void *frame_buffer;
    unsigned frame_size;
    pjmedia_port *capture_port;
    std::list<string> frames;
    std::mutex frames_mtx;
};

class AudioMediaStream : public AudioMedia {
public:
    AudioMediaStream();
    void createMediaStream(pjsua_call_id);
    virtual ~AudioMediaStream();
    static pj_status_t processFrames(pjmedia_port *, void *);
    // static void processFrames(pjmedia_port *, void *);
    void putFrame(char *data, size_t datasize);
    void putFrameAsString(string);
private:
    pj_pool_t *pool;
    pjmedia_port *stream_port;
    void *frame_buffer;
    unsigned frame_size;
    std::list<string> frames;
    std::mutex frames_mtx;
};

} // namespace pj

#endif  /* __PJSUA2_MEDIA_EXT_HPP__ */

