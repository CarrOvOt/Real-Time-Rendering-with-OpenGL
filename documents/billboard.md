### billboard（广告牌）效果

广告牌使用最多的场景就是角色/NPC的名称和血条之类的信息展示，无论摄像机如何移动，广告牌在屏幕上都是正向的，也就是一直面对摄像机。

<img src="MDImages/2022-07-19-00-15-47-image.png" title="" alt="" width="260"> <img title="" src="MDImages/2022-07-19-00-16-05-image.png" alt="" width="300">

所以实现广告牌效果比较直观的理解就是旋转物体，从而使物体一直面向摄像机。所以我们要做的就是计算出正确的旋转矩阵。理论上这个旋转矩阵的计算方法有很多，比如用法向量夹角之类的，这里我展示一下我目前使用的方法：

```cpp
    // billboard effect by roate the Transform
    glm::vec3 trans = glm::vec3(Transform[3][0], Transform[3][1], Transform[3][2]);
    glm::mat4 new_transform = glm::translate(glm::mat4(1.0f), -trans) * Transform;
    glm::mat4 rot = camera.GetViewMatrix();
    rot[3][0] = 0;
    rot[3][1] = 0;
    rot[3][2] = 0;
    new_transform = glm::inverse(rot) * new_transform;
    new_transform = glm::translate(glm::mat4(1.0f), trans)* new_transform;

    DrawShader.setMat4("model_sp", new_transform);
    DrawShader.setMat4("view_sp", camera.GetViewMatrix());
    DrawShader.setMat4("proj_sp", camera.GetProjMatrix());
```

物体的坐标变化是需要按一定顺序的，比如要先缩放旋转再平移（如果先平移的话，旋转和缩放的效果就会叠加到平移上，这时候平移就不是最开始的平移了），所以我们先将物体移回原点（变换矩阵的第四列就是代表平移的向量）

```cpp
    glm::vec3 trans = glm::vec3(Transform[3][0], Transform[3][1], Transform[3][2]);
    glm::mat4 new_transform = glm::translate(glm::mat4(1.0f), -trans) * Transform;
```

这样之后的旋转就是物体的局部空间的旋转了。

旋转矩阵我们是这样得到的

```cpp
    glm::mat4 rot = camera.GetViewMatrix();
    rot[3][0] = 0;
    rot[3][1] = 0;
    rot[3][2] = 0;
    new_transform = glm::inverse(rot) * new_transform;
```

相机的观察矩阵（view matrix）反映的是相机的位置和朝向。

我们将观察矩阵应用到场景上，也就是将它旋转平移到摄像机面前，如果我们取消这个旋转，那么这个物体和摄像机就会永远保持同一个方向，所以我们将物体反方向旋转一下（这里不知道怎么讲得更清楚，还请自行理解一下），然后将平移加回去

```cpp
new_transform = glm::translate(glm::mat4(1.0f), trans)* new_transform;
```

就得到了最终的物体的变换矩阵了。
