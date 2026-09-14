// FontConfig.h
#ifndef FONT_CONFIG_H
#define FONT_CONFIG_H

#include <string>

class FontConfig {
public:
    // 获取适合当前系统的中文字体
    static std::string getChineseFont() {
#if defined(_WIN32)
        // Windows系统
        return "\"Microsoft YaHei\"";
#elif defined(__APPLE__)
        // macOS系统
        return "\"PingFang SC\"";
#else
        // Linux系统
        return "\"WenQuanYi Micro Hei\"";
#endif
    }
    
    // 获取节点样式
    static std::string getNodeStyle() {
        return "shape=circle, style=filled, fillcolor=lightblue, fontname=" + getChineseFont();
    }
    
    // 获取边样式
    static std::string getEdgeStyle() {
        return "fontname=" + getChineseFont();
    }
    
    // 获取HTML字体标签
    static std::string getFontTag(const std::string& text, int size = 10) {
        return "<<font face=" + getChineseFont() + " point-size=\"" 
               + std::to_string(size) + "\">" + text + "</font>>";
    }
    
    // 检查系统是否支持中文
    static bool isChineseSupported() {
        // 简单检测，实际应该检查字体是否存在
        return true;
    }
};

#endif // FONT_CONFIG_H