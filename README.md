# SmartMonitor     
智能监控系统项目     
     
  项目简介：该系统在铁塔建设的施工现场，通过PC机处理采集的图像信息和倾角传感器数据，对施工人     
员的行为进行分析，检测施工现场的违规操作和危险情况，提醒现场作业管理人员处理。     
     
  实现方式：通过MFC应用程序调用图像处理算法和传感器数据采集函数，通过全局MFC界面组件显示检测结果     
通过多线程临界区控制主界面，避免显示控件占用冲突     
     
  代码结构：界面代码（包括界面布局和界面事件）开发、任务调度、传感器数据采集、摄像头图像采集     
     
##代码文件功能描述     
###主界面和任务调度     
主界面文件：     
WorkDlg     
1、左侧栏为用户操作控件组     
（1）用户点击连接视频，则开始连接摄像头并对视频进行行为检测；     
点击时间将连接摄像头，并调用检测线程不断定时检测视频帧；     
     
（2）用户点击录像，开始录像；点击停止，则保存录像；     
     
（3）用户点击检测角度，开始通过传感器检测抱杆倾角。倾角数据实时显示到右侧     
     
2、中间为视频显示控件     
3、右侧为视频检测结果显示     
4、下边为状态栏     
     
CommonFunc     
共用函数     
     
     
     
任务调度文件：     
DetectControl     
1、全局显示控件     
包括视频显示控件、检测结果显示列表控件、状态栏控件     
     
2、任务调度线程（检测高空作业、绞磨机工作、现场施工检测线程）     
     
     
###检测函数     
ConstructionSiteDetection     
施工现场施工行为检测     
DetectHelmetBelt     
安全帽带子检测     
HelmetsDetection     
安全帽检测     
SmokeDetection     
吸烟检测     
GrinderDetection     
绞磨机检测     
PedestrianClassDetection     
行人检测     
WorkAtHeight     
高空检测     
PedestrianClassDetection     
行人分类检测     
FaceRecognition     
人脸检测     
ColorDetection     
颜色检测     
MovementDetection     
运动检测     
     
     
###抱杆倾角采集     
ComParse     
解析串口数据     
CMSComm     
微软串口控件     
     
     
     
     
     