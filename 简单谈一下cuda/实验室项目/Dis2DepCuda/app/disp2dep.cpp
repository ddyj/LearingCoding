//
// Created by zcb on 2022/10/24.
//

#include <iostream>
#include <string>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

    string disp_path, xml_path, depth_path;
    int image_group_num = 1;
    int width = 2160;
    int height = 3840;

    disp_path = "../data/output/0/%04d.tiff";
    xml_path = "../data/xml/stereo/%04d.xml";
    depth_path = "../data/%04d.png";

    for (int i = 0; i < image_group_num; i++) {
        char disp[255];
        char dep[255];
        char xml[255];

        sprintf(disp,disp_path.c_str(),i);
        sprintf(dep,depth_path.c_str(),i);
        sprintf(xml,xml_path.c_str(),i);

        std::cout<<disp<<std::endl;

        FileStorage xml_stereo(xml, FileStorage::READ);
        Mat P1, P2;
        xml_stereo["P1"] >> P1;
        xml_stereo["P2"] >> P2;
        P1.convertTo(P1, CV_64F);
        P2.convertTo(P2, CV_64F);

        double fx = P2.ptr<double>(0)[0];
        //! colmap：乘以1000
        double val = P2.ptr<double>(0)[3] * 1000;
//        double val = P2.ptr<double>(0)[3];
        double baseline = abs(val / fx);
        double scale = P2.ptr<double>(0)[2] - P1.ptr<double>(0)[2];
        cout << depth_path << " | baseline: " << baseline << " | scale: " << scale << " | fx: " << fx << endl;

        Mat img = imread(disp, -1);
        Mat depth_png(img.rows, img.cols, CV_16UC1);

        //! 视差图转深度图
        for (int v = 0; v < depth_png.rows; v++) {
            for (int u = 0; u < depth_png.cols; u++) {
                float d;
                d = static_cast<float>(img.ptr<float>(v)[u]);
               if ( fx * baseline / (d + scale) >= 5000 ||  fx * baseline / (d + scale) <= 0.0)
                   continue;
                depth_png.ptr<ushort>(v)[u] = fx * baseline / (d + scale);
            }
        }
        imwrite(dep, depth_png);
    }
    return 0;
}
