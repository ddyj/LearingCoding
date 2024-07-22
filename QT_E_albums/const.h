#pragma once
#ifndef CONST_H
#define CONST_H

#endif // CONST_H



enum TreeItemType{
    TreeItemPro = 1,   // 表示项目的条目
    TreeItemDir = 2,   // 表示项目的文件夹
    TreeItemPic = 3,   // 表示项目的图片
};

//默认读取文件最大长度
const int PROGRESS_MAX_WIDTH = 300;

enum PicBtnState {
    PicBtnStateNormal = 1,
    PicBtnStateHover = 2,
    PicBtnStatePressed = 3,

    PicBtnState2Normal = 4,
    PicBtnState2Hover = 5,
    PicBtnState2Pressed = 6,
};

//在滑动预览界面下方的滚动轴中 预览图片的预设大小和icon预设 这个值和ui界面中设置的宽度相关
const int PREITEM_SIZE = 100;
const int PREICON_SIZE = 90;