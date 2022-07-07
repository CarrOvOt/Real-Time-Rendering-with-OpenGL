学习中。。。

在window环境下使用openGL学习实时渲染相关的各种基础/算法/实现/技巧等。。。

### ver0.1

**搭建基础环境，能够显示出一个窗口。**

使用GLFW实现windows下的窗口创建，输入处理等；使用GLAD实现OpenGL相关函数的处理；

参考资料：[你好，窗口 - LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/)

<img title="" src="MDImages/2022-07-03-17-36-00-image.png" alt="" width="404">

### ver0.2

**以默认模式和线框模式（Wireframe Mode）显示一个立方体**

OpenGL可以理解为一个状态机，所以我们的代码主要就是设置状态机的各种设置，使用的顶点数据，使用的shader，然后让他执行渲染即可。

pipeline：

- 创建并绑定VAO，VBO，EBO

- 创建编译顶点着色器，片段着色器，链接

- 主循环中渲染即可

<img title="" src="MDImages/2022-07-04-17-52-40-image.png" alt="" width="310"> <img title="" src="MDImages/2022-07-04-17-53-19-image.png" alt="" width="310">

现在我们只能看到立方体的一个面，而且是长方形不是正方形，这是因为目前没有对坐标进行各个空间上的变换（局部空间，世界空间，观察空间），这个问题之后再解决。

- 这里存在一个小bug，一个立方体应该渲染2x6x3个顶点（所以左图应有两条对角线），实际代码中只渲染了6个顶点，该bug已在ver0.2.1中修复。

参考资料：[你好，三角形 - LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/)

### ver0.21

**整理代码，将着色器程序，顶点数据单独构建成类**

Shader：从文件中读取着色器代码，编译并链接。

Mesh：加载顶点数据，生成VAO，VBO，EBO，以及主循环中使用的Draw函数。这里我们把顶点定义为一个结构体，因为顶点除了坐标外还有其他数据，方便之后的更新。

<img src="MDImages/2022-07-05-00-44-58-image.png" title="" alt="" width="307">

参考资料：[着色器 - LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/)

### ver0.3

**引入[ImGUI](https://github.com/ocornut/imgui)，方便后续各种参数的调试，简单说明在[这里](./documents/ImGUI-usage.md)**

<img title="" src="MDImages/2022-07-05-21-55-00-image.png" alt="" width="321">

### ver0.4

**加入坐标在各个空间上的变换，这样就可以看到立体的mesh了**

这里使用GLM进行各种数学上的计算，然后用ImGUI调整参数（mesh的旋转，缩放，平移），像fps游戏一样的摄像机系统（键鼠控制移动/方向）在后面的版本再实现。

因为现在mesh顶点的坐标会根据各空间的变换矩阵变化，所以在shader中（vertex shader）我们需要将变换矩阵告诉shader，这里主要使用shader中的uniform（变量），所以我们要完善一下Shader类并修改shader的源代码。

（这个版本简单设置了一下包含目录，将不包含.lib和.dll的第三方库（glm，ImGUI）都移至`External/include`中了）（这样可用尖括号include了，这样三方库中的一些include语句就不会报错了）

![ver0.4.gif](MDImages/dcb0a14cdc22238aa2dc3ca362e99480e09a0738.gif)

参考资料：

[坐标系统 - LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/)

[着色器 - LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/)

### ver0.5

**把ImGUI的初始化放在另外一个文件中，让main函数看起来干净一点**

将一个结构体传给GUI类让其自动生成相应的能修改结构体内容的ImGUI窗口需要用到**反射**，现在并不会，就先放一下吧，所以现在ImGUI窗口构建的代码仍然写在main.cpp里。

然后加入了输入处理（esc退出）和窗口大小调整的适配。

参考资料：[从零开始手敲次世代游戏引擎（八十四） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/517828683)

### ver0.6

**给立方体加上贴图**

首先我们需要用[stb_image.h]([stb/stb_image.h at master · nothings/stb (github.com)](https://github.com/nothings/stb/blob/master/stb_image.h))这个库来加载图片文件（当然其他的库也行），把这个.h文件加入我们的项目即可。

<img title="" src="MDImages/3e234bb222b6876e349cdeb4cc5b9a0cec59a64a.png" alt="" width="281">

贴图（纹理/texture）一般是和mesh绑定的，所以我们在simpleMesh类中添加相关代码。

我们将一个2D的图片映射到一个3D的mesh上，需要告诉每个顶点对应的贴图上的坐标（这里需要拆分顶点：[一些说明](./documents/texture.md)），所以我们还需要修改顶点属性并告诉OpenGL（用glVertexAttribPointer）如何解析数据。

拆分顶点且坐标设置无误后效果应该是这样的

![ver0.6-1.gif](MDImages/1ad34ad076f87ab741cda0759179de1ff02adde9.gif)



可以看到有点奇怪，首先我们看到的是cube的里侧，其次有时候后面的面反而把前面的面遮住了。其实这是同一个原因导致的：没有进行[深度测试](https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/#3d)，三角面片会根据渲染顺序显示，而不是远近顺序。使用Z缓冲（Z-buffer）进行深度测试即可

![](MDImages/2022-07-07-15-01-36-image.png)



参考资料：[纹理 - LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/)
