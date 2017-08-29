# dailyCoding
### 1. 2017/8/8 记录实现空间三角面的相交或重叠的判断。
> 引用博客：http://blog.csdn.net/u010944926/article/details/19896263
> 			http://blog.csdn.net/fourierfeng/article/details/11969915
---
### 2. 2017/8/9 记录构建OpenGl框架的简单想法，方便测试、编写项目中需要的 Maintenance Model
---
### 3. 2017/8/10 记录构建完成 Model FrameWork，注意enable选项的选择，方便更进一步的coding function module
---
### 4. 2017/8/14 Using the blog method in model files makes a lot of mistakes. 我忽略的模型文件中的三角面片非常紧凑的特点.
---
### 5. 2017/8/15 Today is bad.想重新构建HDS,在读取model，insertface,insertedge时存在逻辑错误。
---
### 6. 2017/8/16 Today,测试model，放缩数据处理，intersect明显下降(delete).但是发现一个 a large bug,虽然在8/14有所了解,但是没有认识到问题的严重.round Mesh3D class
---
### 7. 2017/8/17 修改了空间三角面相交检测算法中copy_point的问题，注意引用问题及要明确复制的坐标点(不明白为什么,赋值x,z.YZ轴面就能通过).还存在放缩范围过大的问题，应修改.
---
### 8. 2017/8/18 针对检测相交算法中copy_point存在的问题，明确是三维转二维时投影判断出现问题.面临密集网格中剥离判断问题
---
### 9. 2017/8/21 对sameplane中面片的判断，要控制一些条件，共顶点的，共边的包含关系，只一点在边上面应return true;
---
### 10.2017/8/22 对边的处理，并不是那么理想.float值得判断需要处理进位的问题.
---
### 11.2017/8/23 还是决定实现包围盒进行处理，避免太多的循环检测，出现的不可控问题。Triangle class添加了center，radius构建包围球.
---
### 12.2017/8/23 转移到Meshprint上进行整合开发，Dialog与Mainwindow之间的消息传递.
---
### 13.2017/8/28 calculate the volume DEMO,尝试构建八叉树
---
### 14.2017/8/29 review AABB support octree