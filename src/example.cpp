#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include "jpeg_decoder.hpp"

std::string jpeg_to_string(const std::string& jpeg_file)
{
    std::ifstream ifs(jpeg_file);
    std::istreambuf_iterator<char> begin(ifs);
    std::istreambuf_iterator<char> end;
    return std::string(begin, end);
}

int main(void)
{
    {
        // test1: ファイルからJPEG画像を読み込む。
        std::cout << "test1: ファイルからJPEG画像を読み込む" << std::endl;
        const std::string jpeg_file = "img/lenna.jpg";
        JpegDecoder decoder;
        cv::Mat image;
        decoder.decode_from_file(jpeg_file, &image);
        cv::imshow("decode from file to Mat.", image);
        cv::waitKey();
    }
    {
        // test2: メモリからJPEG画像をデコードする。
        std::cout << "test2: メモリからJPEG画像をデコードする" << std::endl;
        const std::string jpeg_file = "img/lenna.jpg";
        std::string jpeg_str = jpeg_to_string(jpeg_file);
        JpegDecoder decoder;
        cv::Mat image;
        decoder.decode_from_memory(jpeg_str, &image);
        cv::imshow("decode from mem to Mat.", image);
        cv::waitKey();
    }
    {
        // test3: ファイルからPNG画像を読み込む。
        std::cout << "test3: ファイルからPNG画像を読み込む" << std::endl;
        const std::string jpeg_file = "img/lenna.png";
        JpegDecoder decoder;
        cv::Mat image;
        decoder.decode_from_file(jpeg_file, &image);
    }
    {
        // test4: メモリからPNG画像をデコードする。
        std::cout << "test4: メモリからPNG画像をデコードする" << std::endl;
        const std::string jpeg_file = "img/lenna.png";
        std::string jpeg_str = jpeg_to_string(jpeg_file);
        JpegDecoder decoder;
        cv::Mat image;
        decoder.decode_from_memory(jpeg_str, &image);
    }
    {
        // test5: 画像ファイルがない場合
        std::cout << "test5: 画像ファイルがない場合" << std::endl;
        const std::string jpeg_file = "img/miss.jpg";
        JpegDecoder decoder;
        cv::Mat image;
        decoder.decode_from_file(jpeg_file, &image);
    }

    return 0;
}
