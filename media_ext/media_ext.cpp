///////////////////////////////////////////////////////////////////////////////

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
	received_frames = 0;
	
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
	
    status = pjmedia_mem_capture_set_eof_cb2(capture_port, this, AudioMediaCapture::processFrame);
	if(status != PJ_SUCCESS) return status;
	
	pjsua_conf_port_id port_id;
	pjsua_conf_add_port(pool, capture_port, &port_id);
	pjsua_conf_connect(ci.conf_slot, port_id); //connect port with conference

    registerMediaPort2(capture_port, pool);
	return PJ_SUCCESS;
}

unsigned AudioMediaCapture::getFrameSize() 
{
	return frame_size;
}

unsigned AudioMediaCapture::getFramesCaptured() 
{
	return received_frames;
}

void *AudioMediaCapture::getFrameBuffer()
{
	return frame_buffer;
}

void AudioMediaCapture::processFrame(pjmedia_port *port, void *usr_data) 
{
    AudioMediaCapture *capture = static_cast<AudioMediaCapture *>(usr_data);

	if(!capture->frame_buffer) return;
	
	capture->received_frames++;
    const std::lock_guard<std::mutex> lock(capture->frames_mtx);
	capture->onNewFrame();
}

void AudioMediaCapture::stopMediaCapture() 
{
	frame_buffer = NULL;
	unregisterMediaPort();
}

///////////////////////////////////////////////////////////////////////////////


AudioMediaPlayback::AudioMediaPlayback() 
{
    pool = pjsua_pool_create("stream_pool", 2000, 2000);
    frame_buffer = NULL;
    stream_port = NULL;
}

AudioMediaPlayback::~AudioMediaPlayback() 
{
    if(stream_port) 
	{
        unregisterMediaPort();
        pjmedia_port_destroy(stream_port);
        stream_port = NULL;
        frame_buffer = NULL;
    }
    if(pool) 
	{
        pj_pool_safe_release(&pool);
    }
}

unsigned AudioMediaPlayback::getFrameSize() 
{
	return frame_size;
}

void AudioMediaPlayback::stopPlayback() 
{
	frames.clear();
}

void AudioMediaPlayback::putFrame(void *frameData, size_t datasize) 
{
    const std::lock_guard<std::mutex> lock(frames_mtx);
    frames.push_front(std::string((char *)frameData, datasize));
	playback = true;
}

void AudioMediaPlayback::processFrames(pjmedia_port *port, void *usr_data) 
{
    AudioMediaPlayback *stream = static_cast<AudioMediaPlayback *>(usr_data);
    if(stream->frames.size() > 0) {
        string f = stream->frames.back();
        unsigned buf_size = f.length();
        if(buf_size > stream->frame_size) buf_size = stream->frame_size;
        f.copy((char*)stream->frame_buffer, buf_size, 0);
        const std::lock_guard<std::mutex> lock(stream->frames_mtx);
        stream->frames.pop_back();
    } else {
        memset((char*)stream->frame_buffer, 0, stream->frame_size);
		if(stream->playback) {
			stream->playback = false;
			stream->onPlaybackDone();
		}
    }
}

pj_status_t AudioMediaPlayback::createMediaPlayback(pjsua_call_id id)
{
	pj_status_t status;
	
    pjsua_call_info ci;
    pjsua_call_get_info(id, &ci);

    pjsua_conf_port_info cpi;
    pjsua_conf_get_port_info(ci.conf_slot, &cpi);
    frame_size = cpi.bits_per_sample*cpi.samples_per_frame*cpi.channel_count/8;
    frame_buffer = pj_pool_zalloc(pool, frame_size);
    status = pjmedia_mem_player_create( pool, //Pool
                          frame_buffer, //Buffer
                          frame_size, //Buffer Size
                          cpi.clock_rate,
                          cpi.channel_count,
                          cpi.samples_per_frame,
                          cpi.bits_per_sample,
                          0, //Options
                          // PJMEDIA_MEM_NO_LOOP,
                          &stream_port); //The return port}
	if(status != PJ_SUCCESS) return status;
	
    status = pjmedia_mem_player_set_eof_cb2(stream_port, this, AudioMediaPlayback::processFrames);
	if(status != PJ_SUCCESS) return status;
	
    registerMediaPort2(stream_port, pool);
	return PJ_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
