#ifndef LIB_JPEG_DECODER_HPP
#define LIB_JPEG_DECODER_HPP

#include <string>
#include <jpeglib.h>
#include <opencv2/core/core.hpp>

/**
 * JpegDecoder
 * @brief JPEGファイルのデコーダ。
 *        ファイルだけでなく、メモリからもデコードできる。
 */
class JpegDecoder
{
public:
    JpegDecoder(void);
    ~JpegDecoder(void);

    /**
     * @brief 画像ファイルを読み込んでデコードする。
     * @param[in] jpeg_file 画像ファイル名
     * @param[out] out_image デコードした画像の出力先
     * @return void なし
     */
    void decode_from_file(const std::string& jpeg_file, cv::Mat* out_image);

    /**
     * @brief JPEGフォーマットの文字列を読み込んでデコードする。
     * @param[in] jpeg_file JPEGフォーマットの文字列
     * @param[out] out_image デコードした画像の出力先
     * @return void なし
     */
    void decode_from_memory(const std::string& jpeg_mem, cv::Mat* out_image);

private:
    /**
     * @brief JPEGフォーマットの文字列をデコードする。
     * @param[out] out_image デコードした画像の出力先
     * @return void なし
     */
    void decode_jpeg(cv::Mat* out_image);

    struct jpeg_decompress_struct cinfo_;
    struct jpeg_error_mgr jerr_;
};

#endif  // LIB_JPEG_DECODER_HPP
