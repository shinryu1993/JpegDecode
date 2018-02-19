#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "jpeg_decoder.hpp"

namespace {

static void jpeg_error_exit(j_common_ptr cinfo)
{
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message)(cinfo, buffer);
    throw std::runtime_error(buffer);
}

}  // unnamed namespace

JpegDecoder::JpegDecoder(void)
{
    cinfo_.err = jpeg_std_error(&jerr_);
    jerr_.error_exit = jpeg_error_exit;
    jpeg_create_decompress(&cinfo_);
}

JpegDecoder::~JpegDecoder(void)
{
    jpeg_destroy_decompress(&cinfo_);
}

void JpegDecoder::decode_from_file(const std::string& jpeg_file, cv::Mat* out_image)
{
    // JPEGファイルを開く
    FILE* fp = fopen(jpeg_file.c_str(), "r");
    if (fp == NULL) {
        std::cerr << "Failed to open a JPEG file!" << std::endl;
        *out_image = cv::Mat();
        return;
    }

    // JPEGファイルのデコード
    try {
        jpeg_stdio_src(&cinfo_, fp);
        this->decode_jpeg(out_image);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // 終了処理
    fclose(fp);
}

void JpegDecoder::decode_from_memory(const std::string& jpeg_mem, cv::Mat* out_image)
{
    try {
        const unsigned char* buffer = reinterpret_cast<const unsigned char*>(jpeg_mem.c_str());
        const size_t buffer_size = jpeg_mem.size();
        jpeg_mem_src(&cinfo_, buffer, buffer_size);
        this->decode_jpeg(out_image);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void JpegDecoder::decode_jpeg(cv::Mat* out_image)
{
    /* ヘッダを確認してJPEGフォーマットか判断 */
    if (jpeg_read_header(&cinfo_, TRUE) != JPEG_HEADER_OK) {
        throw std::runtime_error("Not a JPEG file!");
    }

    /* デコード開始 */
    // デコードするための準備
    jpeg_start_decompress(&cinfo_);

    // 出力先のメモリを確保
    const int out_height   = cinfo_.output_height;
    const int out_width    = cinfo_.output_width;
    const int out_channels = cinfo_.out_color_components;
    const int color_type = (out_channels == 3) ? CV_8UC3 : CV_8UC1;
    out_image->create(cv::Size(out_width, out_height), color_type);

    // デコード
    JSAMPROW buffer = reinterpret_cast<JSAMPROW>(calloc(out_width * out_channels, sizeof(JSAMPLE)));
    if (buffer == NULL) {
        throw std::runtime_error("Not allocated a memory in buffer!");
    }
    uchar* out_data = out_image->data;
    for (int row = 0; row < out_height; ++row) {
        jpeg_read_scanlines(&cinfo_, &buffer, 1);
        JSAMPROW row_buf = buffer;  // bufferのポインタは動かしたくないため
        for (int col = 0; col < out_width; ++col, row_buf += 3) {
            for (int ch = out_channels - 1; 0 <= ch; --ch) {  // B -> G -> R or Gray
                *out_data++ = *(row_buf + ch);
            }
        }
    }

    // 後処理
    jpeg_finish_decompress(&cinfo_);
    free(buffer);
}
