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
	
    status = pjmedia_mem_capture_set_eof_cb(capture_port, this, AudioMediaCapture::processFrame);
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

pj_status_t AudioMediaCapture::processFrame(pjmedia_port *port, void *usr_data) 
{
    AudioMediaCapture *capture = static_cast<AudioMediaCapture *>(usr_data);

	if(!capture->frame_buffer) return !PJ_SUCCESS;
	
	capture->received_frames++;
    const std::lock_guard<std::mutex> lock(capture->frames_mtx);
	capture->onNewFrame();
	return PJ_SUCCESS;
}

void AudioMediaCapture::stopMediaCapture() 
{
	frame_buffer = NULL;
	unregisterMediaPort();
}

///////////////////////////////////////////////////////////////////////////////
