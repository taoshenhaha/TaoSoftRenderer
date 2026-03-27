#include "core/platform.h"
#include "graphics/RenderPipe.h"

#include <iostream>
#include <unistd.h>

int main()
{
    // 初始化平台
    platform_initialize();
    
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 800;
    constexpr int channels = 4;
    // 创建窗口
    window_t *window = window_create("TaoSoftRenderer", WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (!window) {
        std::cerr << "创建窗口失败!" << std::endl;
        return -1;
    }
    
    std::cout << "窗口创建成功!" << std::endl;
    std::cout << "按 ESC 键或关闭窗口退出程序" << std::endl;
    RenderPipe *renderPipe = new RenderPipe();
    renderPipe->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // 主循环
    while (!window_should_close(window)) {
        // 处理输入事件
        input_poll_events();
        
        if(renderPipe != nullptr)
        {
            renderPipe->render();
        }

        // 检查按键
        if (input_key_pressed(window, KEY_SPACE)) {
            std::cout << "空格键被按下!" << std::endl;
        }
        
        // 绘制缓冲区
        window_draw_buffer(window, renderPipe->getFrameBuffer());

        // 渲染场景
        // ...

        // 获取鼠标位置
        float mouse_x, mouse_y;
        input_query_cursor(window, &mouse_x, &mouse_y);
        
        // 简单延迟，避免CPU占用过高
        usleep(16000); // 约60FPS
    }
    
    // 清理资源
    window_destroy(window);
    platform_terminate();
    
    std::cout << "程序正常退出" << std::endl;
    return 0;
}