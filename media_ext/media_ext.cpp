///////////////////////////////////////////////////////////////////////////////

// pj_status_t incomingFrameHandler(pjmedia_port *port, void *usr_data);

AudioMediaCapture::AudioMediaCapture()
{
    pool = pjsua_pool_create("capture_pool", 2000, 2000);
    frame_buffer = NULL;
    capture_port = NULL;
}

AudioMediaCapture::~AudioMediaCapture()
{
    if(capture_port) {
        unregisterMediaPort();
        pjmedia_port_destroy(capture_port);
        capture_port = NULL;
        frame_buffer = NULL;
    }
    if(pool) {
        pj_pool_safe_release(&pool);
    }
}

pj_status_t AudioMediaCapture::createMediaCapture(pjsua_call_id id)
{
	pj_status_t status;
    pjsua_call_info ci;
    pjsua_call_get_info(id, &ci);

    pjsua_conf_port_info cpi;
    pjsua_conf_get_port_info(ci.conf_slot, &cpi);
    frame_size = cpi.bits_per_sample*cpi.samples_per_frame*cpi.channel_count/8;
    frame_buffer = pj_pool_zalloc(pool, frame_size);
    status = pjmedia_mem_capture_create( pool, //Pool
                          frame_buffer, //Buffer
                          frame_size, //Buffer Size
                          cpi.clock_rate,
                          cpi.channel_count,
                          cpi.samples_per_frame,
                          cpi.bits_per_sample,
                          0, //Options
                          &capture_port); //The return port}
	if(status != PJ_SUCCESS) return status;
	
    status = pjmedia_mem_capture_set_eof_cb2(capture_port, this, AudioMediaCapture::processFrames);
	if(status != PJ_SUCCESS) return status;
	
    registerMediaPort2(capture_port, pool);
	return PJ_SUCCESS;
}

void AudioMediaCapture::processFrames(pjmedia_port *port, void *usr_data) {
    AudioMediaCapture *capture = static_cast<AudioMediaCapture *>(usr_data);
    const std::lock_guard<std::mutex> lock(capture->frames_mtx);
    capture->frames.push_back(string((char *)capture->frame_buffer, capture->frame_size));
}

void AudioMediaCapture::stopMediaCapture() {
	 unregisterMediaPort();
}

string AudioMediaCapture::getFramesAsString()
{
    const std::lock_guard<std::mutex> lock(frames_mtx);
    std::string s="";
    for (const auto &piece : frames) s += piece;
    frames.clear();
    return s;
}

void AudioMediaCapture::getFrames(char **data, size_t *datasize)
{
    std::string s = getFramesAsString();
    size_t size = s.length();
    char *c = new char[size];
    s.copy(c, size, 0);
    *datasize = size;
    *data = c;
}

AudioMediaStream::AudioMediaStream() {
    pool = pjsua_pool_create("stream_pool", 2000, 2000);
    frame_buffer = NULL;
    stream_port = NULL;
}

AudioMediaStream::~AudioMediaStream() {
    if(stream_port) {
        unregisterMediaPort();
        pjmedia_port_destroy(stream_port);
        stream_port = NULL;
        frame_buffer = NULL;
    }
    if(pool) {
        pj_pool_safe_release(&pool);
    }
}

void AudioMediaStream::putFrameAsString(string data) {
    const std::lock_guard<std::mutex> lock(frames_mtx);
    frames.push_front(data);
}

void AudioMediaStream::putFrame(char *data, size_t datasize) {
    const std::lock_guard<std::mutex> lock(frames_mtx);
    frames.push_front(std::string(data, datasize));
    //frames.push_front(frame);
}

pj_status_t AudioMediaStream::processFrames(pjmedia_port *port, void *usr_data) {
// void AudioMediaStream::processFrames(pjmedia_port *port, void *usr_data) {
    AudioMediaStream *stream = static_cast<AudioMediaStream *>(usr_data);
    if(stream->frames.size() > 0) {
        string f = stream->frames.back();
        unsigned buf_size = f.length();
        if(buf_size > stream->frame_size) buf_size = stream->frame_size;
        f.copy((char*)stream->frame_buffer, buf_size, 0);
        const std::lock_guard<std::mutex> lock(stream->frames_mtx);
        stream->frames.pop_back();
    } else {
        memset((char*)stream->frame_buffer, 0, stream->frame_size);
    }
    return PJ_SUCCESS;
}

void AudioMediaStream::createMediaStream(pjsua_call_id id)
{
    pjsua_call_info ci;
    pjsua_call_get_info(id, &ci);

    pjsua_conf_port_info cpi;
    pjsua_conf_get_port_info(ci.conf_slot, &cpi);
    frame_size = cpi.bits_per_sample*cpi.samples_per_frame*cpi.channel_count/8;
    frame_buffer = pj_pool_zalloc(pool, frame_size);
    pjmedia_mem_player_create( pool, //Pool
                          frame_buffer, //Buffer
                          frame_size, //Buffer Size
                          cpi.clock_rate,
                          cpi.channel_count,
                          cpi.samples_per_frame,
                          cpi.bits_per_sample,
                          0, //Options
                          // PJMEDIA_MEM_NO_LOOP,
                          &stream_port); //The return port}
    pjmedia_mem_player_set_eof_cb(stream_port, this, AudioMediaStream::processFrames);
    // pjmedia_mem_player_set_eof_cb2(stream_port, this, AudioMediaStream::processFrames);
    registerMediaPort2(stream_port, pool);
}

///////////////////////////////////////////////////////////////////////////////
