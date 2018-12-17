# Auto_Car_Tracking_Carry
这是校级比赛完成的作品，题目也会放出，主要功能为实现循迹搬运小车。可以参考题目结合报考查看。使用了两个单片机实现，主要是怪学识短浅没法充分发挥一个单片机
的性能。<br>
### 功能介绍  
- 可以沿着黑线前进   
- 再任务区通过独立键盘操作指定抓取瓶子    
- 将瓶子抓取后运送到指定终点并停止计时    

### 提供的文件
- 相关模块的介绍   *来源卖家，主要参考.c文件就可以了*<br>
- 作品报告  
- 比赛题目以及要求  
- testbefore的文件是比赛之前各类调试代码，有一定参考价值
- STC12C5A60S2 的开发手册以及头文件
- 12c5a60s2.c与89c52rc.c两个文件为最后比赛写入单片机的程序实现最终效果  

### 使用注意
1.因为使用的keil4没有提供STC12C5A60S2的头文件需要自行添加   
2.代码做了相关注释，但不能做到百分百易读性  
3.提供一种解决方案，相信有更多更好的    
4.代码可以免费无偿使用，但是希望看到更多新的东西    
~~5.因为成品小车以及拆除，后续会放比赛前的照片~~   
6. 题目要求的最后一个抓取随机瓶子没有完成但已有完整思路，以后可能会补上
大概是使用三个参数表示出口，三个参数表示自身，然后设计路线全部走完所有线来让小车自行规划路线到终点。当然也可以只用摄像头等高端操作  

#### 作品实物图
![Car](https://github.com/Twelvecat/Auto_Car_Tracking_Carry/blob/master/作品实物图.jpg)
