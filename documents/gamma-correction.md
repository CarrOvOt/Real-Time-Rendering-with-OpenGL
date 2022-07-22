### Gamma校正

简单来说Gamma校正存在的原因有两点：

1. 当光的物理强度线性变化时，人眼的感知到的强度是非线性的，人眼对较低的强度更敏感

2. 光强（灰阶）这个信息在电脑中存储的精度是有限的

基于这两个原因，我们在存储灰阶信息时不会按照物理强度线性划分，而是根据人眼的感知的强度划分，也就是说我们会用更多的位数保存暗部的信息。

具体来说，一个像素想表示一个物理强度为E的颜色（$E\in[0,1]$），那么这个像素在计算机中存储的强度会是$E_0=E^\gamma$ （大部分时候gamma=2.2）。当计算机告诉显示器一个像素的强度为$E_0$时，显示器会负责将E_0转化为实际的物理强度$E=E_0^{1/\gamma}$，这也是显示器的gamma这个参数的意义。

知道了Gamma校正了之后就会发现我们在shader中的各种计算是基于存储的数据（$E_0$）而不是物理的线性的数据（$E$），所以计算光照衰减时我们希望衰减的是物理强度E，但实际却用了$E_0$，这就是之前光照衰减过快的原因。

综上，我们进行gamma校正的过程就是

1. 在计算着色之前将颜色/漫反射贴图从sRGB空间（这个空间中gamma约等于2.2）转化为线性空间。
   
   ```cpp
   // 着色器中手动转换
   float gamma = 2.2;
   vec3 diffuseColor = pow(texture(diffuse, texCoords).rgb, vec3(gamma));
   
   // 或者
   
   // OpenGL中指定sRGB模式
   glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
   ```

2. 进行各种计算。

3. 最终计算完成后将线性空间的结果校正到sRGB空间。
   
   ```cpp
   // 着色器中手动转换
   float gamma = 2.2;
   fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
   
   // 或者
   
   // OpenGL中指定输出sRGB空间的帧缓存
   glEnable(GL_FRAMEBUFFER_SRGB);
   ```

参考资料：

[Gamma校正 - LearnOpenGL CN (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/02%20Gamma%20Correction/)

[色彩校正中的 gamma 值是什么？ - 韩世麟的回答 - 知乎](https://www.zhihu.com/question/27467127/answer/37555901)
