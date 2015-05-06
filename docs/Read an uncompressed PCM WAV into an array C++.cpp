void Read_wav::read_wav(const string & filename, int * * * data,
    unsigned int * * data_size, wav_params * * params) {
    c_file_handle file(filename, "r");
    unsigned int num_ch, samp_rate, bits_per_samp, num_samp;
    read_wav_header(file, & num_ch, & samp_rate, & bits_per_samp, & num_samp);

    * params = new wav_params(num_ch, samp_rate, bits_per_samp); * data = (int * * ) calloc(num_ch, sizeof(int * ));
    assert( * data); * data_size = (unsigned int * ) malloc(num_ch * sizeof(unsigned int));
    assert( * data_size);
    for (unsigned int i = 0; i < num_ch; ++i) { * data[i] = (int * ) malloc(num_samp * sizeof(int)); * data_size[i] = num_samp;
    }

    read_wav_data(file, * data, * * params, num_samp);
}


inline void Read_wav_impl::read_wav_header(c_file_handle & file,
    unsigned int * num_ch, unsigned int * samp_rate,
    unsigned int * bits_per_samp, unsigned int * num_samp) {
    unsigned char buffer[5];

    /* ChunkID (RIFF for little-endian, RIFX for big-endian) */
    file.read(buffer, 4);
    buffer[4] = '\0';
    if (strcmp((char * ) buffer, "RIFF")) {
        throw except("RIFF file format not detected", file.filename);
    }

    /* ChunkSize */
    file.read(buffer, 4);
    /* UNUSED
    const unsigned int chunk_size = buffer[0] + (buffer[1] << 8) +
    (buffer[2] << 16) + (buffer[3] << 24);
    */

    /* Format */
    file.read(buffer, 4);
    buffer[4] = '\0';
    if (strcmp((char * ) buffer, "WAVE")) {
        throw except("WAVE file format not detected", file.filename);
    }

    /* Subchunk1ID */
    file.read(buffer, 4);
    buffer[4] = '\0';
    if (strcmp((char * ) buffer, "fmt ")) {
        throw except("fmt section not found", file.filename);
    }

    /* Subchunk1Size (16 for PCM) */
    file.read(buffer, 4);
    if (buffer[0] != 16 || buffer[1] || buffer[2] || buffer[3]) {
        throw except("PCM format not detected", file.filename);
    }

    /* AudioFormat (PCM = 1, other values indicate compression) */
    file.read(buffer, 2);
    if (buffer[0] != 1 || buffer[1]) {
        throw except("PCM format not detected", file.filename);
    }

    /* NumChannels (Mono = 1, Stereo = 2, etc) */
    file.read(buffer, 2); * num_ch = buffer[0] + (buffer[1] << 8);

    /* SampleRate (8000, 44100, etc) */
    file.read(buffer, 4); * samp_rate = buffer[0] + (buffer[1] << 8) +
        (buffer[2] << 16) + (buffer[3] << 24);

    /* ByteRate (SampleRate * NumChannels * BitsPerSample / 8) */
    file.read(buffer, 4);
    const unsigned int byte_rate = buffer[0] + (buffer[1] << 8) +
        (buffer[2] << 16) + (buffer[3] << 24);

    /* BlockAlign (NumChannels * BitsPerSample / 8) */
    file.read(buffer, 2);
    const unsigned int block_align = buffer[0] + (buffer[1] << 8);

    /* BitsPerSample */
    file.read(buffer, 2); * bits_per_samp = buffer[0] + (buffer[1] << 8);

    if (byte_rate != (( * samp_rate * * num_ch * * bits_per_samp) >> 3)) {
        throw except("ByteRate != SampleRate * NumChannels * "
            "BitsPerSample / 8", file.filename);
    }

    if (block_align != (( * num_ch * * bits_per_samp) >> 3)) {
        throw except("BlockAlign != NumChannels * "
            "BitsPerSample / 8", file.filename);
    }

    /* Subchunk2ID */
    file.read(buffer, 4);
    buffer[4] = '\0';
    if (strcmp((char * ) buffer, "data")) {
        throw except("data section not found", file.filename);
    }

    /* Subchunk2Size (NumSamples * NumChannels * BitsPerSample / 8) */
    file.read(buffer, 4);
    const unsigned int subchunk2_size = buffer[0] + (buffer[1] << 8) +
        (buffer[2] << 16) + (buffer[3] << 24); * num_samp = (subchunk2_size << 3) / ( * num_ch * * bits_per_samp);
}


inline void Read_wav_impl::read_wav_data(c_file_handle & file, int * * data,
    const wav_params & params, unsigned int num_samp) {
    const unsigned int num_ch = params.num_ch;
    const unsigned int bits_per_samp = params.bits_per_samp;
    unsigned char buffer;
    for (unsigned int i = 0; i != num_samp; ++i) {
        for (unsigned int j = 0; j != num_ch; ++j) {
            unsigned int tmp = 0;
            for (unsigned int k = 0; k != bits_per_samp; k += 8) {
                file.read( & buffer, 1);
                tmp += buffer << k;
            }
            data[j][i] = conv_bit_size(tmp, bits_per_samp);
        }
    }
}


inline int Read_wav_impl::conv_bit_size(unsigned int in , int bps) {
    const unsigned int max = (1 << (bps - 1)) - 1;
    return in > max ? in -(max << 1) : in ;
}